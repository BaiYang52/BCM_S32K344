/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* STARTSINGLE_OF_MULTIPLE */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*  \file     Can.c / Can_Irq.c / Can_Local.h
 *  \brief    Implementation of the CAN driver
 *  \details  see functional description below
 *
 *********************************************************************************************************************/
/* ***************************************************************************
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver   Author  Description
| ---------  ---  ------  ----------------------------------------------------
| 2012-01-09  1.00.00  Rse    - ESCAN00056163: Initial version: Hll - Flexcan3 - Imx6 - QCC - High-End - QNX
| 2012-09-20  1.01.00  Rse    - ESCAN00062414: Support workaround for erratum ERR005829
| 2013-01-30  1.02.00  Rse    - ESCAN00065347: Support platform MPC5700
|                             - ESCAN00064665: CANbedded only: CAN messages will not be sent any more
|                             - ESCAN00066667: Support workaround for erratum e5769
|                             - ESCAN00067035: Inconsistent combination of receive handle and receive information (id, dlc, data) is indicated to upper layer
| 2013-05-21  1.03.00  Rse    - ESCAN00067478: Support FlexCAN2 derivatives for RxFifo usage
|                             - ESCAN00067660: Support the feature Mirror Mode
| 2013-07-22  1.04.00  Rse    - ESCAN00069229: Support CAN message retransmit feature for FBL driver
|                             - ESCAN00070121: Support Vybrid with FlexCAN3
| 2013-11-21  1.05.00  Rse    - ESCAN00072129: ASR4 only: change BusOff behavior to ensure recovery time of 128x11 recessive bits
|                             - ESCAN00072322: AR4-328: Predefined Runtime Measurement Points
| 2013-12-09  1.06.00  Rse    - ESCAN00072515: AutoSar only: Can_SetControllerMode(controller, CAN_T_STOP) returns CAN_OK instead of CAN_NOT_OK
|                             - ESCAN00072597: CANbedded only: Retransmit does not work if more than 22 RxFullCAN objects are used
| 2014-01-08  1.07.00  Rse    - ESCAN00072828: CANbedded only: Compiler error: identifier "canCanInterruptCounter" is undefined
|                             - ESCAN00072863: Support Cancel in Hardware with Transmission Abort feature
|                             - ESCAN00072869: Support new interface for Bus Mirroring
| 2014-01-30  1.07.01  Rse    - ESCAN00073299: Parity check only available for Vybrid and MPC5700 derivatives
| 2014-02-06  1.07.02  Rse    - Some minor MISRA improvements
|                      Rse    - ESCAN00073889: CANbedded only: Compiler error: struct has unknown field
| 2014-03-24  1.07.03  Rse    - ESCAN00074424: Indication function is called twice for the reception of one BasicCAN message
| 2014-04-16  1.08.00  Rse    - ESCAN00075110: BusOff notification is called twice for one single BusOff event (FlexCAN2)
|                             - ESCAN00075955: Remove obsolete workaround implementation for e5769
|                             - ESCAN00075964: Delete unnecessary interrupt locks
|                             - ESCAN00075507: IAR Compiler warning: the order of volatile accesses is undefined
| 2014-06-24  1.09.00  Rse    - ESCAN00076588: CAN clock settings are invalid after reinitialization, error frames occur continuously on the bus
|                             - ESCAN00076653: Implement proper mode switch to init (FREEZE) mode in CAN controller initialization process
|                             - ESCAN00076657: MAXMB value is obsolete
|                             - ESCAN00077049: CANbedded only: Support Tx FullCAN Send Delay (External PIA)
|                             - ESCAN00077083: CANbedded only: LowLevel Message is not transmitted
|                             - ESCAN00077485: OS Exception when CAN driver is handled in User Mode
| 2014-08-13  1.10.00  Rse    - ESCAN00077825: Support derivative MPC5748G CUT2
| 2014-11-10  1.10.01  Rse    - ESCAN00079385: Compiler warning: argument of type "volatile uint32 *" is incompatible with parameter of type "volatile uint16 *"
| 2014-11-27  2.00.00  Rse    - ESCAN00078072: Support full functionality for CAN-FD (Mode 2)
|                             - ESCAN00079900: CANbedded only: Extended ID message with dominant SRR-Bit does not receive
|                             - ESCAN00079901: Support ComStackLib tables for ASR403 R11
|                             - ESCAN00079494: Add loop checks for mode switching in CAN Controller initialization process
| 2015-01-21  2.00.01  Rse    - ESCAN00080716: Standard CAN message is sent as CAN-FD message with baudrate switch
| 2015-04-09  2.00.02  Was    - ESCAN00082335: Update to Core 5.00.02, R11 (Vybrid)
| 2015-05-02  2.01.00  Rse    - ESCAN00082800: Support CANbedded compatibility
| 2015-09-02  2.02.00  Zam    - ESCAN00084930: Support Extended CAN RAM Check feature
| 2015-10-14  2.03.00  Pl     - ESCAN00085860: Support S32K with FlexCAN3
| 2015-12-03  3.00.00  Rse    - ESCAN00086974: Support R14 and process 3.0
| 2016-03-17  3.01.00  Rse    - ESCAN00088969: Support Extended RAM check for MSR403
| 2016-05-30  3.01.01  Rse    - ESCAN00090210: Compiler error: parse error near ';'
| 2016-06-20  3.01.02  Rse    - Update HL-ASR 5.04.01 for issue 89754
| 2016-08-03  3.01.03  Rse    - ESCAN00091088: Callback that indicates corrupt CAN Controller is called erroneously
| 2016-08-25  3.02.00  Rse    - ESCAN00091626: Support R14 for Arm32 platforms
|                             - ESCAN00092139: Support ISO CAN-FD
|                             - ESCAN00092218: Support possibility to use a non-CAN-FD baudrate on a CAN-FD channel
|                             - ESCAN00092333: Support asymmetric mailboxes
| 2016-10-26  3.02.01  Rse    - ESCAN00092531: Linker Error: undefined symbol found 'Can_GetCanNumberOfMaxMailboxes'
| 2016-12-12  3.02.02  Rli    - ESCAN00093255: CAN FD FULL support for Little Endian Architectures
| 2017-03-30  3.02.03  Rli    - ESCAN00094552: Compiler error: identifier Controller not declared
| 2017-04-11  4.00.00  Rse    - ESCAN00094734: Beta support of RI2.0
| 2017-11-30  4.00.01  yoe    - ESCAN00097612: Beta support of RI2.0 for Arm32Flexcan3
| 2018-02-26  4.01.00  Rli    - ESCAN00098508: Final support of RI2.0 for Arm32Flexcan3
| 2018-03-07  4.01.01  Rli    - ESCAN00098514: add MISRA relevant changes
| 2018-04-27  4.02.00  Rli    - ESCAN00099193: Final support of RI2.0 for Mpc5700Flexcan3
| 2017-10-31  5.00.00  Rli    - ESCAN00097302 Support of SafeBSW for FlexCAN3
| 2018-03-08  5.00.01  Bns    - Update HL-ASR 5.07.02 for issue 96367
|                             - ESCAN00098678 Support Extended RAM Check for S32
| 2018-03-12  5.01.00  Gaz    - ESCAN00098459 Update to Core 6.00.00
| 2018-06-19  5.02.00  Rli    - ESCAN00099727 Support of Virtual Addressing with CAN FD Mode 2
| 2018-08-02  5.03.00  Rli    - STORYC-6186: Support Silent Mode
|                             - ESCAN00100316 IMX8: Messages not received with CAN FD configuration
| 2018-10-19  5.04.00  Yoe    - STORYC-6683: Support SafeBsw for Asymmetric Mailboxes
| 2018-10-31  5.04.01  Yoe    - STORYC-6877: Support SafeBsw for S32k1xx derivatives
| 2018-11-22  5.04.02  Rli    - STORYC-7138: Update code due to Cfg5 bugfix
| 2019-01-16  5.04.03  Rli    - ESCAN00101634: FlexCAN3 e10368: Transition of the CAN FD operation enable bit may lead FlexCAN logic to an inconsistent state
| 2019-02-02  6.00.00  Rli    - ESCAN00101108: RxFifo support for FlexCAN2 removed
|                             - STORYC-6864: Support AR4-R21 and update to new HighLevel interface
|                             - STORYC-6865: Mirror Mode support with FD  inclusively
|                             - STORYC-6866: Virtual Addressing support with FD mode 2 inclusively
|                             - STORYC-6867: Silent Mode support for baudrate detection
|                             - STORYC-7901: Support AR4-R22
| 2019-05-20  6.00.01  Rli    - STORYC-8800: Support IMX ASIL and Extended Ramcheck
| 2019-07-17  6.01.00  Bns    - STORYC-8855: Support Extended Ramcheck for canbedded
| 2019-07-17  6.01.01  Bns    - STORYC-8860: Core Update & Extended RAM Check fix
| 2019-09-25  6.01.02  Rli    - ESCAN00104425: Unlock RX FullCAN and RX BasicCAN mailbox may not work correctly (AUTOSAR only)
| 2020-04-21  6.02.00  Rli    - CAN1421: [FLEXCAN3] S32K2/3 support
|                             - ESCAN00106152: FlexCAN3: CAN memmap error
| 2020-10-06  6.02.01  Rli    - ESCAN00107557: Call to CanResetBusSleep() stays in STOPPED mode (Freeze mode) (CBD only)
| 2021-03-08  6.02.02  Dzi    - CAN-2167 Vybrid Support
| 2021-07-05  6.03.00  Ped    - CANHAL-395: MSR4 R26 Implementation of Security Event Reporting Feature
| 2021-10-12  6.04.00  Rli    - CANHAL-643: G9X Implementation
|                             - ESCAN00110843: Flexcan3: ErrorEventReporting results of CAN_ERROR_BIT_MONITORING0 and CAN_ERROR_BIT_MONITORING1 swapped
| 2021-12-06  6.04.01  Rli    - CANHAL-807: Canbedded S32K3xx Implementation
|                             - ESCAN00110773: Canbedded Flexcan3 wrong macro definition in can_def.h
| 2022-01-27  6.05.00  Rli    - CANHAL-877: Issue Fix Errata ERR008341
| 2022-08-04  6.06.00  Rli    - CANHAL-1476: Vayyar support
|                             - CANHAL-1404: E3xx support 
| 2022-11-22  6.06.01  Rli    - ESCAN00113164: Additional E3 CAN Registers not taken into account
| 2023-01-19  6.06.02  Rli    - ESCAN00113552: Vayyar 32 bit only register access not considered
|************************************************************************** */

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_INTERNAL
#define CAN_IRQ_SOURCE     /* testability */

