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

#if !defined(CAN_30_FLEXCAN4_LOCAL_H)
#define  CAN_30_FLEXCAN4_LOCAL_H

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/
/* \trace SPEC-1583 */
#include "SchM_Can_30_Flexcan4.h"
#include "Can_30_Flexcan4.h"         /* include also all needed types */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_30_FLEXCAN4_COREVERSION           != 0x1000u) /* \trace SPEC-1699 */
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

/* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */

/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/
#if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID) || defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
# define CAN_30_FLEXCAN4_ID_IDE_MASK            0x80000000UL
# define CAN_30_FLEXCAN4_ID_FD_MASK             0x40000000UL /* \trace SPEC-50553 */
# define CAN_30_FLEXCAN4_ID_MASK                0x1FFFFFFFUL
# define CAN_30_FLEXCAN4_ID_MASK_STD            0x000007FFUL
# define CAN_30_FLEXCAN4_ID_UNUSED_MASK         0x20000000UL
#else
# define CAN_30_FLEXCAN4_ID_IDE_MASK            0x0000U
# define CAN_30_FLEXCAN4_ID_FD_MASK             0x4000U /* \trace SPEC-50553 */
# define CAN_30_FLEXCAN4_ID_MASK                0x07FFU
# define CAN_30_FLEXCAN4_ID_MASK_STD            0x07FFU
# define CAN_30_FLEXCAN4_ID_UNUSED_MASK         0x3800U
#endif
#define kCan_30_Flexcan4AllChannels  ((Can_30_Flexcan4ChannelHandle)0xFFU)

#if !defined(C_DRV_30_FLEXCAN4_INTERNAL) /* COV_CAN_INTERNAL */
# error "Can_30_Flexcan4_Local.h should only be included by Can.c"
#endif
/* real amount of controllers in this system (LT and PB) */
#define kCan_30_Flexcan4NumberOfChannels  Can_30_Flexcan4_GetControllerMax()
/* if kCan_30_Flexcan4NumberOfChannels used to declare a table/dimension this table has to be generated to be also Link-Time-capable */
/*  -> see ESCAN00028069 controller-amount has to be Link-Time-capable */
/* real amount of controllers in this system (LT and PB) */
#define kCan_30_Flexcan4NumberOfHwChannels  kCan_30_Flexcan4NumberOfChannels

#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
/* have to be done here to avoid declaration of "channel" in Can.h */
# define channel                                ((Can_30_Flexcan4ChannelHandle)0)
# define canHwChannel                           ((Can_30_Flexcan4ChannelHandle)0)
# define CAN_30_FLEXCAN4_HL_HW_CHANNEL_STARTINDEX(ch)       ((Can_30_Flexcan4ChannelHandle)0)
# define CAN_30_FLEXCAN4_HL_HW_CHANNEL_STOPINDEX(ch)        ((Can_30_Flexcan4ChannelHandle)0)
#else
# define channel                                Controller
# define canHwChannel                           Controller
# define CAN_30_FLEXCAN4_HL_HW_CHANNEL_STARTINDEX(ch)       (ch)
# define CAN_30_FLEXCAN4_HL_HW_CHANNEL_STOPINDEX(ch)        (ch)
#endif
#if !defined(CAN_30_FLEXCAN4_TRANSITION_CHECK) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_TRANSITION_CHECK STD_ON /* default may be deactivated by user configuration */
#endif
#if !defined(CAN_30_FLEXCAN4_REINIT_START) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_REINIT_START STD_OFF
#endif

#if !defined(CAN_30_FLEXCAN4_OS_TICK2MS) /* COV_CAN_COMPATIBILITY */
# error "CAN_30_FLEXCAN4_OS_TICK2MS() should be defined to OS_TICKS2MS_<counterShortName>() macro from OS"
#endif
#if !defined(CAN_30_FLEXCAN4_OS_COUNTER_ID) /* COV_CAN_COMPATIBILITY */
# error "CAN_30_FLEXCAN4_OS_COUNTER_ID should be defined to 'counterShortName' from OS"
#endif

#if !defined(CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT (CAN_30_FLEXCAN4_ID_IDE_MASK | CAN_30_FLEXCAN4_ID_FD_MASK | CAN_30_FLEXCAN4_ID_MASK)
/* May be changed to mask out FD-bit: (CAN_30_FLEXCAN4_ID_IDE_MASK | CAN_30_FLEXCAN4_ID_MASK) */
#endif

#if !defined( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT ) && !defined( C_30_FLEXCAN4_DISABLE_BUSOFF_RECOVERY_COMPLIANT )
# define C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT
#endif

# if ( CAN_30_FLEXCAN4_PROTECTED_MODE == STD_ON )
#  define C_30_FLEXCAN4_ENABLE_USER_MODE_OS
# endif

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
/* for variable Can_ActiveSendState */
#define kCan_30_Flexcan4BufferFree              (uint8)0xFF     /* mark objects as free */
#define kCan_30_Flexcan4BufferCancel            (uint8)0xFE     /* mark object as currently cancelling */
#define kCan_30_Flexcan4BufferCancelSw          (uint8)0xFD
#define kCan_30_Flexcan4BufferSend              (uint8)0x01     /* occupied send object */

/* HL return values */
#define kCan_30_Flexcan4Failed                  (uint8)0x00     /* similar to CAN_NOT_OK but value may differ --> converted in HL (LL only use kCan...) */
#define kCan_30_Flexcan4Ok                      (uint8)0x01     /* similar to CAN_OK */
#define kCan_30_Flexcan4Requested               (uint8)0x03

#define kCan_30_Flexcan4False                   kCan_30_Flexcan4Failed
#define kCan_30_Flexcan4True                    kCan_30_Flexcan4Ok

#define Can_30_Flexcan4_GetControllerMax()                        Can_30_Flexcan4_GetSizeOfControllerData()
#define Can_30_Flexcan4_GetCanToCanIfChannelMapping(Index)        Can_30_Flexcan4_GetCanIfChannelId((Index))

/* ControllerConfig */
#define Can_30_Flexcan4_GetCanControllerDefaultBaudrate(logCh)    Can_30_Flexcan4_GetCanControllerDefaultBaudrateOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetCanControllerDefaultBaudrateIdx(logCh) Can_30_Flexcan4_GetCanControllerDefaultBaudrateIdxOfControllerConfig(logCh)
#if !defined(Can_30_Flexcan4_GetMailboxRxBasicLengthOfControllerConfig) /* If no Object is available ComStackLib do not generate start-stop index */
# define Can_30_Flexcan4_GetMailboxRxBasicLengthOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxRxBasicStartIdxOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxRxBasicEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_30_Flexcan4_GetRxBasicHandleMax(logCh)                Can_30_Flexcan4_GetMailboxRxBasicLengthOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxBasicHandleStart(logCh)              (Can_HwHandleType)Can_30_Flexcan4_GetMailboxRxBasicStartIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxBasicHandleStop(logCh)               (Can_HwHandleType)Can_30_Flexcan4_GetMailboxRxBasicEndIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxBasicHwStart(logCh)                  Can_30_Flexcan4_GetRxBasicHwStartOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxBasicHwStop(logCh)                   Can_30_Flexcan4_GetRxBasicHwStopOfControllerConfig(logCh)
#if !defined(Can_30_Flexcan4_GetMailboxRxFullLengthOfControllerConfig)
# define Can_30_Flexcan4_GetMailboxRxFullLengthOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxRxFullStartIdxOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxRxFullEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_30_Flexcan4_GetRxFullHandleMax(logCh)                 Can_30_Flexcan4_GetMailboxRxFullLengthOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxFullHandleStart(logCh)               (Can_HwHandleType)Can_30_Flexcan4_GetMailboxRxFullStartIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxFullHandleStop(logCh)                (Can_HwHandleType)Can_30_Flexcan4_GetMailboxRxFullEndIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxFullHwStart(logCh)                   Can_30_Flexcan4_GetRxFullHwStartOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetRxFullHwStop(logCh)                    Can_30_Flexcan4_GetRxFullHwStopOfControllerConfig(logCh)
#if !defined(Can_30_Flexcan4_GetMailboxTxBasicLengthOfControllerConfig) /* COV_CAN_GENDATA_FAILURE */
# define Can_30_Flexcan4_GetMailboxTxBasicLengthOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxTxBasicStartIdxOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxTxBasicEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_30_Flexcan4_GetTxBasicHandleMax(logCh)                Can_30_Flexcan4_GetMailboxTxBasicLengthOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxBasicHandleStart(logCh)              (Can_HwHandleType)Can_30_Flexcan4_GetMailboxTxBasicStartIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxBasicHandleStop(logCh)               (Can_HwHandleType)Can_30_Flexcan4_GetMailboxTxBasicEndIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxBasicHwStart(logCh)                  Can_30_Flexcan4_GetTxBasicHwStartOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxBasicHwStop(logCh)                   Can_30_Flexcan4_GetTxBasicHwStopOfControllerConfig(logCh)
#if !defined(Can_30_Flexcan4_GetMailboxTxFullLengthOfControllerConfig)
# define Can_30_Flexcan4_GetMailboxTxFullLengthOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxTxFullStartIdxOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxTxFullEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_30_Flexcan4_GetTxFullHandleMax(logCh)                 Can_30_Flexcan4_GetMailboxTxFullLengthOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxFullHandleStart(logCh)               (Can_HwHandleType)Can_30_Flexcan4_GetMailboxTxFullStartIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxFullHandleStop(logCh)                (Can_HwHandleType)Can_30_Flexcan4_GetMailboxTxFullEndIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxFullHwStart(logCh)                   Can_30_Flexcan4_GetTxFullHwStartOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetTxFullHwStop(logCh)                    Can_30_Flexcan4_GetTxFullHwStopOfControllerConfig(logCh)
#if !defined(Can_30_Flexcan4_GetMailboxUnusedLengthOfControllerConfig)  /* COV_CAN_GENDATA_FAILURE */
# define Can_30_Flexcan4_GetMailboxUnusedLengthOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxUnusedStartIdxOfControllerConfig(logCh) 0u
# define Can_30_Flexcan4_GetMailboxUnusedEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_30_Flexcan4_GetUnusedHandleMax(logCh)                 Can_30_Flexcan4_GetMailboxUnusedLengthOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetUnusedHandleStart(logCh)               (Can_HwHandleType)Can_30_Flexcan4_GetMailboxUnusedStartIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetUnusedHandleStop(logCh)                (Can_HwHandleType)Can_30_Flexcan4_GetMailboxUnusedEndIdxOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetUnusedHwStart(logCh)                   Can_30_Flexcan4_GetUnusedHwStartOfControllerConfig(logCh)
#define Can_30_Flexcan4_GetUnusedHwStop(logCh)                    Can_30_Flexcan4_GetUnusedHwStopOfControllerConfig(logCh)

