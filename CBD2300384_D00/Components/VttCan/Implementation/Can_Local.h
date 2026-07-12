/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* STARTSINGLE_OF_MULTIPLE */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                              All rights reserved.
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
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Refer to the module's header file.
|************************************************************************** */

#if !defined(CAN_LOCAL_H)
#define  CAN_LOCAL_H

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/
/* \trace SPEC-1583 */
#include "SchM_Can.h"
#include "Can.h"         /* include also all needed types */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x1000u) /* \trace SPEC-1699 */
# error "Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x03u)
# error "Header file are inconsistent!"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_VERSION != 0x0601u)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN_CANOEEMUCANOEASR_RELEASE_VERSION != 0x01u)
# error "Source and Header file are inconsistent!"
#endif

# if( CAN_GEN_CanoeemuCanoeAsr_VERSION != 0x0102u)
#  error "Generated Data are inconsistent!"
# endif

/* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */

/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/
#if defined(C_ENABLE_MIXED_ID) || defined(C_ENABLE_EXTENDED_ID)
# define CAN_ID_IDE_MASK            0x80000000UL
# define CAN_ID_FD_MASK             0x40000000UL /* \trace SPEC-50553 */
# define CAN_ID_MASK                0x1FFFFFFFUL
# define CAN_ID_MASK_STD            0x000007FFUL
# define CAN_ID_UNUSED_MASK         0x20000000UL
#else
# define CAN_ID_IDE_MASK            0x0000U
# define CAN_ID_FD_MASK             0x4000U /* \trace SPEC-50553 */
# define CAN_ID_MASK                0x07FFU
# define CAN_ID_MASK_STD            0x07FFU
# define CAN_ID_UNUSED_MASK         0x3800U
#endif
#define kCanAllChannels  ((CanChannelHandle)0xFFU)

#if !defined(C_DRV_INTERNAL) /* COV_CAN_INTERNAL */
# error "Can_Local.h should only be included by Can.c"
#endif
/* real amount of controllers in this system (LT and PB) */
#define kCanNumberOfChannels  Can_GetControllerMax()
/* if kCanNumberOfChannels used to declare a table/dimension this table has to be generated to be also Link-Time-capable */
/*  -> see ESCAN00028069 controller-amount has to be Link-Time-capable */
/* real amount of controllers in this system (LT and PB) */
#define kCanNumberOfHwChannels  kCanNumberOfChannels

#if defined( C_SINGLE_RECEIVE_CHANNEL )
/* have to be done here to avoid declaration of "channel" in Can.h */
# define channel                                ((CanChannelHandle)0)
# define canHwChannel                           ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STARTINDEX(ch)       ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STOPINDEX(ch)        ((CanChannelHandle)0)
#else
# define channel                                Controller
# define canHwChannel                           Controller
# define CAN_HL_HW_CHANNEL_STARTINDEX(ch)       (ch)
# define CAN_HL_HW_CHANNEL_STOPINDEX(ch)        (ch)
#endif
#if !defined(CAN_TRANSITION_CHECK) /* COV_CAN_COMPATIBILITY */
# define CAN_TRANSITION_CHECK STD_ON /* default may be deactivated by user configuration */
#endif
#if !defined(CAN_REINIT_START) /* COV_CAN_COMPATIBILITY */
# define CAN_REINIT_START STD_OFF
#endif

#if !defined(CAN_OS_TICK2MS) /* COV_CAN_COMPATIBILITY */
# error "CAN_OS_TICK2MS() should be defined to OS_TICKS2MS_<counterShortName>() macro from OS"
#endif
#if !defined(CAN_OS_COUNTER_ID) /* COV_CAN_COMPATIBILITY */
# error "CAN_OS_COUNTER_ID should be defined to 'counterShortName' from OS"
#endif

#if !defined(CAN_ID_MASK_IN_GENERIC_CALLOUT) /* COV_CAN_COMPATIBILITY */
# define CAN_ID_MASK_IN_GENERIC_CALLOUT (CAN_ID_IDE_MASK | CAN_ID_FD_MASK | CAN_ID_MASK)
/* May be changed to mask out FD-bit: (CAN_ID_IDE_MASK | CAN_ID_MASK) */
#endif

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
/* for variable Can_ActiveSendState */
#define kCanBufferFree              (uint8)0xFF     /* mark objects as free */
#define kCanBufferCancel            (uint8)0xFE     /* mark object as currently cancelling */
#define kCanBufferCancelSw          (uint8)0xFD
#define kCanBufferSend              (uint8)0x01     /* occupied send object */

/* HL return values */
#define kCanFailed                  (uint8)0x00     /* similar to CAN_NOT_OK but value may differ --> converted in HL (LL only use kCan...) */
#define kCanOk                      (uint8)0x01     /* similar to CAN_OK */
#define kCanRequested               (uint8)0x03

#define kCanFalse                   kCanFailed
#define kCanTrue                    kCanOk

#define Can_GetControllerMax()                        Can_GetSizeOfControllerData()
#define Can_GetCanToCanIfChannelMapping(Index)        Can_GetCanIfChannelId((Index))

/* ControllerConfig */
#define Can_GetCanControllerDefaultBaudrate(logCh)    Can_GetCanControllerDefaultBaudrateOfControllerConfig(logCh)
#define Can_GetCanControllerDefaultBaudrateIdx(logCh) Can_GetCanControllerDefaultBaudrateIdxOfControllerConfig(logCh)
#if !defined(Can_GetMailboxRxBasicLengthOfControllerConfig) /* If no Object is available ComStackLib do not generate start-stop index */
# define Can_GetMailboxRxBasicLengthOfControllerConfig(logCh) 0u
# define Can_GetMailboxRxBasicStartIdxOfControllerConfig(logCh) 0u
# define Can_GetMailboxRxBasicEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_GetRxBasicHandleMax(logCh)                Can_GetMailboxRxBasicLengthOfControllerConfig(logCh)
#define Can_GetRxBasicHandleStart(logCh)              (Can_HwHandleType)Can_GetMailboxRxBasicStartIdxOfControllerConfig(logCh)
#define Can_GetRxBasicHandleStop(logCh)               (Can_HwHandleType)Can_GetMailboxRxBasicEndIdxOfControllerConfig(logCh)
#define Can_GetRxBasicHwStart(logCh)                  Can_GetRxBasicHwStartOfControllerConfig(logCh)
#define Can_GetRxBasicHwStop(logCh)                   Can_GetRxBasicHwStopOfControllerConfig(logCh)
#if !defined(Can_GetMailboxRxFullLengthOfControllerConfig)
# define Can_GetMailboxRxFullLengthOfControllerConfig(logCh) 0u
# define Can_GetMailboxRxFullStartIdxOfControllerConfig(logCh) 0u
# define Can_GetMailboxRxFullEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_GetRxFullHandleMax(logCh)                 Can_GetMailboxRxFullLengthOfControllerConfig(logCh)
#define Can_GetRxFullHandleStart(logCh)               (Can_HwHandleType)Can_GetMailboxRxFullStartIdxOfControllerConfig(logCh)
#define Can_GetRxFullHandleStop(logCh)                (Can_HwHandleType)Can_GetMailboxRxFullEndIdxOfControllerConfig(logCh)
#define Can_GetRxFullHwStart(logCh)                   Can_GetRxFullHwStartOfControllerConfig(logCh)
#define Can_GetRxFullHwStop(logCh)                    Can_GetRxFullHwStopOfControllerConfig(logCh)
#if !defined(Can_GetMailboxTxBasicLengthOfControllerConfig) /* COV_CAN_GENDATA_FAILURE */
# define Can_GetMailboxTxBasicLengthOfControllerConfig(logCh) 0u
# define Can_GetMailboxTxBasicStartIdxOfControllerConfig(logCh) 0u
# define Can_GetMailboxTxBasicEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_GetTxBasicHandleMax(logCh)                Can_GetMailboxTxBasicLengthOfControllerConfig(logCh)
#define Can_GetTxBasicHandleStart(logCh)              (Can_HwHandleType)Can_GetMailboxTxBasicStartIdxOfControllerConfig(logCh)
#define Can_GetTxBasicHandleStop(logCh)               (Can_HwHandleType)Can_GetMailboxTxBasicEndIdxOfControllerConfig(logCh)
#define Can_GetTxBasicHwStart(logCh)                  Can_GetTxBasicHwStartOfControllerConfig(logCh)
#define Can_GetTxBasicHwStop(logCh)                   Can_GetTxBasicHwStopOfControllerConfig(logCh)
#if !defined(Can_GetMailboxTxFullLengthOfControllerConfig)
# define Can_GetMailboxTxFullLengthOfControllerConfig(logCh) 0u
# define Can_GetMailboxTxFullStartIdxOfControllerConfig(logCh) 0u
# define Can_GetMailboxTxFullEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_GetTxFullHandleMax(logCh)                 Can_GetMailboxTxFullLengthOfControllerConfig(logCh)
#define Can_GetTxFullHandleStart(logCh)               (Can_HwHandleType)Can_GetMailboxTxFullStartIdxOfControllerConfig(logCh)
#define Can_GetTxFullHandleStop(logCh)                (Can_HwHandleType)Can_GetMailboxTxFullEndIdxOfControllerConfig(logCh)
#define Can_GetTxFullHwStart(logCh)                   Can_GetTxFullHwStartOfControllerConfig(logCh)
#define Can_GetTxFullHwStop(logCh)                    Can_GetTxFullHwStopOfControllerConfig(logCh)
#if !defined(Can_GetMailboxUnusedLengthOfControllerConfig)  /* COV_CAN_GENDATA_FAILURE */
# define Can_GetMailboxUnusedLengthOfControllerConfig(logCh) 0u
# define Can_GetMailboxUnusedStartIdxOfControllerConfig(logCh) 0u
# define Can_GetMailboxUnusedEndIdxOfControllerConfig(logCh) 0u
#endif
#define Can_GetUnusedHandleMax(logCh)                 Can_GetMailboxUnusedLengthOfControllerConfig(logCh)
#define Can_GetUnusedHandleStart(logCh)               (Can_HwHandleType)Can_GetMailboxUnusedStartIdxOfControllerConfig(logCh)
#define Can_GetUnusedHandleStop(logCh)                (Can_HwHandleType)Can_GetMailboxUnusedEndIdxOfControllerConfig(logCh)
#define Can_GetUnusedHwStart(logCh)                   Can_GetUnusedHwStartOfControllerConfig(logCh)
#define Can_GetUnusedHwStop(logCh)                    Can_GetUnusedHwStopOfControllerConfig(logCh)

