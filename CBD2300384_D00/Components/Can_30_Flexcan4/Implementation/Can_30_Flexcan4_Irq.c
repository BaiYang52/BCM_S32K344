/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* STARTSINGLE_OF_MULTIPLE */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*  \file     Can_30_Flexcan4.c / Can_30_Flexcan4_Irq.c / Can_30_Flexcan4_Local.h
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
| 2019-11-07  1.00.00  Rli   - CAN-790     [FLEXCAN4] Support of new platform
| 2020-01-28  1.01.00  Rli   - CAN-961     [Flexcan4] MSR4 R24 LL - new HW feature (extended Bittiming)
| 2020-04-30  1.01.01  Rli   - CAN-1447    [Flexcan4] S32K2/3 support
| 2020-12-10  1.01.02  Rli   - CAN-1566    [Flexcan4] Flexcan4: Solve review findings from LL Review for 1.01.01 release
| 2021-06-16  1.02.00  Rli   - CANHAL-188  [Drvcan Flexcan4] Support S32R455
|                            - CANHAL-408  [FlexCan4] MSR4 R26 Implementation
| 2021-08-12  1.03.00  Rli   - CANHAL-463  [DrvCan][Flexcan4] KW45 Implementation
|                            - CANHAL-467  [Drvcan][Flexcan4] KW45 Test Execution
| 2021-11-05  1.04.00  Rli   - CANHAL-704  [DrvCan][Flexcan4] Implementation (ASIL Level)
|                            - CANHAL-704  [Drvcan][Flexcan4] Test Execution (ASIL Level)
|                            - ESCAN00110599 Arm32 Flexcan4: CTRL1 register cannot be written / no CAN communication
|************************************************************************** */

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_30_FLEXCAN4_INTERNAL
#define CAN_30_FLEXCAN4_IRQ_SOURCE     /* testability */

#include "Can_30_Flexcan4.h"
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
#if (CAN_30_FLEXCAN4_COREVERSION           != 0x1000u) /* \trace SPEC-1699, SPEC-3837 */
# error "Header file are inconsistent!"
#endif
#if (CAN_30_FLEXCAN4_RELEASE_COREVERSION   != 0x03u)
# error "Header file are inconsistent!"
#endif
#if( DRVCAN_ARM32FLEXCAN4ASR_VERSION != 0x0104u)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN_ARM32FLEXCAN4ASR_RELEASE_VERSION != 0x00u)
# error "Source and Header file are inconsistent!"
#endif

# if( CAN_30_FLEXCAN4_GEN_Arm32Flexcan4Asr_VERSION != 0x0101u)
#  error "Generated Data are inconsistent!"
# endif

#if defined(CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_30_FLEXCAN4_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
V_DEF_30_FLEXCAN4_P2CONST (extern, Can_30_Flexcan4_ConfigType, VAR_INIT, CONST_PBCFG) Can_30_Flexcan4_ConfigDataPtr; /* PRQA S 3447,3449,3451,3210 */ /* MD_Can_ExternalScope */
# define CAN_30_FLEXCAN4_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/***************************************************************************/
/* Interrupt Service Routine                                               */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_CODE  /*-----------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \trace SPEC-1579, SPEC-1395, SPEC-1567 */
#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
# define Can_30_Flexcan4PhysToLogChannel_0
# define Can_30_Flexcan4PhysToLogChannelIndex_0
# define Can_30_Flexcan4PhysToLogChannel_1
# define Can_30_Flexcan4PhysToLogChannelIndex_1
# define Can_30_Flexcan4PhysToLogChannel_2
# define Can_30_Flexcan4PhysToLogChannelIndex_2
# define Can_30_Flexcan4PhysToLogChannel_3
# define Can_30_Flexcan4PhysToLogChannelIndex_3
# define Can_30_Flexcan4PhysToLogChannel_4
# define Can_30_Flexcan4PhysToLogChannelIndex_4
# define Can_30_Flexcan4PhysToLogChannel_5
# define Can_30_Flexcan4PhysToLogChannelIndex_5
# define Can_30_Flexcan4PhysToLogChannel_6
# define Can_30_Flexcan4PhysToLogChannelIndex_6
# define Can_30_Flexcan4PhysToLogChannel_7
# define Can_30_Flexcan4PhysToLogChannelIndex_7
#else
# define Can_30_Flexcan4PhysToLogChannel_0       kCan_30_Flexcan4PhysToLogChannelIndex_0
# define Can_30_Flexcan4PhysToLogChannelIndex_0  kCan_30_Flexcan4PhysToLogChannelIndex_0,
# define Can_30_Flexcan4PhysToLogChannel_1       kCan_30_Flexcan4PhysToLogChannelIndex_1
# define Can_30_Flexcan4PhysToLogChannelIndex_1  kCan_30_Flexcan4PhysToLogChannelIndex_1,
# define Can_30_Flexcan4PhysToLogChannel_2       kCan_30_Flexcan4PhysToLogChannelIndex_2
# define Can_30_Flexcan4PhysToLogChannelIndex_2  kCan_30_Flexcan4PhysToLogChannelIndex_2,
# define Can_30_Flexcan4PhysToLogChannel_3       kCan_30_Flexcan4PhysToLogChannelIndex_3
# define Can_30_Flexcan4PhysToLogChannelIndex_3  kCan_30_Flexcan4PhysToLogChannelIndex_3,
# define Can_30_Flexcan4PhysToLogChannel_4       kCan_30_Flexcan4PhysToLogChannelIndex_4
# define Can_30_Flexcan4PhysToLogChannelIndex_4  kCan_30_Flexcan4PhysToLogChannelIndex_4,
# define Can_30_Flexcan4PhysToLogChannel_5       kCan_30_Flexcan4PhysToLogChannelIndex_5
# define Can_30_Flexcan4PhysToLogChannelIndex_5  kCan_30_Flexcan4PhysToLogChannelIndex_5,
# define Can_30_Flexcan4PhysToLogChannel_6       kCan_30_Flexcan4PhysToLogChannelIndex_6
# define Can_30_Flexcan4PhysToLogChannelIndex_6  kCan_30_Flexcan4PhysToLogChannelIndex_6,
# define Can_30_Flexcan4PhysToLogChannel_7       kCan_30_Flexcan4PhysToLogChannelIndex_7
# define Can_30_Flexcan4PhysToLogChannelIndex_7  kCan_30_Flexcan4PhysToLogChannelIndex_7,
#endif