/* Mailbox */
/* GetMailboxSize: logical size of mailbox - use LL specific generated data to get hardware object size if needed */
#define Can_30_Flexcan4_GetMailboxSize(htrh)                      Can_30_Flexcan4_GetMailboxSizeOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxHwHandle(htrh)                  Can_30_Flexcan4_GetHwHandleOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxIDValue(htrh)                   Can_30_Flexcan4_GetIDValueOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxType(htrh)                      Can_30_Flexcan4_GetMailboxTypeOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxDataLen(htrh)                   Can_30_Flexcan4_GetMaxDataLenOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxFdPadding(htrh)                 Can_30_Flexcan4_GetFdPaddingOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxController(htrh)                (Can_30_Flexcan4ChannelHandle)Can_30_Flexcan4_GetControllerConfigIdxOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxActiveSendObject(htrh)          Can_30_Flexcan4_GetActiveSendObjectOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxActiveSendData(htrh)            Can_30_Flexcan4_GetActiveSendDataOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxMemSecObjIndex(htrh)            Can_30_Flexcan4_GetMemorySectionsIndexOfMailbox(htrh)
#define Can_30_Flexcan4_IsMailboxIndivPolling(htrh)               Can_30_Flexcan4_IsIsIndivPollingOfMailbox(htrh)
#define Can_30_Flexcan4_GetMailboxTxHwFifo(htrh)                  Can_30_Flexcan4_GetTxHwFifoOfMailbox(htrh)

/* MemorySectionInfo */
#define Can_30_Flexcan4_GetMemSecAddress(memSec)                  Can_30_Flexcan4_GetMemoryStartAddressOfMemorySectionInfo(memSec)
#define Can_30_Flexcan4_GetMemSecStartIndex(memSec)               Can_30_Flexcan4_GetMemorySectionStartOfMemorySectionInfo(memSec)

/* MemorySectionObjects */
#define Can_30_Flexcan4_GetMemSecObjHwHandle(memSecObj)           Can_30_Flexcan4_GetHwHandleOfMemorySectionObjects(memSecObj)
#define Can_30_Flexcan4_GetMemSecObjMailboxElement(memSecObj)     Can_30_Flexcan4_GetMailboxElementOfMemorySectionObjects(memSecObj)
#define Can_30_Flexcan4_GetMemSecObjMailboxHandle(memSecObj)      Can_30_Flexcan4_GetMailboxHandleOfMemorySectionObjects(memSecObj)
#define Can_30_Flexcan4_IsMemSecObjIndivPolling(memSecObj)        Can_30_Flexcan4_IsIsIndivPollingOfMemorySectionObjects(memSecObj)

/* VAR */
#define Can_30_Flexcan4_GetLastInitObject(Index)                   Can_30_Flexcan4_GetLastInitObjectOfControllerData(Index)
#define Can_30_Flexcan4_SetLastInitObject(Index, Value)            Can_30_Flexcan4_SetLastInitObjectOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetCanInterruptOldStatus(Index)            Can_30_Flexcan4_GetCanInterruptOldStatusOfControllerData(Index)
#define Can_30_Flexcan4_SetCanInterruptOldStatus(Index, Value)     Can_30_Flexcan4_SetCanInterruptOldStatusOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetCanInterruptCounter(Index)              Can_30_Flexcan4_GetCanInterruptCounterOfControllerData(Index)
#define Can_30_Flexcan4_SetCanInterruptCounter(Index, Value)       Can_30_Flexcan4_SetCanInterruptCounterOfControllerData(Index, Value)
#define Can_30_Flexcan4_IncCanInterruptCounter(Index)              Can_30_Flexcan4_IncCanInterruptCounterOfControllerData(Index)
#define Can_30_Flexcan4_DecCanInterruptCounter(Index)              Can_30_Flexcan4_DecCanInterruptCounterOfControllerData(Index)
#define Can_30_Flexcan4_IsIsWakeup(Index)                          Can_30_Flexcan4_IsIsWakeupOfControllerData(Index)
#define Can_30_Flexcan4_SetIsWakeup(Index, Value)                  Can_30_Flexcan4_SetIsWakeupOfControllerData(Index, Value)
#define Can_30_Flexcan4_IsIsBusOff(Index)                          Can_30_Flexcan4_IsIsBusOffOfControllerData(Index)
#define Can_30_Flexcan4_SetIsBusOff(Index, Value)                  Can_30_Flexcan4_SetIsBusOffOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetBusOffCounter(Index)                    Can_30_Flexcan4_GetBusOffCounterOfControllerData(Index)
#define Can_30_Flexcan4_SetBusOffCounter(Index, Value)             Can_30_Flexcan4_SetBusOffCounterOfControllerData(Index, Value)
#define Can_30_Flexcan4_DecBusOffCounter(Index)                    Can_30_Flexcan4_DecBusOffCounterOfControllerData(Index)
#define Can_30_Flexcan4_GetModeTransitionRequest(Index)            Can_30_Flexcan4_GetModeTransitionRequestOfControllerData(Index)
#define Can_30_Flexcan4_SetModeTransitionRequest(Index, Value)     Can_30_Flexcan4_SetModeTransitionRequestOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetBusOffTransitionRequest(Index)          Can_30_Flexcan4_GetBusOffTransitionRequestOfControllerData(Index)
#define Can_30_Flexcan4_SetBusOffTransitionRequest(Index, Value)   Can_30_Flexcan4_SetBusOffTransitionRequestOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetRamCheckTransitionRequest(Index)        Can_30_Flexcan4_GetRamCheckTransitionRequestOfControllerData(Index)
#define Can_30_Flexcan4_SetRamCheckTransitionRequest(Index, Value) Can_30_Flexcan4_SetRamCheckTransitionRequestOfControllerData(Index, Value)
/* controller states */
#define Can_30_Flexcan4_GetLogStatus(Index)                        Can_30_Flexcan4_GetLogStatusOfControllerData(Index)
#define Can_30_Flexcan4_SetLogStatus(Index, Value)                 Can_30_Flexcan4_SetLogStatusOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetCanLoopTimeout(ch, Index)               (Can_30_Flexcan4_GetLoopTimeoutOfControllerData(ch))[(Index)]
#define Can_30_Flexcan4_GetMirrorModeState(Index)                  Can_30_Flexcan4_GetMirrorModeStateOfControllerData(Index)
#define Can_30_Flexcan4_SetMirrorModeState(Index, Value)           Can_30_Flexcan4_SetMirrorModeStateOfControllerData(Index, Value)
#define Can_30_Flexcan4_GetSilentModeState(Index)                  Can_30_Flexcan4_GetSilentModeStateOfControllerData(Index)
#define Can_30_Flexcan4_SetSilentModeState(Index, Value)           Can_30_Flexcan4_SetSilentModeStateOfControllerData(Index, Value)
/* ActiveSend */
#define Can_30_Flexcan4_GetActiveSendPdu(Index)                    Can_30_Flexcan4_GetPduOfActiveSendObject(Index)
#define Can_30_Flexcan4_SetActiveSendPdu(Index, Value)             Can_30_Flexcan4_SetPduOfActiveSendObject(Index, Value)
#define Can_30_Flexcan4_GetActiveSendState(Index)                  Can_30_Flexcan4_GetStateOfActiveSendObject(Index)
#define Can_30_Flexcan4_SetActiveSendState(Index, Value)           Can_30_Flexcan4_SetStateOfActiveSendObject(Index, Value)
#define Can_30_Flexcan4_GetActiveSendPduInfo(Index)                Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)
#define Can_30_Flexcan4_GetActiveSendId(Index)                     (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).id
#define Can_30_Flexcan4_SetActiveSendId(Index, Value)              (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).id = (Value)
#define Can_30_Flexcan4_GetActiveSendLength(Index)                 (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).length
#define Can_30_Flexcan4_SetActiveSendLength(Index, Value)          (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).length = (Value)
#define Can_30_Flexcan4_GetActiveSendSdu(Index)                    (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).sdu
#define Can_30_Flexcan4_SetActiveSendSdu(Index, Value)             (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).sdu = (Value)
#define Can_30_Flexcan4_GetActiveSendSwPduHandle(Index)            (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).swPduHandle
#define Can_30_Flexcan4_SetActiveSendSwPduHandle(Index, Value)     (Can_30_Flexcan4_GetPduInfoOfActiveSendObject(Index)).swPduHandle = (Value)
/* Use already ComStackLib name #define Can_30_Flexcan4_GetActiveSendData(Index)                   Can_30_Flexcan4_GetActiveSendData(Index) */
/* TxHwFifo */
#define Can_30_Flexcan4_GetTxHwFifoWriteIndex(fifo)                Can_30_Flexcan4_GetWriteIndexOfTxHwFifo(fifo)
#define Can_30_Flexcan4_SetTxHwFifoWriteIndex(fifo, Value)         Can_30_Flexcan4_SetWriteIndexOfTxHwFifo(fifo, Value)
#define Can_30_Flexcan4_IncTxHwFifoWriteIndex(fifo)                Can_30_Flexcan4_IncWriteIndexOfTxHwFifo(fifo)
#define Can_30_Flexcan4_GetTxHwFifoReadIndex(fifo)                 Can_30_Flexcan4_GetReadIndexOfTxHwFifo(fifo)
#define Can_30_Flexcan4_SetTxHwFifoReadIndex(fifo, Value)          Can_30_Flexcan4_SetReadIndexOfTxHwFifo(fifo, Value)
#define Can_30_Flexcan4_IncTxHwFifoReadIndex(fifo)                 Can_30_Flexcan4_IncReadIndexOfTxHwFifo(fifo)
#define Can_30_Flexcan4_GetTxHwFifoFillCount(fifo)                 Can_30_Flexcan4_GetFillCountOfTxHwFifo(fifo)
#define Can_30_Flexcan4_SetTxHwFifoFillCount(fifo, Value)          Can_30_Flexcan4_SetFillCountOfTxHwFifo(fifo, Value)
#define Can_30_Flexcan4_IncTxHwFifoFillCount(fifo)                 Can_30_Flexcan4_IncFillCountOfTxHwFifo(fifo)
#define Can_30_Flexcan4_DecTxHwFifoFillCount(fifo)                 Can_30_Flexcan4_DecFillCountOfTxHwFifo(fifo)

# define Can_30_Flexcan4_GetFDHwChannelIndirection(Index)                              Can_30_Flexcan4_GetFDHwChannelIndirectionOfControllerConfig(Index)
# define Can_30_Flexcan4_GetHwToMbHandle(logCh, Index)                                 Can_30_Flexcan4_GetMemSecObjMailboxHandle(Can_30_Flexcan4_GetMemSecStartIndex((logCh) << 1) + (Index))
# define Can_30_Flexcan4_GetHwToERxFifoHandle(logCh, Index)                            Can_30_Flexcan4_GetMemSecObjMailboxHandle(Can_30_Flexcan4_GetMemSecStartIndex(((logCh) << 1) + 1u) + (Index))
# define CAN_30_FLEXCAN4_HL_MB_TX_NORMAL_INDEX                                         CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX


/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
/***************************************************************************/
/*  Safe Check for FlexCAN4                                               */
/***************************************************************************/
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
#  if defined (C_30_FLEXCAN4_ENABLE_WAKEUP_INTERRUPT) /* COV_CAN_SAFE_BSW_EXCLUSION_FLEXCAN4 */
#    error "Unsupported Feature activated for SafeBSW"
#  endif
# endif

#define C_30_FLEXCAN4_HL_DISABLE_OVERRUN_IN_STATUS

#define C_30_FLEXCAN4_HL_DISABLE_HW_RANGES_FILTER

#define C_30_FLEXCAN4_HL_ENABLE_IDTYPE_IN_DLC




#define C_30_FLEXCAN4_HL_DISABLE_DUMMY_FCT_CALL