/* Mailbox */
/* GetMailboxSize: logical size of mailbox - use LL specific generated data to get hardware object size if needed */
#define Can_GetMailboxSize(htrh)                      Can_GetMailboxSizeOfMailbox(htrh)
#define Can_GetMailboxHwHandle(htrh)                  Can_GetHwHandleOfMailbox(htrh)
#define Can_GetMailboxIDValue(htrh)                   Can_GetIDValueOfMailbox(htrh)
#define Can_GetMailboxType(htrh)                      Can_GetMailboxTypeOfMailbox(htrh)
#define Can_GetMailboxDataLen(htrh)                   Can_GetMaxDataLenOfMailbox(htrh)
#define Can_GetMailboxFdPadding(htrh)                 Can_GetFdPaddingOfMailbox(htrh)
#define Can_GetMailboxController(htrh)                (CanChannelHandle)Can_GetControllerConfigIdxOfMailbox(htrh)
#define Can_GetMailboxActiveSendObject(htrh)          Can_GetActiveSendObjectOfMailbox(htrh)
#define Can_GetMailboxActiveSendData(htrh)            Can_GetActiveSendDataOfMailbox(htrh)
#define Can_GetMailboxMemSecObjIndex(htrh)            Can_GetMemorySectionsIndexOfMailbox(htrh)
#define Can_IsMailboxIndivPolling(htrh)               Can_IsIsIndivPollingOfMailbox(htrh)
#define Can_GetMailboxTxHwFifo(htrh)                  Can_GetTxHwFifoOfMailbox(htrh)

/* MemorySectionInfo */
#define Can_GetMemSecAddress(memSec)                  Can_GetMemoryStartAddressOfMemorySectionInfo(memSec)
#define Can_GetMemSecStartIndex(memSec)               Can_GetMemorySectionStartOfMemorySectionInfo(memSec)

/* MemorySectionObjects */
#define Can_GetMemSecObjHwHandle(memSecObj)           Can_GetHwHandleOfMemorySectionObjects(memSecObj)
#define Can_GetMemSecObjMailboxElement(memSecObj)     Can_GetMailboxElementOfMemorySectionObjects(memSecObj)
#define Can_GetMemSecObjMailboxHandle(memSecObj)      Can_GetMailboxHandleOfMemorySectionObjects(memSecObj)
#define Can_IsMemSecObjIndivPolling(memSecObj)        Can_IsIsIndivPollingOfMemorySectionObjects(memSecObj)

/* VAR */
#define Can_GetLastInitObject(Index)                   Can_GetLastInitObjectOfControllerData(Index)
#define Can_SetLastInitObject(Index, Value)            Can_SetLastInitObjectOfControllerData(Index, Value)
#define Can_GetCanInterruptOldStatus(Index)            Can_GetCanInterruptOldStatusOfControllerData(Index)
#define Can_SetCanInterruptOldStatus(Index, Value)     Can_SetCanInterruptOldStatusOfControllerData(Index, Value)
#define Can_GetCanInterruptCounter(Index)              Can_GetCanInterruptCounterOfControllerData(Index)
#define Can_SetCanInterruptCounter(Index, Value)       Can_SetCanInterruptCounterOfControllerData(Index, Value)
#define Can_IncCanInterruptCounter(Index)              Can_IncCanInterruptCounterOfControllerData(Index)
#define Can_DecCanInterruptCounter(Index)              Can_DecCanInterruptCounterOfControllerData(Index)
#define Can_IsIsWakeup(Index)                          Can_IsIsWakeupOfControllerData(Index)
#define Can_SetIsWakeup(Index, Value)                  Can_SetIsWakeupOfControllerData(Index, Value)
#define Can_IsIsBusOff(Index)                          Can_IsIsBusOffOfControllerData(Index)
#define Can_SetIsBusOff(Index, Value)                  Can_SetIsBusOffOfControllerData(Index, Value)
#define Can_GetModeTransitionRequest(Index)            Can_GetModeTransitionRequestOfControllerData(Index)
#define Can_SetModeTransitionRequest(Index, Value)     Can_SetModeTransitionRequestOfControllerData(Index, Value)
#define Can_GetBusOffTransitionRequest(Index)          Can_GetBusOffTransitionRequestOfControllerData(Index)
#define Can_SetBusOffTransitionRequest(Index, Value)   Can_SetBusOffTransitionRequestOfControllerData(Index, Value)
#define Can_GetRamCheckTransitionRequest(Index)        Can_GetRamCheckTransitionRequestOfControllerData(Index)
#define Can_SetRamCheckTransitionRequest(Index, Value) Can_SetRamCheckTransitionRequestOfControllerData(Index, Value)
/* controller states */
#define Can_GetLogStatus(Index)                        Can_GetLogStatusOfControllerData(Index)
#define Can_SetLogStatus(Index, Value)                 Can_SetLogStatusOfControllerData(Index, Value)
#define Can_GetCanLoopTimeout(ch, Index)               (Can_GetLoopTimeoutOfControllerData(ch))[(Index)]
#define Can_GetMirrorModeState(Index)                  Can_GetMirrorModeStateOfControllerData(Index)
#define Can_SetMirrorModeState(Index, Value)           Can_SetMirrorModeStateOfControllerData(Index, Value)
#define Can_GetSilentModeState(Index)                  Can_GetSilentModeStateOfControllerData(Index)
#define Can_SetSilentModeState(Index, Value)           Can_SetSilentModeStateOfControllerData(Index, Value)
/* ActiveSend */
#define Can_GetActiveSendPdu(Index)                    Can_GetPduOfActiveSendObject(Index)
#define Can_SetActiveSendPdu(Index, Value)             Can_SetPduOfActiveSendObject(Index, Value)
#define Can_GetActiveSendState(Index)                  Can_GetStateOfActiveSendObject(Index)
#define Can_SetActiveSendState(Index, Value)           Can_SetStateOfActiveSendObject(Index, Value)
#define Can_GetActiveSendPduInfo(Index)                Can_GetPduInfoOfActiveSendObject(Index)
#define Can_GetActiveSendId(Index)                     (Can_GetPduInfoOfActiveSendObject(Index)).id
#define Can_SetActiveSendId(Index, Value)              (Can_GetPduInfoOfActiveSendObject(Index)).id = (Value)
#define Can_GetActiveSendLength(Index)                 (Can_GetPduInfoOfActiveSendObject(Index)).length
#define Can_SetActiveSendLength(Index, Value)          (Can_GetPduInfoOfActiveSendObject(Index)).length = (Value)
#define Can_GetActiveSendSdu(Index)                    (Can_GetPduInfoOfActiveSendObject(Index)).sdu
#define Can_SetActiveSendSdu(Index, Value)             (Can_GetPduInfoOfActiveSendObject(Index)).sdu = (Value)
#define Can_GetActiveSendSwPduHandle(Index)            (Can_GetPduInfoOfActiveSendObject(Index)).swPduHandle
#define Can_SetActiveSendSwPduHandle(Index, Value)     (Can_GetPduInfoOfActiveSendObject(Index)).swPduHandle = (Value)
/* Use already ComStackLib name #define Can_GetActiveSendData(Index)                   Can_GetActiveSendData(Index) */
/* TxHwFifo */
#define Can_GetTxHwFifoWriteIndex(fifo)                Can_GetWriteIndexOfTxHwFifo(fifo)
#define Can_SetTxHwFifoWriteIndex(fifo, Value)         Can_SetWriteIndexOfTxHwFifo(fifo, Value)
#define Can_IncTxHwFifoWriteIndex(fifo)                Can_IncWriteIndexOfTxHwFifo(fifo)
#define Can_GetTxHwFifoReadIndex(fifo)                 Can_GetReadIndexOfTxHwFifo(fifo)
#define Can_SetTxHwFifoReadIndex(fifo, Value)          Can_SetReadIndexOfTxHwFifo(fifo, Value)
#define Can_IncTxHwFifoReadIndex(fifo)                 Can_IncReadIndexOfTxHwFifo(fifo)
#define Can_GetTxHwFifoFillCount(fifo)                 Can_GetFillCountOfTxHwFifo(fifo)
#define Can_SetTxHwFifoFillCount(fifo, Value)          Can_SetFillCountOfTxHwFifo(fifo, Value)
#define Can_IncTxHwFifoFillCount(fifo)                 Can_IncFillCountOfTxHwFifo(fifo)
#define Can_DecTxHwFifoFillCount(fifo)                 Can_DecFillCountOfTxHwFifo(fifo)