/* ISR functions */
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
#  define CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING
# endif

#if defined( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_SINGLE */
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
     defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_0()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_0
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_0 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_0( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_0) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_1()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_1
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_1 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_1( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_1) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_2()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_2
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_2 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_2( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_2) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_3()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_3
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_3 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_3( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_3) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_4()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_4
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_4 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_4( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_4) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_5()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_5
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_5 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_5( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_5) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_6()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_6
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_6 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_6( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_6) */
#  if defined( kCan_30_Flexcan4PhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
/****************************************************************************
| NAME:             Can_30_Flexcan4Isr_7()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4Isr_7
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4Isr_7 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat)/* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4Isr_7( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4Interrupt(Can_30_Flexcan4PhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4Isr */
/* CODE CATEGORY 1 END */
#  endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_7) */
# endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT */
#endif /* C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE */

#if defined( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_0()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_0
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_0 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_0( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_0()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_0
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_0 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_0Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_0( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_0); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_0()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_0
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_0Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_0 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_0Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_0Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_0( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(0)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_0); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_0) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_1()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_1
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_1 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_1( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_1()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_1
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_1 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_1Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_1( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_1); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_1()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_1
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_1Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_1 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_1Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_1Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_1( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(1)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_1); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_1) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_2()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_2
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_2 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_2( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_2()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_2
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_2 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_2Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_2( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_2); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_2()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_2
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_2Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_2 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_2Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_2Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_2( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(2)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_2); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_2) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_3()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_3
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_3 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_3( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_3()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_3
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_3 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_3Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_3( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_3); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_3()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_3
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_3Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_3 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_3Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_3Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_3( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(3)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_3); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_3) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_4()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_4
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_4 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_4( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_4()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_4
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_4 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_4Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_4( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_4); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_4()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_4
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_4Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_4 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_4Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_4Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_4( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(4)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_4); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_4) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_5()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_5
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_5 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_5( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_5()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_5
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_5 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_5Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_5( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_5); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_5()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_5
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_5Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_5 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_5Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_5Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_5( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(5)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_5); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_5) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_6()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_6
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_6 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_6( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_6()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_6
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_6 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_6Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_6( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_6); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_6()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_6
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_6Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_6 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_6Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_6Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_6( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(6)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_6); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_6) */
# if defined( kCan_30_Flexcan4PhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffIsr_7()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffIsr_7
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4BusOffIsr_7 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4BusOffIsr_7( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */

#    if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/****************************************************************************
| NAME:             Can_30_Flexcan4ErrorIsr_7()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4ErrorIsr_7
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
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 2u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
ISR( Can_30_Flexcan4ErrorIsr_7 ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    else
#     if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#      if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#       if (osdIsrCanIsr_7Cat != 1u)
#        error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#       endif
#      endif
#     endif
void Can_30_Flexcan4ErrorIsr_7( void ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#    endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#    if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING ) /* COV_CAN_PHYSTOLOG_MAPPING */
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#    else
  Can_30_Flexcan4BusOffInterrupt(Can_30_Flexcan4PhysToLogChannel_7); /* call Interrupthandling with logical channel */
#    endif
} /* END OF Can_30_Flexcan4BusOffIsr */
/* CODE CATEGORY 1 END */
#    endif
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT */

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxIsr_7()
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxIsr_7
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
#   if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) && defined( C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2 ) /* COV_CAN_OS_USAGE */
#    if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#     if (osdIsrCanIsr_7Cat != 2u)
#      error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#     endif
#    endif
ISR( Can_30_Flexcan4MailboxIsr_7 ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   else
#    if defined( C_30_FLEXCAN4_ENABLE_OSEK_OS ) /* COV_CAN_OS_USAGE */
#     if defined (osdIsrCanIsr_7Cat) /* COV_CAN_LL_OSCAT_CONFIG_CHECK XF */
#      if (osdIsrCanIsr_7Cat != 1u)
#       error "inconsistent configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (Can_30_Flexcan4Isr)"
#      endif
#     endif
#    endif
void Can_30_Flexcan4MailboxIsr_7( void ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
#   endif /* C_30_FLEXCAN4_ENABLE_OSEK_OS */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 call interrupt handler */
#   if defined( CAN_30_FLEXCAN4_USE_PHYSTOLOG_MAPPING )
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4_GetPhysToLogChannel(7)); /* call Interrupthandling with identity dependend logical channel */
#   else
  Can_30_Flexcan4MailboxInterrupt(Can_30_Flexcan4PhysToLogChannel_7); /* call Interrupthandling with logical channel */
#   endif
} /* END OF Can_30_Flexcan4MailboxIsr */
/* CODE CATEGORY 1 END */
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */
# endif /* (kCan_30_Flexcan4PhysToLogChannelIndex_7) */
#endif

#define CAN_30_FLEXCAN4_STOP_SEC_CODE  /*------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/************   Organi, Version 3.9.2 Vector-Informatik GmbH  ************/