#define C_30_FLEXCAN4_HL_DISABLE_TX_MSG_DESTROYED


#define C_30_FLEXCAN4_HL_DISABLE_CANCEL_IN_HW_TASK

#define C_30_FLEXCAN4_HL_DISABLE_HW_EXIT_TRANSMIT

#define C_30_FLEXCAN4_HL_DISABLE_ADJUST_RXHANDLE


#define C_30_FLEXCAN4_HL_DISABLE_REJECT_UNWANTED_IDTYPE

#define C_30_FLEXCAN4_HL_DISABLE_REJECT_REMOTE_FRAME

#define C_30_FLEXCAN4_HL_DISABLE_REJECT_REMOTE_FRAME_FULLCAN

#define C_30_FLEXCAN4_HL_ENABLE_REJECT_ILLEGAL_DLC

#define C_30_FLEXCAN4_HL_DISABLE_COPROCESSOR_SUPPORT



#define C_30_FLEXCAN4_HL_DISABLE_AVOID_REENTRANT_APPLCANCANCELNOTIFICATION

/* Possibility to disable the prototypes of interrupt service routines in the driver header file */

#define C_30_FLEXCAN4_DISABLE_TX_ACTUAL_MACROS

#define CAN_30_FLEXCAN4_HL_HW_LAYOUT_TXBASIC_FIRST


#if !defined( C_30_FLEXCAN4_ENABLE_CLKSRC_SELECTION ) && !defined( C_30_FLEXCAN4_DISABLE_CLKSRC_SELECTION ) /* COV_CAN_RAMCHECK_EXTENDED_CLKSRC_COMPATABILITY */
# define C_30_FLEXCAN4_DISABLE_CLKSRC_SELECTION
#endif

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
#define txErr_30_Flexcan4CMask      (vuint32)0x000000FFu
#define rxErr_30_Flexcan4CMask      (vuint32)0x0000FF00u
#define txErr_30_Flexcan4CFastMask  (vuint32)0x00FF0000u
#define rxErr_30_Flexcan4CFastMask  (vuint32)0xFF000000u



/* macros to set DLC and ID in RX-,TX-structures -------------------------- */
/* MK... macros are used by the generation tool to fill the RX and TX structures */
/* In case the IDE bit is located in the ID registers, this bit has to be handled as must match.
   In general, this is done by setting the IDE bit to 0 in the .._MASK_.. macros.
*/

#define MK_RX_RANGE_MASK_IDSTD0(id)   ( MK_STDID0(id) )
#define MK_RX_RANGE_MASK_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_MASK_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_MASK_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_MASK_IDSTD4(id)   ( MK_STDID4(id) )

#define MK_RX_RANGE_CODE_IDSTD0(id)   ( MK_STDID0(id) )
#define MK_RX_RANGE_CODE_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_CODE_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_CODE_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_CODE_IDSTD4(id)   ( MK_STDID4(id) )

#define MK_RX_RANGE_MASK_IDEXT0(id)   ( MK_EXTID0(id))
#define MK_RX_RANGE_MASK_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_MASK_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_MASK_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_MASK_IDEXT4(id)   ( MK_EXTID4(id) )

#define MK_RX_RANGE_CODE_IDEXT0(id)   ( MK_EXTID0(id))
#define MK_RX_RANGE_CODE_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_CODE_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_CODE_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_CODE_IDEXT4(id)   ( MK_EXTID4(id) )


#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID ) 
# define MK_STDID0(id)           (((id) & 0x07FFUL) << 18)
#else
# define MK_STDID0(id)           (((id) & 0x07FFu) << 2)
#endif
#define MK_STDID1(id)            (id)
#define MK_STDID2(id)            (id)
#define MK_STDID3(id)            (id)
#define MK_STDID4(id)            (id)



#define MK_EXTID0(id)            (((vuint32)(id)) & 0x1FFFFFFFUL)
#define MK_EXTID1(id)            ((id))
#define MK_EXTID2(id)            ((id))
#define MK_EXTID3(id)            ((id))
#define MK_EXTID4(id)            ((id))




# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
#  if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
#   define MK_TX_DLC(dlc)     ((tCan_30_Flexcan4Dlc)((vuint8)(dlc) & (vuint8)0x0Fu))
#  else
#   define  MK_TX_DLC(dlc)    ((tCan_30_Flexcan4Dlc)(((vuint8)(dlc) & (vuint8)0x0Fu) | (vuint8)kCan_30_Flexcan4IdTypeExt | (vuint8)kCan_30_Flexcan4IdTypeSRR))
#  endif
# else
#  define MK_TX_DLC(dlc)      ((tCan_30_Flexcan4Dlc)((vuint8)(dlc) & (vuint8)0x0Fu))
# endif

/* extract  DLC from register format to logical format */
#define XT_TX_DLC(dlc)       ((vuint8)((vuint8)(dlc) & (vuint8)0x0Fu))