# define  Can_GetRxMsg(Index)                   Can_GetRxMsgOfControllerData(Index)
# define  Can_GetgCanController(Index)          Can_GetgCanControllerOfControllerData(Index)
# define  Can_GetBusName(Index)                 Can_GetBusNameOfControllerConfig(Index)

/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
#define C_HL_DISABLE_OVERRUN_IN_STATUS

#define C_HL_DISABLE_HW_RANGES_FILTER

#define C_HL_ENABLE_IDTYPE_IN_ID

/* Specify the ID tables which contain the ID type */
# define C_LL_RX_IDTYPE_TABLE      CanRxId0
# define C_LL_TX_IDTYPE_TABLE      CanTxId0

#define C_HL_DISABLE_DUMMY_FCT_CALL

#define C_HL_DISABLE_TX_MSG_DESTROYED

#define C_HL_DISABLE_CANCEL_IN_HW_TASK

#define C_HL_ENABLE_HW_EXIT_TRANSMIT /* do not enter TX outside START mode - only emulated STOP! */

#define C_HL_ENABLE_CAN_IRQ_DISABLE

#define C_HL_DISABLE_ADJUST_RXHANDLE

#define C_HL_ENABLE_REJECT_UNWANTED_IDTYPE

#define C_HL_DISABLE_REJECT_REMOTE_FRAME

#define C_HL_DISABLE_REJECT_REMOTE_FRAME_FULLCAN

#define C_HL_ENABLE_REJECT_ILLEGAL_DLC

#define C_HL_DISABLE_COPROCESSOR_SUPPORT


#define C_HL_DISABLE_AVOID_REENTRANT_APPLCANCANCELNOTIFICATION

/* Possibility to disable the prototypes of interrupt service routines in the driver header file */

#define C_DISABLE_TX_ACTUAL_MACROS

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
#define CAN_LL_MIN(x,y) (((x)>(y)) ? (y) : (x)) /* COV_CAN_GENDATA_FAILURE */


#define kCanNumberOfUsedCanRxIdTables 1u
#define kCanNumberOfUsedCanTxIdTables 1u

/* macros to set DLC and ID in RX-,TX-structures -------------------------- */
/* MK... macros are used by the generation tool to fill the RX and TX structures */
/* In case the IDE bit is located in the ID registers, this bit has to be handled as must match.
   In general, this is done by setting the IDE bit to 0 in the .._MASK_.. macros.
*/

#define MK_RX_RANGE_MASK_IDSTD0(id)   ( MK_STDID0(id) & ~kCanIdTypeExt )
/*
#define MK_RX_RANGE_MASK_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_MASK_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_MASK_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_MASK_IDSTD4(id)   ( MK_STDID4(id) )
*/

#define MK_RX_RANGE_CODE_IDSTD0(id)   ( MK_STDID0(id) )
/*
#define MK_RX_RANGE_CODE_IDSTD1(id)   ( MK_STDID1(id) )
#define MK_RX_RANGE_CODE_IDSTD2(id)   ( MK_STDID2(id) )
#define MK_RX_RANGE_CODE_IDSTD3(id)   ( MK_STDID3(id) )
#define MK_RX_RANGE_CODE_IDSTD4(id)   ( MK_STDID4(id) )
*/

#define MK_RX_RANGE_MASK_IDEXT0(id)   ( MK_EXTID0(id)  & ~kCanIdTypeExt)
/*
#define MK_RX_RANGE_MASK_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_MASK_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_MASK_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_MASK_IDEXT4(id)   ( MK_EXTID4(id) )
*/

#define MK_RX_RANGE_CODE_IDEXT0(id)   ( MK_EXTID0(id))
/*
#define MK_RX_RANGE_CODE_IDEXT1(id)   ( MK_EXTID1(id) )
#define MK_RX_RANGE_CODE_IDEXT2(id)   ( MK_EXTID2(id) )
#define MK_RX_RANGE_CODE_IDEXT3(id)   ( MK_EXTID3(id) )
#define MK_RX_RANGE_CODE_IDEXT4(id)   ( MK_EXTID4(id) )
*/

#define MK_STDID0(id)            ( (vuint32)((vuint32)(id) & 0x07FFu) )
/*
#define MK_STDID1(id)            (0)
#define MK_STDID2(id)            (0)
#define MK_STDID3(id)            (0)
#define MK_STDID4(id)            (0)
*/

#define MK_EXTID0(id)            (  kCanIdTypeExt | (vuint32)((id) & 0x1FFFFFFFUL) )
/*
#define MK_EXTID1(id)            (0)
#define MK_EXTID2(id)            (0)
#define MK_EXTID3(id)            (0)
#define MK_EXTID4(id)            (0)
*/

#define MK_TX_DLC(dlc)       (dlc)
#define MK_TX_DLC_EXT(dlc)   (dlc)

/* extract  DLC from register format to logical format */
#define XT_TX_DLC(dlc)       ((vuint8)(dlc))        /* return code has to be vuint8 */

/* The macros CanRxActualIdRawx() have to provide the register context of the ID registers.
   It is not necessary to mask any bits. These macros are for internal use only and masking
   will be done in can_hls.c
*/
#if defined( C_ENABLE_EXTENDED_ID )
# define CanRxActualIdRaw0(rxStruct)     ((rxStruct)->pChipMsgObj->IdRaw)
/* #define CanRxActualIdRaw1(rxStruct)   0
   #define CanRxActualIdRaw2(rxStruct)   0
   #define CanRxActualIdRaw3(rxStruct)   0
   #define CanRxActualIdRaw4(rxStruct)   0 */
#else
# define CanRxActualIdRaw0(rxStruct)     (vuint16)((rxStruct)->pChipMsgObj->IdRaw)
/* #define CanRxActualIdRaw1(rxStruct)   0
   #define CanRxActualIdRaw2(rxStruct)   0
   #define CanRxActualIdRaw3(rxStruct)   0
   #define CanRxActualIdRaw4(rxStruct)   0 */
#endif

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */
#if defined( C_ENABLE_EXTENDED_ID )
# define CanRxActualIdExtHi(rxStruct)     ((vuint8) (((rxStruct)->pChipMsgObj->IdRaw) >> 24) & 0x7Fu )
# define CanRxActualIdExtMidHi(rxStruct)  ((vuint8) (((rxStruct)->pChipMsgObj->IdRaw) >> 16)         )
# define CanRxActualIdExtMidLo(rxStruct)  ((vuint8) (((rxStruct)->pChipMsgObj->IdRaw) >> 8 )         )
# define CanRxActualIdExtLo(rxStruct)     ((vuint8) (((rxStruct)->pChipMsgObj->IdRaw)      )         )
#endif

#define CanRxActualExtId(rxStruct)        ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & 0x1FFFFFFFUL))
#define CanRxActualStdId(rxStruct)        ((vuint16)(((rxStruct)->pChipMsgObj->IdRaw) & 0x7FFu))