#include "Can.h"
#if defined(V_OSTYPE_OSEK) /* COV_CAN_OS_USAGE */
# include "osek.h"
#endif
/* \trace SPEC-1408 */
#if defined(V_OSTYPE_AUTOSAR) /* COV_CAN_OS_USAGE */
# include "Os.h"
#endif


/* ECO_IGNORE_BLOCK_END */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x1000u) /* \trace SPEC-1699, SPEC-3837 */
# error "Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x03u)
# error "Header file are inconsistent!"
#endif
#if( DRVCAN_IMXFLEXCAN3ASR_VERSION != 0x0606u)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN_IMXFLEXCAN3ASR_RELEASE_VERSION != 0x02u)
# error "Source and Header file are inconsistent!"
#endif

# if( CAN_GEN_ImxFlexcan3Asr_VERSION != 0x0101u)
#  error "Generated Data are inconsistent!"
# endif

#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
V_DEF_P2CONST (extern, Can_ConfigType, VAR_INIT, CONST_PBCFG) Can_ConfigDataPtr; /* PRQA S 3447,3449,3451,3210 */ /* MD_Can_ExternalScope */
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/***************************************************************************/
/* Interrupt Service Routine                                               */
/***************************************************************************/
#define CAN_START_SEC_CODE  /*-----------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \trace SPEC-1579, SPEC-1395, SPEC-1567 */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define CanPhysToLogChannel_0
# define CanPhysToLogChannelIndex_0
# define CanPhysToLogChannel_1
# define CanPhysToLogChannelIndex_1
# define CanPhysToLogChannel_2
# define CanPhysToLogChannelIndex_2
# define CanPhysToLogChannel_3
# define CanPhysToLogChannelIndex_3
# define CanPhysToLogChannel_4
# define CanPhysToLogChannelIndex_4
# define CanPhysToLogChannel_5
# define CanPhysToLogChannelIndex_5
# define CanPhysToLogChannel_6
# define CanPhysToLogChannelIndex_6
# define CanPhysToLogChannel_7
# define CanPhysToLogChannelIndex_7
# define CanPhysToLogChannel_8
# define CanPhysToLogChannelIndex_8
# define CanPhysToLogChannel_9
# define CanPhysToLogChannelIndex_9
# define CanPhysToLogChannel_10
# define CanPhysToLogChannelIndex_10
# define CanPhysToLogChannel_11
# define CanPhysToLogChannelIndex_11
# define CanPhysToLogChannel_12
# define CanPhysToLogChannelIndex_12
# define CanPhysToLogChannel_13
# define CanPhysToLogChannelIndex_13
# define CanPhysToLogChannel_14
# define CanPhysToLogChannelIndex_14
# define CanPhysToLogChannel_15
# define CanPhysToLogChannelIndex_15
# define CanPhysToLogChannel_16
# define CanPhysToLogChannelIndex_16
# define CanPhysToLogChannel_17
# define CanPhysToLogChannelIndex_17
# define CanPhysToLogChannel_18
# define CanPhysToLogChannelIndex_18
# define CanPhysToLogChannel_19
# define CanPhysToLogChannelIndex_19
# define CanPhysToLogChannel_20
# define CanPhysToLogChannelIndex_20
# define CanPhysToLogChannel_21
# define CanPhysToLogChannelIndex_21
# define CanPhysToLogChannel_22
# define CanPhysToLogChannelIndex_22
# define CanPhysToLogChannel_23
# define CanPhysToLogChannelIndex_23
#else
# define CanPhysToLogChannel_0       kCanPhysToLogChannelIndex_0
# define CanPhysToLogChannelIndex_0  kCanPhysToLogChannelIndex_0,
# define CanPhysToLogChannel_1       kCanPhysToLogChannelIndex_1
# define CanPhysToLogChannelIndex_1  kCanPhysToLogChannelIndex_1,
# define CanPhysToLogChannel_2       kCanPhysToLogChannelIndex_2
# define CanPhysToLogChannelIndex_2  kCanPhysToLogChannelIndex_2,
# define CanPhysToLogChannel_3       kCanPhysToLogChannelIndex_3
# define CanPhysToLogChannelIndex_3  kCanPhysToLogChannelIndex_3,
# define CanPhysToLogChannel_4       kCanPhysToLogChannelIndex_4
# define CanPhysToLogChannelIndex_4  kCanPhysToLogChannelIndex_4,
# define CanPhysToLogChannel_5       kCanPhysToLogChannelIndex_5
# define CanPhysToLogChannelIndex_5  kCanPhysToLogChannelIndex_5,
# define CanPhysToLogChannel_6       kCanPhysToLogChannelIndex_6
# define CanPhysToLogChannelIndex_6  kCanPhysToLogChannelIndex_6,
# define CanPhysToLogChannel_7       kCanPhysToLogChannelIndex_7
# define CanPhysToLogChannelIndex_7  kCanPhysToLogChannelIndex_7,
# define CanPhysToLogChannel_8       kCanPhysToLogChannelIndex_8
# define CanPhysToLogChannelIndex_8  kCanPhysToLogChannelIndex_8,
# define CanPhysToLogChannel_9       kCanPhysToLogChannelIndex_9
# define CanPhysToLogChannelIndex_9  kCanPhysToLogChannelIndex_9,
# define CanPhysToLogChannel_10       kCanPhysToLogChannelIndex_10
# define CanPhysToLogChannelIndex_10  kCanPhysToLogChannelIndex_10,
# define CanPhysToLogChannel_11       kCanPhysToLogChannelIndex_11
# define CanPhysToLogChannelIndex_11  kCanPhysToLogChannelIndex_11,
# define CanPhysToLogChannel_12       kCanPhysToLogChannelIndex_12
# define CanPhysToLogChannelIndex_12  kCanPhysToLogChannelIndex_12,
# define CanPhysToLogChannel_13       kCanPhysToLogChannelIndex_13
# define CanPhysToLogChannelIndex_13  kCanPhysToLogChannelIndex_13,
# define CanPhysToLogChannel_14       kCanPhysToLogChannelIndex_14
# define CanPhysToLogChannelIndex_14  kCanPhysToLogChannelIndex_14,
# define CanPhysToLogChannel_15       kCanPhysToLogChannelIndex_15
# define CanPhysToLogChannelIndex_15  kCanPhysToLogChannelIndex_15,
# define CanPhysToLogChannel_16       kCanPhysToLogChannelIndex_16
# define CanPhysToLogChannelIndex_16  kCanPhysToLogChannelIndex_16,
# define CanPhysToLogChannel_17       kCanPhysToLogChannelIndex_17
# define CanPhysToLogChannelIndex_17  kCanPhysToLogChannelIndex_17,
# define CanPhysToLogChannel_18       kCanPhysToLogChannelIndex_18
# define CanPhysToLogChannelIndex_18  kCanPhysToLogChannelIndex_18,
# define CanPhysToLogChannel_19       kCanPhysToLogChannelIndex_19
# define CanPhysToLogChannelIndex_19  kCanPhysToLogChannelIndex_19,
# define CanPhysToLogChannel_20       kCanPhysToLogChannelIndex_20
# define CanPhysToLogChannelIndex_20  kCanPhysToLogChannelIndex_20,
# define CanPhysToLogChannel_21       kCanPhysToLogChannelIndex_21
# define CanPhysToLogChannelIndex_21  kCanPhysToLogChannelIndex_21,
# define CanPhysToLogChannel_22       kCanPhysToLogChannelIndex_22
# define CanPhysToLogChannelIndex_22  kCanPhysToLogChannelIndex_22,
# define CanPhysToLogChannel_23       kCanPhysToLogChannelIndex_23
# define CanPhysToLogChannelIndex_23  kCanPhysToLogChannelIndex_23,
#endif