/* The macros Can_30_Flexcan4RxActualIdRawx() have to provide the register context of the ID registers.
   It is not necessary to mask any bits. These macros are for internal use only and masking
   will be done in can_hls.c
*/
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define Can_30_Flexcan4RxActualIdRaw0(rxStruct)      ((vuint32)(((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID))
# define Can_30_Flexcan4RxActualIdRaw1(rxStruct)   0
# define Can_30_Flexcan4RxActualIdRaw2(rxStruct)   0
# define Can_30_Flexcan4RxActualIdRaw3(rxStruct)   0
# define Can_30_Flexcan4RxActualIdRaw4(rxStruct)   0
#else
# define Can_30_Flexcan4RxActualIdRaw0(rxStruct)      ((vuint16)((((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID) >> 16) & (vuint16)0x1FFFu)
# define Can_30_Flexcan4RxActualIdRaw1(rxStruct)    0
# define Can_30_Flexcan4RxActualIdRaw2(rxStruct)    0
# define Can_30_Flexcan4RxActualIdRaw3(rxStruct)    0
# define Can_30_Flexcan4RxActualIdRaw4(rxStruct)    0
#endif


/* Macros for CAN message access within Appl_30_Flexcan4CanMsgReceived() or PreCopy() function */
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define Can_30_Flexcan4RxActualIdExtHi(rxStruct)        ((vuint8)((((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID) >> 24) & (vuint8)0x1Fu)
# define Can_30_Flexcan4RxActualIdExtMidHi(rxStruct)     ((vuint8)((((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID) >> 16) & (vuint8)0xFFu)
# define Can_30_Flexcan4RxActualIdExtMidLo(rxStruct)     ((vuint8)((((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID) >> 8) & (vuint8)0xFFu)
# define Can_30_Flexcan4RxActualIdExtLo(rxStruct)        ((vuint8)(((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->msgID) & (vuint8)0xFFu)
#endif

#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID ) 
# define kFlexCAN_30_Flexcan4_XT_ID(id)         ((vuint16)(((vuint32)(id) & (vuint32)0x1FFC0000u) >> 18))
#else
# define kFlexCAN_30_Flexcan4_XT_ID(id)         ((vuint16)(((vuint16)(id) & 0x1FFCu) >> 2))
#endif
#define kFlexCAN_30_Flexcan4_XT_EXTID(id)       ((vuint32)((vuint32)(id) & (vuint32)0x1FFFFFFFu))

#define Can_30_Flexcan4RxActualExtId(rxStruct)        (kFlexCAN_30_Flexcan4_XT_EXTID(Can_30_Flexcan4RxActualIdRaw0(rxStruct)))
#define Can_30_Flexcan4RxActualStdId(rxStruct)        (kFlexCAN_30_Flexcan4_XT_ID(Can_30_Flexcan4RxActualIdRaw0(rxStruct)))
#define Can_30_Flexcan4RxActualData(rxStruct, i)      ((vuint8)((rxStruct)->pChipData[(i)]))
#define Can_30_Flexcan4RxActualDLC(rxStruct)          CAN_30_FLEXCAN4_DLC2LEN((vuint8)(((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->control) & (vuint8)0x0F)
#define Can_30_Flexcan4RxActualIdType(rxStruct)       ((vuint8)(((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->control) & (vuint8)kCan_30_Flexcan4IdTypeExt)
#define Can_30_Flexcan4RxActualFdType(rxStruct)       (((tCan_30_Flexcan4RxMsgBufferPtr)((rxStruct)->pChipMsgObj))->control & kCan_30_Flexcan4FdTypeFd)

/* support reading the error counters */
#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
# if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
#  define Can_30_Flexcan4RxActualErrorCounter()   ((vuint8)(((((tFlex_30_Flexcan4CANPtr)canLL_30_Flexcan4_VirtualPtrField[0])->ecr) & rxErr_30_Flexcan4CMask) >> 8))
#  define Can_30_Flexcan4TxActualErrorCounter()   ((vuint8)((((tFlex_30_Flexcan4CANPtr)canLL_30_Flexcan4_VirtualPtrField[0])->ecr) & txErr_30_Flexcan4CMask))
# else
#  define Can_30_Flexcan4RxActualErrorCounter()   ((vuint8)(((pFlex_30_Flexcan4CAN(0)->ecr) & rxErr_30_Flexcan4CMask) >> 8))
#  define Can_30_Flexcan4TxActualErrorCounter()   ((vuint8)((pFlex_30_Flexcan4CAN(0)->ecr) & txErr_30_Flexcan4CMask))
# endif
#else
# if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
#  define Can_30_Flexcan4RxActualErrorCounter(x)  ((vuint8)(((((tFlex_30_Flexcan4CANPtr)canLL_30_Flexcan4_VirtualPtrField[(x)])->ecr) & rxErr_30_Flexcan4CMask) >> 8))
#  define Can_30_Flexcan4TxActualErrorCounter(x)  ((vuint8)((((tFlex_30_Flexcan4CANPtr)canLL_30_Flexcan4_VirtualPtrField[(x)])->ecr) & txErr_30_Flexcan4CMask))
# else
#  define Can_30_Flexcan4RxActualErrorCounter(x)  ((vuint8)(((pFlex_30_Flexcan4CAN(x)->ecr) & rxErr_30_Flexcan4CMask) >> 8))
#  define Can_30_Flexcan4TxActualErrorCounter(x)  ((vuint8)((pFlex_30_Flexcan4CAN(x)->ecr) & txErr_30_Flexcan4CMask))
# endif
#endif

/* Macros for CAN message access within GenericConfirmation() function */
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define Can_30_Flexcan4TxActualIdRaw0(txStruct)      ((vuint32)(((tCan_30_Flexcan4MsgObjPtr)((txStruct)->pChipMsgObj))->msgID))
#else
# define Can_30_Flexcan4TxActualIdRaw0(txStruct)      ((vuint16)((((tCan_30_Flexcan4MsgObjPtr)((txStruct)->pChipMsgObj))->msgID) >> 16) & (vuint16)0x1FFFu)
#endif /* C_30_FLEXCAN4_ENABLE_EXTENDED_ID */

#define Can_30_Flexcan4TxActualStdId(txStruct)        (kFlexCAN_30_Flexcan4_XT_ID(Can_30_Flexcan4TxActualIdRaw0(txStruct)))
#define Can_30_Flexcan4TxActualExtId(txStruct)        (kFlexCAN_30_Flexcan4_XT_EXTID(Can_30_Flexcan4TxActualIdRaw0(txStruct)))
#define Can_30_Flexcan4TxActualDLC(txStruct)          CAN_30_FLEXCAN4_DLC2LEN((vuint8)(((tCan_30_Flexcan4MsgObjPtr)((txStruct)->pChipMsgObj))->control) & (vuint8)0x0Fu)
#define Can_30_Flexcan4TxActualIdType(txStruct)       ((vuint8)(((tCan_30_Flexcan4MsgObjPtr)((txStruct)->pChipMsgObj))->control) & (vuint8)kCan_30_Flexcan4IdTypeExt)
#define Can_30_Flexcan4TxActualData(txStruct, i)      ((vuint8)((txStruct)->pChipData[(i)]))
#define Can_30_Flexcan4TxActualFdType(txStruct)       (((tCan_30_Flexcan4MsgObjPtr)((txStruct)->pChipMsgObj))->control & kCan_30_Flexcan4FdTypeFd)

/* Macros for CAN message access within PreTransmit() function */
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define Can_30_Flexcan4TxWriteActId(txStruct, id)       (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = MK_STDID0(id); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) &= (vuint16)0xFF0Fu

# define Can_30_Flexcan4TxWriteActFdId(txStruct, id)     (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = MK_STDID0(id); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control)  |= (vuint16)kCan_30_Flexcan4FdTypeFd | (vuint16)kCan_30_Flexcan4FdBrsTypeTrue; \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) &= (vuint16)0xFF0Fu

# define Can_30_Flexcan4TxWriteActExtId(txStruct, id)    (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = MK_EXTID0(id); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) |= (vuint16)kCan_30_Flexcan4IdTypeExt

# define Can_30_Flexcan4TxWriteActFdExtId(txStruct, id)  (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = MK_STDID0(id); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control)  |= (vuint16)kCan_30_Flexcan4IdTypeExt | (vuint16)kCan_30_Flexcan4FdTypeFd | (vuint16)kCan_30_Flexcan4FdBrsTypeTrue;
#else
# define Can_30_Flexcan4TxWriteActId(txStruct, id)       (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = ((((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) & 0x0000FFFFu) | ((vuint32)MK_STDID0(id) << 16); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) &= (vuint16)0xFF0Fu

#  define Can_30_Flexcan4TxWriteActFdId(txStruct, id)    (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) = ((((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->msgID) & 0x0000FFFFu) | ((vuint32)MK_STDID0(id) << 16); \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control)  |= (vuint16)kCan_30_Flexcan4FdTypeFd | (vuint16)kCan_30_Flexcan4FdBrsTypeTrue; \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) &= (vuint16)0xFF0Fu
#endif
#define Can_30_Flexcan4TxWriteActDLC(txStruct, dlc)     (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) &= (vuint8)kCan_30_Flexcan4IdTypeExt; \
                                            (((tCan_30_Flexcan4MsgObjPtr)((txStruct).pChipMsgObj))->control) |= ((vuint8)(dlc) & (vuint8)0x0Fu)

/* Macros to fill variables of the type tCan_30_Flexcan4MsgTransmitStruct */

/* macros to copy data bytes to unint32 */
#define Can_30_Flexcan4CopyDataBytes2Uint32(dst, src) \
            (dst)[0] = (((vuint32)((src)[0])) << 24) | \
                       (((vuint32)((src)[1])) << 16) | \
                       (((vuint32)((src)[2])) << 8) | \
                       ((vuint32)((src)[3])); \
            (dst)[1] = (((vuint32)((src)[4])) << 24) | \
                       (((vuint32)((src)[5])) << 16) | \
                       (((vuint32)((src)[6])) << 8) | \
                       ((vuint32)((src)[7]));

#define Can_30_Flexcan4CopyFDDataBytes2Uint32(dst, src) \
            (dst) = (((vuint32)((src)[0])) << 24) | \
                    (((vuint32)((src)[1])) << 16) | \
                    (((vuint32)((src)[2])) << 8) | \
                    ((vuint32)((src)[3]));

#define Can_30_Flexcan4Add1FDByte2Uint32(dst, src) \
            (dst) =  (((dst) << 8)); \
            (dst) |= ((vuint32)((src)));

#define Can_30_Flexcan4CopyFDPadding2Uint32(dst, src) \
            (dst) = (((vuint32)((src))) << 24) | \
                    (((vuint32)((src))) << 16) | \
                    (((vuint32)((src))) << 8) | \
                    ((vuint32)((src)));


#define CAN_30_FLEXCAN4_HL_MIN(x, y) (((x)>(y)) ? (y) : (x))
#define CAN_30_FLEXCAN4_HL_MAX(x, y) (((x)<(y)) ? (y) : (x))

/***************************************************************************************************/
/* Macros for dummy statements */
#if !defined(CAN_30_FLEXCAN4_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_30_FLEXCAN4_DUMMY_STATEMENT(x) (x) = (x) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define CAN_30_FLEXCAN4_DUMMY_STATEMENT(x)
# endif
#endif
#if !defined(CAN_30_FLEXCAN4_DUMMY_STATEMENT_CONST) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_30_FLEXCAN4_DUMMY_STATEMENT_CONST(x) (void)(x)
# else
#  define CAN_30_FLEXCAN4_DUMMY_STATEMENT_CONST(x)
# endif
#endif

/***************************************************************************************************/
/* Macros for DET and assertions */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON) /* \trace SPEC-1404, SPEC-1696 */
/* \trace SPEC-1725, SPEC-1403, SPEC-1568 */
# define Can_30_Flexcan4_CallDetReportError(api, err) ((void) Det_ReportError(CAN_30_FLEXCAN4_MODULE_ID, CAN_30_FLEXCAN4_INSTANCE_ID, (api), (err)))
# define assertHardware(a, b, c)  if (!(a)) {Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, (c));}
# define assertInternal(a, b, c)  if (!(a)) {Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, (c));}
# define assertUser(a, b, c)      if (!(a)) {Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, (c));}
# define assertGen(a, b, c)       if (!(a)) {Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, (c));}
# define C_30_FLEXCAN4_ENABLE_HARDWARE_CHECK
# define C_30_FLEXCAN4_ENABLE_INTERNAL_CHECK
# define C_30_FLEXCAN4_ENABLE_USER_CHECK
# define C_30_FLEXCAN4_ENABLE_GEN_CHECK
#else
# define Can_30_Flexcan4_CallDetReportError(api, err)
# define assertHardware(a, b, c)
# define assertInternal(a, b, c)
# define assertUser(a, b, c)
# define assertGen(a, b, c)
#endif

/***************************************************************************************************/
/* Macros for Critical Area 0 abstraction */

#define Can_30_Flexcan4DeclareGlobalInterruptOldStatus
/* CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0 called only by LL code */
#define Can_30_Flexcan4SingleGlobalInterruptDisable()     SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()
#define Can_30_Flexcan4SingleGlobalInterruptRestore()     SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()
#define Can_30_Flexcan4GlobalInterruptDisable()           SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()
#define Can_30_Flexcan4GlobalInterruptRestore()           SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()
#define Can_30_Flexcan4NestedGlobalInterruptDisable()     SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()
#define Can_30_Flexcan4NestedGlobalInterruptRestore()     SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_0()

/***************************************************************************************************/
/* Macros for LL-HLL compatibility */

/***************************************************************************************************/
/* Macros for ProtectedMode / PeripheralAccess */
# if defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_OS )
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8(area, regAdr, val)         osWritePeripheral8((area), (uint32)(regAdr), (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16(area, regAdr, val)        osWritePeripheral16((area), (uint32)(regAdr), (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32(area, regAdr, val)        osWritePeripheral32((area), (uint32)(regAdr), (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_RESET(area, regAdr, bits)  osModifyPeripheral8((area), (uint32)(regAdr), (uint8)~(bits), (uint8)0x00u)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_RESET(area, regAdr, bits) osModifyPeripheral16((area), (uint32)(regAdr), (uint16)~(bits), (uint16)0x0000u)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_RESET(area, regAdr, bits) osModifyPeripheral32((area), (uint32)(regAdr), (uint32)~(bits), (uint32)0x00000000UL)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_SET(area, regAdr, bits)    osModifyPeripheral8((area), (uint32)(regAdr), (uint8)0xFFu, (bits))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_SET(area, regAdr, bits)   osModifyPeripheral16((area), (uint32)(regAdr), (uint16)0xFFFFu, (bits))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_SET(area, regAdr, bits)   osModifyPeripheral32((area), (uint32)(regAdr), (uint32)0xFFFFFFFFUL, (bits))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG8(area, regAdr)               osReadPeripheral8((area), (uint32)(regAdr))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG16(area, regAdr)              osReadPeripheral16((area), (uint32)(regAdr))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG32(area, regAdr)              osReadPeripheral32((area), (uint32)(regAdr))
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_APPL )
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8(area, regAdr, val)         Appl_30_Flexcan4CanWriteProtectedRegister8((regAdr), (uint8)0xFFu, (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16(area, regAdr, val)        Appl_30_Flexcan4CanWriteProtectedRegister16((regAdr), (uint16)0xFFFFu, (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32(area, regAdr, val)        Appl_30_Flexcan4CanWriteProtectedRegister32((regAdr), (uint32)0xFFFFFFFFUL, (val))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_RESET(area, regAdr, bits)  Appl_30_Flexcan4CanWriteProtectedRegister8((regAdr), (bits), (uint8)0x00u)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_RESET(area, regAdr, bits) Appl_30_Flexcan4CanWriteProtectedRegister16((regAdr), (bits), (uint16)0x0000u)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_RESET(area, regAdr, bits) Appl_30_Flexcan4CanWriteProtectedRegister32((regAdr), (bits), (uint32)0x00000000UL)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_SET(area, regAdr, bits)    Appl_30_Flexcan4CanWriteProtectedRegister8((regAdr), (bits), (bits))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_SET(area, regAdr, bits)   Appl_30_Flexcan4CanWriteProtectedRegister16((regAdr), (bits), (bits))
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_SET(area, regAdr, bits)   Appl_30_Flexcan4CanWriteProtectedRegister32((regAdr), (bits), (bits))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG8(area, regAdr)               Appl_30_Flexcan4CanReadProtectedRegister8((regAdr))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG16(area, regAdr)              Appl_30_Flexcan4CanReadProtectedRegister16((regAdr))
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG32(area, regAdr)              Appl_30_Flexcan4CanReadProtectedRegister32((regAdr))
# endif
# if !defined(C_30_FLEXCAN4_ENABLE_USER_MODE_OS) && !defined(C_30_FLEXCAN4_ENABLE_USER_MODE_APPL)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8(area, regAdr, val)         *(regAdr) = (val)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16(area, regAdr, val)        *(regAdr) = (val)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32(area, regAdr, val)        *(regAdr) = (val)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_RESET(area, regAdr, bits)  *(regAdr) &= (uint8)~(bits)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_RESET(area, regAdr, bits) *(regAdr) &= (uint16)~(bits)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_RESET(area, regAdr, bits) *(regAdr) &= (uint32)~(bits)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG8_SET(area, regAdr, bits)    *(regAdr) |= (bits)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16_SET(area, regAdr, bits)   *(regAdr) |= (bits)
#  define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG32_SET(area, regAdr, bits)   *(regAdr) |= (bits)
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG8(area, regAdr)               *(regAdr)
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG16(area, regAdr)              *(regAdr)
#  define CAN_30_FLEXCAN4_READ_PROTECTED_REG32(area, regAdr)              *(regAdr)
# endif

/***************************************************************************/
/* CAN-Hardware Data Definitions                                           */
/***************************************************************************/

/***************************************************************************/
/* Special switches need by CAN driver                                     */
/***************************************************************************/

/***************************************************************************/
/* Defines / data types / structures / unions                              */
/***************************************************************************/
# define kCan_30_Flexcan4NumberOfUsedCanRxIdTables 1
# define kCan_30_Flexcan4NumberOfUsedCanTxIdTables 1
/* Define CAN Chip hardware; segment must be located in locator file    */
/* register layout of the can chip                                      */
/* Structure describing CAN receive buffer. */

#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
# if( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id); \
                                   (paraStruct)->idRaw3 = MK_STDID3(id); \
                                   (paraStruct)->idRaw4 = MK_STDID4(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id); \
                                   (paraStruct)->idRaw3 = MK_EXTID3(id); \
                                   (paraStruct)->idRaw4 = MK_EXTID4(id)
# elif ( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id); \
                                   (paraStruct)->idRaw3 = MK_STDID3(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id); \
                                   (paraStruct)->idRaw3 = MK_EXTID3(id)
# elif ( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id)
# elif ( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id)
# else
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id)
# endif
#endif

#if( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id); \
                                   (paraStruct)->idRaw3 = MK_STDID3(id); \
                                   (paraStruct)->idRaw4 = MK_STDID4(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id); \
                                   (paraStruct)->idRaw3 = MK_EXTID3(id); \
                                   (paraStruct)->idRaw4 = MK_EXTID4(id)
#elif ( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id); \
                                   (paraStruct)->idRaw3 = MK_STDID3(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id); \
                                   (paraStruct)->idRaw3 = MK_EXTID3(id)
#elif ( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id)
#elif ( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id)
#else
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id)
#endif

/***************************************************************************/
/* Core - LowLevel API                                                     */
/***************************************************************************/
typedef struct sCan_30_Flexcan4RxInfoStruct
{
  Can_30_Flexcan4ChipMsgPtr     pChipMsgObj;
  Can_30_Flexcan4ChipDataPtr    pChipData;
  Can_30_Flexcan4ObjectHandle   localMailboxHandle; 
  Can_IdType               localId; 
  uint8                    localDlc;
  /* LL specific part */
} tCan_30_Flexcan4RxInfoStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4RxInfoStruct, VAR_NOINIT) CanRxInfoStructPtr;

typedef struct sCan_30_Flexcan4TxConfInfoStruct
{
  Can_30_Flexcan4ChipMsgPtr     pChipMsgObj;
  Can_30_Flexcan4ChipDataPtr    pChipData;
  /* LL specific part */
} tCan_30_Flexcan4TxConfInfoStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4TxConfInfoStruct, VAR_NOINIT) Can_30_Flexcan4TxConfInfoStructPtr;

typedef struct sCan_30_Flexcan4InitParaStruct
{
  Can_30_Flexcan4InitHandle     initObject;
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   hwObjHandle;
#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
  tCan_30_Flexcan4RxId0                idRaw0;
# if( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4RxId1                idRaw1;
# endif
# if( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4RxId2                idRaw2;
# endif
# if( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4RxId3                idRaw3;
# endif
# if( kCan_30_Flexcan4NumberOfUsedCanRxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4RxId4                idRaw4;
# endif
  tCan_30_Flexcan4IdType               idType;
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  uint8                    doRamCheck;
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
  uint8                    isChRamCheckFail;
# endif
#endif
  uint8                    isInitOk;
  /* LL specific part */
} tCan_30_Flexcan4InitParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4InitParaStruct, VAR_NOINIT) CanInitParaStructPtr;

#define TxDataPtr Can_SduPtrType /* CBD compatibility */
typedef struct sCan_30_Flexcan4TxTransmissionParaStruct
{
  /* logical formats */
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  Can_30_Flexcan4ObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  Can_30_Flexcan4ObjectHandle   activeSendObject; /* index of the logical Tx mailbox (Tx * Element) */
  tCan_30_Flexcan4TxId0                idRaw0;
#if( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4TxId1                idRaw1;
#endif
#if( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4TxId2                idRaw2;
#endif
#if( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4TxId3                idRaw3;
#endif
#if( kCan_30_Flexcan4NumberOfUsedCanTxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
  tCan_30_Flexcan4TxId4                idRaw4;
#endif
#if defined( C_30_FLEXCAN4_ENABLE_MIXED_ID ) 
  tCan_30_Flexcan4IdType               idType;
#endif
  tCan_30_Flexcan4Dlc                  dlcRaw;
  TxDataPtr                CanMemCopySrcPtr;
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
  tCan_30_Flexcan4FdType               fdType;
  tCan_30_Flexcan4FdBrsType            fdBrsType;
  uint8                    messageLen;
  uint8                    frameLen;
  uint8                    paddingVal;
#endif
  Can_PduType              pdu;
  /* LL specific part */
} tCan_30_Flexcan4TxTransmissionParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4TxTransmissionParaStruct, VAR_NOINIT) CanTxTransmissionParaStructPtr;

typedef struct sCan_30_Flexcan4RxBasicParaStruct
{
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   hwObjHandle;
#if defined( C_30_FLEXCAN4_ENABLE_OVERRUN )
  uint8                    isOverrun;
#endif
  tCan_30_Flexcan4RxInfoStruct         rxStruct;
  /* LL specific part */
} tCan_30_Flexcan4RxBasicParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4RxBasicParaStruct, VAR_NOINIT) CanRxBasicParaStructPtr;

typedef struct sCan_30_Flexcan4RxFullParaStruct
{
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   hwObjHandle;
#if defined( C_30_FLEXCAN4_ENABLE_FULLCAN_OVERRUN )
  uint8                    isOverrun;
#endif
  tCan_30_Flexcan4RxInfoStruct         rxStruct;
  /* LL specific part */
} tCan_30_Flexcan4RxFullParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4RxFullParaStruct, VAR_NOINIT) CanRxFullParaStructPtr;

#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW ) || defined( C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API )
typedef struct
{
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  Can_30_Flexcan4ObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  Can_30_Flexcan4ObjectHandle   activeSendObject; /* index of the logical mailbox + muxTx or FIFO element 0..n */
  Can_30_Flexcan4TransmitHandle   canHandleCurTxObj;
} tCan_30_Flexcan4TxCancellationParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4TxCancellationParaStruct, VAR_NOINIT) CanTxCancellationParaStructPtr;
#endif

typedef struct
{
  Can_30_Flexcan4ObjectHandle   mailboxHandle;
  Can_30_Flexcan4ObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  Can_30_Flexcan4ObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  Can_30_Flexcan4ObjectHandle   activeSendObject; /* index of the logical mailbox + muxTx or FIFO element 0..n */
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
  Can_30_Flexcan4TxConfInfoStructPtr   txStructConf;
#endif
  #if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  # if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
  #  if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  vuint32 canDataBuffer[16];
  #  else
  vuint32 canDataBuffer[2];
  #  endif
  # endif
  #endif
} tCan_30_Flexcan4TxConfirmationParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4TxConfirmationParaStruct, VAR_NOINIT) CanTxConfirmationParaStructPtr;

typedef struct sCan_30_Flexcan4TaskParaStruct
{
  Can_30_Flexcan4ObjectHandle mailboxHandle;
  Can_30_Flexcan4ObjectHandle hwObjHandle;
  /* LL specific part */
} tCan_30_Flexcan4TaskParaStruct;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4TaskParaStruct, VAR_NOINIT) CanTaskParaStructPtr;
    
/***************************************************************************/
/* module global variable declaration                                      */
/***************************************************************************/

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CAN_30_FLEXCAN4_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***************************************************************************/
/* local function prototypes (only internal usage)                         */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



/************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxWriteReg16()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck inside InitMailbox functions
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck.
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller.
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxWriteReg16(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr16 regPtr, vuint16 value, vuint16 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/************************************************************************************************************
 *  CanLL_30_Flexcan4_WriteReg32()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck.
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller.
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \param[in,out] initPara         Pointer to local variables of Can_30_Flexcan4Init()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr32 regPtr, vuint32 value, vuint32 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxWriteReg32()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck inside InitMailbox functions
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck.
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller.
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \param[in,out] initPara         Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxWriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr32 regPtr, vuint32 value, vuint32 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */


/************************************************************************************************************
 *  CanLL_30_Flexcan4_WriteProtectedReg16()
 ***********************************************************************************************************/
/*! \brief         Write value via OS and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck.
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller.
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     area             Memory area to be written.
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            Value to be written.
 *  \param[in]     readMask         Mask to check the read back value.
 *  \param[in,out] initPara         Pointer to local variables of Can_30_Flexcan4Init()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_WriteProtectedReg16(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST vuint16 area, Can_30_Flexcan4ChipMsgPtr16 regPtr, vuint16 value, vuint16 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBegin()
 *********************************************************************************************************************/
/*! \brief         Starts the channel initialization
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBegin( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt()
 *********************************************************************************************************************/
/*! \brief         Performs the channel register RAM check
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4False           No corrupt register RAM found
 *                 kCan_30_Flexcan4True            Register RAM is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBeginSetRegisters()
 *********************************************************************************************************************/
/*! \brief         Performs the channel register initialization
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBeginSetRegisters( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxTx()
 *********************************************************************************************************************/
/*! \brief         Initializes a transmit mailbox
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxTx( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

# if (defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK ))

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxTxDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a transmit mailbox if it is considered corrupt by the RAM check
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxTxDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */
# endif


#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxFullCan()
 *********************************************************************************************************************/
/*! \brief         Initializes a receive FullCAN mailbox
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxFullCan( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

# if ((defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )))
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a receive FullCAN mailbox if it is considered corrupt by the RAM check
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */
# endif

#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

#if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxBasicCan()
 *********************************************************************************************************************/
/*! \brief         Initializes a receive BasicCAN mailbox
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxBasicCan( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

# if ((defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK ))) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a receive BasicCAN mailbox if it is considered corrupt by the RAM check
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */
# endif
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitEndSetRegisters()
 *********************************************************************************************************************/
/*! \brief         Finishes the channel register initialization
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitEndSetRegisters( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitEnd()
 *********************************************************************************************************************/
/*! \brief         Finishes the channel initialization
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitEnd( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitIsMailboxCorrupt()
 *********************************************************************************************************************/
/*! \brief         Performs the RAM check for a mailbox
 *  \details       Called by Can_30_Flexcan4Init()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of Can_30_Flexcan4Init()
 *  \return        kCan_30_Flexcan4False           Mailbox is not corrupt
 *                 kCan_30_Flexcan4True            Mailbox is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitIsMailboxCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara );
/* CODE CATEGORY 4 END */
#endif


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitPowerOn()
 *********************************************************************************************************************/
/*! \brief         Performs the hardware specific global module initialization
 *  \details       Called by Can_30_Flexcan4InitPowerOn()
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitPowerOn( void );
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMemoryPowerOn()
 *********************************************************************************************************************/
/*! \brief         Internal function, called by Can_30_Flexcan4_InitMemory().
 *  \details       Performs the platform specific memory initialization.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMemoryPowerOn( void );
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitPowerOnChannelSpecific()
 *********************************************************************************************************************/
/*! \brief         Performs the channel dependent hardware specific global module initialization
 *  \details       Called by Can_30_Flexcan4InitPowerOn()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitPowerOnChannelSpecific( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END */

#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxCancelInHw()
 *********************************************************************************************************************/
/*! \brief         Performs cancellation of the transmission in hardware
 *  \details       Called by cancellation
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txCancellationPara  Pointer to local variables of cancellation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 3 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxCancelInHw(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxCancellationParaStructPtr txCancellationPara);
/* CODE CATEGORY 3 END */
#endif
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxBegin()
 *********************************************************************************************************************/
/*! \brief         Perform start of transmission
 *  \details       Called by transmission to prepare send object
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxSetMailbox()
 *********************************************************************************************************************/
/*! \brief         Set mailbox data for transmission
 *  \details       Called by transmission to set ID, DLC
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxSetMailbox(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxCopyToCan()
 *********************************************************************************************************************/
/*! \brief         Set mailbox data for transmission
 *  \details       Called by transmission to set data part in mailbox
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxCopyToCan(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxStart()
 *********************************************************************************************************************/
/*! \brief         Trigger mailbox to start the transmission
 *  \details       Called by transmission to start transmission
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \return        kCan_30_Flexcan4Ok              No issue
 *                 kCan_30_Flexcan4Failed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxStart(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxEnd()
 *********************************************************************************************************************/
/*! \brief         Perform end handling of the transmission
 *  \details       Called by transmission to finish transmission
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
#if defined( C_30_FLEXCAN4_ENABLE_TX_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxIsGlobalConfPending()
 *********************************************************************************************************************/
/*! \brief         Check global pending of transmission
 *  \details       Called by transmission to get global pending flag
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4True            pending confirmation
 *                 kCan_30_Flexcan4Failed          no pending confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxIsGlobalConfPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxProcessPendings()
 *********************************************************************************************************************/
/*! \brief         Check pending of transmission object
 *  \details       Called by transmission to get pending flag of mailbox
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of confirmation task
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
#endif
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfBegin()
 *********************************************************************************************************************/
/*! \brief         Perform start of confirmation
 *  \details       Called by confirmation
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txConfPara          Pointer to local variables of confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfIsMsgTransmitted()
 *********************************************************************************************************************/
/*! \brief         Check message is transmitted
 *  \details       Called by confirmation
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txConfPara          Pointer to local variables of confirmation
 *  \return        kCan_30_Flexcan4True            message is transmitted
 *                 kCan_30_Flexcan4Failed          message is not transmitted
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxConfIsMsgTransmitted(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#endif
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfSetTxConfStruct()
 *********************************************************************************************************************/
/*! \brief         Set confirmation struct
 *  \details       Called by confirmation to set confirmation data struct
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txConfPara          Pointer to local variables of confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfSetTxConfStruct(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#endif
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfEnd()
 *********************************************************************************************************************/
/*! \brief         Perform confirmation end handling
 *  \details       Called by confirmation
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txConfPara          Pointer to local variables of confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */

#if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicMsgReceivedBegin()
 *********************************************************************************************************************/
/*! \brief         Perform BasicCAN receive begin
 *  \details       Called by reception to begin handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \return        kCan_30_Flexcan4Ok              No Issue occur
 *                 kCan_30_Flexcan4Failed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxBasicMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicReleaseObj()
 *********************************************************************************************************************/
/*! \brief         Release BasicCAN mailbox
 *  \details       Called by reception to release object
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicReleaseObj(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicMsgReceivedEnd()
 *********************************************************************************************************************/
/*! \brief         Release BasicCAN receive end
 *  \details       Called by reception to finish handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
# if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicIsGlobalIndPending()
 *********************************************************************************************************************/
/*! \brief         Check BasicCAN receive global pending
 *  \details       Called by reception to get pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4True            global pending
 *                 kCan_30_Flexcan4Failed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxBasicIsGlobalIndPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicProcessPendings()
 *********************************************************************************************************************/
/*! \brief         Check BasicCAN receive pending
 *  \details       Called by reception to get mailbox pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] taskPara            Pointer to local variables of reception task
 *  \return        kCan_30_Flexcan4True            mailbox pending
 *                 kCan_30_Flexcan4Failed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
# endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */
#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullMsgReceivedBegin()
 *********************************************************************************************************************/
/*! \brief         Perform FullCAN receive begin
 *  \details       Called by reception to begin handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \return        kCan_30_Flexcan4Ok              No Issue occur
 *                 kCan_30_Flexcan4Failed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxFullMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullReleaseObj()
 *********************************************************************************************************************/
/*! \brief         Release FullCAN mailbox
 *  \details       Called by reception to release object
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullReleaseObj(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullMsgReceivedEnd()
 *********************************************************************************************************************/
/*! \brief         Release FullCAN receive end
 *  \details       Called by reception to finish handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
# if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullIsGlobalIndPending()
 *********************************************************************************************************************/
/*! \brief         Check FullCAN receive global pending
 *  \details       Called by reception to get pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4True            global pending
 *                 kCan_30_Flexcan4Failed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxFullIsGlobalIndPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullProcessPendings()
 *********************************************************************************************************************/
/*! \brief         Check FullCAN receive pending
 *  \details       Called by reception to get mailbox pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] taskPara            Pointer to local variables of reception task
 *  \return        kCan_30_Flexcan4True            mailbox pending
 *                 kCan_30_Flexcan4Failed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
# endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured()
 *********************************************************************************************************************/
/*! \brief         Error state passive check
 *  \details       Checks if a transition to error state passive occurred
 *  \param[in]     channel            Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4True    Transition to passive occurred
 *                 kCan_30_Flexcan4Failed  Transition to passive has not occurred
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorNotificationHandling()
 *********************************************************************************************************************/
/*! \brief         Perform error notification handling for IDsM
 *  \details       Checks the Rx and Tx Errors in the controller and reports to Core
 *  \param[in]     channel   Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorNotificationHandling(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured()
 *********************************************************************************************************************/
/*! \brief         Error state active check
 *  \details       Checks for a transition to error state active and activates passive state reporting
 *  \param[in]     channel   Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
#endif /* CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorHandlingBegin()
 *********************************************************************************************************************/
/*! \brief         Perform error handling begin
 *  \details       Called by error handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorHandlingBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_BusOffOccured()
 *********************************************************************************************************************/
/*! \brief         Check BusOff occur
 *  \details       Called by error handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4True            BusOff occur
 *                 kCan_30_Flexcan4Failed          no BusOff occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_BusOffOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorHandlingEnd()
 *********************************************************************************************************************/
/*! \brief         Perform error handling end
 *  \details       Called by error handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorHandlingEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_GetStatusBegin()
 *********************************************************************************************************************/
/*! \brief         Perform read out of status information
 *  \details       Called by GetStatus API
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 3 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_GetStatusBegin(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 3 END */
#endif
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ModeTransition()
 *********************************************************************************************************************/
/*! \brief         Perform mode change
 *  \details       Called by mode handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in]     mode                mode to be set
 *  \param[in]     busOffRecovery      BusOff recovery should be done or not
 *  \param[in]     ramCheck            RamCheck should be done or not
 *  \return        kCan_30_Flexcan4Ok              Successfully completed
 *                 kCan_30_Flexcan4Requested       Mode not yet reached
 *                 kCan_30_Flexcan4Failed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 busOffRecovery, vuint8 ramCheck);
/* CODE CATEGORY 4 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_StopReinit()
 *********************************************************************************************************************/
/*! \brief         Perform reinitialization for mode change
 *  \details       Called by mode handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_StopReinit(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */

#if (( CAN_30_FLEXCAN4_INTLOCK != CAN_30_FLEXCAN4_APPL) ) /* COV_CAN_LOCK_ISR_BY_APPL */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_CanInterruptDisable()
 ***********************************************************************************************************/
/*! \brief         Disables CAN interrupts
 *  \details       Saves the current state (enable or disable) of all 
 *                 CAN interrupt sources to localInterruptOldFlagPtr
 *                 and disables all CAN interrupt sources (RX, TX, error, wakeup).
 *  \param[in]     canHwChannel              CAN channel
 *  \param[out]    localInterruptOldFlagPtr  Pointer to global variable that holds the interrupt state
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanInterruptDisable(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST tCan_30_Flexcan4LLCanIntOldPtr localInterruptOldFlagPtr);
/* CODE CATEGORY 1 END */

/************************************************************************************************************
 *  CanLL_30_Flexcan4_CanInterruptRestore()
 **********************************************************************************************************/
/*! \brief         Restores CAN interrupts.
 *  \details       Restores the previous state of all CAN interrupt sources (RX, TX, error, wakeup).
 *  \param[in]     canHwChannel             CAN channel
 *  \param[in]     localInterruptOldFlag    Global variable that holds the interrupt state
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanInterruptRestore(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST tCan_30_Flexcan4LLCanIntOld localInterruptOldFlag);
/* CODE CATEGORY 1 END */
#endif



/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_SetFlexCANToInitMode()
 ***********************************************************************************************************/
/*! \brief         Set FlexCAN Controller to INIT mode.
 *  \details       Sets the FlexCAN Controller from any mode to INIT mode.
 *  \param[in]     Controller     CAN controller (Controller must be in valid range).
 *                                (only if not using "Optimize for one controller")
 *  \param[in]     Can_30_Flexcan4InitHandle  handle to INIT-structure
 *  \return        kCan_30_Flexcan4Requested: FlexCAN successfully set to init mode
 *  \return        kCan_30_Flexcan4Failed:    FlexCAN failure
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_SetFlexCANToInitMode(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4InitHandle initObject);
/* CODE CATEGORY 4 END */
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_ExecuteSoftReset()
 ***********************************************************************************************************/
/*! \brief         Perform FlexCAN soft-reset
 *  \details       Triggers the FlexCAN Controller to perform internal soft-reset.
 *  \param[in]     Controller     CAN controller (Controller must be in valid range).
 *                                (only if not using "Optimize for one controller")
 *  \return        kCan_30_Flexcan4Requested: SoftReset ongoing
 *  \return        kCan_30_Flexcan4Ok:        SoftReset successfully finished
 *  \return        kCan_30_Flexcan4Failed:    transition failure
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ExecuteSoftReset(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_StopTransition()
 ***********************************************************************************************************/
/*! \brief         Perform FlexCAN for stop mode and control the transition.
 *  \details       Request stop mode to FlexCAN Controller and control the transition.
 *  \param[in]     channel          current application CAN channel
 *  \param[in]     canHwChannel     current HW CAN channel
 *  \param[in]     busOffRecovery   flag whether BusOff recovery shall be terminated (if relevant and possible)
 *  \param[in]     doRamCheck       flag whether RamCheck shall be executed (if relevant and possible)
 *  \return        kCan_30_Flexcan4Requested:   Transition to STOP ongoing
 *  \return        kCan_30_Flexcan4Ok           Transition to STOP successfully finished
 *  \return        kCan_30_Flexcan4Failed       Transition to STOP not possible
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_StopTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 busOffRecovery, vuint8 doRamCheck);
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_StartTransition()
 ***********************************************************************************************************/
/*! \brief         Perform FlexCAN start mode and control the transition
 *  \details       Request start mode to FlexCAN Controller and control the transition.
 *  \param[in]     channel          current application CAN channel
 *  \param[in]     canHwChannel     current HW CAN channel
 *  \param[in]     mode             target mode for the transition
 *  \param[in]     doRamCheck       flag whether RamCheck shall be executed (if relevant and possible)
 *  \return        kCan_30_Flexcan4Requested:   Transition to START ongoing
 *  \return        kCan_30_Flexcan4Ok:          Transition to START successfully finished
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_StartTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 doRamCheck);
/* CODE CATEGORY 4 END */

#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_BusOffEndTransition()
 ***********************************************************************************************************/
/*! \brief         Perform FlexCAN BusOffEnd transition and control the transition
 *  \details       Request BusOff Auto Recovery Mode to FlexCAN Controller. When busoff end is detected
 *                 request start mode and control the transition.
 *  \param[in]     channel          current application CAN channel
 *  \param[in]     canHwChannel     current HW CAN channel
 *  \param[in]     doRamCheck       flag whether RamCheck shall be executed (if relevant and possible)
 *  \return        kCan_30_Flexcan4Requested:   Transition to START ongoing
 *  \return        kCan_30_Flexcan4Ok:          Transition to START successfully finished
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_BusOffEndTransition(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST vuint8 doRamCheck);
/* CODE CATEGORY 4 END */
#endif

#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_HandleBusOff()
 ***********************************************************************************************************/
/*! \brief         Handle BusOff recovery
 *  \details       Call Can_30_Flexcan4Init and set BusOff flag to FALSE if BusOff recovery was successful
 *  \param[in]     Controller    CAN controller (Controller must be in valid range).
 *                               (only if not using "Optimize for one controller")
 *  \pre           Interrupts are disabled.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_HandleBusOff(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED ) && defined( C_30_FLEXCAN4_ENABLE_CLKSRC_SELECTION ) /* COV_CAN_RAMCHECK_EXTENDED_CLKSRC_AVAILABILITY */
/* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_CheckClockSource()
 ***********************************************************************************************************/
/*! \brief         Check the CAN[CTRL1].CLKSRC bit
 *  \details       check the CAN[CTRL1].CLKSRC bit by write and read back patterns.
 *  \param[in]     Controller   CAN controller
 *  \return        kCan_30_Flexcan4True:    CAN[CTRL1].CLKSRC bit is corrupt
 *  \return        kCan_30_Flexcan4False:   CAN[CTRL1].CLKSRC bit is not corrupt  
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckClockSource(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_ComputeMailboxInterrupt()
 ***********************************************************************************************************/
/*! \brief         Computes Mailbox interrupt.
 *  \details       Determines the index of the Mailbox that notified an interrupt and call
 *                 CanLL_30_Flexcan4_ComputeInterruptType.
 *  \param[in]     Controller    CAN controller (Controller must be in valid range).
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     iFlags        Interrupt Flags that should be used for computation.
 *  \param[in]     iMask         Interrupt Mask that determines which iFlags are considered in interrupt context.
 *  \param[in]     startIndex    Index of first Mailbox that is part of the corresponding iFlags/iMask values.
 *  \pre           -
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST vuint32 iFlags, vuint32 iMask, Can_30_Flexcan4ObjectHandle startIndex);
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_ComputeInterruptType()
 ***********************************************************************************************************/
/*! \brief         Computes the communication type of the notified Mailbox interrupt (Tx/Rx FullCAN/BasicCAN).
 *  \details       Determines which communication event occurred and calls internal notification function.
 *  \param[in]     Controller    CAN controller (Controller must be in valid range)
 *                               (only if not using "Optimize for one controller")
 *  \param[in]     index         Index of the Mailbox that notified an interrupt.
 *  \pre           -
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ComputeInterruptType(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle index);
/* CODE CATEGORY 1 END */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_ComputeInterruptType()
 ***********************************************************************************************************/
/*! \brief         Checks all mailbox interrupt flags (Tx/Rx FullCAN/BasicCAN).
 *  \details       Determines which mailbox interrupt flags are set and processes it accordingly.
 *  \param[in]     Controller    CAN controller (Controller must be in valid range).
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanMailboxInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END */
#endif

/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_ClearPendingInterrupt()
 ***********************************************************************************************************/
/*! \brief         Clears the interrupt flag related to the hwObjectHandle.
 *  \details       Determines which which interrupt flags needs to be cleared and clears the bit related to
 *                 the hwObjectHandle.
 *  \param[in]     pFlex_30_Flexcan4CANLocal    base address of the CAN controller
 *  \param[in]     hwObjHandle      current hardware object to handle
 *  \pre           -
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ClearPendingInterrupt( tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal, Can_30_Flexcan4ObjectHandle hwObjHandle );
/* CODE CATEGORY 1 END */

#if defined (C_30_FLEXCAN4_ENABLE_TX_POLLING) || ( defined (C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) && defined (C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS) )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_GetInterruptFlag()
 ***********************************************************************************************************/
/*! \brief         Gets the corresponding interrupt flag related to a hwObjectHandle.
 *  \details       Determines which which interrupt flags needs to be returned related to the hwObjectHandle.
 *  \param[in]     pFlex_30_Flexcan4CANLocal    base address of the CAN controller
 *  \param[in]     hwObjHandle      current hardware object to handle
 *  \return        iFlags           returns the content of the iFlag related to hwObjHandle
 *  \pre           -
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint32, STATIC_CODE) CanLL_30_Flexcan4_GetInterruptFlag( tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal, Can_30_Flexcan4ObjectHandle hwObjHandle );
/* CODE CATEGORY 1 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK ) && defined (C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt()
 *********************************************************************************************************************/
/*! \brief         Performs the RAM check for a mailbox
 *  \details       Called by CanLL_30_Flexcan4_InitIsMailboxCorrupt()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCan_30_Flexcan4False           Mailbox is not corrupt
 *                 kCan_30_Flexcan4True            Mailbox is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END */
#endif

#if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  /* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_CheckMaxHwMailboxHandle()
 ***********************************************************************************************************/
/*! \brief         Check for maximum value of HW mailbox handle
 *  \details       If canHwChannel is greater or equal to number of available mailboxes throw a DET
 *  \param[in]     hwChannel           hardware channel of the corresponding CAN Controller
 *  \param[in]     hwObjHandle         Handle to hardware object
 *  \return        retVal     kCan_30_Flexcan4Ok: no error, kCan_30_Flexcan4False: error
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_SAFE_BSW == STD_ON
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(Can_30_Flexcan4ChannelHandle hwChannel, Can_HwHandleType hwObjHandle);
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/************************************************************************************************************
*  CanLL_30_Flexcan4_CheckEnhancedRxFifoFilter()
***********************************************************************************************************/
/*! \brief         Check for valid generated filter tables
 *  \details       check for valid number of Rx Fifo filters
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in]     initObject handle to init struct
 *  \return        retVal     kCan_30_Flexcan4Ok: no error, kCan_30_Flexcan4False: error
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_SAFE_BSW == STD_ON
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckEnhancedRxFifoFilter(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4InitHandle initObject);
/* CODE CATEGORY 4 END */

# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
      defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
  /* CODE CATEGORY 4 START */
/************************************************************************************************************
 *  CanLL_30_Flexcan4_CheckSizeOfHwChannelData()
 ***********************************************************************************************************/
/*! \brief         Check size of HwChannelData
 *  \details       If canHwChannel is greater or equal to the size of HwChannelData throw a DET
 *  \param[in]     hwChannel   hardware channel of the corresponding CAN Controller
 *  \return        retVal     kCan_30_Flexcan4Ok: no error, kCan_30_Flexcan4False: error
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckSizeOfHwChannelData(Can_30_Flexcan4ChannelHandle hwChannel);
/* CODE CATEGORY 4 END */
# endif
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_CleanUpSendState()
 ***********************************************************************************************************/
/*! \brief         Clean up send state
 *  \details       Reset active send state (semaphore blocking TX while send).
 *                 Called by Can_30_Flexcan4_SetControllerMode(), Can_30_Flexcan4_MainFunction_Mode() (CanHL_30_Flexcan4_ReInit() do same but without this call).
 *  \param[in]     Controller    CAN controller \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_CleanUpSendState(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY);

# if (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_OFF)
/************************************************************************************************************
 *  Appl_30_Flexcan4CanTimerLoop()
 ***********************************************************************************************************/
/*! \brief         Time out monitoring
 *  \details       Service function to check (against generated maximum loop value) whether a hardware loop shall be continued or broken.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *                             (only if not using "Optimize for one controller")
 *  \param[in]     source      Source for the hardware observation.
 *  \return        CAN_NOT_OK  when loop shall be broken (observation stops) \n
 *                             CAN_NOT_OK should only be used in case of a time out occurs due to a hardware issue. \n
 *                             After this an appropriate error handling is needed (see chapter Hardware Loop Check / Time out Monitoring).
 *  \return        CAN_OK      when loop shall be continued (observation continues)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Please refer to chapter "Hardware Loop Check".
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) Appl_30_Flexcan4CanTimerLoop(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 source);
# endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_ModeTransition()
 ***********************************************************************************************************/
/*! \brief         Request mode transition
 *  \details       Handle/Request transition and wait asynchronous-short time.
 *                 Called by Can_30_Flexcan4_SetControllerMode().
 *  \param[in]     Controller         CAN controller (Controller must be in valid range). \n
 *                                    (only if not using "Optimize for one controller") \n
 *  \param[in]     transitionRequest  transition request used to switch mode (kCan_30_Flexcan4ModeInit, kCan_30_Flexcan4ModeSleep, kCan_30_Flexcan4ModeWakeupStopReinit, \n
 *                                    kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4ModeStopReinitFast, kCan_30_Flexcan4ModeStart, kCan_30_Flexcan4ModeStartReinit, kCan_30_Flexcan4ModeSilent, \n
 *                                    kCan_30_Flexcan4ModeResetBusOffStart, kCan_30_Flexcan4ModeResetBusOffEnd). \n
 *  \param[in]     busOffRecovery     handling of the busoff behaviour (kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4ContinueBusOffRecovery). \n
 *  \param[in]     doRamCheck         do a RAM check while reinit or not (kCan_30_Flexcan4ExecuteRamCheck, kCan_30_Flexcan4SuppressRamCheck). \n
 *  \return        kCan_30_Flexcan4Failed         transition rejected
 *  \return        kCan_30_Flexcan4Requested      transition requested but is ongoing asynchronous
 *  \return        kCan_30_Flexcan4Ok             transition finished successful
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 transitionRequest, uint8 busOffRecovery, uint8 doRamCheck);

/************************************************************************************************************
 *  CanHL_30_Flexcan4_NotifyTransition()
 ***********************************************************************************************************/
/*! \brief         notify about sucessful mode transition
 *  \details       notify to upper layer.
 *                 Called by Can_30_Flexcan4_SetControllerMode().
 *  \param[in]     Controller         CAN controller (Controller must be in valid range). \n
 *                                    (only if not using "Optimize for one controller") \n
 *  \param[in]     canState           CAN controller mode to be switched to (CAN_STATUS_STOP, CAN_STATUS_START, CAN_STATUS_SLEEP). \n
 *  \param[in]     canIfState         CAN controller mode to be switched to (CANIF_CS_STOPPED, CANIF_CS_STARTED, CANIF_CS_SLEEP). \n
 *  \param[in]     transitionState    transition request state (kCan_30_Flexcan4Failed, kCan_30_Flexcan4Requested, kCan_30_Flexcan4Ok). \n
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 canState, CanIf_ControllerModeType canIfState, uint8 transitionState);

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_IsHigherCanIdPrio()
 ***********************************************************************************************************/
/*! \brief         check CAN ID priority
 *  \details       compare two CAN IDs and check if first one is of higher priority.
 *  \param[in]     CanId1        first CAN ID for compare
 *  \param[in]     CanId2        second CAN ID for compare
 *  \return        CAN_OK        first CAN ID has higher priority
 *  \return        CAN_NOT_OK    first CAN ID has no higher priority
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_30_Flexcan4_IsHigherCanIdPrio(Can_IdType CanId1, Can_IdType CanId2);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_WritePrepare()
 ***********************************************************************************************************/
/*! \brief         TX preparation
 *  \details       Search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary.
 *                 calculate txPara member activeSendObject, and mailboxElement
 *                 Called by Can_30_Flexcan4_Write().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in,out] txPara           mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WritePrepare(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) 
/************************************************************************************************************
 *  CanHL_30_Flexcan4_WriteSavePduInfo()
 ***********************************************************************************************************/
/*! \brief         Save PduInfo before send
 *  \details       Cancellation or TxHwFifo need to save send data to get this information in confirmation context.
 *  \param[in,out] txPara                    mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WriteSavePduInfo(CanTxTransmissionParaStructPtr txPara);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_WritePrepareCancelTx()
 ***********************************************************************************************************/
/*! \brief         TX cancellation
 *  \details       Cancel the BasicCAN TX object or store message data for later cancelation
 *                 Called by CanHL_30_Flexcan4_WritePrepare().
 *  \param[in]     Controller                CAN controller. \n
 *                                           (only if not using "Optimize for one controller")
 *  \param[in,out] txPara                    mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WritePrepareCancelTx(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_WriteStart()
 ***********************************************************************************************************/
/*! \brief         TX start
 *  \details       Write ID, DLC and Data to FullCAN or BasicCAN object and set transmit request
 *                 Called by Can_30_Flexcan4_Write().
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in,out] txPara          mailbox information
 *  \return        CAN_NOT_OK      transmission failed.
 *  \return        CAN_OK          transmission successful.
 *  \return        CAN_BUSY        transition object busy - repeat it.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_30_Flexcan4_WriteStart(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);

/************************************************************************************************************
 *  CanHL_30_Flexcan4_GetActiveSendObject()
 ***********************************************************************************************************/
/*! \brief         Convert Handle
 *  \details       Hardware to logical Handle conversion.
 *  \param[in]     mailboxHandle   Handle to mailbox object
 *  \param[in]     mailboxElement  mailbox element
 *  \return        logical handle for the given hardware mailbox
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_30_Flexcan4ObjectHandle, STATIC_CODE) CanHL_30_Flexcan4_GetActiveSendObject(Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement);

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) 
/************************************************************************************************************
 *  CanHL_30_Flexcan4_GetActiveSendData()
 ***********************************************************************************************************/
/*! \brief         Convert Handle
 *  \details       Hardware to logical Handle conversion.
 *  \param[in]     mailboxHandle   Handle to mailbox object
 *  \param[in]     mailboxElement  mailbox element
 *  \return        start index in the data buffer for the given mailbox
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint16, STATIC_CODE) CanHL_30_Flexcan4_GetActiveSendData(Can_HwHandleType mailboxHandle, uint8 mailboxElement);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_RamCheckMailboxNotification()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \param[in]     htrh             Hrh or Hth of the hardware object
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RamCheckMailboxNotification(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitBegin()
 ***********************************************************************************************************/
/*! \brief         Start Initialization
 *  \details       LL implementation see token CanLL_30_Flexcan4_InitBegin description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitBegin(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

#if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitTxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX FullCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitTxFullCAN(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitTxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX BasicCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitTxBasicCAN(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);


#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitRxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX FullCAN
 *  \details       LL implementation see token CanLL_InitFullCANObj description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitRxFullCAN(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitRxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX BasicCAN
 *  \details       LL implementation see token CanLL_InitBasicCANObj description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitRxBasicCAN(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_InitEnd_InitMode()
 ***********************************************************************************************************/
/*! \brief         End Initialization
 *  \details       LL implementation see token CanLL_InitEnd_InitMode description.
 *                 Called by CanHL_30_Flexcan4_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitEnd_InitMode(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_RxBasicCanPolling()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN polling
 *  \details       RX BasicCAN polling handling.
 *                 Called by Can_30_Flexcan4_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxBasicCanPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_RxFullCanPolling()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN polling
 *  \details       RX FullCAN polling handling.
 *                 Called by Can_30_Flexcan4_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxFullCanPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_RxMsgReceivedNotification()
 ***********************************************************************************************************/
/*! \brief         RX notification
 *  \details       RX queue and notification handling for BasicCAN and FullCAN reception.
 *                 Called by CanHL_30_Flexcan4_BasicCanMsgReceived(),CanHL_30_Flexcan4_FullCanMsgReceived().
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxStructPtr     include general rx info and all LL specific data.
 *  \return        errorId         error identifier reporting to DET
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_RxMsgReceivedNotification(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanRxInfoStructPtr rxStructPtr);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_RxQueueExecution()
 ***********************************************************************************************************/
/*! \brief         RX queue
 *  \details       RX queue execution and notification.
 *                 Called by Can_30_Flexcan4_MainFunction_Read().
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxQueueExecution(void);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_TxConfirmationPolling()
 ***********************************************************************************************************/
/*! \brief         TX Confirmation intermediate call
 *  \details       intermediate call to do interrupt locks between polling call from LL and HL handling.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     mailboxHandle   mailbox handle
 *  \param[in]     mailboxElement  index of the mailboxElement (muxTx, FIFO)
 *  \param[in]     hwObjHandle     hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_TxConfirmationPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle mailboxHandle, Can_30_Flexcan4ObjectHandle mailboxElement, Can_30_Flexcan4ObjectHandle hwObjHandle);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_TxConfirmation()
 ***********************************************************************************************************/
/*! \brief         TX Confirmation handling
 *  \details       Called for a transmit or cancel event. Finish transmission by free send mailbox and call confirmation. \n
 *                 Called for FullCAN and BasicCAN handling.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     mailboxHandle   mailbox handle
 *  \param[in]     mailboxElement  index of the mailboxElement (muxTx, FIFO)
 *  \param[in]     hwObjHandle     transmitted hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_TxConfirmation(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle mailboxHandle, Can_30_Flexcan4ObjectHandle mailboxElement, Can_30_Flexcan4ObjectHandle hwObjHandle);


#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_SetCanPduInfo()
 ***********************************************************************************************************/
/*! \brief         copy the confirmation parameter info to the PduInfo struct
 *  \details       Called while confirmation handling to set data struct need for confirmation API2. \n
 *  \param[in,out] PduInfo             Pointer to local PduInfo struct
 *  \param[in]     txConfPara          Pointer to local variables of confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_SetCanPduInfo(Can_30_Flexcan4_PduInfoPtrType_var PduInfo, CanTxConfirmationParaStructPtr txConfPara);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_BasicCanMsgReceivedPolling()
 ***********************************************************************************************************/
/*! \brief         RX Indication intermediate call
 *  \details       intermediate call to do interrupt locks between polling call from LL and HL handling.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxMailboxHandle mailbox handle
 *  \param[in]     hwObjHandle     received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_BasicCanMsgReceivedPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle);
# endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_BasicCanMsgReceived()
 ***********************************************************************************************************/
/*! \brief         Handle reception
 *  \details       Receive BasicCAN data and call indication function.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxMailboxHandle mailbox handle
 *  \param[in]     hwObjHandle     received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_BasicCanMsgReceived(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle);
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/************************************************************************************************************
 *  CanHL_30_Flexcan4_FullCanMsgReceivedPolling()
 ***********************************************************************************************************/
/*! \brief         RX Indication intermediate call
 *  \details       intermediate call to do interrupt locks between polling call from LL and HL handling.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxMailboxHandle mailbox handle
 *  \param[in]     hwObjHandle     received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_FullCanMsgReceivedPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle);
# endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_FullCanMsgReceived()
 ***********************************************************************************************************/
/*! \brief         Handle reception
 *  \details       Receive FullCAN data and call indication function.
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxMailboxHandle mailbox handle
 *  \param[in]     hwObjHandle     received hardware handle
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_FullCanMsgReceived(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle);
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_ErrorHandling()
 ***********************************************************************************************************/
/*! \brief         BUSOFF handling
 *  \details       Handler for bus off situation.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_ErrorHandling(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY);

#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
/************************************************************************************************************
 *  CanHL_30_Flexcan4_ControllerErrorStatePassive()
 ***********************************************************************************************************/
/*! \brief         Security Event Reporting of Rx/Tx error counters
 *  \details       Forwards the Rx/Tx error counters to CanIf_ControllerErrorStatePassive()
 *  \param[in]     Controller       CAN controller (only if not using "Optimize for one controller")
 *  \param[in]     RxErrorCounter   Value of the Rx error counter
 *  \param[in]     TxErrorCounter   Value of the Tx error counter
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_ControllerErrorStatePassive( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint16 RxErrorCounter, uint16 TxErrorCounter );

/************************************************************************************************************
 *  CanHL_30_Flexcan4_ErrorNotification()
 ***********************************************************************************************************/
/*! \brief         Security Event Reporting of errors
 *  \details       Forwards the error type to CanIf_ErrorNotification()
 *  \param[in]     Controller       CAN controller (only if not using "Optimize for one controller")
 *  \param[in]     CanError         Reported CAN error
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_ErrorNotification( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_ErrorType CanError );
#endif /* CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING */


#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW) 
#endif

/************************************************************************************************************
 *  CanHL_30_Flexcan4_ReInit()
 ***********************************************************************************************************/
/*! \brief         Initialization of controller specific CAN hardware.
 *  \details       Initialization of the CAN controller hardware registers and CAN driver variables. \n
 *                 after that the controller is fully initialized and can be swtiched to "STOP Mode".
 *  \param[in]     Controller            CAN controller. \n
 *                                       (only if not using "Optimize for one controller")
 *  \param[in]     doRamCheck            RAM check exectuion (kCan_30_Flexcan4ExecuteRamCheck, kCan_30_Flexcan4SuppressRamCheck).
 *  \return        error                 return kCan_30_Flexcan4Failed in case of issue or kCan_30_Flexcan4Ok if initialization pass.
 *  \pre           controller is ready to be configured (INIT mode).
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         no mode change must be done inside. No CAN bus depended hardware loops allowed.
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, uint8, STATIC_CODE) CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST uint8 doRamCheck);

#define CAN_30_FLEXCAN4_STOP_SEC_STATIC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* CAN_30_FLEXCAN4_LOCAL_H */

/* End of channel */



/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/************   Organi, Version 3.9.2 Vector-Informatik GmbH  ************/