#define CanRxActualData(rxStruct,i)       (((rxStruct)->pChipMsgObj->DataFld[i]))
#define CanRxActualDLC(rxStruct)          CAN_DLC2LEN((vuint8)(((rxStruct)->pChipMsgObj->DlcRaw) & 0x0Fu ))
#define CanRxActualIdType(rxStruct)       ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & kCanIdTypeExt ))

#define CanRxActualFdType(rxStruct)       ((vuint32)(((rxStruct)->pChipMsgObj->IdRaw) & kCanFdTypeFd ))

#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define CanRxActualErrorCounter()        ( Can_GetgCanController(0).ErrCntRx )
# define CanTxActualErrorCounter()        ( Can_GetgCanController(0).ErrCntTx )
#else
# define CanRxActualErrorCounter(ch)      ( Can_GetgCanController(ch).ErrCntRx )
# define CanTxActualErrorCounter(ch)      ( Can_GetgCanController(ch).ErrCntTx )
#endif

/* Macros for CAN message access within GenericConfirmation() function */
#define CanTxActualStdId(txStruct)        ((vuint16)((txStruct)->pChipMsgObj->IdRaw) & 0x7FFu)
#define CanTxActualExtId(txStruct)        ((vuint32)((txStruct)->pChipMsgObj->IdRaw) & 0x1FFFFFFFUL)
#define CanTxActualDLC(txStruct)          CAN_DLC2LEN((txStruct)->pChipMsgObj->DlcRaw)
#define CanTxActualIdType(txStruct)       ((vuint32)((txStruct)->pChipMsgObj->IdRaw) & kCanIdTypeExt )
#define CanTxActualData(txConfStruct,i)   ((vuint8)((txConfStruct)->pChipMsgObj->DataFld[i]))
#define CanTxActualFdType(txStruct)       ((vuint32)(((txStruct)->pChipMsgObj->IdRaw) & kCanFdTypeFd ))

/* Macros for CAN message access within PreTransmit() function */
#if defined( C_ENABLE_CAN_FD_USED )
# define CanTxWriteActExtId(txStruct,id)   {(txStruct).pChipMsgObj->IdRaw = (MK_EXTID0(id) | kCanIdTypeExt) & kCanFdTypeFdMaskOut;}
# define CanTxWriteActFdExtId(txStruct,id) {(txStruct).pChipMsgObj->IdRaw = MK_EXTID0(id) | kCanIdTypeExt | kCanFdTypeFd;}
# define CanTxWriteActId(txStruct,id)      {(txStruct).pChipMsgObj->IdRaw = MK_STDID0(id) & kCanFdTypeFdMaskOut;}
# define CanTxWriteActFdId(txStruct,id)    {(txStruct).pChipMsgObj->IdRaw = MK_STDID0(id) | kCanFdTypeFd;}
#else
# define CanTxWriteActExtId(txStruct,id)   {(txStruct).pChipMsgObj->IdRaw = MK_EXTID0(id) | kCanIdTypeExt;}
# define CanTxWriteActId(txStruct,id)      {(txStruct).pChipMsgObj->IdRaw = MK_STDID0(id);}
#endif
#define CanTxWriteActDLC(txStruct,len)     {(txStruct).pChipMsgObj->DlcRaw = (vuint8)(CAN_LEN2DLC(len));}

/* Macros to fill variables of the type tCanMsgTransmitStruct */
#if defined( C_ENABLE_CAN_FD_USED )
# define CanMsgTransmitSetStdId(pCanMsgTransmitStruct, id)              {(pCanMsgTransmitStruct)->IdRaw = MK_STDID0(id) & kCanFdTypeFdMaskOut;}
# define CanMsgTransmitSetFdStdId(pCanMsgTransmitStruct, id)            {(pCanMsgTransmitStruct)->IdRaw = MK_STDID0(id) | kCanFdTypeFd | kCanFdBrsTypeTrue;}
# define CanMsgTransmitSetExtId(pCanMsgTransmitStruct, id)              {(pCanMsgTransmitStruct)->IdRaw = (MK_EXTID0(id) | kCanIdTypeExt) & kCanFdTypeFdMaskOut;}
# define CanMsgTransmitSetFdExtId(pCanMsgTransmitStruct, id)            {(pCanMsgTransmitStruct)->IdRaw = MK_EXTID0(id) | kCanIdTypeExt | kCanFdTypeFd | kCanFdBrsTypeTrue;}
#else
# define CanMsgTransmitSetStdId(pCanMsgTransmitStruct, id)               {(pCanMsgTransmitStruct)->IdRaw  = MK_STDID0(id);}
# define CanMsgTransmitSetExtId(pCanMsgTransmitStruct, id)               {(pCanMsgTransmitStruct)->IdRaw  = MK_EXTID0(id) | kCanIdTypeExt;}
#endif
#if defined( C_ENABLE_CAN_FD_FULL )
# define  CanMsgTransmitSetDlc(pCanMsgTransmitStruct, len)                {(pCanMsgTransmitStruct)->DlcRaw = MK_TX_DLC(CAN_LEN2DLC(len)); (pCanMsgTransmitStruct)->Len = (len);}
#else
# define  CanMsgTransmitSetDlc(pCanMsgTransmitStruct, len)                {(pCanMsgTransmitStruct)->DlcRaw = MK_TX_DLC(CAN_LEN2DLC(len));}
#endif
#define  CanMsgTransmitSetData(pCanMsgTransmitStruct, dataByte, msgData) {(pCanMsgTransmitStruct)->DataFld[dataByte] = (msgData);}

/* macros for buffer access */
#define StartRxReadSync()
#define EndRxReadSync()

#define StartRxWriteSync()
#define EndRxWriteSync()

#define StartTxReadSync()
#define EndTxReadSync()

#define StartTxWriteSync()
#define EndTxWriteSync()

#define StartRxWriteCANSync()
#define EndRxWriteCANSync()

#define StartRxReadCANSync()
#define EndRxReadCANSync()

#define StartRxFullCANReadSync(Obj)   
#define EndRxFullCANReadSync(Obj)     

#define StartRxFullCANWriteSync(obj)
#define EndRxFullCANWriteSync(obj)

/* macros for flag access */
#define CanStartFlagWriteSync()
#define CanEndFlagWriteSync()

#define CAN_HL_MIN(x, y) (((x)>(y)) ? (y) : (x))
#define CAN_HL_MAX(x, y) (((x)<(y)) ? (y) : (x))

/***************************************************************************************************/
/* Macros for dummy statements */
#if !defined(CAN_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_DUMMY_STATEMENT(x) (x) = (x) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define CAN_DUMMY_STATEMENT(x)
# endif
#endif
#if !defined(CAN_DUMMY_STATEMENT_CONST) /* COV_CAN_COMPATIBILITY */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
#  define CAN_DUMMY_STATEMENT_CONST(x) (void)(x)
# else
#  define CAN_DUMMY_STATEMENT_CONST(x)
# endif
#endif

/***************************************************************************************************/
/* Macros for DET and assertions */
#if (CAN_DEV_ERROR_REPORT == STD_ON) /* \trace SPEC-1404, SPEC-1696 */
/* \trace SPEC-1725, SPEC-1403, SPEC-1568 */
# define Can_CallDetReportError(api, err) ((void) Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, (api), (err)))
# define assertHardware(a, b, c)  if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertInternal(a, b, c)  if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertUser(a, b, c)      if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define assertGen(a, b, c)       if (!(a)) {Can_CallDetReportError(CAN_HW_ACCESS_ID, (c));}
# define C_ENABLE_HARDWARE_CHECK
# define C_ENABLE_INTERNAL_CHECK
# define C_ENABLE_USER_CHECK
# define C_ENABLE_GEN_CHECK
#else
# define Can_CallDetReportError(api, err)
# define assertHardware(a, b, c)
# define assertInternal(a, b, c)
# define assertUser(a, b, c)
# define assertGen(a, b, c)
#endif

/***************************************************************************************************/
/* Macros for Critical Area 0 abstraction */

#define CanDeclareGlobalInterruptOldStatus
/* CAN_EXCLUSIVE_AREA_0 called only by LL code */
#define CanSingleGlobalInterruptDisable()     SchM_Enter_Can_CAN_EXCLUSIVE_AREA_0()
#define CanSingleGlobalInterruptRestore()     SchM_Exit_Can_CAN_EXCLUSIVE_AREA_0()
#define CanGlobalInterruptDisable()           SchM_Enter_Can_CAN_EXCLUSIVE_AREA_0()
#define CanGlobalInterruptRestore()           SchM_Exit_Can_CAN_EXCLUSIVE_AREA_0()
#define CanNestedGlobalInterruptDisable()     SchM_Enter_Can_CAN_EXCLUSIVE_AREA_0()
#define CanNestedGlobalInterruptRestore()     SchM_Exit_Can_CAN_EXCLUSIVE_AREA_0()