/* ISR functions */
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
#  define CAN_USE_PHYSTOLOG_MAPPING
# endif

#if defined( C_ENABLE_INTERRUPT_SOURCE_SINGLE ) /* COV_CAN_FLEXCAN3_DERIVATIVE_INTERRUPT_SOURCE_SINGLE */
# if defined( C_ENABLE_CAN_RXTX_INTERRUPT )   || \
     defined( C_ENABLE_CAN_BUSOFF_INTERRUPT ) 
#  if defined( kCanPhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_0()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_0
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_0 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_0( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_0) */
#  if defined( kCanPhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_1()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_1
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_1 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_1( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_1) */
#  if defined( kCanPhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_2()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_2
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_2 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_2( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_2) */
#  if defined( kCanPhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_3()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_3
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_3 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_3( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_3) */
#  if defined( kCanPhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_4()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_4
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_4 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_4( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_4) */
#  if defined( kCanPhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_5()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_5
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_5 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_5( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_5) */
#  if defined( kCanPhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_6()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_6
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_6 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_6( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_6) */
#  if defined( kCanPhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_7()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_7
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_7 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_7( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_7) */
#  if defined( kCanPhysToLogChannelIndex_8 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_8()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_8
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_8Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_8Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_8 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_8Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_8Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_8( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(8)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_8); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_8) */
#  if defined( kCanPhysToLogChannelIndex_9 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_9()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_9
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_9Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_9Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_9 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_9Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_9Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_9( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(9)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_9); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_9) */
#  if defined( kCanPhysToLogChannelIndex_10 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_10()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_10
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_10Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_10Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_10 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_10Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_10Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_10( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(10)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_10); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_10) */
#  if defined( kCanPhysToLogChannelIndex_11 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_11()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_11
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_11Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_11Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_11 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_11Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_11Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_11( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(11)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_11); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_11) */
#  if defined( kCanPhysToLogChannelIndex_12 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_12()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_12
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_12Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_12Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_12 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_12Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_12Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_12( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(12)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_12); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_12) */
#  if defined( kCanPhysToLogChannelIndex_13 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_13()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_13
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_13Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_13Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_13 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_13Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_13Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_13( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(13)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_13); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_13) */
#  if defined( kCanPhysToLogChannelIndex_14 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_14()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_14
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_14Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_14Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_14 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_14Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_14Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_14( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(14)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_14); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_14) */
#  if defined( kCanPhysToLogChannelIndex_15 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_15()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_15
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_15Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_15Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_15 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_15Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_15Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_15( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(15)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_15); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_15) */
#  if defined( kCanPhysToLogChannelIndex_16 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_16()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_16
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_16Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_16Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_16 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_16Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_16Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_16( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(16)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_16); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_16) */
#  if defined( kCanPhysToLogChannelIndex_17 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_17()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_17
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_17Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_17Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_17 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_17Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_17Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_17( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(17)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_17); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_17) */
#  if defined( kCanPhysToLogChannelIndex_18 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_18()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_18
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_18Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_18Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_18 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_18Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_18Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_18( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(18)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_18); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_18) */
#  if defined( kCanPhysToLogChannelIndex_19 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_19()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_19
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_19Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_19Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_19 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_19Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_19Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_19( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(19)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_19); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_19) */
#  if defined( kCanPhysToLogChannelIndex_20 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_20()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_20
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_20Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_20Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_20 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_20Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_20Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_20( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(20)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_20); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_20) */
#  if defined( kCanPhysToLogChannelIndex_21 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_21()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_21
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_21Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_21Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_21 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_21Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_21Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_21( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(21)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_21); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_21) */
#  if defined( kCanPhysToLogChannelIndex_22 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_22()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_22
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_22Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_22Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_22 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_22Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_22Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_22( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(22)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_22); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_22) */
#  if defined( kCanPhysToLogChannelIndex_23 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             CanIsr_23()
****************************************************************************/
/*
|<DataModelStart>| CanIsr_23
Relation_Context:
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_23Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_23Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanIsr_23 )
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_23Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_23Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanIsr_23( void )
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanInterrupt(Can_GetPhysToLogChannel(23)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanInterrupt(CanPhysToLogChannel_23); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanIsr */
/* CODE CATEGORY 1 END */
#  endif /* (kCanPhysToLogChannelIndex_23) */
# endif /* C_ENABLE_CAN_RXTX_INTERRUPT || C_ENABLE_CAN_BUSOFF_INTERRUPT || C_ENABLE_CAN_WAKEUP_INTERRUPT */
#endif /* C_ENABLE_INTERRUPT_SOURCE_SINGLE */