/***************************************************************************************************/
/* Macros for LL-HLL compatibility */
#if defined(C_ENABLE_SLEEP_WAKEUP)
# define APPL_CAN_WAKEUP(ch)   (CanHL_WakeUpHandling( CAN_CHANNEL_CANPARA_ONLY ))
# define CAN_WAKEUP(ch)              ((void) Can_SetControllerMode((uint8)(ch), CAN_T_WAKEUP))
#endif


/***************************************************************************/
/* CAN-Hardware Data Definitions                                           */
/***************************************************************************/

/***************************************************************************/
/* Special switches need by CAN driver                                     */
/***************************************************************************/

/***************************************************************************/
/* Defines / data types / structures / unions                              */
/***************************************************************************/
/* Define CAN Chip hardware; segment must be located in locator file    */
/* register layout of the can chip                                      */
/* Structure describing CAN receive buffer. */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
# if( kCanNumberOfUsedCanRxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
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
# elif ( kCanNumberOfUsedCanRxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
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
# elif ( kCanNumberOfUsedCanRxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
#  define SET_PARASTRUCT_IDRAW_RX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id)
#  define SET_PARASTRUCT_IDRAW_RX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id)
# elif ( kCanNumberOfUsedCanRxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
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

#if( kCanNumberOfUsedCanTxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
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
#elif ( kCanNumberOfUsedCanTxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
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
#elif ( kCanNumberOfUsedCanTxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
# define SET_PARASTRUCT_IDRAW_TX_STD(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_STDID0(id); \
                                   (paraStruct)->idRaw1 = MK_STDID1(id); \
                                   (paraStruct)->idRaw2 = MK_STDID2(id)
# define SET_PARASTRUCT_IDRAW_TX_EXT(paraStruct, id) \
                                   (paraStruct)->idRaw0 = MK_EXTID0(id); \
                                   (paraStruct)->idRaw1 = MK_EXTID1(id); \
                                   (paraStruct)->idRaw2 = MK_EXTID2(id)
#elif ( kCanNumberOfUsedCanTxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
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
typedef struct sCanRxInfoStruct
{
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
  CanObjectHandle   localMailboxHandle; 
  Can_IdType               localId; 
  uint8                    localDlc;
  /* LL specific part */
} tCanRxInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxInfoStruct, VAR_NOINIT) CanRxInfoStructPtr;

typedef struct sCanTxConfInfoStruct
{
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
  /* LL specific part */
} tCanTxConfInfoStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanTxConfInfoStruct, VAR_NOINIT) CanTxConfInfoStructPtr;

typedef struct sCanInitParaStruct
{
  CanInitHandle     initObject;
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   hwObjHandle;
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
  tCanRxId0                idRaw0;
# if( kCanNumberOfUsedCanRxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
  tCanRxId1                idRaw1;
# endif
# if( kCanNumberOfUsedCanRxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
  tCanRxId2                idRaw2;
# endif
# if( kCanNumberOfUsedCanRxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
  tCanRxId3                idRaw3;
# endif
# if( kCanNumberOfUsedCanRxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
  tCanRxId4                idRaw4;
# endif
  tCanIdType               idType;
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK)
  uint8                    doRamCheck;
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  uint8                    isChRamCheckFail;
# endif
#endif
  uint8                    isInitOk;
  /* LL specific part */
  volatile Can_ControllerType *canLLController;
} tCanInitParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanInitParaStruct, VAR_NOINIT) CanInitParaStructPtr;

#define TxDataPtr Can_SduPtrType /* CBD compatibility */
typedef struct sCanTxTransmissionParaStruct
{
  /* logical formats */
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  CanObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  CanObjectHandle   activeSendObject; /* index of the logical Tx mailbox (Tx * Element) */
  tCanTxId0                idRaw0;
#if( kCanNumberOfUsedCanTxIdTables > 1 ) /* COV_CAN_COMPATIBILITY */
  tCanTxId1                idRaw1;
#endif
#if( kCanNumberOfUsedCanTxIdTables > 2 ) /* COV_CAN_COMPATIBILITY */
  tCanTxId2                idRaw2;
#endif
#if( kCanNumberOfUsedCanTxIdTables > 3 ) /* COV_CAN_COMPATIBILITY */
  tCanTxId3                idRaw3;
#endif
#if( kCanNumberOfUsedCanTxIdTables > 4 ) /* COV_CAN_COMPATIBILITY */
  tCanTxId4                idRaw4;
#endif
  tCanDlc                  dlcRaw;
  TxDataPtr                CanMemCopySrcPtr;
#if defined( C_ENABLE_CAN_FD_USED )
  tCanFdType               fdType;
  tCanFdBrsType            fdBrsType;
  uint8                    messageLen;
  uint8                    frameLen;
  uint8                    paddingVal;
#endif
  Can_PduType              pdu;
  /* LL specific part */
  volatile Can_ObjectType *canLLMsgObj;
} tCanTxTransmissionParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanTxTransmissionParaStruct, VAR_NOINIT) CanTxTransmissionParaStructPtr;

typedef struct sCanRxBasicParaStruct
{
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   hwObjHandle;
#if defined( C_ENABLE_OVERRUN )
  uint8                    isOverrun;
#endif
  tCanRxInfoStruct         rxStruct;
  /* LL specific part */
  volatile Can_ObjectType *canLLMsgObj;
} tCanRxBasicParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxBasicParaStruct, VAR_NOINIT) CanRxBasicParaStructPtr;

typedef struct sCanRxFullParaStruct
{
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   hwObjHandle;
#if defined( C_ENABLE_FULLCAN_OVERRUN )
  uint8                    isOverrun;
#endif
  tCanRxInfoStruct         rxStruct;
  /* LL specific part */
  volatile Can_ObjectType *canLLMsgObj;
} tCanRxFullParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanRxFullParaStruct, VAR_NOINIT) CanRxFullParaStructPtr;

#if defined( C_ENABLE_CANCEL_IN_HW ) || defined( C_ENABLE_CANCEL_SUPPORT_API )
typedef struct
{
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  CanObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  CanObjectHandle   activeSendObject; /* index of the logical mailbox + muxTx or FIFO element 0..n */
  CanTransmitHandle   canHandleCurTxObj;
} tCanTxCancellationParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanTxCancellationParaStruct, VAR_NOINIT) CanTxCancellationParaStructPtr;
#endif

typedef struct
{
  CanObjectHandle   mailboxHandle;
  CanObjectHandle   mailboxElement; /* logical index within the mailbox for muxTx or FIFO element */
  CanObjectHandle   hwObjHandle;    /* set to the initial hwHandle by core but has to be modified by LL in case of muxTx or FIFO */
  CanObjectHandle   activeSendObject; /* index of the logical mailbox + muxTx or FIFO element 0..n */
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
  CanTxConfInfoStructPtr   txStructConf;
#endif
  volatile Can_ObjectType *canLLMsgObj;
} tCanTxConfirmationParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanTxConfirmationParaStruct, VAR_NOINIT) CanTxConfirmationParaStructPtr;

typedef struct sCanTaskParaStruct
{
  CanObjectHandle mailboxHandle;
  CanObjectHandle hwObjHandle;
  /* LL specific part */
} tCanTaskParaStruct;
V_DEF_P2VAR_TYPE(V_NONE, tCanTaskParaStruct, VAR_NOINIT) CanTaskParaStructPtr;
    