#if defined( C_ENABLE_INTERRUPT_SOURCE_MULTIPLE ) /* COV_CAN_FLEXCAN3_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE */
# if defined( kCanPhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_0()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_0
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_0 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_0( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_0()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_0
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_0 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_0Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_0( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_0); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_0()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_0
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_0 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_0( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_0) */
# if defined( kCanPhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_1()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_1
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_1 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_1( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_1()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_1
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_1 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_1Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_1( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_1); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_1()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_1
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_1 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_1( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_1) */
# if defined( kCanPhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_2()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_2
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_2 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_2( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_2()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_2
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_2 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_2Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_2( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_2); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_2()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_2
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_2 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_2( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_2) */
# if defined( kCanPhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_3()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_3
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_3 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_3( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_3()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_3
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_3 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_3Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_3( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_3); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_3()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_3
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_3 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_3( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_3) */
# if defined( kCanPhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_4()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_4
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_4 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_4( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_4()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_4
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_4 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_4Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_4( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_4); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_4()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_4
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_4 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_4( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_4) */
# if defined( kCanPhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_5()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_5
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_5 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_5( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_5()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_5
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_5 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_5Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_5( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_5); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_5()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_5
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_5 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_5( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_5) */
# if defined( kCanPhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_6()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_6
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_6 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_6( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_6()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_6
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_6 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_6Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_6( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_6); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_6()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_6
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_6 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_6( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_6) */
# if defined( kCanPhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_7()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_7
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_7 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_7( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_7()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_7
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_7 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_7Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_7( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_7); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_7()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_7
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_7 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_7( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_7) */
# if defined( kCanPhysToLogChannelIndex_8 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_8()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_8
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_8Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_8 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_8Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_8( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(8)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_8); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_8()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_8
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_8Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_8 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_8Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_8( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(8)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_8); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_8()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_8
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_8Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_8 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_8Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_8Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_8( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(8)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_8); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_8) */
# if defined( kCanPhysToLogChannelIndex_9 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_9()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_9
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_9Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_9 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_9Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_9( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(9)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_9); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_9()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_9
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_9Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_9 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_9Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_9( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(9)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_9); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_9()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_9
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_9Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_9 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_9Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_9Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_9( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(9)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_9); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_9) */
# if defined( kCanPhysToLogChannelIndex_10 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_10()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_10
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_10Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_10 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_10Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_10( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(10)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_10); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_10()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_10
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_10Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_10 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_10Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_10( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(10)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_10); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_10()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_10
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_10Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_10 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_10Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_10Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_10( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(10)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_10); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_10) */
# if defined( kCanPhysToLogChannelIndex_11 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_11()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_11
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_11Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_11 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_11Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_11( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(11)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_11); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_11()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_11
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_11Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_11 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_11Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_11( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(11)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_11); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_11()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_11
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_11Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_11 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_11Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_11Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_11( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(11)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_11); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_11) */
# if defined( kCanPhysToLogChannelIndex_12 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_12()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_12
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_12Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_12 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_12Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_12( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(12)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_12); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_12()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_12
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_12Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_12 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_12Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_12( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(12)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_12); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_12()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_12
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_12Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_12 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_12Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_12Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_12( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(12)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_12); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_12) */
# if defined( kCanPhysToLogChannelIndex_13 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_13()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_13
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_13Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_13 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_13Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_13( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(13)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_13); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_13()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_13
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_13Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_13 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_13Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_13( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(13)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_13); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_13()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_13
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_13Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_13 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_13Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_13Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_13( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(13)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_13); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_13) */
# if defined( kCanPhysToLogChannelIndex_14 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_14()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_14
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_14Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_14 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_14Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_14( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(14)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_14); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_14()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_14
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_14Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_14 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_14Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_14( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(14)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_14); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_14()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_14
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_14Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_14 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_14Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_14Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_14( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(14)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_14); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_14) */
# if defined( kCanPhysToLogChannelIndex_15 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_15()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_15
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_15Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_15 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_15Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_15( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(15)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_15); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_15()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_15
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_15Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_15 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_15Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_15( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(15)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_15); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_15()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_15
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_15Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_15 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_15Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_15Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_15( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(15)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_15); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_15) */
# if defined( kCanPhysToLogChannelIndex_16 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_16()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_16
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_16Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_16 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_16Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_16( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(16)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_16); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_16()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_16
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_16Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_16 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_16Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_16( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(16)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_16); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_16()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_16
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_16Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_16 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_16Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_16Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_16( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(16)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_16); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_16) */
# if defined( kCanPhysToLogChannelIndex_17 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_17()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_17
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_17Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_17 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_17Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_17( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(17)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_17); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_17()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_17
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_17Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_17 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_17Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_17( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(17)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_17); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_17()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_17
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_17Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_17 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_17Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_17Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_17( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(17)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_17); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_17) */
# if defined( kCanPhysToLogChannelIndex_18 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_18()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_18
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_18Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_18 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_18Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_18( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(18)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_18); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_18()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_18
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_18Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_18 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_18Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_18( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(18)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_18); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_18()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_18
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_18Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_18 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_18Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_18Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_18( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(18)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_18); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_18) */
# if defined( kCanPhysToLogChannelIndex_19 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_19()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_19
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_19Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_19 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_19Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_19( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(19)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_19); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_19()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_19
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_19Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_19 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_19Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_19( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(19)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_19); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_19()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_19
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_19Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_19 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_19Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_19Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_19( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(19)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_19); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_19) */
# if defined( kCanPhysToLogChannelIndex_20 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_20()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_20
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_20Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_20 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_20Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_20( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(20)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_20); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_20()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_20
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_20Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_20 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_20Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_20( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(20)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_20); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_20()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_20
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_20Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_20 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_20Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_20Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_20( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(20)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_20); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_20) */
# if defined( kCanPhysToLogChannelIndex_21 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_21()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_21
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_21Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_21 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_21Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_21( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(21)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_21); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_21()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_21
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_21Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_21 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_21Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_21( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(21)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_21); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_21()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_21
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_21Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_21 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_21Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_21Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_21( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(21)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_21); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_21) */
# if defined( kCanPhysToLogChannelIndex_22 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_22()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_22
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_22Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_22 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_22Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_22( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(22)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_22); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_22()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_22
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_22Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_22 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_22Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_22( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(22)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_22); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_22()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_22
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_22Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_22 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_22Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_22Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_22( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(22)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_22); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_22) */
# if defined( kCanPhysToLogChannelIndex_23 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             CanBusOffIsr_23()
****************************************************************************/
/*
|<DataModelStart>| CanBusOffIsr_23
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_23Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanBusOffIsr_23 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_23Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanBusOffIsr_23( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanBusOffInterrupt(Can_GetPhysToLogChannel(23)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanBusOffInterrupt(CanPhysToLogChannel_23); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanBusOffIsr */
/* CODE CATEGORY 1 END */
#    if defined (C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             CanErrorIsr_23()
****************************************************************************/
/*
|<DataModelStart>| CanErrorIsr_23
Relation_Context:
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
CanSecurityEventReporting
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#    if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_23Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
ISR( CanErrorIsr_23 ) 
#    else
#     if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_23Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#       endif
#      endif
#     endif
void CanErrorIsr_23( void ) 
#    endif 
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  CanBusOffInterrupt(Can_GetPhysToLogChannel(23)); /* call Interrupthandling with identity dependend logical channel */
#    else
  CanBusOffInterrupt(CanPhysToLogChannel_23); /* call Interrupthandling with logical channel */
#    endif
} /* END OF CanErrorIsr */
/* CODE CATEGORY 1 END */
#    endif /* C_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE && CAN_ENABLE_SECURITY_EVENT_REPORTING */
#  endif /* C_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             CanMailboxIsr_23()
****************************************************************************/
/*
|<DataModelStart>| CanMailboxIsr_23
Relation_Context:
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
Variant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 1 START */
#   if defined( C_ENABLE_OSEK_OS ) && defined( C_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_23Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#     endif
#    endif
ISR( CanMailboxIsr_23 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_23Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_23Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#      endif
#     endif
#    endif
void CanMailboxIsr_23( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_USE_PHYSTOLOG_MAPPING )
  CanMailboxInterrupt(Can_GetPhysToLogChannel(23)); /* call Interrupthandling with identity dependend logical channel */
#   else
  CanMailboxInterrupt(CanPhysToLogChannel_23); /* call Interrupthandling with logical channel */
#   endif
} /* END OF CanMailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCanPhysToLogChannelIndex_23) */
#endif

#define CAN_STOP_SEC_CODE  /*------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 479 */

/************   Organi, Version 3.9.2 Vector-Informatik GmbH  ************/