/***************************************************************************/
/* module global variable declaration                                      */
/***************************************************************************/

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/
#define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***************************************************************************/
/* local function prototypes (only internal usage)                         */
/***************************************************************************/
#define CAN_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/************************************************************************************************************
 *  CanLL_WriteReg8()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \param[in]     doRamCheck       execute the RAM check (kCanExecuteRamCheck, kCanSuppressRamCheck).
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg8(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr8 regPtr, uint8 value, uint8 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/************************************************************************************************************
 *  CanLL_WriteReg16()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \param[in]     doRamCheck       execute the RAM check (kCanExecuteRamCheck, kCanSuppressRamCheck).
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg16(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr16 regPtr, uint16 value, uint16 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/************************************************************************************************************
 *  CanLL_WriteReg32()
 ***********************************************************************************************************/
/*! \brief         Write value and do RamCheck
 *  \details       Write the value of the hardware mailbox or controller register and read back for RamCheck. 
 *                 set initPara->isChRamCheckFail or initPara->isMbRamCheckFail in case of an HW issue to deactivate hardware.
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     regPtr           valid pointer to Register to be written.
 *  \param[in]     value            value to be written.
 *  \param[in]     readMask         mask to check the read back value.
 *  \param[in]     doRamCheck       execute the RAM check (kCanExecuteRamCheck, kCanSuppressRamCheck).
 *  \return        kCanTrue         RamCheck failed.
 *  \return        kCanFalse        RamCheck passed.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg32(CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr32 regPtr, uint32 value, uint32 readMask, CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitBegin()
 *********************************************************************************************************************/
/*! \brief         Starts the channel initialization
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_InitBeginIsRegisterCorrupt()
 *********************************************************************************************************************/
/*! \brief         Performs the channel register RAM check
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanFalse           No corrupt register RAM found
 *                 kCanTrue            Register RAM is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginIsRegisterCorrupt(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_InitBeginSetRegisters()
 *********************************************************************************************************************/
/*! \brief         Performs the channel register initialization
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginSetRegisters(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitMailboxTx()
 *********************************************************************************************************************/
/*! \brief         Initializes a transmit mailbox
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxTx(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

#if defined(C_ENABLE_CAN_RAM_CHECK)
/**********************************************************************************************************************
 *  CanLL_InitMailboxTxDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a transmit mailbox if it is considered corrupt by the RAM check
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxTxDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_InitMailboxUnused()
 *********************************************************************************************************************/
/*! \brief         Initializes a unused mailbox
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxUnused(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxFullCan()
 *********************************************************************************************************************/
/*! \brief         Initializes a receive FullCAN mailbox
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxRxFullCan(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxFullCanDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a receive FullCAN mailbox if it is considered corrupt by the RAM check
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxRxFullCanDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
# endif

#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxBasicCan()
 *********************************************************************************************************************/
/*! \brief         Initializes a receive BasicCAN mailbox
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxRxBasicCan(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxBasicCanDeactivate()
 *********************************************************************************************************************/
/*! \brief         Deactivates a receive BasicCAN mailbox if it is considered corrupt by the RAM check
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxRxBasicCanDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
# endif
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_InitEndIsRegisterCorrupt()
 *********************************************************************************************************************/
/*! \brief         Finishes the channel register RAM check
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanFalse           No corrupt register RAM found
 *                 kCanTrue            Register RAM is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndIsRegisterCorrupt(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_InitEndSetRegisters()
 *********************************************************************************************************************/
/*! \brief         Finishes the channel register initialization
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndSetRegisters(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitEnd()
 *********************************************************************************************************************/
/*! \brief         Finishes the channel initialization
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitIsMailboxCorrupt()
 *********************************************************************************************************************/
/*! \brief         Performs the RAM check for a mailbox
 *  \details       Called by CanInit()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] initPara            Pointer to local variables of CanInit()
 *  \return        kCanFalse           Mailbox is not corrupt
 *                 kCanTrue            Mailbox is considered corrupt or failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitIsMailboxCorrupt(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
/* CODE CATEGORY 4 END */
#endif


/**********************************************************************************************************************
 *  CanLL_InitPowerOn()
 *********************************************************************************************************************/
/*! \brief         Performs the hardware specific global module initialization
 *  \details       Called by CanInitPowerOn()
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitPowerOn(void);
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitMemoryPowerOn()
 *********************************************************************************************************************/
/*! \brief         Internal function, called by Can_InitMemory().
 *  \details       Performs the platform specific memory initialization.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMemoryPowerOn(void);
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitPowerOnChannelSpecific()
 *********************************************************************************************************************/
/*! \brief         Performs the channel dependent hardware specific global module initialization
 *  \details       Called by CanInitPowerOn()
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanOk              Successfully completed
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitPowerOnChannelSpecific(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_TxCancelInHw()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxCancelInHw(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxCancellationParaStructPtr txCancellationPara);
/* CODE CATEGORY 3 END */
#endif
/**********************************************************************************************************************
 *  CanLL_TxBegin()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_TxSetMailbox()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxSetMailbox(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_TxCopyToCan()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxCopyToCan(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_TxStart()
 *********************************************************************************************************************/
/*! \brief         Trigger mailbox to start the transmission
 *  \details       Called by transmission to start transmission
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txPara              Pointer to local variables of transmission
 *  \return        kCanOk              No issue
 *                 kCanFailed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxStart(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_TxEnd()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
/* CODE CATEGORY 1 END */
#if defined( C_ENABLE_TX_POLLING )
/**********************************************************************************************************************
 *  CanLL_TxIsGlobalConfPending()
 *********************************************************************************************************************/
/*! \brief         Check global pending of transmission
 *  \details       Called by transmission to get global pending flag
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanTrue            pending confirmation
 *                 kCanFailed          no pending confirmation
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxIsGlobalConfPending(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_TxProcessPendings()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxProcessPendings(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
#endif
/**********************************************************************************************************************
 *  CanLL_TxConfBegin()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#if defined( C_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_TxConfIsMsgTransmitted()
 *********************************************************************************************************************/
/*! \brief         Check message is transmitted
 *  \details       Called by confirmation
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] txConfPara          Pointer to local variables of confirmation
 *  \return        kCanTrue            message is transmitted
 *                 kCanFailed          message is not transmitted
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxConfIsMsgTransmitted(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#endif
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
/**********************************************************************************************************************
 *  CanLL_TxConfSetTxConfStruct()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfSetTxConfStruct(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */
#endif
/**********************************************************************************************************************
 *  CanLL_TxConfEnd()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara);
/* CODE CATEGORY 1 END */

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_RxBasicMsgReceivedBegin()
 *********************************************************************************************************************/
/*! \brief         Perform BasicCAN receive begin
 *  \details       Called by reception to begin handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \return        kCanOk              No Issue occur
 *                 kCanFailed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxBasicMsgReceivedBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_RxBasicReleaseObj()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicReleaseObj(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_RxBasicMsgReceivedEnd()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicMsgReceivedEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara);
/* CODE CATEGORY 1 END */
# if defined( C_ENABLE_RX_BASICCAN_POLLING ) /* COV_CAN_RX_BASICCAN_POLLING */
/**********************************************************************************************************************
 *  CanLL_RxBasicIsGlobalIndPending()
 *********************************************************************************************************************/
/*! \brief         Check BasicCAN receive global pending
 *  \details       Called by reception to get pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanTrue            global pending
 *                 kCanFailed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxBasicIsGlobalIndPending(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_RxBasicProcessPendings()
 *********************************************************************************************************************/
/*! \brief         Check BasicCAN receive pending
 *  \details       Called by reception to get mailbox pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] taskPara            Pointer to local variables of reception task
 *  \return        kCanTrue            mailbox pending
 *                 kCanFailed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicProcessPendings(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
# endif /* C_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */
#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_RxFullMsgReceivedBegin()
 *********************************************************************************************************************/
/*! \brief         Perform FullCAN receive begin
 *  \details       Called by reception to begin handling
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] rxBasicPara         Pointer to local variables of reception
 *  \return        kCanOk              No Issue occur
 *                 kCanFailed          Issue occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxFullMsgReceivedBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_RxFullReleaseObj()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullReleaseObj(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
/**********************************************************************************************************************
 *  CanLL_RxFullMsgReceivedEnd()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullMsgReceivedEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara);
/* CODE CATEGORY 1 END */
# if defined( C_ENABLE_RX_FULLCAN_POLLING ) /* COV_CAN_RX_FULLCAN_POLLING */
/**********************************************************************************************************************
 *  CanLL_RxFullIsGlobalIndPending()
 *********************************************************************************************************************/
/*! \brief         Check FullCAN receive global pending
 *  \details       Called by reception to get pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanTrue            global pending
 *                 kCanFailed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxFullIsGlobalIndPending(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_RxFullProcessPendings()
 *********************************************************************************************************************/
/*! \brief         Check FullCAN receive pending
 *  \details       Called by reception to get mailbox pending state
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in,out] taskPara            Pointer to local variables of reception task
 *  \return        kCanTrue            mailbox pending
 *                 kCanFailed          no pending
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullProcessPendings(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara);
/* CODE CATEGORY 2 END */
# endif /* C_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) 
/**********************************************************************************************************************
 *  CanLL_ErrorStatePassiveTransitionOccured()
 *********************************************************************************************************************/
/*! \brief         Error state passive check
 *  \details       Checks if a transition to error state passive occured
 *  \param[in]     channel            Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanTrue    Transition to passive occured
 *                 kCanFailed  Transition to passive has not occured 
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_ErrorStatePassiveTransitionOccured(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_ErrorNotificationHandling()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorNotificationHandling(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
#endif /* CAN_ENABLE_SECURITY_EVENT_REPORTING */

/**********************************************************************************************************************
 *  CanLL_ErrorHandlingBegin()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorHandlingBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_BusOffOccured()
 *********************************************************************************************************************/
/*! \brief         Check BusOff occur
 *  \details       Called by error handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \return        kCanTrue            BusOff occur
 *                 kCanFailed          no BusOff occur
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_BusOffOccured(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
/**********************************************************************************************************************
 *  CanLL_ErrorHandlingEnd()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorHandlingEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END */
#if defined( C_ENABLE_EXTENDED_STATUS )
/**********************************************************************************************************************
 *  CanLL_GetStatusBegin()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_GetStatusBegin(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 3 END */
#endif
#if defined( C_ENABLE_SLEEP_WAKEUP )
/**********************************************************************************************************************
 *  CanLL_WakeUpHandling()
 *********************************************************************************************************************/
/*! \brief         Perform Wakeup handling
 *  \details       Called by wakeup handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
#endif
/**********************************************************************************************************************
 *  CanLL_ModeTransition()
 *********************************************************************************************************************/
/*! \brief         Perform mode change
 *  \details       Called by mode handler
 *  \param[in]     channel             Index of the CAN channel (only if multiple channels are used)
 *  \param[in]     mode                mode to be set
 *  \param[in]     busOffRecovery      BusOff recovery should be done or not
 *  \param[in]     ramCheck            RamCheck should be done or not
 *  \return        kCanOk              Successfully completed
 *                 kCanRequested       Mode not yet reached
 *                 kCanFailed          Failure occured
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
**********************************************************************************************************************/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_ModeTransition(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 busOffRecovery, vuint8 doRamCheck);
/* CODE CATEGORY 4 END */
#if defined( C_ENABLE_STOP ) /* COV_CAN_COMPATIBILITY */
/**********************************************************************************************************************
 *  CanLL_StopReinit()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_StopReinit(CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END */
#endif

#if defined( C_ENABLE_CAN_CAN_INTERRUPT_CONTROL ) && !defined( C_ENABLE_OSEK_CAN_INTCTRL ) && defined( C_HL_ENABLE_CAN_IRQ_DISABLE ) /* COV_CAN_OS_USAGE TX tx xf tx */
/************************************************************************************************************
 *  CanLL_CanInterruptDisable()
 ***********************************************************************************************************/
/*! \brief         Disables CAN interrupts.
 *  \details       Stores current state and disables the individual CAN interrupt sources.
 *  \param[in]     canHwChannel              CAN channel
 *  \param[out]    localInterruptOldFlagPtr  Pointer to global variable that holds the interrupt state
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 ***********************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptDisable(CAN_HW_CHANNEL_CANTYPE_FIRST tCanLLCanIntOldPtr localInterruptOldFlagPtr);
/* CODE CATEGORY 1 END */

/************************************************************************************************************
 *  CanLL_CanInterruptRestore()
 ***********************************************************************************************************/
/*! \brief         Restores CAN interrupts.
 *  \details       Restores the previous state of the individual CAN interrupt sources.
 *  \param[in]     canHwChannel              CAN channel
 *  \param[in]     localInterruptOldFlag     Global variable that holds the interrupt state
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 ***********************************************************************************************************/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptRestore(CAN_HW_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld localInterruptOldFlag);
/* CODE CATEGORY 1 END */
#endif


#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS ) || defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  CanLL_RxIndicationMsgCopy()
 ***********************************************************************************************************/
/*! \brief         Copy Message data
 *  \details       finish Can_RxIndication by copy data to emulated CAN cell.
 *  \param[in]     canChannel            CAN controller
 *  \param[in]     mailbox               mailbox handle
 *  \param[in]     rxObjHandle           hardware handle
 *  \param[in]     id                    CAN ID
 *  \param[in]     flags                 additional message information
 *  \param[in]     dataLength            data length of the received message
 *  \param[in]     data                  pointer to the received message data
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxIndicationMsgCopy(vuint8 canChannel, CanObjectHandle mailbox, CanObjectHandle hwObjHandle, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data);
/* CODE CATEGORY 1 END */
#endif

/************************************************************************************************************
 *  CanHL_CleanUpSendState()
 ***********************************************************************************************************/
/*! \brief         Clean up send state
 *  \details       Reset active send state (semaphore blocking TX while send).
 *                 Called by Can_SetControllerMode(), Can_MainFunction_Mode() (CanHL_ReInit() do same but without this call).
 *  \param[in]     Controller    CAN controller \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_CleanUpSendState(CAN_CHANNEL_CANTYPE_ONLY);

# if (CAN_HW_LOOP_SUPPORT_API == STD_OFF)
/************************************************************************************************************
 *  ApplCanTimerLoop()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) ApplCanTimerLoop(CAN_CHANNEL_CANTYPE_FIRST uint8 source);
# endif

/************************************************************************************************************
 *  CanHL_ModeTransition()
 ***********************************************************************************************************/
/*! \brief         Request mode transition
 *  \details       Handle/Request transition and wait asynchronous-short time.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller         CAN controller (Controller must be in valid range). \n
 *                                    (only if not using "Optimize for one controller") \n
 *  \param[in]     transitionRequest  transition request used to switch mode (kCanModeInit, kCanModeSleep, kCanModeWakeupStopReinit, \n
 *                                    kCanModeStopReinit, kCanModeStopReinitFast, kCanModeStart, kCanModeStartReinit, kCanModeSilent, \n
 *                                    kCanModeResetBusOffStart, kCanModeResetBusOffEnd). \n
 *  \param[in]     busOffRecovery     handling of the busoff behaviour (kCanFinishBusOffRecovery, kCanContinueBusOffRecovery). \n
 *  \param[in]     doRamCheck         do a RAM check while reinit or not (kCanExecuteRamCheck, kCanSuppressRamCheck). \n
 *  \return        kCanFailed         transition rejected
 *  \return        kCanRequested      transition requested but is ongoing asynchronous
 *  \return        kCanOk             transition finished successful
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_ModeTransition(CAN_CHANNEL_CANTYPE_FIRST uint8 transitionRequest, uint8 busOffRecovery, uint8 doRamCheck);

/************************************************************************************************************
 *  CanHL_NotifyTransition()
 ***********************************************************************************************************/
/*! \brief         notify about sucessful mode transition
 *  \details       notify to upper layer.
 *                 Called by Can_SetControllerMode().
 *  \param[in]     Controller         CAN controller (Controller must be in valid range). \n
 *                                    (only if not using "Optimize for one controller") \n
 *  \param[in]     canState           CAN controller mode to be switched to (CAN_STATUS_STOP, CAN_STATUS_START, CAN_STATUS_SLEEP). \n
 *  \param[in]     canIfState         CAN controller mode to be switched to (CANIF_CS_STOPPED, CANIF_CS_STARTED, CANIF_CS_SLEEP). \n
 *  \param[in]     transitionState    transition request state (kCanFailed, kCanRequested, kCanOk). \n
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_NotifyTransition(CAN_CHANNEL_CANTYPE_FIRST uint8 canState, CanIf_ControllerModeType canIfState, uint8 transitionState);

#if defined(C_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_IsHigherCanIdPrio()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_IsHigherCanIdPrio(Can_IdType CanId1, Can_IdType CanId2);
#endif

/************************************************************************************************************
 *  CanHL_WritePrepare()
 ***********************************************************************************************************/
/*! \brief         TX preparation
 *  \details       Search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary.
 *                 calculate txPara member activeSendObject, and mailboxElement
 *                 Called by Can_Write().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in,out] txPara           mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepare(CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);

#if defined(C_ENABLE_CANCEL_IN_HW) || \
   (defined(C_ENABLE_TX_HW_FIFO) && defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) )
/************************************************************************************************************
 *  CanHL_WriteSavePduInfo()
 ***********************************************************************************************************/
/*! \brief         Save PduInfo before send
 *  \details       Cancellation or TxHwFifo need to save send data to get this information in confirmation context.
 *  \param[in,out] txPara                    mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WriteSavePduInfo(CanTxTransmissionParaStructPtr txPara);
#endif

#if defined(C_ENABLE_CANCEL_IN_HW)
/************************************************************************************************************
 *  CanHL_WritePrepareCancelTx()
 ***********************************************************************************************************/
/*! \brief         TX cancellation
 *  \details       Cancel the BasicCAN TX object or store message data for later cancelation
 *                 Called by CanHL_WritePrepare().
 *  \param[in]     Controller                CAN controller. \n
 *                                           (only if not using "Optimize for one controller")
 *  \param[in,out] txPara                    mailbox information
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepareCancelTx(CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);
#endif

/************************************************************************************************************
 *  CanHL_WriteStart()
 ***********************************************************************************************************/
/*! \brief         TX start
 *  \details       Write ID, DLC and Data to FullCAN or BasicCAN object and set transmit request
 *                 Called by Can_Write().
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
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WriteStart(CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara);

/************************************************************************************************************
 *  CanHL_GetActiveSendObject()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_GetActiveSendObject(Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement);

#if defined(C_ENABLE_CANCEL_IN_HW) || \
   (defined(C_ENABLE_TX_HW_FIFO) && defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) )
/************************************************************************************************************
 *  CanHL_GetActiveSendData()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, uint16, STATIC_CODE) CanHL_GetActiveSendData(Can_HwHandleType mailboxHandle, uint8 mailboxElement);
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
/************************************************************************************************************
 *  CanHL_RamCheckMailboxNotification()
 ***********************************************************************************************************/
/*! \brief         RAM check
 *  \details       Notify upper layer in case of an HW issue and deactivate controller.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \param[in]     htrh             Hrh or Hth of the hardware object
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_InitBegin()
 ***********************************************************************************************************/
/*! \brief         Start Initialization
 *  \details       LL implementation see token CanLL_InitBegin description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitBegin(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_InitTxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX FullCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara          include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxFullCAN(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_InitTxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize TX BasicCAN
 *  \details       LL implementation see token CanLL_InitTxObj description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxBasicCAN(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

/************************************************************************************************************
 *  CanHL_InitUnusedObj()
 ***********************************************************************************************************/
/*! \brief         Initialize unused mailbox objects
 *  \details       LL implementation see token CanLL_InitUnusedObj description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitUnusedObj(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/************************************************************************************************************
 *  CanHL_InitRxFullCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX FullCAN
 *  \details       LL implementation see token CanLL_InitFullCANObj description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxFullCAN(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/************************************************************************************************************
 *  CanHL_InitRxBasicCAN()
 ***********************************************************************************************************/
/*! \brief         Initialize RX BasicCAN
 *  \details       LL implementation see token CanLL_InitBasicCANObj description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxBasicCAN(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);
#endif

/************************************************************************************************************
 *  CanHL_InitEnd_InitMode()
 ***********************************************************************************************************/
/*! \brief         End Initialization
 *  \details       LL implementation see token CanLL_InitEnd_InitMode description.
 *                 Called by CanHL_ReInit().
 *  \param[in]     Controller       CAN controller. \n
 *                                  (only if not using "Optimize for one controller")
 *  \param[in]     initPara         include controller and all LL specific settings.
 *  \return        errorId
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_InitMode(CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara);

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/************************************************************************************************************
 *  CanHL_RxBasicCanPolling()
 ***********************************************************************************************************/
/*! \brief         RX BasicCAN polling
 *  \details       RX BasicCAN polling handling.
 *                 Called by Can_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicCanPolling(CAN_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/************************************************************************************************************
 *  CanHL_RxFullCanPolling()
 ***********************************************************************************************************/
/*! \brief         RX FullCAN polling
 *  \details       RX FullCAN polling handling.
 *                 Called by Can_MainFunctionRead().
 *  \param[in]     Controller    CAN controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullCanPolling(CAN_HW_CHANNEL_CANTYPE_ONLY);
# endif
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/************************************************************************************************************
 *  CanHL_RxMsgReceivedNotification()
 ***********************************************************************************************************/
/*! \brief         RX notification
 *  \details       RX queue and notification handling for BasicCAN and FullCAN reception.
 *                 Called by CanHL_BasicCanMsgReceived(),CanHL_FullCanMsgReceived().
 *  \param[in]     Controller      CAN controller. \n
 *                                 (only if not using "Optimize for one controller")
 *  \param[in]     rxStructPtr     include general rx info and all LL specific data.
 *  \return        errorId         error identifier reporting to DET
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANTYPE_FIRST CanRxInfoStructPtr rxStructPtr);
#endif

#if defined(C_ENABLE_RX_QUEUE)
/************************************************************************************************************
 *  CanHL_RxQueueExecution()
 ***********************************************************************************************************/
/*! \brief         RX queue
 *  \details       RX queue execution and notification.
 *                 Called by Can_MainFunction_Read().
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxQueueExecution(void);
#endif

#if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/************************************************************************************************************
 *  CanHL_TxConfirmationPolling()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_TxConfirmationPolling(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle mailboxHandle, CanObjectHandle mailboxElement, CanObjectHandle hwObjHandle);
#endif

/************************************************************************************************************
 *  CanHL_TxConfirmation()
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
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_TxConfirmation(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle mailboxHandle, CanObjectHandle mailboxElement, CanObjectHandle hwObjHandle);

#if defined(C_ENABLE_TX_HW_FIFO)
/************************************************************************************************************
 *  CanHL_GetNextTxHwFifoReadIndex()
 ***********************************************************************************************************/
/*! \brief         get TxHwFifo element and increment the read index
 *  \details       Called while confirmation handling to handle the TX hardware FIFO index. \n
 *  \param[in]     txConfPara          Pointer to local variables of confirmation
 *  \return        MailboxElement      index of the next mailbox element in the FIFO
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, CanObjectHandle, STATIC_CODE) CanHL_GetNextTxHwFifoReadIndex(CanTxConfirmationParaStructPtr txConfPara);

/************************************************************************************************************
 *  CanHL_SetNextTxHwFifoWriteIndex()
 ***********************************************************************************************************/
/*! \brief         increment TxHwFifo write index
 *  \details       Called while transmission to handle the TX hardware FIFO index. \n
 *  \param[in]     txPara          Pointer to write parameters
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_SetNextTxHwFifoWriteIndex(CanTxTransmissionParaStructPtr txPara);
#endif

#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
/************************************************************************************************************
 *  CanHL_SetCanPduInfo()
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
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_SetCanPduInfo(Can_PduInfoPtrType_var PduInfo, CanTxConfirmationParaStructPtr txConfPara);
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/************************************************************************************************************
 *  CanHL_BasicCanMsgReceivedPolling()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_BasicCanMsgReceivedPolling(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle);
# endif

/************************************************************************************************************
 *  CanHL_BasicCanMsgReceived()
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
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_BasicCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle);
#endif

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/************************************************************************************************************
 *  CanHL_FullCanMsgReceivedPolling()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_FullCanMsgReceivedPolling(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle);
# endif

/************************************************************************************************************
 *  CanHL_FullCanMsgReceived()
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
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_FullCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle);
#endif

/************************************************************************************************************
 *  CanHL_ErrorHandling()
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
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_ErrorHandling(CAN_HW_CHANNEL_CANTYPE_ONLY);

#if defined (CAN_ENABLE_SECURITY_EVENT_REPORTING)
/************************************************************************************************************
 *  CanHL_ControllerErrorStatePassive()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_ControllerErrorStatePassive( CAN_CHANNEL_CANTYPE_FIRST uint16 RxErrorCounter, uint16 TxErrorCounter );

/************************************************************************************************************
 *  CanHL_ErrorNotification()
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_ErrorNotification( CAN_CHANNEL_CANTYPE_FIRST Can_ErrorType CanError );
#endif /* CAN_ENABLE_SECURITY_EVENT_REPORTING */

# if defined(C_ENABLE_SLEEP_WAKEUP)
/************************************************************************************************************
 *  CanHL_WakeUpHandling()
 ***********************************************************************************************************/
/*! \brief         WAKUP handling
 *  \details       Handler for WAKEUP event. Call EcuM for notification. Will be called in case of WAKEUP occurred.
 *  \param[in]     Controller    CAN controller. \n
 *                               (only if not using "Optimize for one controller")
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
***********************************************************************************************************/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);
# endif

#if defined(C_ENABLE_CANCEL_TX_IN_HW) 
#endif

/************************************************************************************************************
 *  CanHL_ReInit()
 ***********************************************************************************************************/
/*! \brief         Initialization of controller specific CAN hardware.
 *  \details       Initialization of the CAN controller hardware registers and CAN driver variables. \n
 *                 after that the controller is fully initialized and can be swtiched to "STOP Mode".
 *  \param[in]     Controller            CAN controller. \n
 *                                       (only if not using "Optimize for one controller")
 *  \param[in]     doRamCheck            RAM check exectuion (kCanExecuteRamCheck, kCanSuppressRamCheck).
 *  \return        error                 return kCanFailed in case of issue or kCanOk if initialization pass.
 *  \pre           controller is ready to be configured (INIT mode).
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         no mode change must be done inside. No CAN bus depended hardware loops allowed.
***********************************************************************************************************/
V_DEF_FUNC(CAN_STATIC, uint8, STATIC_CODE) CanHL_ReInit(CAN_HW_CHANNEL_CANTYPE_FIRST uint8 doRamCheck);

#define CAN_STOP_SEC_STATIC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* CAN_LOCAL_H */

/* End of channel */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCAN Erzeugungsgangnummer: 955 */

