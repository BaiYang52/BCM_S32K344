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

/* \trace SPEC-1570 */
/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_INTERNAL /* compatibility */
#define CAN_SOURCE     /* testability */
#define __CAN_SRC__       /* compatibility RTM not aware of multi driver instance */ /* PRQA S 0602,0603 */ /* MD_Can_ModuleDefine */
#if !defined(__CAN_SRC__) /* driver use ORGANI without infix */ /* COV_CAN_INTERNAL */
# define __CAN_SRC__      /* for all MSR module (used by RTM) */ /* PRQA S 0602,0603 */ /* MD_Can_ModuleDefine */
#endif

/* \trace SPEC-1408, SPEC-1590, SPEC-1588 */
#if !defined( MISRA_CHECK ) /* COV_CAN_LL_EMULATION */
# include <string.h>
#endif

# include "VttCntrl_Can.h"
/* \trace SPEC-1392 */
#include "CanIf_Cbk.h"   /* for callback declaration */
/* \trace SPEC-1707 */
#include "CanIf.h"       /* for version check */
#include "Can_Local.h"    /* have to be last include due to channel define will lead to problems */

#if (CAN_DEV_ERROR_REPORT == STD_ON)
/* \trace SPEC-1596 */
# include "Det.h"
#endif

#if !defined(CAN_AMD_RUNTIME_MEASUREMENT) /* COV_CAN_COMPATIBILITY */
# define CAN_AMD_RUNTIME_MEASUREMENT STD_OFF
#endif
#if (CAN_AMD_RUNTIME_MEASUREMENT == STD_ON) /* COV_CAN_AMD_RUNTIME_MEASUREMENT */
# include "AmdRtm.h"
#endif

#if !defined(CAN_RUNTIME_MEASUREMENT_SUPPORT) /* COV_CAN_COMPATIBILITY */
# define CAN_RUNTIME_MEASUREMENT_SUPPORT STD_OFF
#endif
#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
/* AR4-328 */
# define C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT
# include "Rtm.h"
#endif

#include "vstdlib.h"

/* ECO_IGNORE_BLOCK_END */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_COREVERSION           != 0x1000u) /* \trace SPEC-1699 */
# error "Source and Header file are inconsistent!"
#endif
#if (CAN_RELEASE_COREVERSION   != 0x03u)
# error "Source and Header file are inconsistent!"
#endif
#if defined(CAN_GEN_BASE_CFG5_VERSION) /* COV_CAN_COMPATIBILITY */
# if (CAN_GEN_BASE_CFG5_VERSION         != 0x0103u)
#  error "Source and Generated Header file are inconsistent!"
# endif
#else
# error "No CAN_GEN_BASE_CFG5_VERSION is defined"
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

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
#if defined(CAN_USE_NO_VECTOR_IF) /* for testability */
#else
# if defined(CANIF_VENDOR_ID) /* COV_CAN_COMPATIBILITY */
#  if (CANIF_VENDOR_ID == 0x001E) /* COV_CAN_COMPATIBILITY */
#   define CAN_USE_VECTOR_IF
#  endif
# endif
#endif
/* Compatibility to CBD */
/* instead of direct canCanInterruptCounter[] use Can_GetCanInterruptCounter(Index) abstraction for ComStackLib */

#if (CAN_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
# define CanHookBegin_Can_Init()                              Rtm_Start(RtmConf_RtmMeasurementPoint_Can_Init)
# define CanHookEnd_Can_Init()                                Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_Init)
# define CanHookBegin_Can_SetControllerMode()                 Rtm_Start(RtmConf_RtmMeasurementPoint_Can_SetControllerMode)
# define CanHookEnd_Can_SetControllerMode()                   Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_SetControllerMode)
# define CanHookBegin_Can_Write()                             Rtm_Start(RtmConf_RtmMeasurementPoint_Can_Write)
# define CanHookEnd_Can_Write()                               Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_Write)

#else /* BRS runtime measurement */
# define CanHookBegin_Can_Init()
# define CanHookEnd_Can_Init()
# define CanHookBegin_Can_SetControllerMode() \

# define CanHookEnd_Can_SetControllerMode() \

# define CanHookBegin_Can_Write() \

# define CanHookEnd_Can_Write() \

#endif

# define CanHookBegin_CanHL_ReInit() \

# define CanHookEnd_CanHL_ReInit() \

#define CanHookBegin_CanHL_TxConfirmation() \

#define CanHookEnd_CanHL_TxConfirmation() \

#define CanHookBegin_CanHL_BasicCanMsgReceived() \

#define CanHookEnd_CanHL_BasicCanMsgReceived() \

#define CanHookBegin_CanHL_FullCanMsgReceived() \

#define CanHookEnd_CanHL_FullCanMsgReceived() \

#define CanHookBegin_Can_MainFunction_Write() \

#define CanHookEnd_Can_MainFunction_Write() \

#define CanHookBegin_Can_MainFunction_Read() \

#define CanHookEnd_Can_MainFunction_Read() \

/* Define chiphardware                     */
/* Constants concerning can chip registers */
/* control registers for msg objects       */

/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/

/*! CANoe API is an indirect / abstracted access to hardware / please refer to CANoeAPI description */
/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */
typedef double CanoeApiBaudrateType;
typedef int    CanoeApiDLCType;
# if (CANOEAPI_VERSION < 204) /* support CAN FD */
#  error "This CANOEAPI_VERSION is not supported"
# endif
# define CANCELL_ERRORACTIVE   VTTCAN_ERR_ACTIVE          /* notify Active  is reached */
# define CANCELL_ERRORPASSIVE  VTTCAN_ERR_PASSIVE         /* notify Passive is reached */
# define CANCELL_ERRORPASSIVE_EVENT  9                    /* Passive event occur set to CANCELL_ERRORPASSIVE after execution */
# define CANCELL_WARNINGLIMIT  VTTCAN_ERR_WARNINGLIMIT    /* notify Warning is reached */
# define CANCELL_BUSOFF        VTTCAN_ERR_BUSOFF          /* notify BUSOFF  is reached */

# define CanCell_InitChannel(busName, ch, initObj, mode)      VttCan_SetCanConfiguration((vuint8)(ch), \
                                                                                         (CanoeApiBaudrateType)Can_GetarbBaudrateOfInitObject(initObj), Can_GetarbTseg1OfInitObject(initObj), \
                                                                                         Can_GetarbTseg2OfInitObject(initObj), Can_GetarbSjwOfInitObject(initObj), \
                                                                                         Can_GetarbSamOfInitObject(initObj), (mode), \
                                                                                         (CanoeApiBaudrateType)Can_GetdbrBaudrateOfInitObject(initObj), Can_GetdbrTseg1OfInitObject(initObj), \
                                                                                         Can_GetdbrTseg2OfInitObject(initObj), Can_GetdbrSjwOfInitObject(initObj), \
                                                                                         Can_GetdbrSamOfInitObject(initObj), (mode)); \
                                                              VttCan_SetMode((ch), VTTCAN_MODE_INIT)
# define CanCell_StopChannel(busName, ch)                     VttCan_SetMode((ch), VTTCAN_MODE_STOP)
# define CanCell_StartChannel(busName, ch)                    VttCan_SetMode((ch), VTTCAN_MODE_START)
# define CanCell_SleepChannel(busName, ch)                    VttCan_SetMode((ch), VTTCAN_MODE_SLEEP)
# define CanCell_WakeupChannel(busName, ch)                   VttCan_SetMode((ch), VTTCAN_MODE_WAKEUP)
# define CanCell_SendMessage(ch, id, dataLength, data, flags) VttCan_SendCanMessage((ch), (id), (flags), 0u, CAN_DLC2LEN(dataLength), (data))
# define CanCell_EnableInterrupts(ch, tx, rx, err, wakeup)    Can_GetgCanController(ch).IntEnable = (vuint8)((vuint8)((tx)*CAN_INT_TX) + (vuint8)((rx)*CAN_INT_RX) + (vuint8)((err)*CAN_INT_ERR) + (vuint8)((wakeup)*CAN_INT_WAKEUP)); \
                                                                VttCan_EnableISR((vuint8)(ch), (vuint8)(tx), (vuint8)(rx), (vuint8)(err), (vuint8)(wakeup))
# define CanCell_CallInterrupts(ch)                           /* done by Interrupt controller */
# define CanCell_AtomicBegin()                                /* ESCAN00106620 no more needed: VttCan_AtomicBegin() */
# define CanCell_AtomicEnd()                                  /* ESCAN00106620 no more needed: VttCan_AtomicEnd() */
#define assertHardwareReturn(condition, ch, id) assertHardware((condition), (ch), (id)); if(!(condition)) { \
                                                                                           return; \
                                                                                         }
/* PRQA L:FctLikeMacroLabel */

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */

/* for LL compatibility */
/* hw handles */
#define CAN_HL_HW_TX_NORMAL_INDEX(hwch)     (Can_GetTxBasicHwStart(hwch)) 
# define CAN_HL_HW_TX_STARTINDEX(hwch)      (Can_GetTxFullHwStart(hwch))
#define CAN_HL_HW_TX_FULL_STARTINDEX(hwch)  (Can_GetTxFullHwStart(hwch))
#define CAN_HL_HW_UNUSED_STARTINDEX(hwch)   (Can_GetUnusedHwStart(hwch))
#define CAN_HL_HW_RX_FULL_STARTINDEX(hwch)  (Can_GetRxFullHwStart(hwch))
#define CAN_HL_HW_RX_BASIC_STARTINDEX(hwch) (Can_GetRxBasicHwStart(hwch))
# define CAN_HL_HW_TX_STOPINDEX(hwch)       (Can_GetTxBasicHwStop(hwch))
#define CAN_HL_HW_TX_FULL_STOPINDEX(hwch)   (Can_GetTxFullHwStop(hwch))
#define CAN_HL_HW_UNUSED_STOPINDEX(hwch)    (Can_GetUnusedHwStop(hwch))
#define CAN_HL_HW_RX_FULL_STOPINDEX(hwch)   (Can_GetRxFullHwStop(hwch))
#define CAN_HL_HW_RX_BASIC_STOPINDEX(hwch)  (Can_GetRxBasicHwStop(hwch))
/* mailbox handles */
#define CAN_HL_MB_RX_FULL_STARTINDEX(hwch)  (Can_GetRxFullHandleStart(hwch))
#define CAN_HL_MB_RX_BASIC_STARTINDEX(hwch) (Can_GetRxBasicHandleStart(hwch))
#define CAN_HL_MB_TX_FULL_STARTINDEX(hwch)  (Can_GetTxFullHandleStart(hwch))
#define CAN_HL_MB_TX_BASIC_STARTINDEX(hwch) (Can_GetTxBasicHandleStart(hwch))
#define CAN_HL_MB_RX_FULL_STOPINDEX(hwch)   (Can_GetRxFullHandleStop(hwch))
#define CAN_HL_MB_RX_BASIC_STOPINDEX(hwch)  (Can_GetRxBasicHandleStop(hwch))
#define CAN_HL_MB_TX_FULL_STOPINDEX(hwch)   (Can_GetTxFullHandleStop(hwch))
#define CAN_HL_MB_TX_BASIC_STOPINDEX(hwch)  (Can_GetTxBasicHandleStop(hwch))

#if defined(C_ENABLE_CAN_RAM_CHECK)
# if !defined(CAN_RAM_CHECK_MAILBOX_RESULT) /* May be defined by test suite to stimulate RAM_CHECK failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_MAILBOX_RESULT(ch, initParaPtr) CanLL_InitIsMailboxCorrupt((initParaPtr))
#  else
#   define CAN_RAM_CHECK_MAILBOX_RESULT(ch, initParaPtr) CanLL_InitIsMailboxCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_FINISHED_CTP) /* May be defined by test suite to check RAM_CHECK finished / executed */
#  define CAN_RAM_CHECK_FINISHED_CTP(ch)
# endif
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
# if !defined(CAN_RAM_CHECK_BEGIN_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_BEGIN_REG_RESULT(ch, initParaPtr) CanLL_InitBeginIsRegisterCorrupt((initParaPtr))
#  else
#   define CAN_RAM_CHECK_BEGIN_REG_RESULT(ch, initParaPtr) CanLL_InitBeginIsRegisterCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_END_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#   define CAN_RAM_CHECK_END_REG_RESULT(ch, initParaPtr) CanLL_InitEndIsRegisterCorrupt((initParaPtr))
#  else
#   define CAN_RAM_CHECK_END_REG_RESULT(ch, initParaPtr) CanLL_InitEndIsRegisterCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_RAM_CHECK_READ_BACK_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  define CAN_RAM_CHECK_READ_BACK_RESULT(ch)
# endif
#endif

#define CanHL_IsStart(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_START) == CAN_STATUS_START)
#define CanHL_IsSleep(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_SLEEP) == CAN_STATUS_SLEEP)
#define CanHL_IsStop(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_STOP) == CAN_STATUS_STOP)
#define CanHL_IsBusOff(ch) (Can_IsIsBusOff(ch))
#define CanHL_IsControllerInit(ch) ((Can_GetLogStatus(ch) & CAN_STATUS_INIT) == CAN_STATUS_INIT)

#if defined(C_ENABLE_CAN_FD_USED) /* also for BRS because DLC may be bigger than 8 and will be checked by CanIf */
# define CAN_DLC2LEN(dlc) (Can_DlcToFrameLenght[((uint8)(dlc)) & ((uint8)0xFu)])
# define CAN_LEN2DLC(length) ((uint8)Can_MessageLengthToDlc[(length)])
#else
# define CAN_DLC2LEN(dlc) (dlc)
# define CAN_LEN2DLC(length) (length)
#endif
#if defined(C_ENABLE_CAN_FD_FULL)
# define CAN_MAX_DATALEN_OBJ(Index) Can_GetMailboxDataLen(Index)
#else
# define CAN_MAX_DATALEN_OBJ(Index) 8u
#endif
#define CanHL_IsFdMessage(id) (((id) & (Can_IdType)CAN_ID_FD_MASK) == (Can_IdType)CAN_ID_FD_MASK) /* \trace SPEC-60432, SPEC-50586 */
#define CanHL_IsFdTxBrs(ch)   (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(ch)) == CAN_FD_RXTX) /* \trace SPEC-60432, SPEC-50586 */

/* PRQA L:FctLikeMacroLabel */

/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */
/*
|<DataModelStart>| CanLL_TxIsObjFree
Relation_Context:
Relation:
ChannelAmount, TxBasicAmount
ChannelAmount, TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_TxIsObjFree( hwCh, txObjHandle ) \
                (Can_GetgCanController(hwCh).MsgObj[txObjHandle].mIsFree == (vuint8)kCanTrue)

/*
|<DataModelStart>| CanLL_HwIsSleep
Relation_Context:
Relation:
Wakeup, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#if defined( C_ENABLE_SLEEP_WAKEUP )
# define CanLL_HwIsSleep( hwCh ) (Can_GetgCanController(hwCh).Mode == CAN_MODE_SLEEP)
#else
# define CanLL_HwIsSleep( hwCh ) (kCanFalse != kCanFalse)  /* avoid misra warning */
#endif

/*
|<DataModelStart>| CanLL_HwIsStop
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/

/*
|<DataModelStart>| CanLL_HwIsBusOff
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_HwIsBusOff( hwCh ) \
                         (Can_GetgCanController(hwCh).ErrCode == (vuint32)CANCELL_BUSOFF)

#if defined( C_ENABLE_EXTENDED_STATUS )
/*
|<DataModelStart>| CanLL_HwIsPassive
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# define CanLL_HwIsPassive( hwCh ) \
                           ((Can_GetgCanController(hwCh).ErrCode == (vuint32)CANCELL_ERRORPASSIVE) || (Can_GetgCanController(hwCh).ErrCode == (vuint32)CANCELL_ERRORPASSIVE_EVENT))

/*
|<DataModelStart>| CanLL_HwIsWarning
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# define CanLL_HwIsWarning( hwCh )  \
                           (Can_GetgCanController(hwCh).ErrCode == (vuint32)CANCELL_WARNINGLIMIT)
#endif /* C_ENABLE_EXTENDED_STATUS */

#if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(loop)
#else
# define CanLL_ApplCanTimerStart(loop)  ApplCanTimerStart(channel, loop)
# define CanLL_ApplCanTimerEnd(loop)    ApplCanTimerEnd(channel, loop)
# define CanLL_ApplCanTimerLoop(loop)   ApplCanTimerLoop(channel, loop)
#endif

/* PRQA L:FctLikeMacroLabel */

/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/
#define CAN_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* BR:012 */
V_DEF_CONST(V_NONE, uint8, CONST) Can_MainVersion         = (uint8)CAN_SW_MAJOR_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
V_DEF_CONST(V_NONE, uint8, CONST) Can_SubVersion          = (uint8)CAN_SW_MINOR_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
V_DEF_CONST(V_NONE, uint8, CONST) Can_ReleaseVersion      = (uint8)CAN_SW_PATCH_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */

#if defined(C_ENABLE_CAN_FD_USED)
V_DEF_CONST(CAN_STATIC, uint8, CONST) Can_DlcToFrameLenght[16] = /* PRQA S 3218,3408 */ /* MD_Can_GlobalScope */
{
  0u,  1u,  2u,  3u,
  4u,  5u,  6u,  7u,
  8u, 12u, 16u, 20u,
 24u, 32u, 48u, 64u
};
V_DEF_CONST(CAN_STATIC, uint8, CONST) Can_MessageLengthToDlc[65] = /* PRQA S 3218,3408 */ /* MD_Can_GlobalScope */
{
/* 00..07 */  0u,  1u,  2u,  3u,  4u,  5u,  6u,  7u,
/* 08..15 */  8u,  9u,  9u,  9u,  9u, 10u, 10u, 10u,
/* 16..23 */ 10u, 11u, 11u, 11u, 11u, 12u, 12u, 12u,
/* 24..31 */ 12u, 13u, 13u, 13u, 13u, 13u, 13u, 13u,
/* 32..39 */ 13u, 14u, 14u, 14u, 14u, 14u, 14u, 14u,
/* 40..47 */ 14u, 14u, 14u, 14u, 14u, 14u, 14u, 14u,
/* 48..55 */ 14u, 15u, 15u, 15u, 15u, 15u, 15u, 15u,
/* 56..63 */ 15u, 15u, 15u, 15u, 15u, 15u, 15u, 15u,
/* 64     */ 15u
};
#endif

/* Global constants with CAN driver main and subversion */

#define CAN_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/
#define CAN_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* The emulated CAN-Controller */

/***************************************************************************************/
/* local data definitions   (need also INFIX because STATIC may be defined to nothing) */
/***************************************************************************************/
/* \trace SPEC-1585 */
/*! Semaphore to block reentrancy for Can_MainFunction_Read() */
V_DEF_VAR(CAN_STATIC, uint8, VAR_NOINIT) canIsRxTaskLocked; /* PRQA S 3408 */ /* MD_Can_GlobalScope */

#define CAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ! Mark module to be initialized (used to check double/none initialization) */
V_DEF_VAR(CAN_STATIC, uint8, VAR_INIT) canConfigInitFlag = CAN_STATUS_UNINIT; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* not static because of debug/test usage */
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Pointer to the current configuration */
V_DEF_P2CONST (V_NONE, Can_ConfigType, VAR_INIT, CONST_PBCFG) Can_ConfigDataPtr = NULL_PTR; /* UREQ00035484 */ /* PRQA S 3408 */ /* MD_Can_ExternalScope */
# define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#else
# define CAN_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Pointer to the current configuration */
V_DEF_CONSTP2CONST(V_NONE, Can_ConfigType, CONST, CONST_PBCFG) Can_ConfigDataPtr = NULL_PTR; /* PRQA S 3408,1514 */ /* MD_Can_ExternalScope */
# define CAN_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/***************************************************************************/
/*  Safe Check                                                             */
/***************************************************************************/
#if !defined(CAN_SAFE_BSW) /* COV_CAN_COMPATIBILITY */
# define CAN_SAFE_BSW   STD_OFF
#else
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
#  if (CAN_DEV_ERROR_DETECT == STD_OFF) || (CAN_AMD_RUNTIME_MEASUREMENT == STD_ON) /* COV_CAN_SAFE_BSW_EXCLUSION */
#   error "Unsupported Feature activated for SafeBSW"
#  endif
# endif
#endif

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/
#if (CAN_TRANSMIT_BUFFER == STD_OFF) && defined(C_ENABLE_CANCEL_IN_HW)
# error "C_ENABLE_CANCEL_IN_HW need CAN_TRANSMIT_BUFFER == STD_ON (no FIFO buffering but PRIO_BY_CANID in CanIf) -> check Generated data"
#endif
#if !defined(C_ENABLE_SLEEP_WAKEUP)
# if (CAN_SLEEP_SUPPORT == STD_ON) && (CAN_WAKEUP_SUPPORT == STD_ON)
#  error "C_ENABLE_SLEEP_WAKEUP organified but generated like supported"
# endif
#endif
#if (CAN_WAKEUP_SUPPORT == STD_ON) && (CAN_SLEEP_SUPPORT == STD_OFF)
# error "activated CAN_WAKEUP_SUPPORT need activated CAN_SLEEP_SUPPORT"
#endif
#if defined(C_ENABLE_HW_LOOP_TIMER)
# if (CAN_LOOP_MAX == 0)
#  error "CAN_LOOP_MAX should not be generated as 0"
# endif
#else
# if (CAN_HARDWARE_CANCELLATION == STD_ON)
#  error "C_ENABLE_HW_LOOP_TIMER organified but generated like supported"
# endif
#endif
#if defined(C_ENABLE_CANCEL_IN_HW)
#else
# if (CAN_HW_TRANSMIT_CANCELLATION == STD_ON)
#  error "C_ENABLE_CANCEL_IN_HW organified but generated like supported"
# endif
#endif
#if (CAN_MIRROR_MODE == STD_ON) && (!defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) || (CAN_GENERIC_PRECOPY == STD_OFF))
# error "CAN_MIRROR_MODE need CAN_GENERIC_CONFIRMATION with CAN_API2 and CAN_GENERIC_PRECOPY as STD_ON"
#endif
#if (CAN_MULTIPLE_BASICCAN_TX == STD_ON)
# if (CAN_HW_TRANSMIT_CANCELLATION == STD_ON)
/* cancellation does not work in CanHL_WritePrepareCancelTx for multiple BasicCAN messages, and is not useful for FIFO queue in CanIf (normally combined with multiple BCAN)*/
# error "CAN_MULTIPLE_BASICCAN_TX and CAN_HW_TRANSMIT_CANCELLATION are STD_ON but this combination is not supported"
# endif
#endif

# if (CAN_INTLOCK == CAN_APPL)
#  error "CAN_INTLOCK is set to CAN_APPL but this is not supported by CANoeEmu (CAN Interrupts has to be handled by Driver!)"
# endif

/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/
#define CAN_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define CanHL_ApplCanTimerStart( timerIdx )  ApplCanTimerStart( timerIdx );
#  define CanHL_ApplCanTimerEnd( timerIdx )    ApplCanTimerEnd( timerIdx );
# else
#  define CanHL_ApplCanTimerStart( timerIdx )  ApplCanTimerStart( channel, (timerIdx) );
#  define CanHL_ApplCanTimerEnd( timerIdx )    ApplCanTimerEnd( channel, (timerIdx) );
# endif
# define CanHL_ApplCanTimerLoop( timerIdx )   if (ApplCanTimerLoop(CAN_CHANNEL_CANPARA_FIRST (timerIdx)) != CAN_OK) \
                                              { \
                                                break; \
                                              }

# if (CAN_HW_LOOP_SUPPORT_API == STD_OFF)
/* Internal handling of HW loops (like AutoSar) - otherwise ApplCanTimer... is defined an implemented by Application */
/****************************************************************************
| NAME:             ApplCanTimerStart
****************************************************************************/
/* Used as macro to save runtime and code */
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
/* \trace SPEC-1640 */
#    define ApplCanTimerStart(source)     ((void)GetCounterValue( CAN_OS_COUNTER_ID, &Can_GetCanLoopTimeout(0, (source)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#    define ApplCanTimerStart(ch, source) ((void)GetCounterValue( CAN_OS_COUNTER_ID, &Can_GetCanLoopTimeout((ch), (source)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif
/****************************************************************************
| NAME:             ApplCanTimerLoop
****************************************************************************/
/*
|<DataModelStart>| ApplCanTimerLoop
Relation_Context:
# from mode transition + any LL #
HardwareLoopCheck, HardwareCancelByAppl
Relation:
DevErrorDetect, OneChOpt
HardwareLoopCheck
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) ApplCanTimerLoop( CAN_CHANNEL_CANTYPE_FIRST uint8 source )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  uint8 errorId;
  uint8 apiId;
  errorId = CAN_E_NO_ERROR;
  apiId = CAN_HW_ACCESS_ID;
  retval = CAN_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and source is valid (no INIT check because may be used inside power on INIT) */
  if (source > CAN_LOOP_MAX) /* CM_CAN_HL22 */
  {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TickType elapsedTime;
    TickType startTime;
    /* #110 Calculate the elapsed since time the start of this hardware loop (identified by parameter source). */
    /* Get start time (set by ApplCanTimerStart - temporary variable because GetElapsedValue modify original data */
    startTime = Can_GetCanLoopTimeout(channel, source);
    /* Get elapsed time - from start time to now */
    /* #120 Throw DET error when GetElapsedValue() failed */
    if ( GetElapsedValue( CAN_OS_COUNTER_ID, &startTime, &elapsedTime ) != (StatusType)E_OK ) /* SBSW_CAN_HL37 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_TIMEOUT_DET;
    }
    else
    {
      /* #130 Check if time out occurs for HW loop checks (none mode transition) \trace SPEC-1594 */
      if ( (TickType)CAN_OS_TICK2MS( elapsedTime ) < (TickType)CAN_TIMEOUT_DURATION ) /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 1840, 2985 */ /* MD_Can_IntegerCast, MD_Can_ConstValue */
      { /* no time out occur return OK */
        retval = CAN_OK;
      }
      else /* only used in case C_ENABLE_HW_LOOP_TIMER but removed because of coverage TX for some organiy cases */
      { /* Mode Change synchronous & HW loop check */
        /* #140 Throw DET error and set hardware cancellation flag */
        if (source < CAN_LOOP_MAX) /* COV_CAN_HARDWARE_FAILURE */
        { 
          errorId = CAN_E_TIMEOUT_DET;
        }
      }
    }
    CAN_DUMMY_STATEMENT(startTime); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_DEV_ERROR_REPORT == STD_ON)  
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(apiId, errorId);
  }
#  else
  CAN_DUMMY_STATEMENT(apiId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
/****************************************************************************
| NAME:             ApplCanTimerEnd
****************************************************************************/
/* Used as macro to save runtime and code */
/* because of possible nested timers .. timer has to be reset */
#  define ApplCanTimerEnd ApplCanTimerStart
# endif /* (CAN_HW_LOOP_SUPPORT_API == STD_OFF) */


/****************************************************************************
| NAME:             CanHL_CleanUpSendState
****************************************************************************/
/*
|<DataModelStart>| CanHL_CleanUpSendState
Relation_Context:
# from CanLL_ModeTransition() #
Relation:
OneChOpt, ChannelAmount
MultiplexedTx
TxFullCANSupport
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_CleanUpSendState( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle activeSendObject;
  CanObjectHandle mailboxHandle;
  Can_HwHandleType mailboxElement;
  /* ----- Implementation ----------------------------------------------- */
  /* parameter channel is already checked by caller */
  /* #10 set all BasicCAN TX Objects to state FREE to allow a new transmission */
  mailboxHandle = Can_GetTxBasicHandleStart(canHwChannel);
# if defined(C_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; mailboxHandle < Can_GetTxBasicHandleStop(canHwChannel); mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
# endif
  {
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
    for (mailboxElement = 0u; mailboxElement < Can_GetMailboxSize(mailboxHandle); mailboxElement++) /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
# else
    mailboxElement = 0u; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
# endif
    {
      activeSendObject = CanHL_GetActiveSendObject(mailboxHandle, mailboxElement);
      Can_SetActiveSendState(activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
    }
# if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #20 initialize TxHwFifo index and fill state */
      uint16 txHwFifo;
      txHwFifo = Can_GetMailboxTxHwFifo(mailboxHandle);
#  if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
      if (txHwFifo >= (uint16)Can_GetSizeOfTxHwFifo()) /* CM_CAN_HL32 */ /* COV_CAN_GENDATA_FAILURE */
      {
        txHwFifo = 0u;
      }
#  endif
      Can_SetTxHwFifoWriteIndex(txHwFifo, 0u); /* SBSW_CAN_HL50 */
      Can_SetTxHwFifoReadIndex(txHwFifo, 0u); /* SBSW_CAN_HL50 */
      Can_SetTxHwFifoFillCount(txHwFifo, 0u); /* SBSW_CAN_HL50 */
    }
# endif
  }
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  /* #30 set all FullCAN TX Objects to state FREE to allow a new transmission */
  for (mailboxHandle = Can_GetTxFullHandleStart(canHwChannel); mailboxHandle < Can_GetTxFullHandleStop(canHwChannel); mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
  {
    activeSendObject = CanHL_GetActiveSendObject(mailboxHandle, 0u); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    Can_SetActiveSendState(activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */ 
  }
# endif
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}

/****************************************************************************
| NAME:             CanHL_ReInit
****************************************************************************/
/*
|<DataModelStart>| CanHL_ReInit
Relation_Context:
# from CanHL_ModeTransition() #
Wakeup, OneChOpt, ChannelAmount
Relation:
DevErrorDetect, OneChOpt, ChannelAmount
DevErrorDetect, Variant, ChannelAmount
RamCheck
TxFullCANSupport
RxFullCANSupport
RxBasicCANSupport
Wakeup, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_STATIC, uint8, STATIC_CODE) CanHL_ReInit( CAN_HW_CHANNEL_CANTYPE_FIRST uint8 doRamCheck )
{ /* \trace SPEC-1719, SPEC-1589, SPEC-1346, SPEC-1565 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanInitParaStruct initPara;
  CanHookBegin_CanHL_ReInit();
  errorId = CAN_E_NO_ERROR;
  initPara.isInitOk = kCanFailed; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Config pointer is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1718 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1713 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* \trace SPEC-1713 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 initialization controller states and initialization objects --- */
    initPara.initObject = Can_GetLastInitObject(channel);
#if defined(C_ENABLE_CAN_RAM_CHECK)
    initPara.doRamCheck = doRamCheck;
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    initPara.isChRamCheckFail = kCanFalse;
#endif
    /* #110 clean up status (do not activate communication and let SLEEP and initialization to detect ram-check request) */
#if defined(C_ENABLE_CAN_RAM_CHECK) && !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    if (doRamCheck == kCanExecuteRamCheck)
    { /* #112 standard RAM check will be performed so delete old status CAN_DEACTIVATE_CONTROLLER but leave CAN_STATUS_INIT (info about PowerOn) */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)CAN_STATUS_INIT);  /* SBSW_CAN_HL02 */
    }
    else
#endif
    { /* #114 extended RAM check may be performed so delete old status except CAN_DEACTIVATE_CONTROLLER and CAN_STATUS_INIT */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & ((uint8)(CAN_DEACTIVATE_CONTROLLER) | (uint8)((uint8)CAN_STATUS_INIT)));  /* SBSW_CAN_HL02 */
    }
    /* #125 begin initialization CanHL_InitBegin() */
    initPara.isInitOk = CanHL_InitBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */
    /* #130 block invalid hardware access (silent check) */
    if (initPara.isInitOk == kCanOk) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
    {
#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
      /* #140 do TX FullCAN initialization CanHL_InitTxFullCAN()*/
      initPara.isInitOk &= CanHL_InitTxFullCAN(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
      /* #150 do TX BasicCAN initialization CanHL_InitTxBasicCAN() */
      initPara.isInitOk &= CanHL_InitTxBasicCAN(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
      /* #160 do unused object initialization CanHL_InitUnusedObj()*/
      initPara.isInitOk &= CanHL_InitUnusedObj(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
      /* #170 do RX FullCAN initialization CanHL_InitRxFullCAN() */
      initPara.isInitOk &= CanHL_InitRxFullCAN(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
      /* #180 do RX BasicCAN initialization CanHL_InitRxBasicCAN() */
      initPara.isInitOk &= CanHL_InitRxBasicCAN(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
      /* #190 end initialization CanHL_InitEnd_InitMode() */
      initPara.isInitOk &= CanHL_InitEnd_InitMode(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_ENABLE_CAN_RAM_CHECK)
      if (doRamCheck == kCanExecuteRamCheck)
      { 
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
        /* #200 extended RamChecks new issue occur - notify corrupt register / deactivate controller */
        if (initPara.isChRamCheckFail == kCanTrue) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        {
          CanIf_RamCheckCorruptController((uint8)Can_GetCanToCanIfChannelMapping(channel));
          Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */
        }
# else
        if( (Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) == CAN_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        { /* #210 standard RAM check issue occur - call Appl_CanRamCheckFailed() (any mailbox fail) */
          if (Appl_CanRamCheckFailed((uint8)channel) != CAN_DEACTIVATE_CONTROLLER)
          { /* #220 let the application decide if communication stay disabled as set by RAM check or not */
            Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(~(CAN_DEACTIVATE_CONTROLLER))); /* Application decide to activate communication in case RAM_CHECK failed */  /* SBSW_CAN_HL02 */
          }
        }
# endif
        CAN_RAM_CHECK_FINISHED_CTP(channel)
      }
#endif
    }
    if (initPara.isInitOk == kCanOk) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
    { /* #280 set controller init flag when no issue occure before */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) | (CAN_STATUS_INIT)); /* SBSW_CAN_HL02 */
    }
    else
    { /* #275 set error when issue occure before */
      Can_SetLogStatus(channel, /* CAN_STATUS_UNINIT | */ (Can_GetLogStatus(channel) & (uint8)(CAN_DEACTIVATE_CONTROLLER))); /* SBSW_CAN_HL02 */
      errorId = CAN_E_UNINIT;
    }
#if defined(C_ENABLE_SLEEP_WAKEUP)
    if (Can_IsIsWakeup(channel)) /* \trace SPEC-15124 */ /* COV_CAN_HARDWARE_FAILURE */
    { /* #290 notify EcuM in case controller WAKEUP while initialization (LL: check before) \trace SPEC-15124 */
      Can_SetIsWakeup(channel, FALSE); /* SBSW_CAN_HL09 */
      EcuM_SetWakeupEvent(Can_GetWakeupSourceRef(channel));
    }
#endif
  } /* controller active in multiple ECU configuration */

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_INITCTR_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if !defined(C_ENABLE_CAN_RAM_CHECK)
  CAN_DUMMY_STATEMENT(doRamCheck); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_CanHL_ReInit();
  return initPara.isInitOk;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             CanHL_ModeTransition
****************************************************************************/
/*
|<DataModelStart>| CanHL_ModeTransition
Relation_Context:
# from Can_Init #
ChannelAmount
# from Can_MainFunction_BusOff, CanHL_ErrorHandling #
StatusPolling, ChannelAmount
# from Can_ChangeBaudrate #
ChannelAmount
# from Can_SetBaudrate #
ChannelAmount
# from Can_SetControllerMode #
Wakeup
RamCheck
# from Can_RamCheckExecute #
RamCheck
# from Can_RamCheckEnableController #
RamCheck
Relation:
OneChOpt, HardwareLoopCheck, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_ModeTransition( CAN_CHANNEL_CANTYPE_FIRST uint8 transitionRequest, uint8 busOffRecovery, uint8 doRamCheck )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 transitionState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 request transition in LL */
  transitionState = CanLL_ModeTransition(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
  if(transitionState != kCanOk) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */
  { /* #20 first request fail so start Loop that wait short time for transition \trace SPEC-1642, SPEC-1635, SPEC-1630 */
    CanHL_ApplCanTimerStart((uint8)CAN_LOOP_MAX); /* SBSW_CAN_HL38 */
    do
    { /* #30 HW check mode reached (LL) */
      transitionState = CanLL_ModeTransition(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
      CanHL_ApplCanTimerLoop((uint8)CAN_LOOP_MAX); /* COV_CAN_TRANSITION_REQUEST */
    } while ( transitionState != kCanOk ); /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2990,2995 */ /* MD_Can_ConstValue */
    CanHL_ApplCanTimerEnd ((uint8)CAN_LOOP_MAX); /* SBSW_CAN_HL38 */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */
  }
  if ( transitionState == kCanRequested ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_TRANSITION_REQUEST */
  {
    /* #50 transition requested so remember request to repeat it asynchronous */
    Can_SetModeTransitionRequest(channel, transitionRequest); /* SBSW_CAN_HL01 */
    Can_SetBusOffTransitionRequest(channel, busOffRecovery); /* SBSW_CAN_HL01 */
    Can_SetRamCheckTransitionRequest(channel, doRamCheck); /* SBSW_CAN_HL01 */
  }
  return transitionState;
}

/****************************************************************************
| NAME:             CanHL_NotifyTransition
****************************************************************************/
/*
|<DataModelStart>| CanHL_NotifyTransition
Relation_Context:
# from Can_SetControllerMode() #
Wakeup
RamCheck
Relation:
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_NotifyTransition( CAN_CHANNEL_CANTYPE_FIRST uint8 canState, CanIf_ControllerModeType canIfState, uint8 transitionState )
{
  /* #10 if mode changed successful */
  if ( transitionState == kCanOk ) /* COV_CAN_TRANSITION_REQUEST */
  {
    /* #20 transition finished change internal state */
    Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | canState)); /* SBSW_CAN_HL02 */
    /* #30 remove transition request */
    Can_SetModeTransitionRequest(channel, kCanModeNone); /* SBSW_CAN_HL01 */
    /* #40 call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
    CanIf_ControllerModeIndication( (uint8)Can_GetCanToCanIfChannelMapping(channel), canIfState );
  }
  CAN_DUMMY_STATEMENT(transitionState); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}

#if defined(C_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_IsHigherCanIdPrio
****************************************************************************/
/*
|<DataModelStart>| CanHL_IsHigherCanIdPrio
Relation_Context:
# from CanHL_WritePrepare #
HwCancelTx, MultiplexedTx, IdenticalIdCancel
Relation:
IDType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_IsHigherCanIdPrio( Can_IdType CanId1, Can_IdType CanId2 )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  Can_IdType tmpCanId1, tmpCanId2;
  tmpCanId1 = CanId1;
  tmpCanId2 = CanId2;
  /* ----- Implementation ----------------------------------------------- */
# if defined(C_ENABLE_MIXED_ID)
  /* #10 take care about mixed Extended and Standard ID format (shift Standard ID to match Extended format) */
  if (((tmpCanId1) & CAN_ID_IDE_MASK) == CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    tmpCanId1 = tmpCanId1 & (Can_IdType)(CAN_ID_MASK);
  } else {
    tmpCanId1 = (Can_IdType)(((tmpCanId1) & CAN_ID_MASK_STD) << 18);
  }
  if (((tmpCanId2) & CAN_ID_IDE_MASK) == CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    tmpCanId2 = tmpCanId2 & (Can_IdType)(CAN_ID_MASK);
  } else {
    tmpCanId2 = (Can_IdType)(((tmpCanId2) & CAN_ID_MASK_STD) << 18);
  }
# else
  /* #20 mask out unused bits */
  tmpCanId1 = tmpCanId1 & (Can_IdType)CAN_ID_MASK;
  tmpCanId2 = tmpCanId2 & (Can_IdType)CAN_ID_MASK;
# endif
  /* #30 compare CAN ID priority */
  if(tmpCanId1 < tmpCanId2)
  {
    retval = CAN_OK;
  }
  return retval;
}
#endif

/****************************************************************************
| NAME:             CanHL_WritePrepare
****************************************************************************/
/*
|<DataModelStart>| CanHL_WritePrepare
Relation_Context:
# from Can_Write() #
Relation:
TxFullCANSupport
MultiplexedTx, TxHwFifo
MultiplexedTx, HwCancelTx, IdenticalIdCancel
ChannelAmount
OneChOpt
TxBasicAmount
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepare( CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 calculate logical TX mailbox handle for FullCAN or for BasicCAN (Multiplexed TX) as start value */
  txPara->mailboxElement = 0u; /* PRQA S 2982 */ /* MD_MSR_RetVal */ /* SBSW_CAN_HL42 */
  txPara->activeSendObject = CanHL_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  if ( Can_GetMailboxType(txPara->mailboxHandle) != CAN_TX_FULLCAN_TYPE ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#endif
  { /* #20 BasicCAN preparation - search for hardware and logical transmit handle and do cancellation */
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO) || defined(C_ENABLE_CANCEL_IN_HW)
    CanObjectHandle mailboxElement_max;
    mailboxElement_max = Can_GetMailboxSize(txPara->mailboxHandle); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    /* \trace SPEC-1677, SPEC-1672, SPEC-1679, SPEC-1673, SPEC-1671 */
# if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(txPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #25 in case of FIFO object set logical TX mailbox handle to write index + element start  */
       txPara->mailboxElement = (CanObjectHandle)Can_GetTxHwFifoWriteIndex(Can_GetMailboxTxHwFifo(txPara->mailboxHandle)); /* SBSW_CAN_HL42 */
       txPara->activeSendObject += txPara->mailboxElement; /* SBSW_CAN_HL42 */
    }
    else
# endif
    { /* #27 if normal or Mux Tx search from first to last element 0..1 or 0..3 for free object or one to be cancelled */
      CanObjectHandle mailboxElementFound;
      CanObjectHandle activeSendObjectFound;
      mailboxElementFound = mailboxElement_max; /* PRQA S 2982 */ /* MD_MSR_RetVal */
      activeSendObjectFound = mailboxElement_max; /* PRQA S 2982 */ /* MD_MSR_RetVal */
      for (txPara->mailboxElement = 0u; txPara->mailboxElement < mailboxElement_max; txPara->mailboxElement++) /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
      { /* #30 over all multiplexed TX objects: find free mailbox or mailbox with identical ID */
# if (CAN_IDENTICAL_ID_CANCELLATION == STD_ON) && defined(C_ENABLE_CANCEL_IN_HW)
        if ( ((txPara->pdu.id) == Can_GetActiveSendId(txPara->activeSendObject) ) && (Can_GetActiveSendState(txPara->activeSendObject) != kCanBufferFree) )
        { /* #40 Identical ID: priority 1 - cancel identical (also when free is available) */
          mailboxElementFound = txPara->mailboxElement;
          activeSendObjectFound = txPara->activeSendObject;
          break;
        }
        if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferFree)
        { /* #50 Identical ID: priority 2 - find free mailbox (no cancellation) */
          mailboxElementFound = txPara->mailboxElement; /* do not break ... next may be identical ID */
          activeSendObjectFound = txPara->activeSendObject;
        }
# else
        if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferFree)
        { /* #60 None Identical ID: else priority 1 - find free mailbox (no cancellation) */
          mailboxElementFound = txPara->mailboxElement;
          activeSendObjectFound = txPara->activeSendObject;
          break;
        }
# endif
        txPara->activeSendObject++; /* SBSW_CAN_HL42 */
      }
# if defined(C_ENABLE_CANCEL_IN_HW)
      if (mailboxElementFound == mailboxElement_max)
      { /* #70 No free nor identical ID found but hardware cancellation is on - search for lowest prio message to cancel */
        Can_IdType lowestPrioId = txPara->pdu.id;
        txPara->activeSendObject = CanHL_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
        for (txPara->mailboxElement = 0u; txPara->mailboxElement < mailboxElement_max; txPara->mailboxElement++) /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
        {
          if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferSend)
          { /* #80 mailbox with send request found: check for priority and remember the lowest that could be cancelled */
            if (CanHL_IsHigherCanIdPrio(lowestPrioId, Can_GetActiveSendId(txPara->activeSendObject)) == CAN_OK)
            {
              lowestPrioId = Can_GetActiveSendId(txPara->activeSendObject);
              mailboxElementFound = txPara->mailboxElement;
              activeSendObjectFound = txPara->activeSendObject;
            }
          }
          txPara->activeSendObject++; /* SBSW_CAN_HL42 */
        }
      }
# endif
      if (mailboxElementFound == mailboxElement_max)
      { /* #90 no free or cancel mailbox found: return values set to valid element or to found element and cancel */
        txPara->mailboxElement = 0u; /* SBSW_CAN_HL42 */
        txPara->activeSendObject = CanHL_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
      }
      else
      {
        txPara->mailboxElement = mailboxElementFound; /* SBSW_CAN_HL42 */
        txPara->activeSendObject = activeSendObjectFound; /* SBSW_CAN_HL42 */
# if defined(C_ENABLE_CANCEL_IN_HW) /* \trace SPEC-1674 */
        CanHL_WritePrepareCancelTx(CAN_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
# endif
      }
    } /* normal or mux tx */
#endif
  } /* end BasicCAN handling */
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* #200 no special FullCAN handling needed */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if defined(C_ENABLE_CANCEL_IN_HW) || ( defined(C_ENABLE_TX_HW_FIFO) && defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) )
/****************************************************************************
| NAME:             CanHL_WriteSavePduInfo
****************************************************************************/
/*
|<DataModelStart>| CanHL_WriteSavePduInfo
Relation_Context:
# from CanHL_WritePrepareCancelTx #
HwCancelTx
# from CanHL_WriteStart #
TxHwFifo, GenericConfirmation
Relation:
DevErrorDetect
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WriteSavePduInfo( CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
    /* #10 Get buffer to store data */
    uint16 activeSendData;
    activeSendData = CanHL_GetActiveSendData(txPara->mailboxHandle, txPara->mailboxElement);
    /* #20 SDU pointer may be a null pointer in case the DLC is 0 - so do not copy data in this case */
    if(txPara->pdu.sdu != NULL_PTR)
    {
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
      if(Can_GetSizeOfActiveSendData() >= (activeSendData + txPara->pdu.length)) /* CM_CAN_HL20 */ /* COV_CAN_GENDATA_FAILURE */
# endif
      {
        /* #25 copy data with VStdLib optimized copy routine */
        VStdMemCpy((void*)&Can_GetActiveSendData(activeSendData), (void*)txPara->pdu.sdu, txPara->pdu.length); /* SBSW_CAN_HL22 */ /* PRQA S 0314,0316 */ /* MD_Can_PointerVoidCast */
      }
    }
    /* #30 copy ID, DLC and pointer to data (also pdu as part of PduInfo struct) */
    Can_SetActiveSendId(txPara->activeSendObject, txPara->pdu.id); /* SBSW_CAN_HL23 */
    Can_SetActiveSendLength(txPara->activeSendObject, txPara->pdu.length); /* SBSW_CAN_HL23 */
    Can_SetActiveSendSdu(txPara->activeSendObject, &Can_GetActiveSendData(activeSendData)); /* SBSW_CAN_HL23 */
    Can_SetActiveSendSwPduHandle(txPara->activeSendObject, txPara->pdu.swPduHandle); /* SBSW_CAN_HL23 */
}
#endif

#if defined(C_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_WritePrepareCancelTx
****************************************************************************/
/*
|<DataModelStart>| CanHL_WritePrepareCancelTx
Relation_Context:
# from CanHL_WritePrepare #
HwCancelTx
Relation:
DevErrorDetect
CanFdSupport
MultiplexedTx, IdenticalIdCancel
OneChOpt
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WritePrepareCancelTx( CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferFree)
  { /* #10 free buffer found: backup ID, DLC, data, PDU for possible cancellation - as parameter to IF */
    CanHL_WriteSavePduInfo(txPara); /* SBSW_CAN_HL49 */
  }
  else if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferSend) /* COV_IMPLICITE_TXCANCEL */
  { /* #20 occupied buffer found (lower ID or Identical - found before): cancel it */
    /* \trace SPEC-1747, SPEC-15133, SPEC-1685, SPEC-1690, SPEC-1692, SPEC-1676, SPEC-1683, SPEC-15126 */
    tCanTxCancellationParaStruct txCancellationPara;
    txCancellationPara.mailboxHandle = txPara->mailboxHandle;
    txCancellationPara.hwObjHandle = txPara->hwObjHandle; 
    txCancellationPara.mailboxElement = txPara->mailboxElement;
    txCancellationPara.activeSendObject = txPara->activeSendObject;
    txCancellationPara.canHandleCurTxObj = kCanBufferCancel;
    /* #30 HW cancel mailbox (LL) */
    CanLL_TxCancelInHw(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
    Can_SetActiveSendState(txPara->activeSendObject, kCanBufferCancel); /* SBSW_CAN_HL41 */
    /* #40 do not notify IF about successful cancellation because of recursion will not work! Hardware support not possible in this case */
  }
  else
  { /* avoid MISRA warning (if else if without ending else) */
    /* \trace SPEC-1754 */
  }
}
#endif

/****************************************************************************
| NAME:             CanHL_WriteStart
****************************************************************************/
/*
|<DataModelStart>| Can_WriteStart
Relation_Context:
# from Can_Write #
Relation:
GenericPreTransmit
TxHwFifo, GenericConfirmation
IDType
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_WriteStart( CAN_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 free TX mailbox found so start transmission \trace SPEC-1744 */
  if (Can_GetActiveSendState(txPara->activeSendObject) == kCanBufferFree)
  {
    uint8 llretval;
    /* #20 remember PDU and send state to use it in Confirmation function \trace SPEC-1675 */
    Can_SetActiveSendPdu(txPara->activeSendObject, txPara->pdu.swPduHandle); /* SBSW_CAN_HL05 */
    Can_SetActiveSendState(txPara->activeSendObject, kCanBufferSend); /* SBSW_CAN_HL04 */
#if defined(C_ENABLE_PRETRANSMIT_FCT)
    /* call pretransmit function ----------------------------------------------- */
    Appl_GenericPreTransmit( (uint8)channel, &(txPara->pdu) );  /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL24 */
#endif
#if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(txPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #30 in case of FIFO and GenericConfirmation API2 remember also data for confirmation function */
# if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
      CanHL_WriteSavePduInfo(txPara); /* SBSW_CAN_HL49 */
# endif
      /* #40 set Fifo to next element (writing) */
      CanHL_SetNextTxHwFifoWriteIndex(txPara); /* SBSW_CAN_HL49 */
    }
#endif
    /* #50 HW prepare transmission */
    CanLL_TxBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    /* #60 Set ID, ID type and DLC in HW register depend on ID type */
    txPara->dlcRaw = MK_TX_DLC(CAN_LEN2DLC(txPara->pdu.length)); /* SBSW_CAN_HL42 */
#if defined(C_ENABLE_EXTENDED_ID)
# if defined(C_ENABLE_MIXED_ID)
    if ( ((txPara->pdu.id) & CAN_ID_IDE_MASK) != CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      SET_PARASTRUCT_IDRAW_TX_STD(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
    }
    else
# endif
    {
      SET_PARASTRUCT_IDRAW_TX_EXT(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
    }
#else
    {
      SET_PARASTRUCT_IDRAW_TX_STD(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* PRQA S 4491 */ /* MD_Can_IntegerCast */
    }
#endif
    /* set id, dlc in hardware */
    CanLL_TxSetMailbox(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    if(txPara->pdu.sdu != NULL_PTR)
    { /* #70 Set CAN data in HW register (LL) - only when data pointer is not null \trace SPEC-1680, SPEC-1572, SPEC-1678, SPEC-60421 */
      txPara->CanMemCopySrcPtr = txPara->pdu.sdu; /* SBSW_CAN_HL42 */
      CanLL_TxCopyToCan(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    }
    /* #80 HW start transmission (LL) */
    llretval = CanLL_TxStart(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    /* HW finish transmission (LL) */
    CanLL_TxEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    CAN_DUMMY_STATEMENT(llretval); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }
  else
  { /* #200 no free TX mailbox found so return busy - also in case cancellation (BasicCAN) was possible \trace SPEC-1754, SPEC-1764, SPEC-1747 */
    retval = CAN_BUSY;
  }
  return retval;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/****************************************************************************
| NAME:             CanHL_GetActiveSendObject
****************************************************************************/
/*
|<DataModelStart>| CanHL_GetActiveSendObject
Relation_Context:
# from CanHL_InitXXX() #
Wakeup, OneChOpt, ChannelAmount
# from CanHL_WritePrepare #
# from CanHL_TxTaskCancelationHandling #
HwCancelTx
# from CanHL_CleanUpSendState #
# from Can_CancelTx #
TransmitCancellationAPI
# from CanHL_TxConfirmation #
Relation:
DevErrorDetect
TxFullAmount
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, CanObjectHandle, STATIC_CODE) CanHL_GetActiveSendObject( Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanObjectHandle activeSendObject;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 get logical handle from generated indirection table */
  activeSendObject = (CanObjectHandle) Can_GetMailboxActiveSendObject(mailboxHandle) + mailboxElement;
#if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
  if (activeSendObject >= (Can_HwHandleType)Can_GetSizeOfActiveSendObject()) /* CM_CAN_HL03 */ /* COV_CAN_GENDATA_FAILURE */
  { /* #20 secure SilentBSW */
    activeSendObject = ((Can_HwHandleType)Can_GetSizeOfActiveSendObject() - (Can_HwHandleType)1u);
# if (CAN_DEV_ERROR_REPORT == STD_ON)  
    Can_CallDetReportError(CAN_WRITE_ID, CAN_E_PARAM_HANDLE);
# endif
  }
#endif
  return activeSendObject;
}

#if defined(C_ENABLE_CANCEL_IN_HW) || ( defined(C_ENABLE_TX_HW_FIFO) && defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2) )
/****************************************************************************
| NAME:             CanHL_GetActiveSendData
****************************************************************************/
/*
|<DataModelStart>| CanHL_GetActiveSendData
Relation_Context:
HwCancelTx, TxHwFifo
# from CanHL_WritePrepare #
# from CanHL_TxTaskCancelationHandling #
HwCancelTx
# from Can_CancelTx #
TransmitCancellationAPI
# from CanHL_TxConfirmation #
Relation:
DevErrorDetect
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint16, STATIC_CODE) CanHL_GetActiveSendData( Can_HwHandleType mailboxHandle, uint8 mailboxElement )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 activeSendData;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 get logical handle from generated indirection table */
  activeSendData = (uint16) (Can_GetMailboxActiveSendData(mailboxHandle) + ((uint16)mailboxElement * (uint16)Can_GetMailboxDataLen(mailboxHandle)));
#if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
  if ((activeSendData + Can_GetMailboxDataLen(mailboxHandle)) > Can_GetSizeOfActiveSendData()) /* CM_CAN_HL20 */ /* COV_CAN_GENDATA_FAILURE */
  { /* #20 secure SilentBSW */
    activeSendData = 0u;
# if (CAN_DEV_ERROR_REPORT == STD_ON)  
    Can_CallDetReportError(CAN_WRITE_ID, CAN_E_PARAM_HANDLE);
# endif
  }
#endif
  return activeSendData;
}
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
/****************************************************************************
| NAME:             CanHL_RamCheckMailboxNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_RamCheckMailboxNotification
Relation_Context:
# CanHL_InitXXX() #
Wakeup, OneChOpt, ChannelAmount, RamCheck
Relation:
TxFullCANSupport
RxFullCANSupport
RxBasicCANSupport
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RamCheckMailboxNotification( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */ /* COV_CAN_RAM_CHECK_NO_STIMULATION */
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 deactivate mailbox */
  Can_SetMailboxState(initPara->mailboxHandle, kCanFailed); /* SBSW_CAN_HL34 */
  /* #20 deactivate controller */
  Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */
  /* #30 notify application about corrupt mailbox */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  CanIf_RamCheckCorruptMailbox((uint8)Can_GetCanToCanIfChannelMapping(channel), initPara->mailboxHandle);
# else
#  if defined(C_ENABLE_NOTIFY_CORRUPT_MAILBOX) /* with Mailbox Notification */
  Appl_CanCorruptMailbox((uint8)channel, (initPara->hwObjHandle));
#  endif
# endif
}
#endif

/****************************************************************************
| NAME:             CanHL_InitBegin
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitBegin
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount
Relation:
RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitBegin( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanFailed; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  if (CanLL_InitBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara) == kCanOk) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL31 */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    /* #20 Execute pattern RamCheck */
    if (initPara->doRamCheck == kCanExecuteRamCheck)
    {
      initPara->isChRamCheckFail |= CAN_RAM_CHECK_BEGIN_REG_RESULT(channel, initPara); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL31 */ /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
    } 
#endif
    /* #30 Initialize controller registers (Begin section) */
    initOk = CanLL_InitBeginSetRegisters(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
  }
  return initOk;
}

#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_InitTxFullCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitTxFullCAN
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount, TxFullCANSupport
Relation:
RamCheck
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxFullCAN( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
  CanObjectHandle activeSendObject;
# if defined(C_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif
  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
  /* #10 iterate over all FullCAN TX */
  for (initPara->mailboxHandle = Can_GetTxFullHandleStart(canHwChannel); initPara->mailboxHandle < Can_GetTxFullHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #20 initialize FullCAN TX mailboxes */
    initPara->hwObjHandle = Can_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
    activeSendObject = CanHL_GetActiveSendObject(initPara->mailboxHandle, 0u);
    Can_SetActiveSendState(activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #30 RAM check for FullCAN TX mailboxes */
    isMbRamCheckFail = kCanFalse;
    if (initPara->doRamCheck == kCanExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* SBSW_CAN_HL31 */ /* COV_CAN_HARDWARE_FAILURE */
    } 
# endif
    /* TX initialization */
    mailboxState = CanLL_InitMailboxTx(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCanFailed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCanTrue; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCanTrue) || (Can_GetMailboxState(initPara->mailboxHandle) == kCanFailed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_InitMailboxTxDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

/****************************************************************************
| NAME:             CanHL_InitTxBasicCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitTxBasicCAN
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount
Relation:
MultiplexedTx, TxHwFifo
TxHwFifo, GenericConfirmation, IfTxBuffer
TxBasicAmount
RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitTxBasicCAN( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
  CanObjectHandle activeSendObject;
#if defined(C_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
#endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
  initPara->mailboxHandle = Can_GetTxBasicHandleStart(canHwChannel); /* SBSW_CAN_HL36 */
#if defined(C_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; initPara->mailboxHandle < Can_GetTxBasicHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */
#endif
  { /* #10 iterate over all logical BasicCAN TX */
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO) 
    CanObjectHandle activeSendObjectStop;
#endif
    initPara->hwObjHandle = Can_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
    activeSendObject = CanHL_GetActiveSendObject(initPara->mailboxHandle, 0u);
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO) 
    activeSendObjectStop = activeSendObject + Can_GetMailboxSize(initPara->mailboxHandle);
    for (; activeSendObject < activeSendObjectStop; activeSendObject++)
#endif
    { /* #20 initialize BasicCAN TX mailboxes */
      Can_SetActiveSendState(activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
    }
# if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(initPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #25 initialize TxHwFifo index and fill state */
      uint16 txHwFifo;
      txHwFifo = Can_GetMailboxTxHwFifo(initPara->mailboxHandle);
#  if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
      if (txHwFifo >= (uint16)Can_GetSizeOfTxHwFifo()) /* CM_CAN_HL32 */ /* COV_CAN_GENDATA_FAILURE */
      {
        txHwFifo = 0u;
        initOk = kCanFailed;
      }
#  endif
      Can_SetTxHwFifoWriteIndex(txHwFifo, 0u); /* SBSW_CAN_HL50 */
      Can_SetTxHwFifoReadIndex(txHwFifo, 0u); /* SBSW_CAN_HL50 */
      Can_SetTxHwFifoFillCount(txHwFifo, 0u); /* SBSW_CAN_HL50 */
    }
# endif
#if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #30 RAM check for BasicCAN TX mailboxes */
    isMbRamCheckFail = kCanFalse;
    if (initPara->doRamCheck == kCanExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
#endif
    /* TX initialization */
    mailboxState = CanLL_InitMailboxTx(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCanFailed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCanTrue; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCanTrue) || (Can_GetMailboxState(initPara->mailboxHandle) == kCanFailed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_InitMailboxTxDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
#endif
  }
  return initOk;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */

/****************************************************************************
| NAME:             CanHL_InitUnusedObj
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitUnusedObj
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitUnusedObj( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
  for (initPara->mailboxHandle = Can_GetUnusedHandleStart(canHwChannel); initPara->mailboxHandle < Can_GetUnusedHandleStop(canHwChannel); initPara->mailboxHandle++) /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #10 iterate over all Unused mailboxes */
    initPara->hwObjHandle = Can_GetMailboxHwHandle(initPara->mailboxHandle); /* SBSW_CAN_HL36 */
    /* #20 HW initialize unused mailboxes */
    mailboxState = CanLL_InitMailboxUnused(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  }
  return initOk;
}

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_InitRxFullCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitRxFullCAN
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount, RxFullCANSupport
Relation:
RamCheck
IDType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxFullCAN( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
# if defined(C_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
  for (initPara->mailboxHandle = Can_GetRxFullHandleStart(canHwChannel); initPara->mailboxHandle < Can_GetRxFullHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #10 iterate over all FullCAN RX mailboxes */
    initPara->hwObjHandle=Can_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #20 RAM check for FullCAN RX mailboxes */
    isMbRamCheckFail = kCanFalse;
    if (initPara->doRamCheck == kCanExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
# endif
    /* #30 set ID for FullCAN RX mailboxes (LL) */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
    if ( ((Can_IdType)Can_GetMailboxIDValue(initPara->mailboxHandle) & CAN_ID_IDE_MASK) != CAN_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
      initPara->idType = kCanIdTypeStd; /* SBSW_CAN_HL36 */
      SET_PARASTRUCT_IDRAW_RX_STD(initPara, Can_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* PRQA S 4491 */ /* MD_Can_IntegerCast */
    }
    else
#  endif
    {
      initPara->idType = kCanIdTypeExt; /* SBSW_CAN_HL36 */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      SET_PARASTRUCT_IDRAW_RX_EXT(initPara, Can_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
    }
# else
    initPara->idType = kCanIdTypeStd; /* SBSW_CAN_HL36 */
    SET_PARASTRUCT_IDRAW_RX_STD(initPara, Can_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* PRQA S 4491,2985 */ /* MD_Can_IntegerCast */
# endif
    /* #40 initialize FullCAN RX mailboxes */
    mailboxState = CanLL_InitMailboxRxFullCan(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCanFailed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCanTrue; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCanTrue) || (Can_GetMailboxState(initPara->mailboxHandle) == kCanFailed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_InitMailboxRxFullCanDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/****************************************************************************
| NAME:             CanHL_InitRxBasicCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitRxBasicCAN
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount, RxBasicCANSupport
Relation:
RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitRxBasicCAN( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
# if defined(C_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
  for (initPara->mailboxHandle = Can_GetRxBasicHandleStart(canHwChannel); initPara->mailboxHandle < Can_GetRxBasicHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #10 iterate over all BasicCAN RX mailboxes */
    initPara->hwObjHandle = Can_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #20 RAM check for BasicCAN RX mailboxes */
    isMbRamCheckFail = kCanFalse;
    if (initPara->doRamCheck == kCanExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
# endif
    /* #30 initialize BasicCAN RX mailboxes */
    mailboxState = CanLL_InitMailboxRxBasicCan(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCanFailed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCanTrue; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_GetLogStatus(channel) & CAN_STATUS_INIT) != CAN_STATUS_INIT )
    { /* PowerOn */
      Can_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCanTrue) || (Can_GetMailboxState(initPara->mailboxHandle) == kCanFailed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_InitMailboxRxBasicCanDeactivate(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCanTrue)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_RamCheckMailboxNotification(CAN_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

/****************************************************************************
| NAME:             CanHL_InitEnd_InitMode
****************************************************************************/
/*
|<DataModelStart>| CanHL_InitEnd_InitMode
Relation_Context:
# CanHL_ReInit() #
Wakeup, OneChOpt, ChannelAmount
Relation:
RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_InitEnd_InitMode( CAN_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCanOk;
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  /* #10 Execute pattern RamCheck */
  if (initPara->doRamCheck == kCanExecuteRamCheck)
  {
    initPara->isChRamCheckFail |= CAN_RAM_CHECK_END_REG_RESULT(channel, initPara); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL31 */ /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  } 
#endif

  /* #20 HW specific initialization end */
  initOk &= CanLL_InitEndSetRegisters(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  initOk &= CanLL_InitEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara ); /* SBSW_CAN_HL31 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  return initOk;
}

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_RxBasicCanPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_RxBasicCanPolling
Relation_Context:
# from Can_MainFunction_Read #
RxBasicCANSupport, RxPolling
Relation:
IndividualPolling
OneChOpt
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxBasicCanPolling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  tCanTaskParaStruct taskPara;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  if(CanLL_RxBasicIsGlobalIndPending(CAN_HW_CHANNEL_CANPARA_ONLY) == kCanTrue) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
    /* #20 loop over all BasicCAN mailboxes */
    for (taskPara.mailboxHandle = Can_GetRxBasicHandleStart(canHwChannel); taskPara.mailboxHandle < Can_GetRxBasicHandleStop(canHwChannel); taskPara.mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
    { /* loop over all BasicCAN mailboxes with gap */
      taskPara.hwObjHandle = Can_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      /* #30 check individual polling flag */
      if ( Can_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
      {
        /* #40 call LL handling */
        CanLL_RxBasicProcessPendings(CAN_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
      }
    }
  }
}
# endif /* C_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_RxFullCanPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_RxFullCanPolling
Relation_Context:
# from Can_MainFunction_Read #
RxFullCANSupport, RxPolling
Relation:
IndividualPolling
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxFullCanPolling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  tCanTaskParaStruct taskPara;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  if(CanLL_RxFullIsGlobalIndPending(CAN_HW_CHANNEL_CANPARA_ONLY) == kCanTrue) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
    /* #20 loop over all FullCAN mailboxes */
    for (taskPara.mailboxHandle = Can_GetRxFullHandleStart(canHwChannel); taskPara.mailboxHandle < Can_GetRxFullHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
    {
      taskPara.hwObjHandle = Can_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      /* #30 check individual polling flag */
      if ( Can_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
      {
        /* #40 call LL handling */
        CanLL_RxFullProcessPendings(CAN_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
      }
    }
  }
}
# endif /* C_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/****************************************************************************
| NAME:             CanHL_RxMsgReceivedNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_RxMsgReceivedNotification
Relation_Context:
# from CanHL_FullCanMsgReceived #
RxFullCANSupport
# from CanHL_BasicCanMsgReceived #
RxBasicCANSupport
Relation:
RxQueue, GenericPreCopy, MirrorMode
RxQueue, DevErrorDetect
RxQueue, CanFdSupport
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_RxMsgReceivedNotification( CAN_CHANNEL_CANTYPE_FIRST CanRxInfoStructPtr rxStructPtr ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* ----- Local Variables ---------------------------------------------- */
# if !defined(C_ENABLE_RX_QUEUE) && (CAN_GENERIC_PRECOPY == STD_ON)
  Can_ReturnType generic_retval;
# endif
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 RX queue */
# if defined(C_ENABLE_RX_QUEUE)
  /**************************** RxQueue handling **********************************/
    /* #20 EXCLUSIVE_AREA_4 secure RxQueue data handling */
  SchM_Enter_Can_CAN_EXCLUSIVE_AREA_4();
  if (Can_GetRxQueueInfo().Count < Can_GetSizeOfRxQueueBuffer()) /* COV_CAN_GENDATA_FAILURE */
  { /* #30 Queue not full: */
    /* #45 copy HRH, ID, DLC and data to RX queue */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).Hrh = rxStructPtr->localMailboxHandle; /* SBSW_CAN_HL18 */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).id  = rxStructPtr->localId; /* SBSW_CAN_HL18 */
    Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).dlc = rxStructPtr->localDlc; /* SBSW_CAN_HL18 */

#  if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (((uint8) (sizeof(Can_GetRxQueueBuffer(0).data) / sizeof(Can_GetRxQueueBuffer(0).data[0]))) < rxStructPtr->localDlc) /* CM_CAN_HL17 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_PARAM_DLC;
    }
    else
#  endif
    {
      /* #46 copy data with VStdLib optimized copy routine */
      VStdMemCpy((void*)Can_GetRxQueueBuffer(Can_GetRxQueueInfo().WriteIndex).data, (void*)rxStructPtr->pChipData, rxStructPtr->localDlc); /* SBSW_CAN_HL19 */ /* PRQA S 0312,0314,0316 */ /* MD_Can_NoneVolatilePointerCast,MD_Can_PointerVoidCast,MD_Can_PointerVoidCast */
    }
      /* #47 increase pointer to next queue element */ 
    Can_GetRxQueueInfo().Count++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    if (Can_GetRxQueueInfo().WriteIndex < ((uint16)Can_GetSizeOfRxQueueBuffer() - (uint16)1u) )
    {
      Can_GetRxQueueInfo().WriteIndex++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    }
    else
    {
      Can_GetRxQueueInfo().WriteIndex = 0u; /* CM_CAN_HL24 */ /* SBSW_CAN_HL55 */
    }
  }
#  if (CAN_DEV_ERROR_REPORT == STD_ON)
  else 
  { /* #50 Queue full: last message will be lost -> overrun in queue do not change ReadIndex to override latest message because inconsistency problem (currently reading queue) */
    errorId = CAN_E_RXQUEUE;
  }
#  endif
  SchM_Exit_Can_CAN_EXCLUSIVE_AREA_4();
# else /* C_ENABLE_RX_QUEUE */
  /* #100 no RX queue */
  /* -------------------------- Interface + APPL Notifications --- */
    /* #110 call Appl_GenericPrecopy() depend on configuration and if mirror mode is active */
#  if (CAN_GENERIC_PRECOPY == STD_ON)
#   if defined(C_ENABLE_MIRROR_MODE)
  if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
  { /* do not call Appl_GenericPrecopy() in case mirror mode was disabled */
    generic_retval = CAN_OK;
  }
  else
#   endif
  {
    generic_retval = Appl_GenericPrecopy((uint8)channel, (rxStructPtr->localId & CAN_ID_MASK_IN_GENERIC_CALLOUT), rxStructPtr->localDlc, (Can_DataPtrType)rxStructPtr->pChipData);  /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL07 */
  }
  if (generic_retval == CAN_OK) /* \trace SPEC-15119 */
#  endif
  { /* #120 call CanIf_RxIndication() if mirror mode is no active or Appl_GenericPrecopy() return OK \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
    CanIf_RxIndication(rxStructPtr->localMailboxHandle, rxStructPtr->localId, rxStructPtr->localDlc, (Can_DataPtrType)rxStructPtr->pChipData); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL07 */
  }
# endif
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return errorId;
}
#endif

#if defined(C_ENABLE_RX_QUEUE)
/****************************************************************************
| NAME:             CanHL_RxQueueExecution
****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/*
|<DataModelStart>| CanHL_RxQueueExecution
Relation_Context:
# from Can_MainFunction_Read #
RxQueue
Relation:
GenericPreCopy, OneChOpt, DevErrorDetect
GenericPreCopy, MirrorMode
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_RxQueueExecution( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute RxQueue (over all channels) */
  while ( Can_GetRxQueueInfo().Count != (uint16)0 )
  { /* #20 over all filled RxQueue elements */
# if (CAN_GENERIC_PRECOPY == STD_ON)
    Can_ReturnType generic_retval;
    CAN_CHANNEL_CANTYPE_LOCAL
    generic_retval = CAN_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).Hrh);
#   if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #30 Check controller is valid */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#   endif
#  endif
#  if defined(C_ENABLE_MIRROR_MODE)
    if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
    { /* #40 MirrorMode: only when mirror is activated for this controller */
      generic_retval = CAN_OK;
    }
    else
#  endif
    { /* #50 call Appl_GenericPrecopy() */
      generic_retval = Appl_GenericPrecopy( (uint8)channel,
                (Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).id & CAN_ID_MASK_IN_GENERIC_CALLOUT),
                Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).dlc,
                (Can_DataPtrType)Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).data ); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL06 */
    }
    if (generic_retval == CAN_OK)
# endif
    { /* #60 call CanIf_RxIndication() if Appl_GenericPrecopy() return OK or no generic precopy is used \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
      CanIf_RxIndication( Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).Hrh, /* SBSW_CAN_HL06 */
              Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).id,
              Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).dlc,
              (Can_DataPtrType)Can_GetRxQueueBuffer(Can_GetRxQueueInfo().ReadIndex).data ); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */
    }
    /* #70 EXCLUSIVE_AREA_4 secure RxQueue handling */
    SchM_Enter_Can_CAN_EXCLUSIVE_AREA_4();
    /* #80 get next RxQueue element pointer */
    if (Can_GetRxQueueInfo().ReadIndex >= ((uint16)Can_GetSizeOfRxQueueBuffer() - (uint16)1u) ) /* CM_CAN_HL25 */
    {
      Can_GetRxQueueInfo().ReadIndex = 0u; /* CM_CAN_HL25 */ /* SBSW_CAN_HL55 */
    }
    else
    {
      Can_GetRxQueueInfo().ReadIndex++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    }
    Can_GetRxQueueInfo().Count--; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    SchM_Exit_Can_CAN_EXCLUSIVE_AREA_4();
  }
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_READ_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}
#endif

#if defined(C_ENABLE_CANCEL_TX_IN_HW) 
#endif

/****************************************************************************
| NAME:             CanLL_WriteReg8
****************************************************************************/
/*
|<DataModelStart>| CanLL_WriteReg8
Relation_Context:
# CanHL_InitXXX() #
RamCheck
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg8( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr8 regPtr, uint8 value, uint8 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if (initPara->doRamCheck == kCanExecuteRamCheck)
  {
    /* #20 read back value for RamCheck */
    volatile uint8 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_HL36 */
  }
# else
  CAN_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanLL_WriteReg16
****************************************************************************/
/*
|<DataModelStart>| CanLL_WriteReg16
Relation_Context:
# CanHL_InitXXX() #
RamCheck
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg16( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr16 regPtr, uint16 value, uint16 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if (initPara->doRamCheck == kCanExecuteRamCheck)
  {
    /* #20 read back value for RamCheck */
    volatile uint16 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_HL36 */
  }
# else
  CAN_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanLL_WriteReg32
****************************************************************************/
/*
|<DataModelStart>| CanLL_WriteReg32
Relation_Context:
# CanHL_InitXXX() #
RamCheck
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanLL_WriteReg32( CAN_CHANNEL_CANTYPE_FIRST CanChipMsgPtr32 regPtr, uint32 value, uint32 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  uint8 result = kCanFalse;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_HL35 */
# if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
  if (initPara->doRamCheck == kCanExecuteRamCheck)
  {
    /* #20 read back value for RamCheck */
    volatile uint32 readBack;
    readBack = *regPtr;
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCanTrue; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    }
    CAN_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_HL36 */
  }
# else
  CAN_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return result;
}

/**********************************************************************************************************************
 *  CanLL_InitBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitBegin
Relation_Context:
# CanHL_InitBegin() #
OneChOpt, ChannelAmount
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set CAN cell pointer for this controller/channel */
  initPara->canLLController = &Can_GetgCanController(channel); /* SBSW_CAN_HL36 */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_InitBeginIsRegisterCorrupt
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitBeginIsRegisterCorrupt
Relation_Context:
# CanHL_InitBegin() #
OneChOpt, ChannelAmount, RamCheck
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginIsRegisterCorrupt( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 testPatternIdx;
  uint8 isChRamCheckFail = kCanFalse;
  vuint8 const testPattern[3] = {0x55,0xAA,0x00};
  /* just for simulation no real hardware */
  /* #10 emulate controller register Pattern check */
  for(testPatternIdx = 0; testPatternIdx < 3u; testPatternIdx++)
  {
    /* Use only mode register for simulation - not intended to test all */ 
    initPara->canLLController->Mode = testPattern[testPatternIdx]; /* SBSW_CAN_HL36 */
    if(initPara->canLLController->Mode != testPattern[testPatternIdx]) /* COV_CAN_LL_EMULATION */
    {
      isChRamCheckFail = kCanTrue;
    }
  }
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return isChRamCheckFail;
}
/* CODE CATEGORY 4 END */
#endif /* C_ENABLE_CAN_RAM_CHECK_EXTENDED */

/**********************************************************************************************************************
 *  CanLL_InitBeginSetRegisters
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitBeginSetRegisters
Relation_Context:
# CanHL_InitBegin() #
OneChOpt, ChannelAmount
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitBeginSetRegisters( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint16 dummyReg;
  CanObjectHandle hwObj;
  CanCell_AtomicBegin();
  /* #10 reset controller registers like pendings and error counters */
  /* initPara->canLLController->IntEnable       = CAN_INT_NONE;  will be set with CanCell_EnableInterrupts() */
  /* some Registers writtten with WriteReg() to stimulate RAM_CHECK */
  (void)CanLL_WriteReg8(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &(initPara->canLLController->Mode), CAN_MODE_UNINIT, 0xFFu, initPara); /* SBSW_CAN_HL31 */
  (void)CanLL_WriteReg16(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &dummyReg, 0u, 0xFFu, initPara); /* SBSW_CAN_HL31 */
  (void)CanLL_WriteReg32(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &(initPara->canLLController->ErrCode), CANCELL_ERRORACTIVE, 0xFFFFu, initPara); /* SBSW_CAN_HL31 */
  initPara->canLLController->HighVoltageMode = 0u; /* SBSW_CAN_HL36 */
  initPara->canLLController->ErrCntTx        = 0u; /* SBSW_CAN_HL36 */
  initPara->canLLController->ErrCntRx        = 0u; /* SBSW_CAN_HL36 */
  initPara->canLLController->WakeupPending   = (vuint8)kCanFalse; /* reset pending flags */ /* SBSW_CAN_HL36 */
  initPara->canLLController->BusOffNotify    = (vuint8)kCanFalse; /* unused (just use ErrCode) */ /* SBSW_CAN_HL36 */
  initPara->canLLController->ErrorPending    = (vuint8)kCanFalse; /* unused (just use ErrCode) */ /* SBSW_CAN_HL36 */
  /* #20 setup software RX FIFO pointer */
  for (hwObj = 0u; hwObj < CAN_MAX_BCAN_MAILBOX; hwObj++)
  {
    initPara->canLLController->BCanWrittenPointer[hwObj] = 0u; /* SBSW_CAN_HL36 */
  }
  /* #30 setup bus mapping and bit timing */
  CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, initPara->initObject, CANOEAPI_CAN_SILENTMODE); /* COV_CAN_LL_EMULATION */
  CanCell_AtomicEnd();
  return kCanOk;
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitMailboxTx
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanHL_InitMailboxTx
Relation_Context:
# CanHL_InitTxFullCAN() #
OneChOpt, ChannelAmount
# CanHL_InitTxBasicCAN() #
OneChOpt, ChannelAmount
Relation:
TxFullCANSupport
MultiplexedTx
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxTx( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval = kCanOk;
  CanObjectHandle hwObjIterator;
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
  CanObjectHandle hwObjStop;
  hwObjIterator = initPara->hwObjHandle;
  hwObjStop = initPara->hwObjHandle + Can_GetMailboxSize(initPara->mailboxHandle);
  /* #10 iterate over all hardware objects */
  for (;hwObjIterator < hwObjStop; hwObjIterator++)
# else
  hwObjIterator = initPara->hwObjHandle;
# endif
  {
    /* #20 setup TX mailbox -> free, no pending */
    uint8 isRamCheckFail = CanLL_WriteReg8(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &(initPara->canLLController->MsgObj[hwObjIterator].mIsFree), kCanTrue, 0xFFu, initPara); /* SBSW_CAN_HL31 */
    if (isRamCheckFail == kCanTrue) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
      retval = kCanFailed; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
    initPara->canLLController->MsgObj[hwObjIterator].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  }
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;
}
/* CODE CATEGORY 4 END */

#if defined(C_ENABLE_CAN_RAM_CHECK)
/**********************************************************************************************************************
 *  CanLL_InitMailboxTxDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxTxDeactivate
Relation_Context:
# CanHL_InitTxFullCAN() #
OneChOpt, ChannelAmount
# CanHL_InitTxBasicCAN() #
OneChOpt, ChannelAmount
Relation:
TxFullCANSupport, RamCheck
MultiplexedTx, RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxTxDeactivate( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  CanObjectHandle hwObjIterator;
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
  CanObjectHandle hwObjStop;
  hwObjIterator = initPara->hwObjHandle;
  hwObjStop = initPara->hwObjHandle + Can_GetMailboxSize(initPara->mailboxHandle);
  /* #10 iterate over all hardware objects */
  for (;hwObjIterator < hwObjStop; hwObjIterator++)
# else
  hwObjIterator = initPara->hwObjHandle;
# endif
  {
    /* #20 deactivate mailbox */
    initPara->canLLController->MsgObj[hwObjIterator].mIsFree = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  }
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_InitMailboxUnused
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxUnused
Relation_Context:
# CanHL_InitUnusedObj() #
OneChOpt, ChannelAmount
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxUnused( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  CanObjectHandle hwObjIterator;
  CanObjectHandle hwObjStop;
  hwObjIterator = initPara->hwObjHandle;
  hwObjStop = initPara->hwObjHandle + Can_GetMailboxSize(initPara->mailboxHandle);
  /* #10 iterate over all hardware objects */
  for (;hwObjIterator < hwObjStop; hwObjIterator++)
  {
    /* #20 setup unused mailbox */
    initPara->canLLController->MsgObj[hwObjIterator].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mIsFree     = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  }
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxFullCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxRxFullCan
Relation_Context:
# CanHL_InitRxFullCAN() #
OneChOpt, ChannelAmount, RxFullCANSupport
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxRxFullCan( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval = kCanOk;
  /* #10 setup RX mailbox -> free, no pending */
  uint8 isRamCheckFail = CanLL_WriteReg8(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &(initPara->canLLController->MsgObj[initPara->hwObjHandle].mIsFree), kCanTrue, 0xFFu, initPara); /* SBSW_CAN_HL31 */
  if (isRamCheckFail == kCanTrue) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
    retval = kCanFailed; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
  initPara->canLLController->MsgObj[initPara->hwObjHandle].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  initPara->canLLController->MsgObj[initPara->hwObjHandle].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  initPara->canLLController->MsgObj[initPara->hwObjHandle].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  initPara->canLLController->MsgObj[initPara->hwObjHandle].mMsg.IdRaw  = initPara->idRaw0; /* SBSW_CAN_HL36 */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;
}
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxFullCanDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxRxFullCanDeactivate
Relation_Context:
# CanHL_InitRxFullCAN() #
OneChOpt, ChannelAmount, RxFullCANSupport, RamCheck
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxRxFullCanDeactivate( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 deactivate mailbox */
  initPara->canLLController->MsgObj[initPara->hwObjHandle].mIsFree = (vuint8)kCanFalse;  /* SBSW_CAN_HL36 */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 4 END */
# endif

#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxBasicCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxRxBasicCan
Relation_Context:
# CanHL_InitRxBasicCAN() #
Wakeup, OneChOpt, ChannelAmount, RxBasicCANSupport
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitMailboxRxBasicCan( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval = kCanOk;
  CanObjectHandle hwObjIterator;
  CanObjectHandle hwObjStop;
  hwObjIterator = initPara->hwObjHandle;
  hwObjStop = initPara->hwObjHandle + Can_GetMailboxSize(initPara->mailboxHandle);
  /* #10 iterate over all hardware objects */
  for (;hwObjIterator < hwObjStop; hwObjIterator++)
  {
    /* #20 setup RX mailbox -> free, no pending */
    uint8 isRamCheckFail = CanLL_WriteReg8(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &(initPara->canLLController->MsgObj[hwObjIterator].mIsFree), kCanTrue, 0xFFu, initPara); /* SBSW_CAN_HL31 */
    if (isRamCheckFail == kCanTrue) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
      retval = kCanFailed; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
    initPara->canLLController->MsgObj[hwObjIterator].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
    initPara->canLLController->MsgObj[hwObjIterator].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  }
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;
}
/* CODE CATEGORY 4 END */

# if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitMailboxRxBasicCanDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMailboxRxBasicCanDeactivate
Relation_Context:
# CanHL_InitRxBasicCAN() #
Wakeup, OneChOpt, ChannelAmount, RxBasicCANSupport, RamCheck
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMailboxRxBasicCanDeactivate( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  CanObjectHandle hwObjIterator;
  CanObjectHandle hwObjStop;
  hwObjIterator = initPara->hwObjHandle;
  hwObjStop = initPara->hwObjHandle + Can_GetMailboxSize(initPara->mailboxHandle);
  /* #10 iterate over all hardware objects */
  for (;hwObjIterator < hwObjStop; hwObjIterator++)
  {
    /* #20 deactivate mailbox */
    initPara->canLLController->MsgObj[hwObjIterator].mIsFree = (vuint8)kCanFalse; /* SBSW_CAN_HL36 */
  }
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 4 END */
# endif
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined( C_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_InitEndIsRegisterCorrupt
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitEndIsRegisterCorrupt
Relation_Context:
# CanHL_InitEnd_InitMode() #
Wakeup, OneChOpt, ChannelAmount, RamCheck
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndIsRegisterCorrupt( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanFalse;
}
/* CODE CATEGORY 4 END */
#endif /* C_ENABLE_CAN_RAM_CHECK_EXTENDED */

/**********************************************************************************************************************
 *  CanLL_InitEndSetRegisters
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitEndSetRegisters
Relation_Context:
# CanHL_InitEnd_InitMode() #
Wakeup, OneChOpt, ChannelAmount
Relation:
IndividualPolling, TxPolling, RxPolling
StatusPolling
WakeupPolling
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEndSetRegisters( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 txInt, rxInt, errInt, wakInt;
  CanCell_AtomicBegin();
#if !defined( C_ENABLE_TX_POLLING ) || defined(C_ENABLE_INDIVIDUAL_POLLING)
  txInt = 1u;
#else
  txInt = 0u;
#endif
#if (!defined( C_ENABLE_RX_BASICCAN_POLLING ) && defined( C_ENABLE_RX_BASICCAN_OBJECTS )) || \
    (!defined( C_ENABLE_RX_FULLCAN_POLLING )  && defined( C_ENABLE_RX_FULLCAN_OBJECTS )) || \
    defined(C_ENABLE_INDIVIDUAL_POLLING)
  rxInt = 1u;
#else
  rxInt = 0u;
#endif
#if !defined( C_ENABLE_ERROR_POLLING )
  errInt = 1u;
#else
  errInt = 0u;
#endif
  wakInt = 1u;
  /* #10 enable all configured Interrupts */
  CanCell_EnableInterrupts(channel, txInt, rxInt, errInt, wakInt); /* SBSW_CAN_LL08 */
  CanCell_AtomicEnd();

  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitEnd
Relation_Context:
# CanHL_InitEnd_InitMode() #
Wakeup, OneChOpt, ChannelAmount
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 finish init */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_InitIsMailboxCorrupt
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanHL_InitIsMailboxCorrupt
Relation_Context:
# CanHL_InitXXX() #
Wakeup, OneChOpt, ChannelAmount
TxFullCANSupport, RamCheck
RxFullCANSupport, RamCheck
RxBasicCANSupport, RamCheck
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_InitIsMailboxCorrupt( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 testPatternIdx;
  uint8 isMBRamCheckFail = kCanFalse;
  vuint8 const testPattern[3] = {0x55,0xAA,0x00};
  /* just for simulation no real hardware */
  /* #10 emulate controller register Pattern check */
  for(testPatternIdx = 0u; testPatternIdx < 3u; testPatternIdx++)
  {
    /* Use only mode register for simulation - not intended to test all */ 
    initPara->canLLController->MsgObj[initPara->hwObjHandle].mIsFree = testPattern[testPatternIdx]; /* SBSW_CAN_HL36 */
    if(initPara->canLLController->MsgObj[initPara->hwObjHandle].mIsFree != testPattern[testPatternIdx]) /* COV_CAN_LL_EMULATION */
    {
      isMBRamCheckFail = kCanTrue;
    }
  }
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return isMBRamCheckFail;
}
/* CODE CATEGORY 4 END */
#endif /* C_ENABLE_CAN_RAM_CHECK */


/**********************************************************************************************************************
 *  CanLL_InitPowerOn
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitPowerOn
Relation_Context:
# from Can_Init #
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitPowerOn(void)
{
  /* #10 No global setting needed */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitPowerOnChannelSpecific
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitPowerOnChannelSpecific
Relation_Context:
# from Can_Init #
Relation:
ChannelAmount, OneChOpt
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_InitPowerOnChannelSpecific( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* #10 No global setting needed */
  Can_GetgCanController(channel).Mode = CAN_MODE_UNINIT; /* SBSW_CAN_LL08 */
  CAN_CHANNEL_DUMMY_STATEMENT;  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanOk;
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_InitMemoryPowerOn
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_InitMemoryPowerOn
Relation_Context:
Relation:
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_InitMemoryPowerOn(void)
{
  /* #10 No global initialization needed */
}
/* CODE CATEGORY 4 END */

#if defined( C_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_TxCancelInHw
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxCancelInHw
Relation_Context:
# from Can_CancelTx #
TransmitCancellationAPI, TxFullCANSupport, HwCancelTx
# from CanHL_WritePrepareCancelTx #
Relation:
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
/* CODE CATEGORY 3 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxCancelInHw( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxCancellationParaStructPtr txCancellationPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 just simulate cancellation, but no real cancellation will be executed */
  /*
     Do not activate the following line! CANoe is only a emulation and does not really cancel the message out of hardware
     Can_GetgCanController(channel).MsgObj[txObjHandle].mCanCancel = (vuint8)kCanTrue;  SBSW_CAN_LL01
  */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(txCancellationPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 3 END */
#endif



/**********************************************************************************************************************
 *  CanLL_TxBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxBegin
Relation_Context:
# from Can_Write #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set mailbox pointer */
  txPara->canLLMsgObj = &Can_GetgCanController(channel).MsgObj[txPara->hwObjHandle + txPara->mailboxElement]; /* SBSW_CAN_HL42 */
  /* #20 lock mailbox (mark as busy) */
  txPara->canLLMsgObj->mIsFree = (vuint8)kCanFalse; /* SBSW_CAN_HL42 */
  txPara->canLLMsgObj->mCanCancel = (vuint8)kCanFalse; /* SBSW_CAN_HL42 */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_TxSetMailbox
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxSetMailbox
Relation_Context:
# from Can_Write #
Relation:
IDType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxSetMailbox( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set ID */
  txPara->canLLMsgObj->mMsg.IdRaw = txPara->idRaw0; /* SBSW_CAN_HL42 */
  /* #20 set DLC */
  txPara->canLLMsgObj->mMsg.DlcRaw = txPara->dlcRaw; /* SBSW_CAN_HL42 */
# if defined( C_ENABLE_CAN_FD_USED )
  txPara->canLLMsgObj->mObjType = (txPara->fdType == kCanFdTypeFd) ? (vuint8)kCanTrue : (vuint8)kCanFalse; /* SBSW_CAN_HL42 */
# endif
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */



/**********************************************************************************************************************
 *  CanLL_TxCopyToCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxCopyToCan
Relation_Context:
# from Can_Write #
Relation:
CanFdSupport
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxCopyToCan( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 copy message data to mailbox */
#  if defined( C_ENABLE_CAN_FD_FULL )
  vuint8 idx;
  VStdMemCpy( (void*)txPara->canLLMsgObj->mMsg.DataFld, (void*)txPara->CanMemCopySrcPtr, txPara->messageLen); /* PRQA S 0314,0312,0315,1331,1496,2869,4604,2869 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
  /* #20 fill CAN frame with padding values */
  for (idx=txPara->messageLen; idx < txPara->frameLen; idx++)
  {
    txPara->canLLMsgObj->mMsg.DataFld[idx] = txPara->paddingVal; /* SBSW_CAN_LL02 */
  }
#  else
  VStdMemCpy( (void*)txPara->canLLMsgObj->mMsg.DataFld, (void*)txPara->CanMemCopySrcPtr, 8u); /* PRQA S 0314,0312,0315,1331,1496,2869,4604,2869 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
#  endif
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_TxStart
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxStart
Relation_Context:
# from Can_Write #
Relation:
CanFdSupport, IDType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxStart( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* Message Flags 
     1 RTR-Flag 
     2 Wakeup (high voltage message on single wire CAN)
     4 transceiver error (NERR) on two wire low speed CAN bus */
  vuint32 flags;
  
#  if defined(C_ENABLE_TX_HW_FIFO)
  /* This is a workaround to force FIFO to send in expected order (CANoe will send all requested elements in priority order) 
        --> request only one FIFO element to send and retrigger next in confirmation */
  /* Get next FIFO element to send or skip in order to wait for previous to be send first (simulated mode send prio-oriented, so send only one by one -> delay the others) */
  if (Can_GetMailboxType(txPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE_MAILBOXTYPEOFMAILBOX )
  {
    vuint8 fifoIdx = Can_GetMailboxTxHwFifo(txPara->mailboxHandle);
    vuint8 preFifoElement = ((txPara->mailboxElement-1) >= 0) ? (txPara->mailboxElement-1) : ((txPara->mailboxElement-1) + Can_GetMailboxSize(txPara->mailboxHandle));
    if ( Can_GetFillCountOfTxHwFifo(fifoIdx) == 1 ) 
    { /* 1st write request - always send direct */
      /* nothing to do here txPara is already set */
    }
    else if ( (Can_GetFillCountOfTxHwFifo(fifoIdx) > 1) && (Can_GetgCanController(channel).MsgObj[Can_GetMailboxHwHandle(txPara->mailboxHandle) + preFifoElement].mIsPending == (vuint8)kCanTrue))
    { /* previous FIFO element is already send - so this/next can be send   (request out of CANoe-Tx handler or next send request before confirmation handling finished */
      txPara->canLLMsgObj = &Can_GetgCanController(channel).MsgObj[Can_GetMailboxHwHandle(txPara->mailboxHandle) + txPara->mailboxElement];
      txPara->canLLMsgObj->mIsFree = (vuint8)kCanFalse; /* mark element as !Free - ready for execution in confirmation */
    }
    else
    { /* previous is not send so wait for it */
      txPara->canLLMsgObj->mIsFree = (vuint8)kCanTrue; /* mark element as Free to retrigger send request in CANoe-Tx handler */
      return kCanOk;
    }
  }
#  endif

  /* #10 set HighVoltageMode */
  if ( Can_GetgCanController(channel).HighVoltageMode == 1u ) /* COV_CAN_LL_HIGHVOLTAGE */
  {
    flags = 2u;
  }
  else
  {
    flags = 0u;
  }
# if defined( C_ENABLE_CAN_FD_USED )
  /* #20 set EDL and BRS bit if needed */
  if ( txPara->canLLMsgObj->mObjType == (vuint8)kCanTrue )
  {
    flags |= (vuint32)CANOEAPI_CAN_EDL;
    if ( CanHL_IsFdTxBrs(channel) )
    {
      flags |= (vuint32)CANOEAPI_CAN_BRS;
    }
    txPara->canLLMsgObj->mMsg.IdRaw |= CAN_ID_FD_MASK; /* SBSW_CAN_HL42 */
  }
# endif

  /* #30 send message */
  CanCell_SendMessage((vuint8)channel,                                /* channel */  /* PRQA S 0404,0312,4434 */ /* conversion is ok here */ /* SBSW_CAN_LL09 */
                       (txPara->canLLMsgObj->mMsg.IdRaw & (~(vuint32)CAN_ID_FD_MASK)),            /* can identifier */
                       (CanoeApiDLCType)txPara->canLLMsgObj->mMsg.DlcRaw,      /* data length code */
                       (vuint8*)txPara->canLLMsgObj->mMsg.DataFld, /* data field */
                       flags);                                        /* message flags */ 
  return kCanOk;
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_TxEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxEnd
Relation_Context:
# from Can_Write #
Relation:
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(txPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

#if defined( C_ENABLE_TX_POLLING )
/**********************************************************************************************************************
 *  CanLL_TxIsGlobalConfPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxIsGlobalConfPending
Relation_Context:
# from Can_MainFunction_Write #
TxPolling, TxFullCANSupport
Relation:
ChannelAmount
OneChOpt
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxIsGlobalConfPending( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanTrue;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_TxProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxProcessPendings
Relation_Context:
# from Can_MainFunction_Write #
TxPolling, TxFullCANSupport
Relation:
MultiplexedTx
TxBasicAmount
TxFullAmount
ChannelAmount
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxProcessPendings( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  uint8 mailboxElement = 0;
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
  CanObjectHandle hwObjStop;
#  if defined(C_ENABLE_TX_HW_FIFO)
  CanObjectHandle hwObjMailboxEnd;
  hwObjMailboxEnd = Can_GetMailboxHwHandle(taskPara->mailboxHandle) + Can_GetMailboxSize(taskPara->mailboxHandle);
  if (Can_GetMailboxType(taskPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    uint16 txHwFifo = Can_GetMailboxTxHwFifo(taskPara->mailboxHandle);
    mailboxElement = (CanObjectHandle)Can_GetTxHwFifoReadIndex(txHwFifo);
    taskPara->hwObjHandle = taskPara->hwObjHandle + mailboxElement; /* SBSW_CAN_HL43 */
    hwObjStop = taskPara->hwObjHandle + (CanObjectHandle)Can_GetTxHwFifoFillCount(txHwFifo);
  } else
#  endif
  {
    hwObjStop = taskPara->hwObjHandle + Can_GetMailboxSize(taskPara->mailboxHandle);
  }
  /* #10 iterate over all hardware objects */
  for (; taskPara->hwObjHandle < hwObjStop;) /* PRQA S 2472 */ /* Runtime optimization */ /* COV_CAN_HARDWARE_FAILURE */
# endif
  {
    /* #20 check pending messages */
    if (Can_GetgCanController(channel).MsgObj[taskPara->hwObjHandle].mIsPending == kCanTrue)
    {
      /* #30 call processing */
      CanHL_TxConfirmationPolling(CAN_HW_CHANNEL_CANPARA_FIRST taskPara->mailboxHandle, mailboxElement, taskPara->hwObjHandle);
    }
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
#  if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(taskPara->mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      if (taskPara->hwObjHandle == (hwObjMailboxEnd - 1u)) {
        taskPara->hwObjHandle = Can_GetMailboxHwHandle(taskPara->mailboxHandle); /* SBSW_CAN_HL43 */
        hwObjStop -= Can_GetMailboxSize(taskPara->mailboxHandle);
        mailboxElement=0;
      } else {
        taskPara->hwObjHandle++; /* SBSW_CAN_HL43 */
        mailboxElement++;
      }
    } else
#  endif
    {
      taskPara->hwObjHandle++; /* SBSW_CAN_HL43 */
      mailboxElement++;
    }
# endif
  }
}
/* CODE CATEGORY 2 END */
#endif /* C_ENABLE_TX_POLLING */

/**********************************************************************************************************************
 *  CanLL_TxConfBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxConfBegin
Relation_Context:
# from CanHL_TxConfirmation #
Relation:
TxBasicAmount
TxFullAmount
ChannelAmount
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set mailbox pointer */
  txConfPara->canLLMsgObj = &Can_GetgCanController(channel).MsgObj[txConfPara->hwObjHandle]; /* SBSW_CAN_HL46 */
  /* #20 free mailbox */
  txConfPara->canLLMsgObj->mIsPending = (vuint8)kCanFalse; /* SBSW_CAN_HL46 */
  txConfPara->canLLMsgObj->mIsFree = (vuint8)kCanTrue; /* SBSW_CAN_HL46 */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

#if defined( C_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_TxConfIsMsgTransmitted
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxConfIsMsgTransmitted
Relation_Context:
# from CanHL_TxConfirmation #
HwCancelTx
Relation:
TxBasicAmount
TxFullAmount
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
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_TxConfIsMsgTransmitted( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval = kCanFalse;
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* #10 check mailbox is cancelled sucessful */
  if (txConfPara->canLLMsgObj->mCanCancel == (vuint8)kCanFalse) /* COV_CAN_LL_EMULATION */
  {
    retval = kCanTrue;
  }
  return retval;
}
/* CODE CATEGORY 1 END */
#endif

#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
/**********************************************************************************************************************
 *  CanLL_TxConfSetTxConfStruct
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxConfSetTxConfStruct
Relation_Context:
# from CanHL_TxConfirmation #
GenericConfirmation
Relation:
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfSetTxConfStruct( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set mailbox data pointer */
  txConfPara->txStructConf->pChipData = txConfPara->canLLMsgObj->mMsg.DataFld; /* SBSW_CAN_HL46 */
  /* #20 set mailbox pointer */
  txConfPara->txStructConf->pChipMsgObj = (CanChipMsgPtr) (&(txConfPara->canLLMsgObj->mMsg)); /* only for macros used in pretransmit */ /* SBSW_CAN_HL46 */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */
#endif

/**********************************************************************************************************************
 *  CanLL_TxConfEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_TxConfEnd
Relation_Context:
# from CanHL_TxConfirmation #
Relation:
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_TxConfEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(txConfPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */


#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_RxBasicMsgReceivedBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxBasicMsgReceivedBegin
Relation_Context:
# from CanHL_BasicCanMsgReceived #
RxBasicCANSupport
Relation:
HardwareLoopCheck
CanFdSupport
Overrun
OneChOpt
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxBasicMsgReceivedBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval;
  retval = kCanOk;
  /* #10 set mailbox pointer */
  rxBasicPara->canLLMsgObj = &Can_GetgCanController(channel).MsgObj[rxBasicPara->hwObjHandle]; /* SBSW_CAN_HL29 */

  /* #20 copy data in Atomic section to secure data */
  CanCell_AtomicBegin();
# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerStart(kCanLoopRx); /* SBSW_CAN_HL38 */
# endif
  /* #30 copy receive data until data is consistent */
  do
  { /* repeat until consistent data , this loop is only for emulation of a real hardware (with atomic data access no inconsistent data will appear) */
    rxBasicPara->canLLMsgObj->mIsPending = (vuint8)kCanFalse; /* release pending object*/ /* SBSW_CAN_HL29 */
    Can_GetRxMsg(channel).IdRaw = rxBasicPara->canLLMsgObj->mMsg.IdRaw; /* SBSW_CAN_LL03 */
    Can_GetRxMsg(channel).DlcRaw = rxBasicPara->canLLMsgObj->mMsg.DlcRaw; /* SBSW_CAN_LL03 */
# if (CAN_FD_SUPPORT == CAN_FULL) 
    VStdMemCpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)rxBasicPara->canLLMsgObj->mMsg.DataFld, CAN_DLC2LEN(Can_GetRxMsg(channel).DlcRaw)); /* PRQA S 0314,0312,0315,1331,1496,2869,4604 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
# else
    VStdMemCpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)rxBasicPara->canLLMsgObj->mMsg.DataFld, CAN_LL_MIN(8, CAN_DLC2LEN(Can_GetRxMsg(channel).DlcRaw))); /* PRQA S 0314,0312,0315,1331,1496,2869,4604,1862,1864 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
# endif
# if defined( C_ENABLE_HW_LOOP_TIMER )
    if (CanLL_ApplCanTimerLoop(kCanLoopRx) != CAN_OK) /* COV_CAN_LL_EMULATION */
    {
      retval = kCanFailed;
      break;
    }
# endif
  } while( rxBasicPara->canLLMsgObj->mIsPending == (vuint8)kCanTrue ); /* new message appear meanwhile -> repeat copy */ /* COV_CAN_LL_RECEIVE_LOOP */
# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerEnd(kCanLoopRx); /* SBSW_CAN_HL38 */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */
# endif
# if defined( C_ENABLE_OVERRUN )
  if ( rxBasicPara->canLLMsgObj->mCanOverrun == (vuint8)kCanTrue )
  { /* read out gCanController here (inside CanCell_AtomicBegin) */
    /* reset overrun */
    rxBasicPara->canLLMsgObj->mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL29 */
    rxBasicPara->isOverrun = (vuint8)kCanTrue; /* SBSW_CAN_HL29 */
  } 
  else 
  {
    rxBasicPara->isOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL29 */
  }
# endif
  CanCell_AtomicEnd();
  
  rxBasicPara->rxStruct.pChipMsgObj = (CanChipMsgPtr) (&Can_GetRxMsg(channel)); /* SBSW_CAN_HL29 */
  rxBasicPara->rxStruct.pChipData = (CanChipDataPtr) (&Can_GetRxMsg(channel).DataFld); /* SBSW_CAN_HL29 */ /* CM_CAN_HL31 */
  return retval;
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_RxBasicReleaseObj
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxBasicReleaseObj
Relation_Context:
# from CanHL_BasicCanMsgReceived #
RxBasicCANSupport
Relation:
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicReleaseObj( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(rxBasicPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_RxBasicMsgReceivedEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxBasicMsgReceivedEnd
Relation_Context:
# from CanHL_BasicCanMsgReceived #
RxBasicCANSupport
Relation:
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicMsgReceivedEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(rxBasicPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

# if defined( C_ENABLE_RX_BASICCAN_POLLING ) /* COV_CAN_RX_BASICCAN_POLLING */
/**********************************************************************************************************************
 *  CanLL_RxBasicIsGlobalIndPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxBasicIsGlobalIndPending
Relation_Context:
# from CanHL_RxBasicCanPolling #
RxBasicCANSupport, RxPolling
Relation:
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxBasicIsGlobalIndPending( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanTrue;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_RxBasicProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxBasicProcessPendings
Relation_Context:
# from CanHL_RxBasicCanPolling #
RxBasicCANSupport, RxPolling
Relation:
IndividualPolling
OneChOpt
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxBasicProcessPendings( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  CanObjectHandle bCanWrittenPointer;
  CanObjectHandle hwObjHandle;
  CanObjectHandle bCanSize;
  vuint8 bCanIdx;

  bCanIdx = taskPara->mailboxHandle - Can_GetRxBasicHandleStart(canHwChannel);
  bCanSize = Can_GetMailboxSize(taskPara->mailboxHandle);
  bCanWrittenPointer = Can_GetgCanController(channel).BCanWrittenPointer[bCanIdx] + taskPara->hwObjHandle;
  hwObjHandle = bCanWrittenPointer;
  /* #10 search for next hardware BasicCAN-FIFO-element to be executed  */
  do
  {
    hwObjHandle++;
    if (hwObjHandle >= (taskPara->hwObjHandle+bCanSize))
    {
      hwObjHandle = taskPara->hwObjHandle;
    }
    /* #20 check pending flag - call indication */
    if (Can_GetgCanController(channel).MsgObj[hwObjHandle].mIsPending == (vuint8)kCanTrue)
    {
      CanHL_BasicCanMsgReceivedPolling(CAN_HW_CHANNEL_CANPARA_FIRST taskPara->mailboxHandle, hwObjHandle);
    }
  } while (hwObjHandle != bCanWrittenPointer);
}
/* CODE CATEGORY 2 END */
# endif /* C_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined( C_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_RxFullMsgReceivedBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxFullMsgReceivedBegin
Relation_Context:
# from CanHL_FullCanMsgReceived #
RxFullCANSupport
Relation:
HardwareLoopCheck
CanFdSupport
Overrun
OneChOpt
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxFullMsgReceivedBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 retval;
  retval = kCanOk;
  /* #10 set mailbox pointer */
  rxFullPara->canLLMsgObj = &Can_GetgCanController(channel).MsgObj[rxFullPara->hwObjHandle]; /* SBSW_CAN_HL30 */
  /* #20 copy data in Atomic section to secure data */
  CanCell_AtomicBegin();
# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerStart(kCanLoopRx); /* SBSW_CAN_HL38 */
# endif
  /* #30 copy receive data until data is consistent */
  do
  { /* repeat until consistent data , this loop is only for emulation of a real hardware (with atomic data access no inconsistent data will appear) */
    rxFullPara->canLLMsgObj->mIsPending = (vuint8)kCanFalse; /* release pending object*/ /* SBSW_CAN_HL30 */
    Can_GetRxMsg(channel).IdRaw = rxFullPara->canLLMsgObj->mMsg.IdRaw; /* SBSW_CAN_LL03 */
    Can_GetRxMsg(channel).DlcRaw = rxFullPara->canLLMsgObj->mMsg.DlcRaw; /* SBSW_CAN_LL03 */
# if (CAN_FD_SUPPORT == CAN_FULL) 
    VStdMemCpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)rxFullPara->canLLMsgObj->mMsg.DataFld, CAN_DLC2LEN(Can_GetRxMsg(channel).DlcRaw)); /* PRQA S 0314,0312,0315,1331,1496,2869,4604 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
# else
    VStdMemCpy( (void*)Can_GetRxMsg(channel).DataFld, (void*)rxFullPara->canLLMsgObj->mMsg.DataFld, CAN_LL_MIN(8, CAN_DLC2LEN(Can_GetRxMsg(channel).DlcRaw))); /* PRQA S 0314,0312,0315,1331,1496,4604,1862,1864 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL02 */
# endif
# if defined( C_ENABLE_HW_LOOP_TIMER )
    if (CanLL_ApplCanTimerLoop(kCanLoopRx) != CAN_OK) /* COV_CAN_LL_EMULATION */
    {
      retval = kCanFailed;
      break;
    }
# endif
  } while( rxFullPara->canLLMsgObj->mIsPending == (vuint8)kCanTrue ); /* new message appear meanwhile -> repeat copy */ /* COV_CAN_LL_RECEIVE_LOOP */
# if defined( C_ENABLE_HW_LOOP_TIMER )
  CanLL_ApplCanTimerEnd(kCanLoopRx); /* SBSW_CAN_HL38 */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */
# endif
# if defined(C_ENABLE_FULLCAN_OVERRUN)
  if ( rxFullPara->canLLMsgObj->mCanOverrun == (vuint8)kCanTrue )
  {  /* read out gCanController here (inside CanCell_AtomicBegin) */
     /* reset overrun */
     rxFullPara->canLLMsgObj->mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL30 */
     rxFullPara->isOverrun = (vuint8)kCanTrue; /* SBSW_CAN_HL30 */
  } 
  else
  {
     rxFullPara->isOverrun = (vuint8)kCanFalse; /* SBSW_CAN_HL30 */
  }
# endif
  CanCell_AtomicEnd();
  
  rxFullPara->rxStruct.pChipMsgObj = (CanChipMsgPtr) (&Can_GetRxMsg(channel)); /* SBSW_CAN_HL30 */
  rxFullPara->rxStruct.pChipData = (CanChipDataPtr) (&Can_GetRxMsg(channel).DataFld); /* SBSW_CAN_HL30 */ /* CM_CAN_HL31 */
  return retval;
}
/* CODE CATEGORY 1 END */


/**********************************************************************************************************************
 *  CanLL_RxFullReleaseObj
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxFullReleaseObj
Relation_Context:
# from CanHL_FullCanMsgReceived #
RxFullCANSupport
Relation:
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
 V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullReleaseObj( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(rxFullPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_RxFullMsgReceivedEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxFullMsgReceivedEnd
Relation_Context:
# from CanHL_FullCanMsgReceived #
RxFullCANSupport
Relation:
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullMsgReceivedEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(rxFullPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

# if defined( C_ENABLE_RX_FULLCAN_POLLING ) /* COV_CAN_RX_FULLCAN_POLLING */
/**********************************************************************************************************************
 *  CanLL_RxFullIsGlobalIndPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxFullIsGlobalIndPending
Relation_Context:
# from CanHL_RxFullCanPolling #
RxFullCANSupport, RxPolling
Relation:
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_RxFullIsGlobalIndPending( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* #10 nothing to do here */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return kCanTrue;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_RxFullProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_RxFullProcessPendings
Relation_Context:
# from CanHL_RxFullCanPolling #
RxFullCANSupport, RxPolling
Relation:
OneChOpt
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_RxFullProcessPendings( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #20 check pending messages */
  if (Can_GetgCanController(channel).MsgObj[taskPara->hwObjHandle].mIsPending == kCanTrue)
  {
    /* #30 call processing */
    CanHL_FullCanMsgReceivedPolling(CAN_HW_CHANNEL_CANPARA_FIRST taskPara->mailboxHandle, taskPara->hwObjHandle);
  }
}
/* CODE CATEGORY 2 END */
# endif /* C_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */


#if defined (CAN_ENABLE_SECURITY_EVENT_REPORTING) 
/**********************************************************************************************************************
 *  CanLL_ErrorStatePassiveTransitionOccured
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_ErrorStatePassiveTransitionOccured
Relation_Context:
# from CanHL_ErrorHandling() #
StatusPolling
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
SecurityEvent
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_ErrorStatePassiveTransitionOccured( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint8 retval = kCanFailed;
  /* #10 Check if a transistion to error state passive has occured */
  if (Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_ERRORPASSIVE_EVENT)
  {
    Can_GetgCanController(channel).ErrCode = (vuint32)CANCELL_ERRORPASSIVE; /* notify only once */ /* SBSW_CAN_LL08 */
    retval = kCanTrue;
  }
  return retval;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_ErrorNotificationHandling
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_ErrorNotificationHandling
Relation_Context:
# from CanHL_ErrorHandling() #
StatusPolling
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
SecurityEvent
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorNotificationHandling( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  /* none of the possible Can_ErrorType can be detected by CanoeApi -> VCanErrHandler / refer to Can_GetgCanController(channel).ErrCode */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 2 END */
#endif /* CAN_ENABLE_SECURITY_EVENT_REPORTING */

/**********************************************************************************************************************
 *  CanLL_ErrorHandlingBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_ErrorHandlingBegin
Relation_Context:
# from CanHL_ErrorHandling() #
StatusPolling
Relation:
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorHandlingBegin( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* #10 atomic operation begin */
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CanCell_AtomicBegin();
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_BusOffOccured
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_BusOffOccured
Relation_Context:
# from CanHL_ErrorHandling() #
StatusPolling
Relation:
ChannelAmount
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_BusOffOccured( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  vuint8 retval = kCanFalse;
  /* #10 check bus off occur */
  if ( Can_GetgCanController(channel).ErrCode == (vuint32)CANCELL_BUSOFF )
  {
    retval = kCanTrue;
  }
  return retval;
}
/* CODE CATEGORY 2 END */


/**********************************************************************************************************************
 *  CanLL_ErrorHandlingEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_ErrorHandlingEnd
Relation_Context:
# from CanHL_ErrorHandling() #
StatusPolling
Relation:
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
/* CODE CATEGORY 2 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_ErrorHandlingEnd( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* #10 atomic operation end */
  CanCell_AtomicEnd();
  CAN_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 2 END */

#if defined( C_ENABLE_EXTENDED_STATUS )
/**********************************************************************************************************************
 *  CanLL_GetStatusBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_GetStatusBegin
Relation_Context:
# from Can_GetStatus
CanGetStatus
Relation:
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
/* CODE CATEGORY 3 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_GetStatusBegin( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* #10 nothing to do here */
  CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 3 END */
#endif


#if defined( C_ENABLE_SLEEP_WAKEUP ) 
/**********************************************************************************************************************
 *  CanLL_WakeUpHandling
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_WakeUpHandling
Relation_Context:
# from Can_MainFunction_Wakeup #
WakeupPolling, Wakeup
# from Wakeup Interrupt #
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_WakeUpHandling( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* notify the application here and call WAKEUP handling */
  /* #40 call wakeup transition handling */
  /* stop transition will be triggered after notification by CanIf */
  /* In case of CommonCAN, it might be necessary to call CAN_WAKEUP() even if the hardware
     wakes up automatically to make sure all associated HW channels are awaken. */
  CAN_WAKEUP( channel );
  /* #50 call wakeup indication */
  APPL_CAN_WAKEUP( channel );
}
/* CODE CATEGORY 4 END */
#endif

/**********************************************************************************************************************
 *  CanLL_ModeTransition
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_ModeTransition
Relation_Context:
# from CanHL_ModeTransition() #
Wakeup, OneChOpt, HardwareLoopCheck, ChannelAmount
RamCheck, OneChOpt, HardwareLoopCheck, ChannelAmount
# from Can_Mainfunction_Mode() #
ChannelAmount
Relation:
Wakeup
SilentMode
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_ModeTransition( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 busOffRecovery, vuint8 doRamCheck )
{
  vuint8 retval = kCanFailed;

  { /* no precondition for mode changes necessary */
    switch(mode)
    {
#if defined(C_ENABLE_SLEEP_WAKEUP)
      case kCanModeSleep:
        /* #100 SLEEP request */
# if  defined(C_ENABLE_HW_LOOP_TIMER)
        /* just for emulation of HW behavior do the mode transition within the second call (first just return a kCanRequested - second will do the mode change) */
        if (Can_GetgCanController(channel).Mode != CAN_MODE_INTER)
        {
          Can_GetgCanController(channel).Mode = CAN_MODE_INTER; /* SBSW_CAN_LL08 */
          retval = kCanRequested;
        }
        else
# endif
        {
          CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, Can_GetLastInitObject(channel), CANOEAPI_CAN_NORMALMODE); /* COV_CAN_LL_EMULATION */
          CanCell_SleepChannel((const char*)Can_GetBusName(channel), channel);
          Can_GetgCanController(channel).Mode = CAN_MODE_SLEEP; /* SBSW_CAN_LL08 */
          retval = kCanOk;
        }
        break;


      case kCanModeWakeupStopReinit:
        /* #300 WAKEUP-STOP request */
# if  defined(C_ENABLE_HW_LOOP_TIMER)
        /* just for emulation of HW behavior do the mode transition within the second call (first just return a kCanRequested - second will do the mode change) */
        if (Can_GetgCanController(channel).Mode != CAN_MODE_INTER)
        {
          Can_GetgCanController(channel).Mode = CAN_MODE_INTER; /* SBSW_CAN_LL08 */
          retval = kCanRequested;
        }
        else
# endif
        {
          (void)CanHL_ReInit(CAN_CHANNEL_CANPARA_FIRST doRamCheck);
          CanCell_WakeupChannel((const char*)Can_GetBusName(channel), channel);
          Can_GetgCanController(channel).Mode = CAN_MODE_STOP; /* SBSW_CAN_LL08 */
          retval = kCanOk;
        }
        break;
#endif /* C_ENABLE_SLEEP_WAKEUP */

      case kCanModeStopReinitFast:
      case kCanModeStopReinit:
      case kCanModeResetBusOffStart:
        /* #400 STOP (reinit, busoff) request */
#if  defined(C_ENABLE_HW_LOOP_TIMER)
        /* just for emulation of HW behavior do the mode transition within the second call (first just return a kCanRequested - second will do the mode change) */
        if (Can_GetgCanController(channel).Mode != CAN_MODE_INTER)
        {
          Can_GetgCanController(channel).Mode = CAN_MODE_INTER; /* SBSW_CAN_LL08 */
          retval = kCanRequested;
        }
       else
#endif
        {
          if (mode == kCanModeStopReinit)
          {
            (void)CanHL_ReInit(CAN_CHANNEL_CANPARA_FIRST doRamCheck);
          }
          else 
          {
            CanLL_StopReinit(CAN_CHANNEL_CANPARA_ONLY);
            CanHL_CleanUpSendState(CAN_CHANNEL_CANPARA_ONLY);
          }
          CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, Can_GetLastInitObject(channel), CANOEAPI_CAN_SILENTMODE); /* COV_CAN_LL_EMULATION */
          CanCell_StopChannel((const char*)Can_GetBusName(channel), channel);
          Can_GetgCanController(channel).Mode = CAN_MODE_STOP; /* SBSW_CAN_LL08 */
          retval = kCanOk;
        }
        break;

#if (CAN_REINIT_START == STD_ON)
      case kCanModeStartReinit:
#else
      case kCanModeStart:
#endif
      case kCanModeResetBusOffEnd: /* no special handling necessary - just go to START */
        /* #500 START (reinit, busoff) request */
#if  defined(C_ENABLE_HW_LOOP_TIMER)
        /* just for emulation of HW behavior do the mode transition within the second call (first just return a kCanRequested - second will do the mode change) */
        if (Can_GetgCanController(channel).Mode != CAN_MODE_INTER)
        {
          Can_GetgCanController(channel).Mode = CAN_MODE_INTER; /* SBSW_CAN_LL08 */
          retval = kCanRequested;
        }
        else
#endif
        {
#if (CAN_REINIT_START != STD_ON) /* do it always if Reinit is active */
          if (mode == kCanModeResetBusOffEnd)
#endif
          {
            (void)CanHL_ReInit(CAN_CHANNEL_CANPARA_FIRST doRamCheck);
          }
          CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, Can_GetLastInitObject(channel), CANOEAPI_CAN_NORMALMODE); /* COV_CAN_LL_EMULATION */
          CanCell_StartChannel((const char*)Can_GetBusName(channel), channel);
          Can_GetgCanController(channel).Mode = CAN_MODE_START; /* SBSW_CAN_LL08 */
          retval = kCanOk;
        }
        break;

# if defined(C_ENABLE_SILENT_MODE)
      case kCanModeSilent:
        /* #600 SILENT/STOP request */
#  if  defined(C_ENABLE_HW_LOOP_TIMER)
        /* just for emulation of HW behavior do the mode transition within the second call (first just return a kCanRequested - second will do the mode change) */
        if (Can_GetgCanController(channel).Mode != CAN_MODE_INTER)
        {
          Can_GetgCanController(channel).Mode = CAN_MODE_INTER; /* SBSW_CAN_LL08 */
          retval = kCanRequested;
        }
        else
#  endif
        {
          CanCell_InitChannel((const char*)Can_GetBusName(channel), channel, Can_GetLastInitObject(channel), CANOEAPI_CAN_SILENTMODE); /* COV_CAN_LL_EMULATION */
          CanCell_StartChannel((const char*)Can_GetBusName(channel), channel);
          Can_GetgCanController(channel).Mode = CAN_MODE_START; /* SBSW_CAN_LL08 */
          retval = kCanOk;
        }
        break;
# endif

      default:
        /* just for emulation do also return kCanFailed */
        /* retval is kCanFailed */
        break;
    }
  }
  CAN_DUMMY_STATEMENT(busOffRecovery); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;
} /* PRQA S 6030,6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_StopReinit
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_StopReinit
Relation_Context:
# from CanLL_ModeTransition() #
Wakeup, OneChOpt, HardwareLoopCheck, ChannelAmount
Relation:
RamCheck
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
/* CODE CATEGORY 4 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_StopReinit( CAN_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  CanObjectHandle canHwObjIdx;
  CanCell_AtomicBegin();
  for(canHwObjIdx=0; canHwObjIdx < CAN_MAX_MAILBOX; canHwObjIdx++)
  { /* #10 clean pending flags for all mailboxes */
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mIsPending  = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mIsFree     = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mCanOverrun = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
    Can_GetgCanController(channel).MsgObj[canHwObjIdx].mCanCancel  = (vuint8)kCanFalse; /* SBSW_CAN_LL01 */
  }
  CanCell_AtomicEnd();
}
/* CODE CATEGORY 4 END */


#if defined( C_ENABLE_CAN_CAN_INTERRUPT_CONTROL ) && !defined( C_ENABLE_OSEK_CAN_INTCTRL ) && defined( C_HL_ENABLE_CAN_IRQ_DISABLE ) /* COV_CAN_OS_USAGE TX tx xf tx */
/**********************************************************************************************************************
 *  CanLL_CanInterruptDisable
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_CanInterruptDisable
Relation_Context:
# from Can_DisableControllerInterrupts #
IndividualPolling
TxPolling
RxPolling
StatusPolling
IntLock
Relation:
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptDisable( CAN_HW_CHANNEL_CANTYPE_FIRST tCanLLCanIntOldPtr localInterruptOldFlagPtr )
{
  /* #10 save old CAN interrupt status in given parameter */
  *(localInterruptOldFlagPtr) = Can_GetgCanController(canHwChannel).IntEnable; /* SBSW_CAN_LL12 */
  /* #20 deactivate the CAN interrupt */
  CanCell_EnableInterrupts(canHwChannel, 0u, 0u, 0u, 0u); /* SBSW_CAN_LL08 */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_CanInterruptRestore
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_CanInterruptRestore
Relation_Context:
# from Can_EnableControllerInterrupts #
IndividualPolling
TxPolling
RxPolling
StatusPolling
Relation:
OneChOpt
ChannelAmount
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_CanInterruptRestore( CAN_HW_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld localInterruptOldFlag )
{
  /* #10 restore old CAN interrupt status given as parameter */
  CanCell_EnableInterrupts((canHwChannel), (((localInterruptOldFlag) & CAN_INT_TX) / CAN_INT_TX), (((localInterruptOldFlag) & CAN_INT_RX) / CAN_INT_RX), /* PRQA S 2985 */ /* redundant operation used for understandable code */ /* SBSW_CAN_LL08 */
                                           (((localInterruptOldFlag) & CAN_INT_ERR) / CAN_INT_ERR), (((localInterruptOldFlag) & CAN_INT_WAKEUP) / CAN_INT_WAKEUP));
  CanCell_CallInterrupts(canHwChannel);
}
/* CODE CATEGORY 1 END */
#endif




#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS ) || defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanLL_RxIndicationMsgCopy
****************************************************************************/
/*
|<DataModelStart>| CanLL_RxIndicationMsgCopy
Relation_Context:
# from Can_RxIndication #
RxFullCANSupport
RxBasicCANSupport
Relation:
ChannelAmount
RamCheck
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE)  CanLL_RxIndicationMsgCopy( vuint8 canChannel, CanObjectHandle mailbox, CanObjectHandle hwObjHandle, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data )
{
  /* ----- Local Variables ---------------------------------------------- */
  vuint8 maxDlc;
  vuint8 hwDataLenMax;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 copy message data to RAM */
  if ((flags & (vuint32)CANOEAPI_CAN_EDL) == (vuint32)CANOEAPI_CAN_EDL) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mMsg.IdRaw = id | kCanFdTypeFd; /* SBSW_CAN_LL01 */
  }
  else
  {
    Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mMsg.IdRaw = id; /* SBSW_CAN_LL01 */
  }

  maxDlc = sizeof(Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mMsg.DataFld);
  hwDataLenMax = (dataLength > maxDlc) ? maxDlc : dataLength; /* COV_CAN_LL_EMULATION */
  Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mMsg.DlcRaw = CAN_LEN2DLC(hwDataLenMax); /* SBSW_CAN_LL01 */
  VStdMemCpy( (void*)Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mMsg.DataFld, (void*)data, CAN_LL_MIN(Can_GetMailboxDataLen(mailbox) ,hwDataLenMax)); /* PRQA S 0311,0312,0314,0315,1331,1496,4604,1862,1864 */ /* compiler intrinsic function not declared */ /* SBSW_CAN_LL01 */

  /* #20 set control bits */
# if defined( C_ENABLE_CAN_RAM_CHECK )
  if (Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mIsFree == (vuint8)kCanTrue) /* Mailbox is deactivated by RAM_CHECK */ /* COV_CAN_LL_EMULATION */
# endif
  {
    if (Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mIsPending == (vuint8)kCanTrue)
    {
      Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mCanOverrun = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
    }
    else
    {
      Can_GetgCanController(canChannel).MsgObj[hwObjHandle].mIsPending = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
    }
  }
  CAN_DUMMY_STATEMENT(mailbox); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* PRQA S 6060 */ /* MD_MSR_STPAR */
/* CODE CATEGORY 1 END */
#endif

#if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/****************************************************************************
| NAME:             CanHL_TxConfirmationPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_TxConfirmationPolling
Relation_Context:
# from CanLL_TxProcessPendings #
TxPolling
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_TxConfirmationPolling( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle mailboxHandle, CanObjectHandle mailboxElement, CanObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. (avoid nested call of confirmation (individual polling) and CancelTx interrupt confirmation out of Can interrupt like TP) */
  Can_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_TxConfirmation() for handling */
  CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST mailboxHandle, mailboxElement, hwObjHandle);
  Can_EnableControllerInterrupts((uint8)channel);
}
#endif

/****************************************************************************
| NAME:             CanHL_TxConfirmation
****************************************************************************/
/*
|<DataModelStart>| CanHL_TxConfirmation
Relation_Context:
# from CanHL_TxConfirmationPolling #
# from Tx Interrupt #
TxPolling, IndividualPolling
Relation:
TxHwFifo, GenericConfirmation, CanFdSupport
DevErrorDetect, TxPolling, OneChOpt
TransmitCancellationAPI
GenericConfirmation, IfTxBuffer
HwCancelTx
GenericConfirmation, MirrorMode
HwCancelTx, TransmitCancellationAPI, GenericConfirmation
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_TxConfirmation( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle mailboxHandle, CanObjectHandle mailboxElement, CanObjectHandle hwObjHandle )
{
  /* \trace SPEC-1574 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanTxConfirmationParaStruct txConfPara;
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
  tCanTxConfInfoStruct txConfInfoStruct = {0}; /* init see TAR-171 */
#endif
  CanHookBegin_CanHL_TxConfirmation();
  errorId = CAN_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  txConfPara.mailboxHandle = mailboxHandle;
  txConfPara.mailboxElement = mailboxElement; /* PRQA S 2983 */ /* MD_MSR_DummyStmt */
  txConfPara.hwObjHandle = hwObjHandle; /* PRQA S 2983 */ /* MD_MSR_DummyStmt */
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
  txConfPara.txStructConf = &txConfInfoStruct;
#endif
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  /* #10 Check if parameter controller and hwObjHandle is valid (only for Interrupt system, polling do this by caller) */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( 
#  if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
       ((txConfPara.mailboxHandle < Can_GetTxFullHandleStart(canHwChannel)) || (txConfPara.mailboxHandle >= Can_GetTxFullHandleStop(canHwChannel))) &&  /* PRQA S 2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
#  endif
       ((txConfPara.mailboxHandle < Can_GetTxBasicHandleStart(canHwChannel)) || (txConfPara.mailboxHandle >= Can_GetTxBasicHandleStop(canHwChannel))) ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    PduIdType tmp_pdu;
#if defined(C_ENABLE_CANCEL_SUPPORT_API)
    uint8_least tmp_state;
#endif
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
    Can_ReturnType generic_retval;
#endif
#if defined(C_ENABLE_TX_HW_FIFO)
    if (Can_GetMailboxType(txConfPara.mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #110 in case of Fifo element get the read index */
      txConfPara.mailboxElement = CanHL_GetNextTxHwFifoReadIndex(&txConfPara); /* SBSW_CAN_HL45 */
    }
#endif
    txConfPara.activeSendObject = CanHL_GetActiveSendObject(txConfPara.mailboxHandle, txConfPara.mailboxElement); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    {
      /* #120 Pre Release HW transmit object (LL) */
      CanLL_TxConfBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
      /* #130 Remember PDU for confirmation parameter */
      tmp_pdu   = Can_GetActiveSendPdu(txConfPara.activeSendObject);
#if defined(C_ENABLE_CANCEL_SUPPORT_API)
      /* #140 Remember SEND STATE which may be modified due to new transmit call in indication */
      tmp_state = (uint8_least)Can_GetActiveSendState(txConfPara.activeSendObject);
#endif
#if defined(C_ENABLE_CANCEL_TX_IN_HW) /* cancel interrupt occur */
      /* #150 If the message was successfully transmitted (without cancellation) (LL): */
      if (CanLL_TxConfIsMsgTransmitted(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara) == kCanTrue) /* SBSW_CAN_HL45 */ /* COV_CAN_HW_TRANSMIT_CANCELLATION */
#endif
      { /* #200 Set send state to free to allow next TX out of confirmation or direct (reload queue) */
        /* #210 Notify the application by call Appl_GenericConfirmation() and CanIf_TxConfirmation() */
        /*      - Appl_GenericConfirmation() if configured and mirror mode is active
                and afterwards depend of return value
                - CanIf_TxConfirmation() if generic confirmation return OK, or no generic is used, and TX is not cancelled */
#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API1)
        Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
        if (Appl_GenericConfirmation( tmp_pdu ) == CAN_OK)
#elif defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
# if defined(C_ENABLE_MIRROR_MODE)
        if (Can_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
        { /* MirrorMode is inactive so call the following CanIf_TxConfirmation() */
          Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
          generic_retval = CAN_OK;
        }
        else
# endif
        { /* #230 For generic confirmation with API2 copy PDU data as parameter for Appl_GenericConfirmation() */
# if defined(C_ENABLE_TX_HW_FIFO)
          if (Can_GetMailboxType(txConfPara.mailboxHandle) == CAN_TX_BASICCAN_FIFO_TYPE) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
          { /* #240 in case of FIFO get data out of Fifo storage */
            Can_PduType canPdu;
#  if defined(C_ENABLE_CAN_FD_FULL)
            uint8 canData[64];
#  else
            uint8 canData[8];
#  endif
            canPdu.sdu = (Can_SduPtrType)canData;
            /* copy temporary to secure data because of incoming can_Write() after free buffer */
            canPdu.id = Can_GetActiveSendId(txConfPara.activeSendObject); /* PRQA S 4391 */ /* MD_Can_IntegerCast */
            canPdu.length = Can_GetActiveSendLength(txConfPara.activeSendObject);
#  if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
            canPdu.length = CAN_HL_MIN((canPdu.length), (sizeof(canData)/sizeof(uint8))); /* CM_CAN_HL33 */ /* COV_CAN_TX_CONF_DATA_FAILURE */
#  endif
            VStdMemCpy((void*)canPdu.sdu, (void*)Can_GetActiveSendSdu(txConfPara.activeSendObject), canPdu.length); /* SBSW_CAN_HL56 */ /* PRQA S 0314,0316 */ /* MD_Can_PointerVoidCast */
            canPdu.swPduHandle = Can_GetActiveSendPdu(txConfPara.activeSendObject);
            Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */ /* release after copy data to avoid data inconsistency */
            /* call Appl_GenericConfirmation() and in case of CAN_OK also the following CanIf_TxConfirmation() */
            generic_retval = Appl_GenericConfirmation( (uint8)channel, &canPdu ); /* SBSW_CAN_HL32 */
          }
          else
# endif
          { /* #250 in case of none FIFO get data out of CAN cell buffer (LL txStructConf) */
            Can_PduType canPdu;
            /* #260 get confirmation data out of hardware */
            CanLL_TxConfSetTxConfStruct(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
            CanHL_SetCanPduInfo(&canPdu, &txConfPara); /* SBSW_CAN_HL32 */ /* SBSW_CAN_HL45 */
            canPdu.swPduHandle = tmp_pdu;
            Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */ /* release after copy data to avoid data inconsistency */
            /* call Appl_GenericConfirmation() and in case of CAN_OK also the following CanIf_TxConfirmation() */
            generic_retval = Appl_GenericConfirmation( (uint8)channel, &canPdu ); /* SBSW_CAN_HL32 */
          }
        }
        /* #300 Appl_GenericConfirmation() decide to call Indication */
        if (generic_retval == CAN_OK)
#else /* no generic confirmation */
        Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
#endif
        {
#if defined(C_ENABLE_CANCEL_SUPPORT_API)
          /* #310 If TX is SW cancelled call CanIf_CancelTxNotification() */
          if (tmp_state == kCanBufferCancelSw) /* COV_CAN_CANCEL_SW_STIMULATION */
          {
            CanIf_CancelTxNotification(tmp_pdu, FALSE);
          }
          else
#endif  /* #320 otherwise call CanIf_TxConfirmation() */
          {
            CanIf_TxConfirmation(tmp_pdu); /* \trace SPEC-1726, SPEC-1571 */
          }
        }
#if (defined(CAN_ENABLE_GENERIC_CONFIRMATION_API1) || defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)) && (CAN_TRANSMIT_BUFFER == STD_ON)
        /* #350 otherwise no CanIf_TxConfirmation() will be called so call CanIf_CancelTxNotification() just to notify IF to restart sending out of Queue */
        else
        {
# if defined(C_ENABLE_CANCEL_SUPPORT_API)
          CanIf_CancelTxNotification( tmp_pdu, FALSE );
# else
#  error "When using Generic Confirmation and Transmit buffer (If) the Cancel-support-api (if) has to be activated"
# endif
        }
#endif
      }
#if defined(C_ENABLE_CANCEL_TX_IN_HW)
      /* #400 otherwise mailbox is cancelled successful */
      else
      {
#  if defined(C_ENABLE_CANCEL_SUPPORT_API)
        if (tmp_state == kCanBufferCancelSw) /* COV_CAN_CANCEL_SW_STIMULATION */
        { /* #410 free active PDU to allow next TX out of confirmation or direct (reload queue) */
          Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree); /* SBSW_CAN_HL04 */
          /* #420 SW cancellation: just call CanIf_CancelTxNotification() (no interrupt lock used here! - worst case Can_Write() came in between and a unnecessary cancellation appear) */
          CanIf_CancelTxNotification( tmp_pdu, TRUE );
        }
        else
#  endif
        {
#  if defined(C_ENABLE_CANCEL_IN_HW) /* COV_CAN_CANCEL_IN_HW TX */
          /* always CancelSW when only FullCAN TX is cancelled so never called */
#   if (CAN_DEV_ERROR_DETECT == STD_ON)
          /* #440 Check send state is not free now, to secure that ActiveSendPduInfo is already set before used */
          if (Can_GetActiveSendState(txConfPara.activeSendObject) == kCanBufferFree) /* COV_CAN_HARDWARE_FAILURE */
          {
            errorId = CAN_E_PARAM_HANDLE;
          }
          else
#   endif
          { /* #450 EXCLUSIVE_AREA_3 (needed to avoid another Can_Write() while interface send out of queue (first send out of queue avoid inversion)) -> no call to upper layer here */
            SchM_Enter_Can_CAN_EXCLUSIVE_AREA_3();
            /* #460 free active PDU to allow next TX out of confirmation or direct (reload queue) */
            Can_SetActiveSendState(txConfPara.activeSendObject, kCanBufferFree);  /* SBSW_CAN_HL04 */          
            /* #470 call CanIf_CancelTxConfirmation() to re trigger queue \trace SPEC-1726, SPEC-1725, SPEC-1684, SPEC-1689 */
            CanIf_CancelTxConfirmation( Can_GetActiveSendSwPduHandle(txConfPara.activeSendObject), &Can_GetActiveSendPduInfo(txConfPara.activeSendObject) ); /* SBSW_CAN_HL25 */
            SchM_Exit_Can_CAN_EXCLUSIVE_AREA_3();
          }
#  endif
        }
      }
#endif
      /* #500 Post release HW transmit object (LL) */
      CanLL_TxConfEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
# if !defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_TXCNF_ID, errorId);
  }
# endif
#endif
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CanHookEnd_CanHL_TxConfirmation();
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if defined(C_ENABLE_TX_HW_FIFO)
/****************************************************************************
| NAME:             CanHL_GetNextTxHwFifoReadIndex
****************************************************************************/
/*
|<DataModelStart>| CanHL_GetNextTxHwFifoReadIndex
Relation_Context:
# from CanHL_TxConfirmation #
TxHwFifo
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_STATIC, CanObjectHandle, STATIC_CODE) CanHL_GetNextTxHwFifoReadIndex(CanTxConfirmationParaStructPtr txConfPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  CanObjectHandle mailboxElement;
  uint16 txHwFifo;
  /* #10 need to do the FIFO index access atomic - use EXCLUSIVE_AREA_7 */
  SchM_Enter_Can_CAN_EXCLUSIVE_AREA_7();
  txHwFifo = Can_GetMailboxTxHwFifo(txConfPara->mailboxHandle);
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
  if (txHwFifo >= (uint16)Can_GetSizeOfTxHwFifo()) /* CM_CAN_HL32 */ /* COV_CAN_GENDATA_FAILURE */
  {
    txHwFifo = 0u;
  }
# endif
  /* #20 get Fifo element read index */
  mailboxElement = (CanObjectHandle)Can_GetTxHwFifoReadIndex(txHwFifo);
  /* #30 move to next Fifo element read index */
  if (Can_GetTxHwFifoReadIndex(txHwFifo) < ((uint16)Can_GetMailboxSize(txConfPara->mailboxHandle) - (uint16)1u) ) /* CM_CAN_HL25 */
  {
    Can_IncTxHwFifoReadIndex(txHwFifo); /* SBSW_CAN_HL50 */
  }
  else
  {
    Can_SetTxHwFifoReadIndex(txHwFifo, 0u); /* SBSW_CAN_HL50 */
  }
  Can_DecTxHwFifoFillCount(txHwFifo); /* SBSW_CAN_HL50 */
  SchM_Exit_Can_CAN_EXCLUSIVE_AREA_7();
  return mailboxElement;
}

/****************************************************************************
| NAME:             CanHL_SetNextTxHwFifoWriteIndex
****************************************************************************/
/*
|<DataModelStart>| CanHL_SetNextTxHwFifoWriteIndex
Relation_Context:
# from CanHL_WriteStart #
TxHwFifo
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_SetNextTxHwFifoWriteIndex(CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  uint16 txHwFifo;
  /* #10 need to do the FIFO index access atomic - use EXCLUSIVE_AREA_7 */
  SchM_Enter_Can_CAN_EXCLUSIVE_AREA_7();
  txHwFifo = Can_GetMailboxTxHwFifo(txPara->mailboxHandle);
# if (CAN_SAFE_BSW == STD_ON) /* COV_CAN_SAFE_BSW */
  if (txHwFifo >= (uint16)Can_GetSizeOfTxHwFifo()) /* CM_CAN_HL32 */ /* COV_CAN_GENDATA_FAILURE */
  {
    txHwFifo = 0u;
  }
# endif
  /* #30 move to next Fifo element write index */
  Can_IncTxHwFifoFillCount(txHwFifo); /* SBSW_CAN_HL50 */
  if (Can_GetTxHwFifoWriteIndex(txHwFifo) < ((uint16)Can_GetMailboxSize(txPara->mailboxHandle) - (uint16)1u) )
  {
    Can_IncTxHwFifoWriteIndex(txHwFifo); /* SBSW_CAN_HL50 */
  }
  else
  {
    Can_SetTxHwFifoWriteIndex(txHwFifo,0); /* SBSW_CAN_HL50 */
  }
  SchM_Exit_Can_CAN_EXCLUSIVE_AREA_7();
}
#endif

#if defined(CAN_ENABLE_GENERIC_CONFIRMATION_API2)
/****************************************************************************
| NAME:             CanHL_SetCanPduInfo
****************************************************************************/
/*
|<DataModelStart>| CanHL_SetCanPduInfo
Relation_Context:
# from CanHL_TxConfirmation #
GenericConfirmation
Relation:
IDType
CanFdSupport
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_SetCanPduInfo(Can_PduInfoPtrType_var PduInfo, CanTxConfirmationParaStructPtr txConfPara) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set data pointer */
  PduInfo->sdu = (Can_SduPtrType) txConfPara->txStructConf->pChipData; /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL46 */
  /* #20 set data length */
  PduInfo->length = CanTxActualDLC(txConfPara->txStructConf); /* SBSW_CAN_HL46 */
  /* #30 set ID */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
  if ( CanTxActualIdType(txConfPara->txStructConf) == kCanIdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* Mixed ID */
    PduInfo->id = (Can_IdType)CanTxActualStdId(txConfPara->txStructConf); /* PRQA S 4391 */ /* MD_Can_IntegerCast */ /* SBSW_CAN_HL46 */
  }
  else
#  endif  /* pure Extended ID */
  { /* Extended ID */
    PduInfo->id = (Can_IdType)(CanTxActualExtId(txConfPara->txStructConf) | CAN_ID_IDE_MASK); /* SBSW_CAN_HL46 */
  }
# else    /* Standard ID */
  PduInfo->id = CanTxActualStdId(txConfPara->txStructConf); /* SBSW_CAN_HL46 */
# endif
# if defined(C_ENABLE_CAN_FD_USED)
  if ( CanTxActualFdType(txConfPara->txStructConf) == kCanFdTypeFd )
  {
    PduInfo->id |= (Can_IdType)CAN_ID_FD_MASK; /* SBSW_CAN_HL46 */
  }
# endif
  PduInfo->id &= CAN_ID_MASK_IN_GENERIC_CALLOUT; /* SBSW_CAN_HL46 */
}
#endif

#if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_BasicCanMsgReceivedPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_BasicCanMsgReceivedPolling
Relation_Context:
# from CanLL_RxBasicProcessPendings #
RxBasicCANSupport
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_BasicCanMsgReceivedPolling( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. */
  Can_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_BasicCanMsgReceived() for handling */
  CanHL_BasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxMailboxHandle, hwObjHandle );
  Can_EnableControllerInterrupts((uint8)channel);
}
#endif

/****************************************************************************
| NAME:             CanHL_BasicCanMsgReceived
****************************************************************************/
/*
|<DataModelStart>| CanHL_BasicCanMsgReceived
Relation_Context:
# from CanHL_BasicCanMsgReceivedPolling #
RxBasicCANSupport
# from Rx Interrupt #
Relation:
DevErrorDetect, RxPolling, OneChOpt
RamCheck
Overrun
IDType
CanFdSupport
DevErrorDetect, RxPolling, IndividualPolling
ChannelAmount
RxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_BasicCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanRxBasicParaStruct rxBasicPara;
  CanHookBegin_CanHL_BasicCanMsgReceived();
  errorId = CAN_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  rxBasicPara.mailboxHandle = rxMailboxHandle;
  rxBasicPara.hwObjHandle = hwObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON) && !defined(C_ENABLE_RX_BASICCAN_POLLING)
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif 
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanLL_RxBasicMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanLL_RxBasicMsgReceivedBegin(CAN_CHANNEL_CANPARA_FIRST &rxBasicPara) == kCanOk) /* SBSW_CAN_HL26 */ /* COV_CAN_RxBasicMsgReceivedBegin */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
# if defined(C_ENABLE_CAN_RAM_CHECK)
      /* -------------------------- RAM check failed --- */
      if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
# if defined(C_ENABLE_OVERRUN)
        /* -------------------------- Overrun --- */
        if (rxBasicPara.isOverrun == kCanTrue)
        { /* #40 Overrun occur so notify upper layer \trace SPEC-1686 */
#   if (CAN_OVERRUN_NOTIFICATION == CAN_DET)
          Can_CallDetReportError(CAN_RXINDI_ID, CAN_E_DATALOST); /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_OVERRUN_NOTIFICATION == CAN_APPL */
          Appl_CanOverrun( (uint8)channel );
#   endif
        }
# endif
        /* \trace SPEC-1346 */
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxBasicPara.rxStruct.localDlc = CanRxActualDLC((&(rxBasicPara.rxStruct)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
          if ( CanRxActualIdType((&(rxBasicPara.rxStruct))) == kCanIdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID */
            rxBasicPara.rxStruct.localId = (Can_IdType)CanRxActualStdId((&(rxBasicPara.rxStruct)));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxBasicPara.rxStruct.localId = (Can_IdType)(CanRxActualExtId((&(rxBasicPara.rxStruct))) | CAN_ID_IDE_MASK);
          }
          {
# else 
          { /* pure - standard ID */
            rxBasicPara.rxStruct.localId = CanRxActualStdId((&(rxBasicPara.rxStruct)));
# endif
# if defined(C_ENABLE_CAN_FD_USED)
            if ( CanRxActualFdType((&(rxBasicPara.rxStruct))) == kCanFdTypeFd )
            { /* \trace SPEC-60428 */
              rxBasicPara.rxStruct.localId |= (Can_IdType)CAN_ID_FD_MASK;
            }
            /* DLC is not bigger than expected length (8) for CLASSIC CAN msg (additional to HashTag 145) */
            if ((CanRxActualFdType((&(rxBasicPara.rxStruct))) != kCanFdTypeFd) && (rxBasicPara.rxStruct.localDlc > 8u)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_E_PARAM_DLC;
            }
            else
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_ENABLE_RX_BASICCAN_POLLING) || defined(C_ENABLE_INDIVIDUAL_POLLING))
            /* #140 Check HRH is BasicCAN (already checked in polling task - do it only for interrupt) */
            if ( (rxBasicPara.mailboxHandle < Can_GetRxBasicHandleStart(canHwChannel)) ||
                 (rxBasicPara.mailboxHandle >= Can_GetRxBasicHandleStop(canHwChannel)) ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
            {
              errorId = CAN_E_PARAM_HANDLE;
            } 
            else
#  endif
# endif
            /* #145 DLC is not bigger than expected length from mailbox (MAX over all PDU for this mailbox) */
            if (rxBasicPara.rxStruct.localDlc > CAN_MAX_DATALEN_OBJ(rxBasicPara.mailboxHandle)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_E_PARAM_DLC;
            }
            else
            {
              /* -------------------------- Notification --- */
              /* #150 RX queue and notification handling CanHL_RxMsgReceivedNotification() */
              rxBasicPara.rxStruct.localMailboxHandle = rxBasicPara.mailboxHandle;
              errorId = CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANPARA_FIRST &(rxBasicPara.rxStruct)); /* SBSW_CAN_HL26 */
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_RxBasicMsgReceivedEnd() */
    CanLL_RxBasicReleaseObj(CAN_CHANNEL_CANPARA_FIRST &rxBasicPara); /* SBSW_CAN_HL26 */
    CanLL_RxBasicMsgReceivedEnd(CAN_CHANNEL_CANPARA_FIRST &rxBasicPara); /* SBSW_CAN_HL26 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_RXINDI_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CanHookEnd_CanHL_BasicCanMsgReceived();
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* C_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_FullCanMsgReceivedPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_FullCanMsgReceivedPolling
Relation_Context:
# from CanLL_RxFullProcessPendings #
RxFullCANSupport, RxPolling
Relation:
OneChOpt
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_FullCanMsgReceivedPolling( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. */
  Can_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_FullCanMsgReceived() for handling */
  CanHL_FullCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxMailboxHandle, hwObjHandle );
  Can_EnableControllerInterrupts((uint8)channel);
}
# endif

/****************************************************************************
| NAME:             CanHL_FullCanMsgReceived
****************************************************************************/
/*
|<DataModelStart>| CanHL_FullCanMsgReceived
Relation_Context:
# from CanLL_FullCanMsgReceivedPolling #
RxFullCANSupport
# from Rx Interrupt #
Relation:
OneChOpt
DevErrorDetect, RxPolling, OneChOpt
RamCheck
Overrun
IDType
CanFdSupport
DevErrorDetect, RxPolling, IndividualPolling
ChannelAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_FullCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxMailboxHandle, CanObjectHandle hwObjHandle ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCanRxFullParaStruct rxFullPara;
  CanHookBegin_CanHL_FullCanMsgReceived();
  errorId = CAN_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  rxFullPara.mailboxHandle = rxMailboxHandle;
  rxFullPara.hwObjHandle = hwObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if !defined(C_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
#   if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#   endif
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanLL_RxFullMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanLL_RxFullMsgReceivedBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara) == kCanOk) /* SBSW_CAN_HL27 */ /* COV_CAN_RxFullMsgReceivedBegin */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
  /* -------------------------- RAM check failed --- */
# if defined(C_ENABLE_CAN_RAM_CHECK)
      if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
        /* -------------------------- Overrun --- */
# if defined(C_ENABLE_FULLCAN_OVERRUN)
        if (rxFullPara.isOverrun == kCanTrue)
        {
          /* #40 Overrun occur so notify upper layer */
#   if (CAN_OVERRUN_NOTIFICATION == CAN_DET)
          Can_CallDetReportError(CAN_RXINDI_ID, CAN_E_DATALOST); /* \trace SPEC-1686 */ /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_OVERRUN_NOTIFICATION == CAN_APPL */
          Appl_CanFullCanOverrun( (uint8)channel );
#   endif
        }
# endif
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxFullPara.rxStruct.localDlc = CanRxActualDLC((&(rxFullPara.rxStruct)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_ENABLE_EXTENDED_ID)
#  if defined(C_ENABLE_MIXED_ID)
          if ( CanRxActualIdType((&(rxFullPara.rxStruct))) == kCanIdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID */
            rxFullPara.rxStruct.localId = (Can_IdType)CanRxActualStdId((&(rxFullPara.rxStruct)));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxFullPara.rxStruct.localId = (Can_IdType)(CanRxActualExtId((&(rxFullPara.rxStruct))) | CAN_ID_IDE_MASK);
          }
# else 
          rxFullPara.rxStruct.localId = CanRxActualStdId((&(rxFullPara.rxStruct)));
# endif
          { /* ID has valid IdType */
# if defined(C_ENABLE_CAN_FD_USED)
            if ( CanRxActualFdType((&(rxFullPara.rxStruct))) == kCanFdTypeFd )
            { /* \trace SPEC-60428 */
              rxFullPara.rxStruct.localId |= (Can_IdType)CAN_ID_FD_MASK;
            }
            /* DLC is not bigger than expected length (8) for CLASSIC CAN msg (additional to HashTag 145) */
            if ((CanRxActualFdType((&(rxFullPara.rxStruct))) != kCanFdTypeFd) && (rxFullPara.rxStruct.localDlc > 8u)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_E_PARAM_DLC;
            }
            else
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_ENABLE_RX_FULLCAN_POLLING) || defined(C_ENABLE_INDIVIDUAL_POLLING)) /* COV_CAN_RX_FULLCAN_POLLING */
            /* #140 Check HRH is FullCAN (already checked in polling task - do it only for interrupt) */
            if ( (rxFullPara.mailboxHandle < Can_GetRxFullHandleStart(canHwChannel)) ||
                 (rxFullPara.mailboxHandle >= Can_GetRxFullHandleStop(canHwChannel)) )  /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
            {
              errorId = CAN_E_PARAM_HANDLE;
            }
            else
#  endif
# endif
            /* #145 DLC is not bigger than expected length from mailbox */
            if (rxFullPara.rxStruct.localDlc > CAN_MAX_DATALEN_OBJ(rxFullPara.mailboxHandle)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_E_PARAM_DLC;
            }
            else
            {
              /* -------------------------- Notification --- */
              /* #150 RX queue and notification handling CanHL_RxMsgReceivedNotification() */
              rxFullPara.rxStruct.localMailboxHandle = rxFullPara.mailboxHandle;
              errorId = CanHL_RxMsgReceivedNotification(CAN_CHANNEL_CANPARA_FIRST &(rxFullPara.rxStruct)); /* SBSW_CAN_HL27 */
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_RxFullMsgReceivedEnd() */
    CanLL_RxFullReleaseObj(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara); /* SBSW_CAN_HL27 */
    CanLL_RxFullMsgReceivedEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara); /* SBSW_CAN_HL27 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_RXINDI_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CanHookEnd_CanHL_FullCanMsgReceived();
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* C_ENABLE_RX_FULLCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_ErrorHandling
****************************************************************************/
/*
|<DataModelStart>| CanHL_ErrorHandling
Relation_Context:
# from Can_MainFunction_BusOff() #
StatusPolling
# from BusOff Interrupt #
StatusPolling
Relation:
DevErrorDetect, StatusPolling, OneChOpt
Overrun
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* explicite not inline (big code + multiple calls) */
V_DEF_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_ENABLE_ERROR_POLLING) /* COV_CAN_ERROR_POLLING */
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Pre Error handling (LL) */
    CanLL_ErrorHandlingBegin(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#if defined (CAN_ENABLE_SECURITY_EVENT_REPORTING)
    /* #30 Check if Transition to error state passive occured (LL) */
    if (CanLL_ErrorStatePassiveTransitionOccured(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCanTrue)
    {
      uint16 rxErrorCount; /* ESCAN00110429 */
      uint16 txErrorCount;
      /* #35 Report the Rx/Tx error counters */
# if defined( C_SINGLE_RECEIVE_CHANNEL )
      rxErrorCount = CanRxActualErrorCounter();
      txErrorCount = CanTxActualErrorCounter();
# else
      rxErrorCount = CanRxActualErrorCounter(channel);
      txErrorCount = CanTxActualErrorCounter(channel);
# endif
      CanHL_ControllerErrorStatePassive(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST rxErrorCount, txErrorCount);
    }
    /* #40 Error Notification Handling (LL) */
    CanLL_ErrorNotificationHandling(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#endif
    if ( (CanLL_BusOffOccured(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCanTrue) && (!Can_IsIsBusOff(channel) /* avoid repeated call */)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
    {
      /* #90 EXCLUSIVE_AREA_6 secure mode changes */
      SchM_Enter_Can_CAN_EXCLUSIVE_AREA_6();
      /* #100 BUSOFF occur -> ECU Manager restart Controller (no ResetBusOffStart/End needed) */
      Can_SetIsBusOff(channel, TRUE); /* SBSW_CAN_HL08 */
      /* #110 ResetBusOffStart need when BUSOFF handled by Application */
      (void)CanHL_ModeTransition(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST kCanModeResetBusOffStart, kCanContinueBusOffRecovery, kCanSuppressRamCheck);
      /* #115 Controller is in STOP mode after ResetBusOffStart \trace SPEC-1578, SPEC-1664, SPEC-1663 */
      /* #120 Status changed to STOP - no more call of STOP from CAN Interface */
      Can_SetLogStatus(channel, (uint8)((Can_GetLogStatus(channel) & CAN_STATUS_MASK_NOMODE) | CAN_STATUS_STOP)); /* SBSW_CAN_HL02 */
      SchM_Exit_Can_CAN_EXCLUSIVE_AREA_6();
      /* #130 notify higher layer by call CanIf_ControllerBusOff() \trace SPEC-1726, SPEC-1578 */
      CanIf_ControllerBusOff((uint8)Can_GetCanToCanIfChannelMapping(channel));
    }
    /* #200 Post Error handling (LL) */
    CanLL_ErrorHandlingEnd(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CTRBUSOFF_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if defined (CAN_ENABLE_SECURITY_EVENT_REPORTING)
/****************************************************************************
| NAME:             CanHL_ControllerErrorStatePassive
****************************************************************************/
/*
|<DataModelStart>| CanHL_ControllerErrorStatePassive
Relation_Context:
# from CanHL_ErrorHandling() #
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_ControllerErrorStatePassive( CAN_CHANNEL_CANTYPE_FIRST uint16 RxErrorCounter, uint16 TxErrorCounter )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Notify passive state to the CanIf and pass on the error counters */
  CanIf_ControllerErrorStatePassive(channel, RxErrorCounter, TxErrorCounter);
}

/****************************************************************************
| NAME:             CanHL_ErrorNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_ErrorNotification
Relation_Context:
# from CanLL_ErrorNotificationHandling() #
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
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
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_ErrorNotification( CAN_CHANNEL_CANTYPE_FIRST Can_ErrorType CanError ) /* PRQA S 3219 */ /* MD_Can_HL_UnusedFunction */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Notify the error to CanIf */
  CanIf_ErrorNotification(channel, CanError);
}
#endif /* CAN_ENABLE_SECURITY_EVENT_REPORTING */

# if defined(C_ENABLE_SLEEP_WAKEUP)

/****************************************************************************
| NAME:             CanHL_WakeUpHandling
****************************************************************************/
/*
|<DataModelStart>| CanHL_WakeUpHandling
Relation_Context:
# from CanLL_WakeUpHandling #
Wakeup
Relation:
DevErrorDetect, WakeupPolling, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_WakeUpHandling( CAN_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
#    if !defined( C_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit, only for interrupt systems where the channel parameter may fail */
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_HARDWARE_FAILURE */
  {
#     if (CAN_DEV_ERROR_REPORT == STD_ON)
    /* ----- Development Error Report --------------------------------------- */
    Can_CallDetReportError(CAN_CTRWAKEUP_ID, CAN_E_PARAM_CONTROLLER);
#     endif
  }
  else
#    endif
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 WAKEUP already done in "CanLL_WakeUpHandling" via macro "CanWakeUp" -> (void)Can_SetControllerMode(controller, CAN_T_WAKEUP); */
    /* #30 ESCAN00032401: don't change to STOP mode here this will be done later from inside CANIf after Transceiver is switched to normal mode to avoid blocking while Transceiver produce dominant disturbance */
    Can_SetIsWakeup(channel, TRUE); /* SBSW_CAN_HL09 */
    /* #40 call EcuM_CheckWakeup()  - old API: CanIf_SetWakeupEvent();  \trace SPEC-1725, SPEC-1659, SPEC-1697 */
    EcuM_CheckWakeup(Can_GetWakeupSourceRef(channel));
  }
}
# endif

#define CAN_STOP_SEC_STATIC_CODE  /*---------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*-------------------------------------------------------------------------*/

#define CAN_START_SEC_CODE  /*---------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CAN_VERSION_INFO_API == STD_ON) /* \trace SPEC-1716 */
/****************************************************************************
| NAME:             Can_GetVersionInfo
****************************************************************************/
/*
|<DataModelStart>| Can_GetVersionInfo
Relation_Context:
VersionInfoApi
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_GetVersionInfo( Can_VersionInfoPtrType VersionInfo )
{ /* \trace SPEC-1723 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter versionInfo is an illegal null pointer \trace SPEC-1721 */
  if (VersionInfo == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  { /* #100 copy version info to given pointer parameter \trace SPEC-1717 */
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID   = CAN_VENDOR_ID; /* SBSW_CAN_HL10 */
    VersionInfo->moduleID   = CAN_MODULE_ID; /* SBSW_CAN_HL10 */
    VersionInfo->sw_major_version = (uint8)CAN_SW_MAJOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_minor_version = (uint8)CAN_SW_MINOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_patch_version = (uint8)CAN_SW_PATCH_VERSION; /* SBSW_CAN_HL10 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_VERSION_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif

/****************************************************************************
| NAME:             Can_InitMemory
****************************************************************************/
/*
|<DataModelStart>| Can_InitMemory
Relation_Context:
Relation:
Variant
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_InitMemory( void )
{ /* BR:011 */
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 mark driver as uninitialized \trace SPEC-1650 */
  canConfigInitFlag = CAN_STATUS_UNINIT;
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER)
  /* #20 reset global configuration pointer */
  Can_ConfigDataPtr = NULL_PTR;
#endif /* otherwise constant pointer is used */
  /* #30 HW reset memory */
  CanLL_InitMemoryPowerOn(); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#if(CAN_DEV_ERROR_DETECT == STD_OFF)
  CAN_DUMMY_STATEMENT(canConfigInitFlag); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}

#if ((CAN_CHANGE_BAUDRATE_API == STD_ON) || (CAN_SET_BAUDRATE_API == STD_OFF))
/****************************************************************************
| NAME:             Can_ChangeBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_ChangeBaudrate
Relation_Context:
Relation:
ChangeBaudrate, DevErrorDetect, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_ChangeBaudrate( uint8 Controller, V_DEF_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20314 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  retval = E_NOT_OK;
  errorId = CAN_E_PARAM_BAUDRATE; /* explicit default as Issue - remove when baud rate fit */ /* \trace SPEC-20321 */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-20338 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-20331 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( (Can_GetLogStatus(channel) & CAN_STATUS_STOP) != CAN_STATUS_STOP )
  { /* \trace SPEC-1655, SPEC-20312 */
    errorId = CAN_E_TRANSITION;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_GetInitObjectStartIndex(channel); baudrateIndex < Can_GetInitObjectStartIndex(channel + 1u); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 set baud rate and reinitialize controller to activate baud rate \trace SPEC-1669 */
        uint8 transitionState;
        Can_SetLastInitObject(channel, (uint8)baudrateIndex); /* SBSW_CAN_HL11 */
        transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST kCanModeStopReinit, kCanFinishBusOffRecovery, kCanSuppressRamCheck);
        if (transitionState == kCanOk) /* COV_CAN_TRANSITION_REQUEST */
        { /* #50 check transition STOP (Reinit) is successful */
          Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
        }
        retval = E_OK;
        errorId = CAN_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CHANGE_BR_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (CAN_CHANGE_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_CheckBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_CheckBaudrate
Relation_Context:
ChangeBaudrate
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_CheckBaudrate( uint8 Controller, V_DEF_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20311 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  retval = E_NOT_OK;
  errorId = CAN_E_PARAM_BAUDRATE; /* \trace SPEC-20317 */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT \trace SPEC-20328, SPEC-20318 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels )
  { /* \trace SPEC-20335 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_GetInitObjectStartIndex(channel); baudrateIndex < Can_GetInitObjectStartIndex(channel + 1u); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 requested baud rate is set - return OK */
        retval = E_OK;
        errorId = CAN_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_CHECK_BR_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* CAN_CHANGE_BAUDRATE_API == STD_ON */

#if (CAN_SET_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_SetBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_SetBaudrate
Relation_Context:
Relation:
ChangeBaudrate, DevErrorDetect, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )
{ /* \trace SPEC-50605 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  /* #5 return E_NOT_OK in case no fitting baudrate is found */
  retval = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-50595 */
  if ( canConfigInitFlag == CAN_STATUS_UNINIT )
  {
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCanNumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-50587 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( (Can_GetLogStatus(channel) & CAN_STATUS_STOP) != CAN_STATUS_STOP )
  { /* \trace SPEC-50584 */
    errorId = CAN_E_TRANSITION;
  }
  else
  if ( BaudRateConfigID >= (uint16)(Can_GetInitObjectStartIndex(channel + 1u) - Can_GetInitObjectStartIndex(channel)) ) /* PRQA S 4391 */ /* MD_Can_IntegerCast */
  { /* \trace SPEC-50625, SPEC-50563 */
    errorId = CAN_E_PARAM_BAUDRATE;
  }
  else
# endif
  { /* #50 set baud rate and reinitialize controller to activate baud rate */
    uint8 transitionState;
    /* ----- Implementation ----------------------------------------------- */
    Can_SetLastInitObject(channel, (uint8)(Can_GetInitObjectStartIndex(channel) + BaudRateConfigID)); /* SBSW_CAN_HL11 */
    transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST kCanModeStopReinit, kCanFinishBusOffRecovery, kCanSuppressRamCheck);
    /* #60 check if hardware cancellation appear while mode change, so for the baud rate set is not successful \trace SPEC-60434 */
    if (transitionState == kCanOk) /* COV_CAN_TRANSITION_REQUEST */
    {
      retval = E_OK;
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_SET_BR_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* CAN_SET_BAUDRATE_API == STD_ON */

/****************************************************************************
| NAME:             Can_Init
****************************************************************************/
/*
|<DataModelStart>| Can_Init
Relation_Context:
Relation:
Variant, DevErrorDetect
RxQueue
OneChOpt
Variant
Wakeup
MirrorMode
SilentMode
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_Init( Can_ConfigPtrType ConfigPtr ) /* PRQA S 3206 */ /* MD_MSR_DummyStmt */
{ /* \trace SPEC-1587, SPEC-1708 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CAN_CHANNEL_CANTYPE_LOCAL
  CanHookBegin_Can_Init();
  errorId = CAN_E_NO_ERROR;
#if defined(CAN_ENABLE_USE_INIT_ROOT_POINTER) /* PostBuild (load+sel) */
  Can_ConfigDataPtr = ConfigPtr; /* \trace SPEC-1394, SPEC-1575 */
# if defined(CAN_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* PostBuildLoadable */
  if (ConfigPtr == NULL_PTR)
  { /* #10 EcuM null pointer configuration check and notification */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
  }
  else
#  if (CAN_FINALMAGICNUMBER == STD_ON) /* COV_CAN_FINALMAGICNUMBER */
  if (Can_GetFinalMagicNumber() != 20510u) /* COV_CAN_GENDATA_FAILURE */
  { /* #20 EcuM wrong magic number check and notification */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
#  endif
# else
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #30 Check if all parameters are valid \trace SPEC-1724 */
  if (ConfigPtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
#  endif
# endif
#else
  CAN_DUMMY_STATEMENT(ConfigPtr);  /* PRQA S 1338, 2982, 2983, 3112 */ /* MD_MSR_DummyStmt */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
  ConfigPtr = Can_ConfigDataPtr;  /* PRQA S 2983,1338 */ /* MD_MSR_DummyStmt */
# endif
#endif /* CAN_ENABLE_USE_INIT_ROOT_POINTER */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag != CAN_STATUS_UNINIT)
  {
    /* #60 Check already initialized -> 2nd call is not allowed \trace SPEC-1712, SPEC-1722 */
    errorId = CAN_E_TRANSITION;
  }
  else
#endif
  if (  ( Can_GetBaseDll_GeneratorVersion()     != (uint16)CAN_GEN_BASE_CFG5_VERSION ) ||  /* PRQA S 2742 */ /* MD_Can_ConstValue */
        ( Can_GetPlatformDll_GeneratorVersion() != (uint16)CAN_GEN_CanoeemuCanoeAsr_VERSION ) ) /* COV_CAN_GENDATA_FAILURE */
  {
    /* #70 EcuM generator and compatibility version check and notification */
#if defined(CAN_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* not for PostBuildSelectable */
    EcuM_BswErrorHook((uint16) CAN_MODULE_ID, (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#else
    errorId = CAN_E_UNINIT; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#endif
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    {
      canIsRxTaskLocked = kCanFalse;
#if defined(C_ENABLE_RX_QUEUE)
      /* #110 delete RxQueue (all channels) */
      Can_GetRxQueueInfo().WriteIndex = 0u;  /* CM_CAN_HL24 */ /* SBSW_CAN_HL55 */
      Can_GetRxQueueInfo().ReadIndex = 0u; /* SBSW_CAN_HL55 */
      Can_GetRxQueueInfo().Count = 0u; /* SBSW_CAN_HL55 */
#endif
      /* #120 Set active Identity */
      /* #130 HW specific power on (LL) */
      if (CanLL_InitPowerOn() == kCanOk) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
      {
        canConfigInitFlag = CAN_STATUS_INIT; /* \trace SPEC-1648 */ /* before Controller initialization begin */
#if !defined( C_SINGLE_RECEIVE_CHANNEL )
        for (channel = 0u; channel < kCanNumberOfChannels; channel++) /* CM_CAN_HL01 */
#endif
        { /* #132 for each controller */
#if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
          if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_FAILURE */
          { /* #134 not for inactive controller */
            continue;
          }
#endif
          /* #140 initialize start up values for each controllers */
          Can_SetLogStatus(channel, CAN_STATUS_UNINIT); /* PowerOn mark */ /* SBSW_CAN_HL02 */
          Can_SetModeTransitionRequest(channel, kCanModeNone); /* SBSW_CAN_HL01 */
          Can_SetBusOffTransitionRequest(channel, kCanFinishBusOffRecovery); /* SBSW_CAN_HL01 */
          Can_SetRamCheckTransitionRequest(channel, kCanSuppressRamCheck); /* SBSW_CAN_HL01 */
          Can_SetIsBusOff(channel, FALSE);  /* start up: no BUSOFF */ /* SBSW_CAN_HL08 */
#if defined(C_ENABLE_SLEEP_WAKEUP)
          Can_SetIsWakeup(channel, FALSE); /* SBSW_CAN_HL09 */
#endif
          Can_SetCanInterruptCounter(channel, 0u); /* SBSW_CAN_HL12 */
#if defined(C_ENABLE_MIRROR_MODE)
          Can_SetMirrorModeState(channel, CDDMIRROR_INACTIVE); /* SBSW_CAN_HL20 */
#endif
#if defined(C_ENABLE_SILENT_MODE)
          Can_SetSilentModeState(channel, CAN_SILENT_INACTIVE); /* SBSW_CAN_HL20 */
#endif
          /* #155 HW channel specific power on */
          if (CanLL_InitPowerOnChannelSpecific(CAN_CHANNEL_CANPARA_ONLY) == kCanOk) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
          {
            uint8 transitionState;
            /* #160 set baud rate and initialize all CAN controllers \trace SPEC-1587, SPEC-1656 */
            Can_SetLastInitObject(channel, (uint8)(Can_GetInitObjectStartIndex(channel) + Can_GetCanControllerDefaultBaudrateIdx(CAN_HL_HW_CHANNEL_STARTINDEX(channel)))); /* SBSW_CAN_HL11 */
            transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST kCanModeStopReinit, kCanFinishBusOffRecovery, kCanExecuteRamCheck);
            if (transitionState == kCanOk) /* COV_CAN_TRANSITION_REQUEST */
            { /* #170 check transition STOP (Reinit) is successful */
              Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* STOP mode reached or issue */ /* SBSW_CAN_HL02 */
            }
#if (CAN_DEV_ERROR_DETECT == STD_ON)
            else
            {              
              errorId = CAN_E_TRANSITION;
            }
#endif
          }
        }
      } /* CanLL_InitPowerOn */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_INIT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_Init();
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_SetControllerMode
****************************************************************************/
/*
|<DataModelStart>| Can_SetControllerMode
Relation_Context:
Relation:
DevErrorDetect, OneChOpt, ChannelAmount
DevErrorDetect, Variant, ChannelAmount
DevErrorDetect, TransitionCheck
RamCheck, ChannelAmount
SilentMode, ChannelAmount
ReInitStart
Wakeup
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Can_ReturnType, CODE) Can_SetControllerMode( uint8 Controller, Can_StateTransitionType Transition )
{ /* \trace SPEC-1715 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_ReturnType retval;
  uint8 transitionRequest;
  uint8 busOffRecovery;
  uint8 doRamCheck;
  uint8 transitionState;
  CanHookBegin_Can_SetControllerMode();
  retval = CAN_NOT_OK;  /* \trace SPEC-1407 */ /* For successful transition it is set explicit to CAN_OK */
  transitionState = kCanFailed;
  errorId = CAN_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Transition is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1731 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1732 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { 
    errorId = CAN_E_PARAM_CONTROLLER; /* \trace SPEC3874 */
  }
  else
# endif
#endif
  { /* #50 second level check (modes and transitions) only valid when controller is in active ECU and valid */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
    if (!CanHL_IsControllerInit(channel))
    { /* #60 Check controller is initialized */
      errorId = CAN_E_UNINIT;
    }
    else
    if ((!CanHL_IsStop(channel)) && (Transition == CAN_T_START))
    { /* #70 Check No-STOP -> START-Req is invalid \trace SPEC-1637 */ /* ESCAN00073272 */
      errorId = CAN_E_TRANSITION;
    }
    else
# if (CAN_TRANSITION_CHECK == STD_ON)  /* SREQ00000291 */
    if ( ((!CanHL_IsStop(channel)) && (!CanHL_IsSleep(channel)) && ((Transition == CAN_T_WAKEUP) || (Transition == CAN_T_SLEEP))) ||
         ((CanHL_IsSleep(channel)) && (Transition == CAN_T_STOP)) )
    { /* #80 Check  No STOP Nor SLEEP -> SLEEP-Req   or   No SLEEP Nor STOP --> WAKEUP-Req   is invalid \trace SPEC-1654, SPEC-1649 */
      /* #90 Check  No START Nor STOP -> STOP-Req is invalid \trace SPEC-1652 */
      errorId = CAN_E_TRANSITION;
    }
    else
# endif
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
    /* #95 controller is active (no RAM check issue): */
    if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */ /* COV_CAN_RAM_CHECK_NO_STIMULATION */
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
      SchM_Enter_Can_CAN_EXCLUSIVE_AREA_6();
      /* #125 HW begin mode transition */
      switch(Transition)
      {
      case CAN_T_START:
        /* #200 Handle --- START --- transition */
        if (Can_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
        { /* #210 BUSOFF occur so finish BUSOFF handling and do START transition inside */
          transitionRequest = kCanModeResetBusOffEnd;
        } 
        else
        {
#if defined(C_ENABLE_SILENT_MODE)
          if (Can_GetSilentModeState(channel) == CAN_SILENT_ACTIVE)
          {
            transitionRequest = kCanModeSilent;
          }
          else
#endif
          {
#if (CAN_REINIT_START == STD_ON)
            transitionRequest = kCanModeStartReinit;
#else
            transitionRequest = kCanModeStart;
#endif
          }
        }
        busOffRecovery = kCanContinueBusOffRecovery;
        doRamCheck = kCanSuppressRamCheck;
        transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        if ( transitionState == kCanOk ) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
        {
          Can_SetIsBusOff(channel, FALSE); /* SBSW_CAN_HL08 */
        }
        CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_START, CANIF_CS_STARTED, transitionState);
        break;

      case CAN_T_STOP:
        /* #300 Handle --- STOP --- transition */
        transitionRequest = kCanModeStopReinitFast;
        busOffRecovery = kCanContinueBusOffRecovery;
        doRamCheck = kCanSuppressRamCheck;
        transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
        break;

      case CAN_T_WAKEUP:
        /* #400 Handle --- WAKEUP --- transition */
#if defined(C_ENABLE_SLEEP_WAKEUP)
        if ( (Can_GetModeTransitionRequest(channel) & kCanModeSleep) == kCanModeSleep )
        { /* #410 ASR4: Check WAKEUP not allowed while transition to SLEEP \trace SPEC-1573, SPEC-1402, SPEC-1403, SPEC-1660 */
          errorId = CAN_E_TRANSITION;
          break;
        }
#endif
#if defined(C_ENABLE_CAN_RAM_CHECK) && !defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
        if (!CanHL_IsStop(channel)) { /* do not call RAM_CHECK again in case CanIf call wakeup a second time (buswakeup + stop transition afterwards) */
          /* #415 do standard RAM check CanHL_DoRamCheck() if controller is not in STOP mode */
          transitionRequest = kCanModeStopReinit;
          doRamCheck = kCanExecuteRamCheck;
        }
        else
#endif
        {
          transitionRequest = kCanModeStopReinitFast;
          doRamCheck = kCanSuppressRamCheck;
        }
#if defined(C_ENABLE_SLEEP_WAKEUP)
        if (Can_GetWakeupSourceRef(channel) != 0u) /* COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG */
        { /* #420 hardware is only in SLEEP if WAKEUP over bus is active and supported and wakeup on this controller is activated -> otherwise emulated SLEEP */
          transitionRequest = kCanModeWakeupStopReinit;
          busOffRecovery = kCanFinishBusOffRecovery;
        }
        else
#endif
        {
          busOffRecovery = kCanFinishBusOffRecovery;
        }
        transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
        break;

      case CAN_T_SLEEP: /* \trace SPEC-1639 */
        /* #500 Handle --- SLEEP --- transition */
#if defined(C_ENABLE_SLEEP_WAKEUP)
        /* #510 Transition only when WAKEUP by bus is configured, otherwise use emulated SLEEP \trace SPEC-1633 */
        if (Can_GetWakeupSourceRef(channel) != 0u) /* COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG */
        {
          transitionRequest = kCanModeSleep;
          busOffRecovery = kCanFinishBusOffRecovery;
          doRamCheck = kCanSuppressRamCheck;
          transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        }
        else
#endif
        { /* #520 no WAKEUP-source-ref: do emulated SLEEP mode \trace SPEC-1629, SPEC-1641, SPEC-1645 */
          transitionState = kCanOk;
        }
        CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_SLEEP, CANIF_CS_SLEEP, transitionState);
        break;

      default:
        /* #600 check unknown transition, return NOT_OK \trace SPEC-1573, SPEC-1402, SPEC-1403 */
        errorId = CAN_E_TRANSITION; /* \trace SPEC-1733, SPEC-1407 */
        break;
      } /* switch */
      SchM_Exit_Can_CAN_EXCLUSIVE_AREA_6();
    }
  }
  if ( transitionState != kCanFailed ) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */
  {
    retval = CAN_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_SETCTR_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_SetControllerMode();
  return retval;
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_DisableControllerInterrupts
****************************************************************************/
/*
|<DataModelStart>| Can_DisableControllerInterrupts
Relation_Context:
# from Can_MainFunction_BusOff #
StatusPolling, ChannelAmount
# from CanHL_TxTaskCancelationHandling #
HwCancelTx, IndividualPolling
# from CanHL_TxConfirmationPolling #
TxPolling
# from Rx BasicCan FullCan #
RxPolling
RxBasicCANSupport
RxFullCANSupport
# from Error handling #
StatusPolling
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
IntLock
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_DisableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1746 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1750 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1742 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1750 */
    errorId = CAN_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    SchM_Enter_Can_CAN_EXCLUSIVE_AREA_1();
    if (Can_GetCanInterruptCounter(channel) == 0u)
    { /* #110 Disable only when not disabled before (count nesting) \trace SPEC-1735, SPEC-1745 */
#if (CAN_INTLOCK == CAN_DRIVER) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      { /* #120 Disable CAN interrupt for each HW channel */
        CanLL_CanInterruptDisable(CAN_CHANNEL_CANPARA_FIRST  &Can_GetCanInterruptOldStatus(canHwChannel)); /* SBSW_CAN_HL16 */
      }
#endif
#if (CAN_INTLOCK == CAN_APPL) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      /* #130 Let application Disable CAN interrupt ApplCanInterruptDisable() */
      ApplCanInterruptDisable((uint8)channel);
#endif
    }
    Can_IncCanInterruptCounter(channel); /* \trace SPEC-1748 */ /* SBSW_CAN_HL12 */
    SchM_Exit_Can_CAN_EXCLUSIVE_AREA_1();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_DIINT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_EnableControllerInterrupts
****************************************************************************/
/*
|<DataModelStart>| Can_EnableControllerInterrupts
Relation_Context:
# from Can_MainFunction_BusOff #
StatusPolling, ChannelAmount
# from CanHL_TxTaskCancelationHandling #
HwCancelTx, IndividualPolling
# from CanHL_TxConfirmationPolling #
TxPolling
# from Rx BasicCan FullCan #
RxPolling
RxBasicCANSupport
RxFullCANSupport
# from Error handling #
StatusPolling
Relation:
DevErrorDetect, OneChOpt 
DevErrorDetect, Variant
IntLock
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_EnableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1741 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1739 */
    errorId = CAN_E_UNINIT;
  }
  else
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1752 */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1739 */
    errorId = CAN_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    SchM_Enter_Can_CAN_EXCLUSIVE_AREA_1();
    if (Can_GetCanInterruptCounter(channel) != 0u)
    { /* #110 Enable only when disabled before (count nesting) \trace SPEC-1749, SPEC-1748 */
      Can_DecCanInterruptCounter(channel); /* SBSW_CAN_HL12 */
      if (Can_GetCanInterruptCounter(channel) == 0u) /* \trace SPEC-1736, SPEC-1756 */
      {
#if (CAN_INTLOCK == CAN_DRIVER) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        { /* #120 Disable CAN interrupt for each HW channel */
          CanLL_CanInterruptRestore(CAN_CHANNEL_CANPARA_FIRST Can_GetCanInterruptOldStatus(canHwChannel));
        }
#endif
#if (CAN_INTLOCK == CAN_APPL) || (CAN_INTLOCK == CAN_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        /* #130 Let application Enable CAN interrupt ApplCanInterruptRestore() */
        ApplCanInterruptRestore((uint8)channel);
#endif
      }
    }
    SchM_Exit_Can_CAN_EXCLUSIVE_AREA_1();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_ENINT_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_Write
****************************************************************************/
/*
|<DataModelStart>| Can_Write
Relation_Context:
Relation:
DevErrorDetect
OneChOpt
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
DevErrorDetect, TxFullCANSupport
DevErrorDetect, CanFdSupport
DevErrorDetect, IDType
RamCheck
UseVectorCanIf
CanFdSupport
ChannelAmount
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Can_ReturnType, CODE) Can_Write( Can_HwHandleType Hth, Can_PduInfoPtrType PduInfo )
{ /* \trace SPEC-1751, SPEC-1760, SPEC-1406 */
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  CanHookBegin_Can_Write();
  errorId = CAN_E_NO_ERROR;
  retval = CAN_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* \trace SPEC-1759 */
    errorId = CAN_E_UNINIT;
  }
  else
  if (Hth >= Can_GetSizeOfMailbox()) {
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
#endif
  {
#if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Hth);
#endif
#if (CAN_DEV_ERROR_DETECT == STD_ON)
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
# endif
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
# endif
    if (!CanHL_IsControllerInit(channel))
    { /* #40 Check controller is initialized \trace SPEC-1407 */
      errorId = CAN_E_UNINIT;
    }
    else
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
    if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_FIFO_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
# else
    if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_FIFO_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
# endif
    { /* \trace SPEC-1763 */
      errorId = CAN_E_PARAM_HANDLE;
    }
    else
    if ((PduInfo == NULL_PTR) || ((PduInfo->sdu == NULL_PTR) && (PduInfo->length != 0u)))
    { /* \trace SPEC-1761 */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# if !defined(C_ENABLE_CAN_FD_USED)
    if ( CanHL_IsFdMessage(PduInfo->id) )
    { /* no CAN_FD: Check parameter PduInfo->id should not have FD bit in ID */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# endif
# if defined(C_ENABLE_CAN_FD_FULL)
    if ( ((PduInfo->length > 8u) && (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(channel)) == CAN_NONE)) ||
         (PduInfo->length > CAN_MAX_DATALEN_OBJ(Hth)) ) /* COV_CAN_GENDATA_FAILURE */
         { /* \trace SPEC-1758 */
      /* \trace SPEC-60433 */ /* CAN_FD: Check parameter PduInfo->length is smaller than 9 for none FD configuration
         If development error detection for the CAN module is enabled and the CAN Controller is not in CAN FD mode (no CanControllerFdBaudrateConfig):
         The function Can_Write shall raise the error CAN_E_PARAM_DLC and shall return CAN_NOT_OK if the length is more than 8 byte.() */
      /* CAN_FD: Check parameter PduInfo->length against PDU maximum size */
      errorId = CAN_E_PARAM_DLC;
    }
    else
# else
    if ( PduInfo->length > CAN_MAX_DATALEN_OBJ(Hth) ) /* CM_CAN_HL20 */
    { /* \trace SPEC-1758 */ /* Check parameter PduInfo->length against maximum buffer size */
      errorId = CAN_E_PARAM_DLC;
    }
    else
# endif
# if defined(C_ENABLE_MIXED_ID)
    if ( ((PduInfo->id & (Can_IdType)CAN_ID_IDE_MASK) != (Can_IdType)CAN_ID_IDE_MASK ) && ( (PduInfo->id & (Can_IdType)CAN_ID_MASK) > (Can_IdType)CAN_ID_MASK_STD ) ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* Check parameter PduInfo->id for STD ID is in STD range (EXT is always in range) */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
# endif
    if ( (PduInfo->id & (Can_IdType)CAN_ID_UNUSED_MASK) != (Can_IdType)0UL )
    { /* Check parameter PduInfo->id contains illegal bits (e.g. bigger ID value than MAX for given type) */
      errorId = CAN_E_PARAM_POINTER;
    }
    else
#endif
    /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #150 reject deactivated mailbox (by RamCheck) */
    if (Can_GetMailboxState(Hth) == kCanFailed) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      retval = CAN_NOT_OK;
    }
    else
#endif
    /* #180 check Hardware is in BUSOFF recovery state and is not able to handle a request: return BUSY \trace SPEC-1764 */
    if (Can_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
    {
      retval = CAN_NOT_OK; /* ESCAN00096369 */
    }
    else
#if defined(C_ENABLE_CAN_RAM_CHECK)
    /* #200 controller is active (no RAM check issue): */
    if ((Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) != CAN_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */ /* COV_CAN_HARDWARE_FAILURE */
#endif
    {
      /* #205 temporary pdu buffer to avoid data modification on constant data (modification in generic pretransmit) */
      tCanTxTransmissionParaStruct txPara;
      /* #210 EXCLUSIVE_AREA_2: Interrupts may not be locked and re-entrant call may occur. */
      /*                        Time in between ID search and lock of mailbox is critical 
                                (only when no Vector Interface used otherwise CanIf EXCLUSIVE_AREA is used instead) 
                                Can_Write() could be called re-entrant, there for also for polling systems the interrupts have to be locked  
                                this is essential while check for mailbox is free (in between if (free) and set to not free) and save PDU handle */
#if !defined(CAN_USE_VECTOR_IF)
      SchM_Enter_Can_CAN_EXCLUSIVE_AREA_2();
#endif
      txPara.mailboxHandle = Hth;
      txPara.hwObjHandle = Can_GetMailboxHwHandle(Hth); 
      txPara.pdu.id = PduInfo->id;
      txPara.pdu.length = PduInfo->length;
      txPara.pdu.sdu = PduInfo->sdu;
      txPara.pdu.swPduHandle = PduInfo->swPduHandle;
#if defined(C_ENABLE_CAN_FD_USED)
      if (txPara.pdu.length <= 8u) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      {
        if (Can_GetInitObjectFdBrsConfig(Can_GetLastInitObject(channel)) == CAN_NONE)
        { /* #220 remove FD flag when no FD support is activated in this initialization structure and DLC not greater than 8 */
          /* CAN FD also support an extended payload which allows the transmission of up to 64 bytes.
          This feature also depends on the CAN FD configuration (see CanControllerFdBaudrateConfig).
          Therefore, if the CAN FD feature is enabled and the CAN FD flag is set in CanId passed to
           Can_Write(), CanDrv supports the transmission of PDUs with a length up to 64 bytes.
          If there is a request to transmit a CAN FD frame without the CAN FD feature enabled the
          frame is sent as conventional CAN frame as long as the PDU length <= 8 bytes. */
          txPara.pdu.id = txPara.pdu.id & ((Can_IdType)(CAN_ID_IDE_MASK | CAN_ID_MASK)); /* CAN_FD_   mask out the FD bit */
        }
      }
      txPara.fdBrsType = kCanFdBrsTypeFalse;
      if (CanHL_IsFdMessage(txPara.pdu.id))
      {
        txPara.fdType = kCanFdTypeFd;
        if (CanHL_IsFdTxBrs(channel))
        {
          txPara.fdBrsType = kCanFdBrsTypeTrue;
        }
      } else {
        txPara.fdType = kCanFdTypeClassic;
      }
      txPara.messageLen = txPara.pdu.length;
      txPara.frameLen = CAN_DLC2LEN(CAN_LEN2DLC(txPara.messageLen));
# if defined(C_ENABLE_CAN_FD_FULL)
      txPara.paddingVal = Can_GetMailboxFdPadding(txPara.mailboxHandle);
# endif
#endif
      /* #230 search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary */
      CanHL_WritePrepare(CAN_CHANNEL_CANPARA_FIRST &txPara); /* SBSW_CAN_HL49 */
      /* #240 start transmission for given handle (if free) and return state */
      retval = CanHL_WriteStart(CAN_CHANNEL_CANPARA_FIRST &txPara); /* SBSW_CAN_HL49 */

#if !defined(CAN_USE_VECTOR_IF) /* No Vector Interface used, Interrupts may not be locked */
      /* avoid change of PDU information due to TX interrupt while changing */
      SchM_Exit_Can_CAN_EXCLUSIVE_AREA_2();
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_WRITE_ID, errorId);
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_Write();
  return retval;
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             ASR4.x - Can_CheckWakeup
****************************************************************************/
/*
|<DataModelStart>| Can_CheckWakeup
Relation_Context:
Relation:
Wakeup, DevErrorDetect, OneChOpt
Wakeup, DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
#if defined(C_ENABLE_CHECK_WAKEUP_CAN_RET_TYPE) /* COV_CAN_COMPATIBILITY */
# define CAN_CHECKWAKEUP_RETTYPE Can_ReturnType
# define CAN_CHECKWAKEUP_RETVAL_OK     CAN_OK
# define CAN_CHECKWAKEUP_RETVAL_NOT_OK CAN_NOT_OK
V_DEF_FUNC(V_NONE, Can_ReturnType, CODE) Can_CheckWakeup( uint8 Controller )
#else
# define CAN_CHECKWAKEUP_RETTYPE Std_ReturnType
# define CAN_CHECKWAKEUP_RETVAL_OK     E_OK
# define CAN_CHECKWAKEUP_RETVAL_NOT_OK E_NOT_OK
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_CheckWakeup( uint8 Controller )
#endif
{ /* \trace SPEC-1740 */
  /* ----- Local Variables ---------------------------------------------- */
  CAN_CHECKWAKEUP_RETTYPE retval;
  retval = CAN_CHECKWAKEUP_RETVAL_NOT_OK; /* \trace SPEC-1407 */
#if defined(C_ENABLE_SLEEP_WAKEUP)
  {
    uint8 errorId;
    errorId = CAN_E_NO_ERROR;
  
    /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
    /* #10 Check if parameter controller is valid and state is INIT */
    if (canConfigInitFlag == CAN_STATUS_UNINIT)
    { /* #20 Check Driver is initialized */
      errorId = CAN_E_UNINIT;
    }
    else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
    { /* \trace SPEC-1738 */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
    if (!CanHL_IsControllerInit(channel))
    { /* #40 Check controller is initialized \trace SPEC-1743 */ /* ESCAN00045884 */
      errorId = CAN_E_UNINIT;
    }
    else
# endif
    {
     /* ----- Implementation ----------------------------------------------- */
     /* #100 if WAKEUP occur: */
     if (Can_IsIsWakeup(channel))
      {
        Can_SetIsWakeup(channel, FALSE); /* SBSW_CAN_HL09 */
        /* #110 Driver call EcuM_SetWakeupEvent() to prevent missing WAKEUP notification (multiple call is OK) */
        EcuM_SetWakeupEvent(Can_GetWakeupSourceRef(channel)); /* \trace SPEC-50550 */
        retval = CAN_CHECKWAKEUP_RETVAL_OK; /* \trace SPEC-1755 */
      }
    }

    /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
    if (errorId != CAN_E_NO_ERROR)
    { /* Throw DET if an error detected before */
      Can_CallDetReportError(CAN_CKWAKEUP_ID, errorId);
    }
#  else
    CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
    CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  }
#else
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_MainFunction_Write
****************************************************************************/
/*
|<DataModelStart>| Can_MainFunction_Write
Relation_Context:
Relation:
TxPolling, HwCancelTx, DevErrorDetect
TxPolling, HwCancelTx, OneChOpt
TxPolling, HwCancelTx, Variant
TxPolling, HwCancelTx, Wakeup
TxPolling, IndividualPolling
TxPolling, TxFullCANSupport, IndividualPolling
TxFullCANSupport, HwCancelTx
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Write( void )
{ /* \trace SPEC-1766 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_ENABLE_TX_POLLING) 
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  CanHookBegin_Can_MainFunction_Write();
  /* canSendSemaphor no more needed because of ControllerInterrupts disabled while Can_Write() */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1767 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON) /* \trace SPEC-1573, SPEC-1402, SPEC-1403 */
      if (!CanHL_IsControllerInit(channel))
      { /* #50 Check controller is initialized */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_IsSleep(channel) )
        { /* #60 do not access CAN hardware in SLEEP */
          tCanTaskParaStruct taskPara;
# if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */ /* always true when no hw-cancel is supported */
          /* #100 do confirmation handling for pending mailboxes */
          if ( CanLL_TxIsGlobalConfPending(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCanTrue ) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
          { /* #110 is at least one mailbox pending */
            for ( taskPara.mailboxHandle = Can_GetTxBasicHandleStart(canHwChannel); taskPara.mailboxHandle < Can_GetTxBasicHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
            { /* #120 iterate over all TxBasicCAN */
              taskPara.hwObjHandle = Can_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
              if ( Can_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
              { /* #130 is mailbox handled by polling (individual) */
                /* #140 call LL confirmation handling */
                CanLL_TxProcessPendings(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
              }
            }
          }
# endif
# if defined(C_ENABLE_CANCEL_TX_IN_HW) 
# endif
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
#  if defined(C_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */ /* always true when no hw-cancel is supported */
          /* #200 do confirmation handling for pending mailboxes */
          if ( CanLL_TxIsGlobalConfPending(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCanTrue ) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
          { /* #210 is at least one mailbox pending */
            for ( taskPara.mailboxHandle = Can_GetTxFullHandleStart(canHwChannel); taskPara.mailboxHandle < Can_GetTxFullHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
            { /* #220 iterate over all TxFullCAN */
              taskPara.hwObjHandle = Can_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#   if defined(C_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
              if ( Can_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#   endif
              { /* #230 is mailbox handled by polling (individual) */
                /* #240 call LL confirmation handling */
                CanLL_TxProcessPendings(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
              }
            }
          }
#  endif
#  if defined(C_ENABLE_CANCEL_TX_IN_HW) 
#  endif
# endif
        } /* !IsSleep */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_WRITE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

#else
  CanHookBegin_Can_MainFunction_Write();
#endif /* C_ENABLE_TX_POLLING */
  CanHookEnd_Can_MainFunction_Write();
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_MainFunction_Read
****************************************************************************/
/*
|<DataModelStart>| Can_MainFunction_Read
Relation_Context:
Relation:
DevErrorDetect
RxPolling, OneChOpt
RxPolling, Variant
RxPolling, DevErrorDetect
RxPolling, Wakeup
RxPolling, RxFullCANSupport
RxPolling, RxBasicCANSupport
RxQueue
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Read( void )
{ /* \trace SPEC-1776 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_MainFunction_Read();
  errorId = CAN_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check Driver is initialized \trace SPEC-1784 */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  {
    errorId = CAN_E_UNINIT;
  }
  else
#endif
  /* ----- Implementation ----------------------------------------------- */
  /* #20 semaphore to secure parallel access to RX buffers interrupt or polling \trace SPEC-1569 */
  if ( canIsRxTaskLocked == kCanFalse )  /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */
  {
#if defined(C_ENABLE_RX_POLLING) /* COV_CAN_RX_POLLING */
    CAN_CHANNEL_CANTYPE_LOCAL
#endif
    canIsRxTaskLocked = kCanTrue; /* PRQA S 2982 */ /* MD_MSR_RetVal */
#if defined(C_ENABLE_RX_POLLING) /* \trace SPEC-1782 */ /* COV_CAN_RX_POLLING */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #30 iterate over all active controller */
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_IsControllerInit(channel))
      { /* #60 Check controller is initialized */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_IsSleep(channel) )
        { /* #70 do not access CAN hardware in SLEEP */
          /* #80 over all HW channel \trace SPEC-1774 */
# if defined(C_ENABLE_RX_FULLCAN_OBJECTS)
          /* #90 search for all pending FullCAN mailbox objects and call notification */
          CanHL_RxFullCanPolling(CAN_HW_CHANNEL_CANPARA_ONLY);
# endif
# if defined(C_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
          /* #100 search for all pending BasicCAN mailbox objects and call notification */
          CanHL_RxBasicCanPolling(CAN_HW_CHANNEL_CANPARA_ONLY);
# endif
        }
      }
    }
#endif /* C_ENABLE_RX_POLLING */
#if defined(C_ENABLE_RX_QUEUE)
    /* #200 execute RxQueue CanHL_RxQueueExecution() */
    CanHL_RxQueueExecution(); 
#endif
    canIsRxTaskLocked = kCanFalse;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_READ_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_MainFunction_Read();
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_MainFunction_BusOff
****************************************************************************/
/*
|<DataModelStart>| Can_MainFunction_BusOff
Relation_Context:
Relation:
StatusPolling, DevErrorDetect, ChannelAmount
StatusPolling, OneChOpt
StatusPolling, Variant, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_BusOff( void )
{ /* \trace SPEC-1783 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_ENABLE_ERROR_POLLING) /* \trace SPEC-1778 */ /* COV_CAN_ERROR_POLLING */
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1780 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCanNumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_IsControllerInit(channel))
      { /* #50 Check controller is initialized */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_IsSleep(channel) )
        { /* #60 do not access CAN hardware in SLEEP */
          /* #80 disable CAN interrupts */
          Can_DisableControllerInterrupts((uint8)channel);
          /* #90 call CanHL_ErrorHandling */
          CanHL_ErrorHandling( CAN_HW_CHANNEL_CANPARA_ONLY );
          Can_EnableControllerInterrupts((uint8)channel);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_BO_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
#endif /* C_ENABLE_ERROR_POLLING */
}

/****************************************************************************
| NAME:             Can_MainFunction_Wakeup
****************************************************************************/
/*
|<DataModelStart>| Can_MainFunction_Wakeup
Relation_Context:
Relation:
WakeupPolling, Wakeup, DevErrorDetect
WakeupPolling, Wakeup, OneChOpt
WakeupPolling, Wakeup, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Wakeup( void )
{ /* \trace SPEC-1770 */
  /* ----- Local Variables ---------------------------------------------- */
}

/****************************************************************************
| NAME:             Can_MainFunction_Mode
****************************************************************************/
/*
|<DataModelStart>| Can_MainFunction_Mode
Relation_Context:
Relation:
DevErrorDetect
OneChOpt, ChannelAmount
Variant, ChannelAmount
DevErrorDetect, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_MainFunction_Mode( void )
{ /* \trace SPEC-1775 */
  /* ----- Local Variables ---------------------------------------------- */
  CAN_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1779 */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  { /* #15 EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_Can_CAN_EXCLUSIVE_AREA_6();
# if !defined( C_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCanNumberOfChannels; channel++) /* CM_CAN_HL01 */
# endif
    { /* #20 over all active controller */
# if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_GetCanIfChannelId(channel) == kCanChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_IsControllerInit(channel))
      { /* #50 Check controller is initialized */ /* \trace SPEC-1779 */
        errorId = CAN_E_UNINIT;
      }
      else
# endif
      {
        uint8 transitionRequest;
        uint8 busOffRecovery;
        uint8 doRamCheck;
        uint8 transitionState;
        transitionRequest = Can_GetModeTransitionRequest(channel);
        busOffRecovery = Can_GetBusOffTransitionRequest(channel);
        doRamCheck = Can_GetRamCheckTransitionRequest(channel);
        /* #55 only one transition request at one time is possible - execute it \trace SPEC-1771 */
        if(transitionRequest != kCanModeNone)
        {
          transitionState = CanLL_ModeTransition(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
          switch(transitionRequest)
          {
# if defined(C_ENABLE_SLEEP_WAKEUP)
            case kCanModeSleep: /* COV_CAN_TRANSITION_REQUEST */
              CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_SLEEP, CANIF_CS_SLEEP, transitionState);
              break;
# endif
            case kCanModeStopReinitFast: /* COV_CAN_TRANSITION_REQUEST */
            case kCanModeStopReinit: /* COV_CAN_TRANSITION_REQUEST */
# if defined(C_ENABLE_SLEEP_WAKEUP)
            case kCanModeWakeupStopReinit: /* COV_CAN_TRANSITION_REQUEST */
# endif
              CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
              break;
# if (CAN_REINIT_START == STD_ON)
            case kCanModeStartReinit: /* COV_CAN_TRANSITION_REQUEST */
# else
            case kCanModeStart: /* COV_CAN_TRANSITION_REQUEST */
# endif
            case kCanModeResetBusOffEnd: /* COV_CAN_TRANSITION_REQUEST */
# if defined(C_ENABLE_SILENT_MODE)
            case kCanModeSilent: /* COV_CAN_TRANSITION_REQUEST */
# endif
              CanHL_NotifyTransition(CAN_CHANNEL_CANPARA_FIRST CAN_STATUS_START, CANIF_CS_STARTED, transitionState);
              if ( transitionState == kCanOk ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_TRANSITION_REQUEST */
              {
                Can_SetIsBusOff(channel, FALSE); /* SBSW_CAN_HL08 */
              }
              break;
            case kCanModeResetBusOffStart: /* COV_CAN_TRANSITION_REQUEST */ /* not expected, no repetition request */
            default: /* COV_CAN_TRANSITION_REQUEST */
              break;
          }
        }
      }
    } /* over all channels */
    SchM_Exit_Can_CAN_EXCLUSIVE_AREA_6();
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_MAINFCT_MODE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_CancelTx
****************************************************************************/
/*
|<DataModelStart>| Can_CancelTx
Relation_Context:
TransmitCancellationAPI
Relation:
DevErrorDetect, TxFullCANSupport
OneChOpt
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
TxFullCANSupport, HwCancelTx
ChannelAmount
TxFullAmount
TxBasicAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_CancelTx( Can_HwHandleType Hth, PduIdType PduId )
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_ENABLE_CANCEL_SUPPORT_API)
  CAN_CHANNEL_CANTYPE_LOCAL
  tCanTxCancellationParaStruct txCancellationPara;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
  if (Hth >= Can_GetSizeOfMailbox())
  { /* #20 Check Hth is in range */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
#  if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
  if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_FIFO_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
#  else
  if ((Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_MUX_TYPE) && (Can_GetMailboxType(Hth) != CAN_TX_BASICCAN_FIFO_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
#  endif
  { /* #30 Check Hth is FullCAN or BasicCAN */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
  {
# if !defined( C_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_GetMailboxController(Hth);
# endif
# if (CAN_DEV_ERROR_DETECT == STD_ON)
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #40 Check parameter controller is in range */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_FAILURE */
    { /* #60 Check parameter controller is valid */
      errorId = CAN_E_PARAM_CONTROLLER;
    }
    else
#  endif
    if (!CanHL_IsControllerInit(channel))
    { /* #70 Check controller is initialized */
      errorId = CAN_E_UNINIT;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      txCancellationPara.mailboxHandle = Hth;
# if defined(C_ENABLE_CANCEL_TX_IN_HW)
      txCancellationPara.hwObjHandle = Can_GetMailboxHwHandle(Hth);
# endif
      txCancellationPara.mailboxElement = 0u;
      txCancellationPara.activeSendObject = CanHL_GetActiveSendObject(txCancellationPara.mailboxHandle, txCancellationPara.mailboxElement);

# if defined (C_ENABLE_TX_FULLCAN_OBJECTS)
      if ( Can_GetMailboxType(Hth) != CAN_TX_FULLCAN_TYPE ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
# endif
      { /* #100 is BasicCAN */
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO) /* \trace SPEC-1677 */
        for (; txCancellationPara.mailboxElement < Can_GetMailboxSize(Hth); txCancellationPara.mailboxElement++, txCancellationPara.activeSendObject++) /* PRQA S 2463,3387,3418 */ /* MD_Can_HL_AdditionalExpressionInForLoop,MD_Can_HL_UsingIncOrDec,MD_Can_IncompleteForLoop */
# endif
        { /* #110 over all Multiplexed TX objects - search for handle */
          if (Can_GetActiveSendPdu(txCancellationPara.activeSendObject) == PduId)
          {
            if ((Can_GetActiveSendState(txCancellationPara.activeSendObject) == kCanBufferSend)
# if defined(C_ENABLE_CANCEL_TX_IN_HW)
                || (Can_GetActiveSendState(txCancellationPara.activeSendObject) == kCanBufferCancel)
# endif
                )  /* COV_CAN_CANCEL_SW_STIMULATION */
            { /* #120 PDU found in active send list - handle found */
              /* #130 CANCEL_IN_HW is active - just suppress TX confirmation */
# if defined(C_ENABLE_CANCEL_IN_HW)
              { /* #140 LL TX cancel in HW */ /* \trace SPEC-1690 */
                /* #150 mark as to be cancelled by hardware */
                txCancellationPara.canHandleCurTxObj = kCanBufferCancelSw;
                CanLL_TxCancelInHw(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
                Can_SetActiveSendState(txCancellationPara.activeSendObject, kCanBufferCancelSw); /* SBSW_CAN_HL04 */
              }
# else
              Can_SetActiveSendState(txCancellationPara.activeSendObject, kCanBufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
# endif
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
              break;
# endif
            }
          }
        }
      }
# if defined (C_ENABLE_TX_FULLCAN_OBJECTS)
      else
      { /* #200 is FullCAN */
        if (Can_GetActiveSendPdu(txCancellationPara.activeSendObject) == PduId)
        {
          if ((Can_GetActiveSendState(txCancellationPara.activeSendObject) == kCanBufferSend)
#  if defined(C_ENABLE_CANCEL_TX_IN_HW)
              || (Can_GetActiveSendState(txCancellationPara.activeSendObject) == kCanBufferCancel)
#  endif
             )  /* COV_CAN_CANCEL_SW_STIMULATION */
          { /* #210 mailbox to cancel is valid (PDU, handle, send state OK) */
            /* #220 CANCEL_FULLCAN_IN_HW is active - just suppress TX confirmation */
#  if defined(C_ENABLE_CANCEL_TX_IN_HW)
            { /* #230 LL TX cancel in HW */ /* \trace SPEC-1690 */
              /* #150 mark as to be cancelled by hardware */
              txCancellationPara.canHandleCurTxObj = kCanBufferCancelSw;
              CanLL_TxCancelInHw(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
              Can_SetActiveSendState(txCancellationPara.activeSendObject, kCanBufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
            }
#  endif
            Can_SetActiveSendState(txCancellationPara.activeSendObject, kCanBufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
          }
        }
      }
# endif
    }
    CAN_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_WRITE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
#else /* CAN_ENABLE_CANCEL_SUPPORT_API */
  CAN_DUMMY_STATEMENT(Hth); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(PduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* CAN_ENABLE_CANCEL_SUPPORT_API */
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

#if (CAN_GET_STATUS == STD_ON)
/****************************************************************************
| NAME:             Can_GetStatus
****************************************************************************/
/*
|<DataModelStart>| Can_GetStatus
Relation_Context:
CanGetStatus
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, uint8, CODE) Can_GetStatus( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 canReturnCode;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  canReturnCode = CAN_STATUS_INCONSISTENT; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #20 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* #50 Check controller is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CanLL_GetStatusBegin(CAN_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
    /* #100 set logical state as return value */
    canReturnCode = (uint8)Can_GetLogStatus(channel);
    if ( CanLL_HwIsBusOff(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #150 set BUSOFF - BUSOFF can only happen on first HW channel (with TX objects) */
      canReturnCode |= CAN_STATUS_BUSOFF;
    }
# if defined(C_ENABLE_EXTENDED_STATUS) /* COV_CAN_EXTENDED_STATUS */
    {    
      if ( CanLL_HwIsPassive(channel) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #160 set Error Passive */
        canReturnCode |= CAN_STATUS_PASSIVE;
      }    
      if ( CanLL_HwIsWarning(channel) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #170 set Error Warning */
        canReturnCode |= CAN_STATUS_WARNING;
      }
    }
# endif /* C_ENABLE_EXTENDED_STATUS */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return (uint8)canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_GetControllerMode
****************************************************************************/
/*
|<DataModelStart>| Can_GetControllerMode
Relation_Context:
CanGetStatus
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_GetControllerMode( uint8 Controller, Can_ControllerStatePtrType ControllerModePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameters are valid */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #20 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (ControllerModePtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 get logical controller mode from state machine variable */
    if (!CanHL_IsControllerInit(channel))
    {
      *ControllerModePtr = CAN_CS_UNINIT; /* SBSW_CAN_HL51 */
    }
    else if (CanHL_IsStart(channel))
    {
      *ControllerModePtr = CAN_CS_STARTED; /* SBSW_CAN_HL51 */
    } 
    else if (CanHL_IsSleep(channel))
    {
      *ControllerModePtr = CAN_CS_SLEEP; /* SBSW_CAN_HL51 */
    }
    else /* default is:   if (CanHL_IsStop(channel)) */
    {
      *ControllerModePtr = CAN_CS_STOPPED; /* SBSW_CAN_HL51 */
    }
    canReturnCode = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_GETCTR_MODE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_GetControllerErrorState
****************************************************************************/
/*
|<DataModelStart>| Can_GetControllerErrorState
Relation_Context:
CanGetStatus
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_GetControllerErrorState( uint8 Controller, Can_ErrorStatePtrType ErrorStatePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
  if (ErrorStatePtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CanLL_GetStatusBegin(CAN_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
    if ( CanLL_HwIsBusOff(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #40 set BUSOFF */
      *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF; /* SBSW_CAN_HL52 */
    } 
# if defined(C_ENABLE_EXTENDED_STATUS) /* COV_CAN_EXTENDED_STATUS */
    else if ( CanLL_HwIsPassive(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #50 set Error Passive */
      *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /* SBSW_CAN_HL52 */
    }
# endif /* C_ENABLE_EXTENDED_STATUS */
    else
    { /* #60 default return is ACTIVE */
      *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE; /* SBSW_CAN_HL52 */
    }
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_GETCTR_STATE_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_GetControllerTxErrorCounter
****************************************************************************/
/*
|<DataModelStart>| Can_GetControllerTxErrorCounter
Relation_Context:
CanGetStatus
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_GetControllerTxErrorCounter( uint8 Controller, Can_ErrorCounterPtrType TxErrorCounterPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
  if (TxErrorCounterPtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 get error counter from hardware */
# if defined( C_SINGLE_RECEIVE_CHANNEL )
    *TxErrorCounterPtr = CanTxActualErrorCounter(); /* SBSW_CAN_HL53 */
# else
    *TxErrorCounterPtr = CanTxActualErrorCounter(channel); /* SBSW_CAN_HL53 */
# endif
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_GETCTR_TXCNT_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_GetControllerRxErrorCounter
****************************************************************************/
/*
|<DataModelStart>| Can_GetControllerRxErrorCounter
Relation_Context:
CanGetStatus
Relation:
DevErrorDetect, OneChOpt
DevErrorDetect, Variant
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_GetControllerRxErrorCounter( uint8 Controller, Can_ErrorCounterPtrType RxErrorCounterPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_GetCanIfChannelId(channel) == kCanChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
  if (RxErrorCounterPtr == NULL_PTR)
  {
    errorId = CAN_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 get error counter from hardware */
# if defined( C_SINGLE_RECEIVE_CHANNEL )
    *RxErrorCounterPtr = CanRxActualErrorCounter(); /* SBSW_CAN_HL54 */
# else
    *RxErrorCounterPtr = CanRxActualErrorCounter(channel); /* SBSW_CAN_HL54 */
# endif
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_GETCTR_RXCNT_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#endif

#if defined(C_ENABLE_MIRROR_MODE)
/****************************************************************************
| NAME:             Can_SetMirrorMode
****************************************************************************/
/*
|<DataModelStart>| Can_SetMirrorMode
Relation_Context:
MirrorMode
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_SetMirrorMode( uint8 Controller, CddMirror_MirrorModeType mirrorMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set mirror mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetMirrorModeState(channel, mirrorMode); /* SBSW_CAN_HL20 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif

#if defined(C_ENABLE_SILENT_MODE)
/****************************************************************************
| NAME:             Can_SetSilentMode
****************************************************************************/
/*
|<DataModelStart>| Can_SetSilentMode
Relation_Context:
SilentMode
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, Std_ReturnType, CODE) Can_SetSilentMode( uint8 Controller, Can_SilentModeType silentMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Std_ReturnType retval;
  errorId = CAN_E_NO_ERROR;
  retval = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set silent mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_SetSilentModeState(channel, silentMode); /* SBSW_CAN_HL20 */
    retval = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK_EXTENDED)
/****************************************************************************
| NAME:             Can_RamCheckExecute
****************************************************************************/
/*
|<DataModelStart>| Can_RamCheckExecute
Relation_Context:
Relation:
RamCheck, DevErrorDetect, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckExecute( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 Do RamCheck by execute reinitialization with activated check */
    uint8 transitionState;
    /* ----- Implementation ----------------------------------------------- */
    transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST kCanModeStopReinit, kCanFinishBusOffRecovery, kCanExecuteRamCheck);
    if (transitionState == kCanOk) /* COV_CAN_TRANSITION_REQUEST */
    { /* #40 check transition STOP (Reinit) is successful */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/****************************************************************************
| NAME:             Can_RamCheckEnableMailbox
****************************************************************************/
/*
|<DataModelStart>| Can_RamCheckEnableMailbox
Relation_Context:
Relation:
RamCheck, DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckEnableMailbox( Can_HwHandleType htrh )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
  if (htrh >= Can_GetSizeOfMailbox()) { /* CM_CAN_HL28 */
    errorId = CAN_E_PARAM_HANDLE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #15 disable controller to force reinitialization afterwards in Can_RamCheckEnableController() */
    Can_SetLogStatus(Can_GetMailboxController(htrh), CAN_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL40 */
    /* #20 reactivate mailbox */
    Can_SetMailboxState(htrh, kCanOk); /* SBSW_CAN_HL34 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/****************************************************************************
| NAME:             Can_RamCheckEnableController
****************************************************************************/
/*
|<DataModelStart>| Can_RamCheckEnableController
Relation_Context:
RamCheck
Relation:
DevErrorDetect, OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_RamCheckEnableController( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_DEV_ERROR_DETECT == STD_ON)
  if (canConfigInitFlag == CAN_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_E_UNINIT;
  }
  else
#  if !defined( C_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCanNumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 activate controller only when currently disabled */
    if ( (Can_GetLogStatus(channel) & CAN_DEACTIVATE_CONTROLLER) == CAN_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      uint8 transitionState;
      /* #40 re-initialize to activate mailboxes and controller after RAM check issue */
      Can_SetLogStatus(channel, Can_GetLogStatus(channel) & (uint8)(~(CAN_DEACTIVATE_CONTROLLER))); /* SBSW_CAN_HL02 */
      transitionState = CanHL_ModeTransition(CAN_CHANNEL_CANPARA_FIRST kCanModeStopReinit, kCanFinishBusOffRecovery, kCanSuppressRamCheck);
      if (transitionState == kCanOk) /* COV_CAN_TRANSITION_REQUEST */
      { /* #40 check transition STOP (Reinit) is successful */
        Can_SetLogStatus(channel, Can_GetLogStatus(channel) | CAN_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_CallDetReportError(CAN_HW_ACCESS_ID, errorId);
  }
# else
  CAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif


/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanSetHighVoltageMode
****************************************************************************/
/*
|<DataModelStart>| CanSetHighVoltageMode
Relation_Context:
Relation:
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
V_DEF_FUNC(V_NONE, void, CODE) CanSetHighVoltageMode( CAN_CHANNEL_CANTYPE_FIRST vuint8 mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 set high voltage mode */
  Can_GetgCanController(channel).HighVoltageMode = mode; /* SBSW_CAN_LL08 */
}
/* CODE CATEGORY 1 END */


/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_RxIndication
****************************************************************************/
/*
|<DataModelStart>| Can_RxIndication
Relation_Context:
Relation:
RxFullCANSupport
RxBasicCANSupport
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_RxIndication( vuint8 canChannel, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data ) /* PRQA S 2889 */ /* MD_CBD_14.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 reject while STOP mode */
  /* #20 reject unwanted remote frame */
  /* #30 reject unwanted ID type */
  if ((canChannel < kCanNumberOfChannels)
      && (Can_GetgCanController(canChannel).Mode != CAN_MODE_STOP) /* emulated so ignore message while STOP */
      && ((flags & (vuint32)CANOEAPI_CAN_RemoteFrame) != (vuint32)CANOEAPI_CAN_RemoteFrame) /* reject RemoteFrames */
# if !defined( C_ENABLE_MIXED_ID )
#  if defined( C_ENABLE_EXTENDED_ID )
      && (((id) & kCanIdTypeExt) == kCanIdTypeExt) /* wrong id type passed HW-filter (may not closed so just reject) */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#  else
      && (((id) & kCanIdTypeExt) != kCanIdTypeExt) /* wrong id type passed HW-filter (may not closed so just reject) */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#  endif
# endif
     ) /* COV_CAN_LL_EMULATION */
  {
 
    /* #40 reception while SLEEP is wakeup - call wakeup handling */
    if ( Can_GetgCanController(canChannel).Mode == CAN_MODE_SLEEP ) /* COV_CAN_LL_EMULATION */
    {
      Can_GetgCanController(canChannel).WakeupPending = (vuint8)kCanTrue; /* SBSW_CAN_LL08 */
      /* continue with reception after wakeup notification */
    }

#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
    {
      /* #50 check if message is RX FullCAN - call message handling */
      CanObjectHandle mailbox;
      /* #60 iterate over all hardware objects */
      for (mailbox=Can_GetRxFullHandleStart(canChannel); mailbox < Can_GetRxFullHandleStop(canChannel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
      {
        CanObjectHandle hwObj;
        hwObj = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
        if ( (Can_GetgCanController(canChannel).MsgObj[hwObj].mMsg.IdRaw & kCanFdTypeFdMaskOut) == (id & kCanFdTypeFdMaskOut) )
        {
          if ( dataLength <= Can_GetMailboxDataLen(mailbox) ) /* COV_CAN_HARDWARE_FAILURE */
          {
            CanLL_RxIndicationMsgCopy(canChannel, mailbox, hwObj, id, flags, dataLength, data); /* SBSW_CAN_LL11 */
            return;
          }
        }
      }
    }
#endif
#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS ) && (CAN_BCANHWIDXOFFILTER == STD_ON)
    {
      /* #70 check if message is RX BasicCAN - call message handling */
      vuint8 filterIdx;
      /* #80 iterate over all filter/BasicCAN */
      for (filterIdx = Can_GetFilterStartIdx(canChannel); filterIdx < Can_GetFilterStartIdx(canChannel + 1u); filterIdx++) /* COV_CAN_HARDWARE_FAILURE */
      {
        if ( (Can_GetMaskOfFilter(filterIdx) & id ) == (Can_GetMaskOfFilter(filterIdx) & Can_GetCodeOfFilter(filterIdx)) ) /* COV_CAN_HARDWARE_FAILURE */
        {
          CanObjectHandle hwObj;
          CanObjectHandle mailbox;
          mailbox = Can_GetRxBasicHandleStart(canChannel) + Can_GetBCanLogIdxOfFilter(filterIdx);
          if ( dataLength <= Can_GetMailboxDataLen(mailbox) ) /* COV_CAN_HARDWARE_FAILURE */
          {
            vuint8 bCanLastWrittenPointer = Can_GetgCanController(canChannel).BCanWrittenPointer[Can_GetBCanLogIdxOfFilter(filterIdx)];
            if ( (bCanLastWrittenPointer + 1u) >= Can_GetBCanHwSizeOfFilter(filterIdx) )
            {
              bCanLastWrittenPointer = 0u;
            } else {
              bCanLastWrittenPointer++;
            }
            hwObj = Can_GetBCanHwIdxOfFilter(filterIdx) + bCanLastWrittenPointer;
            CanLL_RxIndicationMsgCopy(canChannel, mailbox, hwObj, id, flags, dataLength, data); /* SBSW_CAN_LL11 */
            Can_GetgCanController(canChannel).BCanWrittenPointer[Can_GetBCanLogIdxOfFilter(filterIdx)] = bCanLastWrittenPointer; /* SBSW_CAN_LL08 */
            return;
          }
        }
      }
    }
#endif
  }
  else 
  {
    assertHardware((canChannel < kCanNumberOfChannels), (canChannel), (kErrorWrongChannelForMsgEvent)); /* COV_CAN_HARDWARE_FAILURE */
  }
  CAN_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(data); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_TxConfirmation
****************************************************************************/
/*
|<DataModelStart>| Can_TxConfirmation
Relation_Context:
Relation:
TxFullCANSupport
MultiplexedTx, TxHwFifo
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) Can_TxConfirmation( vuint8 canChannel, vuint32 id, vuint32 flags, vuint8 dataLength, const vuint8* data ) /* PRQA S 2889 */ /* MD_CBD_14.7 */
{
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((canChannel < kCanNumberOfChannels),canChannel,kErrorWrongChannelForMsgEvent); /* lint !e506 */ /* COV_CAN_HARDWARE_FAILURE */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 reject while not START mode */
  if ( Can_GetgCanController(canChannel).Mode != CAN_MODE_START ) /* COV_CAN_LL_EMULATION */
  {
     return; /* emulated so ignore message while !START */
  }
  /* search the TX object, which sends this message */
  {
    CanObjectHandle mailbox;
#if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
    for (mailbox=Can_GetTxFullHandleStart(canChannel); mailbox < Can_GetTxFullHandleStop(canChannel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    { /* #100 search in TxFullCAN mailboxes for identical ID */
      CanObjectHandle hwObj;
      hwObj = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
      {
        if ( ((Can_GetgCanController(canChannel).MsgObj[hwObj].mMsg.IdRaw & kCanFdTypeFdMaskOut) == (id & kCanFdTypeFdMaskOut)) && (Can_GetgCanController(canChannel).MsgObj[hwObj].mIsFree != (vuint8)kCanTrue)) /* COV_CAN_LL_EMULATION */
        { /* #110 valid message because of FD mask and mailbox is active */
          /* #120 mark as pending */
          Can_GetgCanController(canChannel).MsgObj[hwObj].mIsPending = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
          return;
        }
      }
    }
#endif
    for (mailbox=Can_GetTxBasicHandleStart(canChannel); mailbox < Can_GetTxBasicHandleStop(canChannel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    { /* #200 iterate over all TxBasicCAN mailboxes */
      CanObjectHandle hwObj;
      hwObj = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
      { /* #210 iterate over all hardware objects */
#if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
        CanObjectHandle hwObjStop;
# if defined(C_ENABLE_TX_HW_FIFO)
        CanObjectHandle hwObjMailboxEnd;
        hwObjMailboxEnd = Can_GetMailboxHwHandle(mailbox) + Can_GetMailboxSize(mailbox);
        if (Can_GetMailboxType(mailbox) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
           uint16 txHwFifo = Can_GetMailboxTxHwFifo(mailbox);
           hwObj = hwObj + (CanObjectHandle)Can_GetTxHwFifoReadIndex(txHwFifo);
           hwObjStop = hwObj + (CanObjectHandle)Can_GetTxHwFifoFillCount(txHwFifo);
        } else
# endif
        {
          hwObjStop = hwObj + Can_GetMailboxSize(mailbox);
        }
        for (; hwObj < hwObjStop;) /* PRQA S 2472 */ /* Runtime optimization */ /* COV_CAN_HARDWARE_FAILURE */
#endif
        {
          vuint8 foundPending = (vuint8)kCanFalse;
          if ( ((Can_GetgCanController(canChannel).MsgObj[hwObj].mMsg.IdRaw & kCanFdTypeFdMaskOut) == (id & kCanFdTypeFdMaskOut)) && (Can_GetgCanController(canChannel).MsgObj[hwObj].mIsFree != (vuint8)kCanTrue) && (Can_GetgCanController(canChannel).MsgObj[hwObj].mIsPending != (vuint8)kCanTrue)) /* COV_CAN_LL_EMULATION */
          { /* #220 valid message because of FD mask and mailbox is active */
            /* #230 mark as pending */
            foundPending = (vuint8)kCanTrue;
            Can_GetgCanController(canChannel).MsgObj[hwObj].mIsPending = (vuint8)kCanTrue; /* SBSW_CAN_LL01 */
          }
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
#  if defined(C_ENABLE_TX_HW_FIFO)
          if (Can_GetMailboxType(mailbox) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
            if (hwObj == (hwObjMailboxEnd - 1u)) {
              hwObj = Can_GetMailboxHwHandle(mailbox);
              hwObjStop -= Can_GetMailboxSize(mailbox);
            } else {
              hwObj++;
            }
          } else
#  endif
          {
            hwObj++;
          }
# endif
          if (foundPending == (vuint8)kCanTrue)
          {
# if defined(C_ENABLE_TX_HW_FIFO)
            /* This is a workaround to force FIFO to send in expected order (CANoe will send all requested elements in priority order) 
             --> request only one FIFO element to send and retrigger next in confirmation */
            if (Can_GetMailboxType(mailbox) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
              if ( (hwObj < hwObjStop) &&  (Can_GetgCanController(canChannel).MsgObj[hwObj].mIsFree == (vuint8)kCanTrue) )
              { /* next to send out of FIFO */
                tCanTxTransmissionParaStruct txParaObj;
                vuint8 Controller = canChannel;
                txParaObj.mailboxHandle = mailbox;
                txParaObj.mailboxElement = (hwObj - Can_GetMailboxHwHandle(mailbox));
                (void) CanLL_TxStart(CAN_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txParaObj); /* SBSW_CAN_HL49 */
              }
            }
# endif
            return;
          }
        }
      }
    }
  }
  CAN_DUMMY_STATEMENT(flags); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(data); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* PRQA S 6010,6030 */ /* MD_MSR_STPTH,MD_MSR_STCYC */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             Can_ErrIndication
****************************************************************************/
/*
|<DataModelStart>| Can_ErrIndication
Relation_Context:
Relation:
ChannelAmount
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
V_DEF_FUNC(V_NONE, void, CODE) Can_ErrIndication( vuint8 canChannel, vuint32 errCode, vuint8 errCntTx, vuint8 errCntRx ) /* PRQA S 2889 */ /* MD_CBD_14.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  assertHardwareReturn((canChannel < kCanNumberOfChannels),canChannel,kErrorWrongChannelForMsgEvent); /* lint !e506 */ /* COV_CAN_HARDWARE_FAILURE */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Error handling */
  if ((errCode == (vuint32)CANCELL_ERRORPASSIVE) && (Can_GetgCanController(canChannel).ErrCode != (vuint32)CANCELL_ERRORPASSIVE))
  {
    Can_GetgCanController(canChannel).ErrCode = (vuint32)CANCELL_ERRORPASSIVE_EVENT; /* notify only once */ /* SBSW_CAN_LL08 */
  } else {
    Can_GetgCanController(canChannel).ErrCode = errCode; /* SBSW_CAN_LL08 */
  }
  Can_GetgCanController(canChannel).ErrCntTx = errCntTx; /* SBSW_CAN_LL08 */
  Can_GetgCanController(canChannel).ErrCntRx = errCntRx; /* SBSW_CAN_LL08 */
}
/* CODE CATEGORY 1 END */

/* **************************************************************************
| NAME:             CanInterrupt<Name>
| CALLED BY:        CanIsr<Name>_<physicalChannelIndex>()
| PRECONDITIONS:
| INPUT PARAMETERS: canHwChannel
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt structure
|                   - check for the interrupt reason ( interrupt source )
|                   - work appropriate interrupt:
|                     + status/error interrupt (BUSOFF, wakeup, error warning)
|                     + basic can receive
|                     + full can receive
|                     + can transmit
|
|                   If an RX-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   a wakeup has to be generated. 
|
|                   If an TX-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   an assertion has to be called and the interrupt has to be ignored.
|
|                   The name of BrsTimeStrt...() and BrsTimeStop...() can be adapted to 
|                   really used name of the interrupt functions.
|
************************************************************************** */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptTx
****************************************************************************/
/*
|<DataModelStart>| CanInterruptTx
Relation_Context:
Relation:
OneChOpt
ChannelAmount
TxPolling, IndividualPolling, TxFullCANSupport
TxPolling, IndividualPolling, MultiplexedTx
TxBasicAmount
TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptTx( vuint8 canChannel ) /* PRQA S 2889 */ /* MD_CBD_14.7 */ /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#else
  CanChannelHandle channel;
  channel = canChannel;
  CAN_DUMMY_STATEMENT(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent); /* PRQA S 2742 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
#endif
  /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptTx);
#endif
#if !defined( C_ENABLE_TX_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    CanObjectHandle mailbox;
# if defined(C_ENABLE_TX_FULLCAN_OBJECTS)
    for (mailbox=Can_GetTxFullHandleStart(channel); mailbox < Can_GetTxFullHandleStop(channel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    { /* #100 iterate over all TxFullCAN mailboxes */
      CanObjectHandle hwObj;
      hwObj = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      if ( Can_GetgCanController(channel).MsgObj[hwObj].mIsPending == (vuint8)kCanTrue )
      { /* #110 is pending hardware */
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
        /* #120 check interrupt or polling */
        if ( !Can_IsMailboxIndivPolling(mailbox) ) /* COV_CAN_HWOBJINDIVPOLLING */
# endif
        {
          /* #130 call indication */
          CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST mailbox, 0u, hwObj);
        }
      }
    } /* FullCANs */
# endif    
    for (mailbox=Can_GetTxBasicHandleStart(channel); mailbox < Can_GetTxBasicHandleStop(channel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    { /* #200 iterate over all TxBasicCAN mailboxes */
      CanObjectHandle hwObj;
      CanObjectHandle mailboxElement = 0u; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      hwObj = Can_GetMailboxHwHandle(mailbox);
      { /* #210 iterate over all hardware objects */
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
        CanObjectHandle hwObjStop;
# if defined(C_ENABLE_TX_HW_FIFO)
        CanObjectHandle hwObjMailboxEnd;
        hwObjMailboxEnd = Can_GetMailboxHwHandle(mailbox) + Can_GetMailboxSize(mailbox);
        if (Can_GetMailboxType(mailbox) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
           uint16 txHwFifo = Can_GetMailboxTxHwFifo(mailbox);
           hwObj = hwObj + (CanObjectHandle)Can_GetTxHwFifoReadIndex(txHwFifo);
           hwObjStop = hwObj + (CanObjectHandle)Can_GetTxHwFifoFillCount(txHwFifo);
        } else
# endif
        {
          hwObjStop = hwObj + Can_GetMailboxSize(mailbox);
        }
        for (; hwObj < hwObjStop; ) /* PRQA S 2472 */ /* Runtime optimization */ /* COV_CAN_HARDWARE_FAILURE */
# endif
        { /* #220 is pending hardware */
          if ( Can_GetgCanController(channel).MsgObj[hwObj].mIsPending == (vuint8)kCanTrue )
          {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
            /* #230 check interrupt or polling */
            if ( !Can_IsMailboxIndivPolling(mailbox) ) /* COV_CAN_HWOBJINDIVPOLLING */
# endif
            {
              /* #240 call indication */
              CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST mailbox, mailboxElement, hwObj);
            }
          }
# if defined(C_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_ENABLE_TX_HW_FIFO)
#  if defined(C_ENABLE_TX_HW_FIFO)
          if (Can_GetMailboxType(mailbox) == CAN_TX_BASICCAN_FIFO_TYPE) { /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
            if (hwObj == (hwObjMailboxEnd - 1u)) {
              hwObj = Can_GetMailboxHwHandle(mailbox);
              hwObjStop -= Can_GetMailboxSize(mailbox);
            } else {
              hwObj++;
            }
          } else
#  endif
          {
            hwObj++;
            mailboxElement++;
          }
# endif
        }
      }
    } /* BasicCANs */
  }
#endif
#if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptTx);
#endif
} /* CanInterruptTx */ /* PRQA S 6010,6030 */ /* MD_MSR_STPTH,MD_MSR_STCYC */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptWakeup
****************************************************************************/
/*
|<DataModelStart>| CanInterruptWakeup
Relation_Context:
Relation:
WakeupPolling
Wakeup
OneChOpt
ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptWakeup( vuint8 canChannel ) /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_ENABLE_SLEEP_WAKEUP )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# else
  CanChannelHandle channel;
  channel = canChannel;
  CAN_DUMMY_STATEMENT(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  /* ----- Implementation ----------------------------------------------- */
# if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptWakeup);
# endif
  /* #10 Check Wakeup pending */
  if (CanLL_HwIsSleep(channel) && (Can_GetgCanController(channel).WakeupPending == (vuint8)kCanTrue)) /* COV_CAN_LL_EMULATION */
  {
    /* #20 call wakeup handling */
    CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);
    /* this message only WAKEUP the cell, and is received */
    /* visbir: VTT only: WAKEUP message will be lost because WAKEUP clean message buffer (possible workaround, remember WAKEUP message) */
  }
  Can_GetgCanController(channel).WakeupPending = (vuint8)kCanFalse; /* reset old pending flags */ /* SBSW_CAN_LL08 */
# if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptWakeup);
# endif
#else
  CAN_DUMMY_STATEMENT(canChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* CanInterruptWakeup */
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptRx
****************************************************************************/
/*
|<DataModelStart>| CanInterruptRx
Relation_Context:
Relation:
OneChOpt
ChannelAmount
RxFullCANSupport, RxPolling, IndividualPolling
RxBasicCANSupport, RxPolling, IndividualPolling
RxBasicAmount
RxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptRx( vuint8 canChannel ) /* PRQA S 2889 */ /* MD_CBD_14.7 */ /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#else
  CanChannelHandle channel;
  channel = canChannel;
  CAN_DUMMY_STATEMENT(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent);  /* PRQA S 2742 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
#endif
  /* ----- Implementation ----------------------------------------------- */
#if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptRx);
#endif
#if defined ( C_ENABLE_RX_FULLCAN_OBJECTS )
# if !defined( C_ENABLE_RX_FULLCAN_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    CanObjectHandle mailbox;
    /* #10 search for any RXFullCAN which generate interrupt for this message */
    for (mailbox=Can_GetRxFullHandleStart(channel); mailbox < Can_GetRxFullHandleStop(channel); mailbox++) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    {
      CanObjectHandle hwObj;
      hwObj = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      /* #20 check pending flag */
      if ( Can_GetgCanController(channel).MsgObj[hwObj].mIsPending == (vuint8)kCanTrue )
      {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
        /* #30 check interrupt or polling */
        if ( !Can_IsMailboxIndivPolling(mailbox) ) /* COV_CAN_HWOBJINDIVPOLLING */
# endif
        {
          /* #40 call indication */
          CanHL_FullCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST mailbox, hwObj);
        }
      }
    }
  }
# endif
#endif
#if defined ( C_ENABLE_RX_BASICCAN_OBJECTS )
# if !defined( C_ENABLE_RX_BASICCAN_POLLING ) || defined( C_ENABLE_INDIVIDUAL_POLLING )
  {
    CanObjectHandle mailbox;
    CanObjectHandle hwObjHandle;
    vuint8 bCanIdx;
    CanObjectHandle bCanSize;
    CanObjectHandle bCanBaseHwIdx;
    CanObjectHandle bCanWrittenPointer;
    boolean isPending;
    bCanIdx=0;
    /* #50 search for any RXBasicCAN which generate interrupt for this message */
    for (mailbox = Can_GetRxBasicHandleStart(channel); mailbox < Can_GetRxBasicHandleStop(channel); ) /* PRQA S 2991,2992,2994,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
    { /* loop over all BasicCAN mailboxes */
      bCanBaseHwIdx = Can_GetMailboxHwHandle(mailbox); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
      /* #60 check interrupt or polling - skip */
      if ( Can_IsMailboxIndivPolling(mailbox) ) /* COV_CAN_HWOBJINDIVPOLLING */
      {
        mailbox++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
        bCanIdx++;
      }
      else
#  endif
      {
        bCanSize = Can_GetMailboxSize(mailbox);
        bCanWrittenPointer = Can_GetgCanController(channel).BCanWrittenPointer[bCanIdx] + bCanBaseHwIdx;
        isPending = FALSE;
        hwObjHandle = bCanWrittenPointer;
        /* #70 search for next mailbox BasicCAN-FIFO-element to be executed  */
        do
        {
          hwObjHandle++;
          if (hwObjHandle >= (bCanBaseHwIdx+bCanSize))
          {
            hwObjHandle = bCanBaseHwIdx;
          }
          /* #80 check pending flag - call indication */
          if (Can_GetgCanController(channel).MsgObj[hwObjHandle].mIsPending == (vuint8)kCanTrue)
          {
            isPending = TRUE;
            break;
          }
        } while (hwObjHandle != bCanWrittenPointer);
        if (isPending == TRUE)
        {
          CanHL_BasicCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST mailbox, hwObjHandle);
        } else { /* no more pending element in BasicCAN-FIFO so continue with next BasicCAN */ 
          mailbox++; /* PRQA S 2469 */ /* Used to acelerate search - no risk for endless loop */
          bCanIdx++;
        }
      }
    }
  }
# endif
#endif
#if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptRx);
#endif
} /* CanInterruptRx */ /* PRQA S 6010,6030 */ /* MD_MSR_STPTH,MD_MSR_STCYC */
/* CODE CATEGORY 1 END */

/* CANoe Error Callback for Emulation  (new with CANOEAPI 1.06)*/
/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanInterruptErr
****************************************************************************/
/*
|<DataModelStart>| CanInterruptErr
Relation_Context:
Relation:
OneChOpt
StatusPolling, RuntimeMeasurement
ChannelAmount
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
V_DEF_FUNC(V_NONE, void, CODE) CanInterruptErr( vuint8 canChannel ) /* PRQA S 2889 */ /* MD_CBD_14.7 */ /* COV_CAN_LL_ISR_USAGE */
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined( C_SINGLE_RECEIVE_CHANNEL )
  CAN_DUMMY_STATEMENT(canChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#else
  CanChannelHandle channel;
  channel = canChannel; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_DUMMY_STATEMENT(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Checks ------------------------------------- */
  assertHardwareReturn((channel < kCanNumberOfChannels),channel,kErrorWrongChannelForMsgEvent);  /* PRQA S 2742 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_EMULATION */
#endif
  /* ----- Implementation ----------------------------------------------- */
#if !defined( C_ENABLE_ERROR_POLLING )
# if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Start(RtmConf_RtmMeasurementPoint_CanInterruptErr);
# endif
  /* #10 call error handling */
  CanHL_ErrorHandling(CAN_HW_CHANNEL_CANPARA_ONLY);
# if defined(C_ENABLE_RUNTIME_MEASUREMENT_SUPPORT)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_CanInterruptErr);
# endif
#endif
}
/* CODE CATEGORY 1 END */

#define CAN_STOP_SEC_CODE  /*----------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/****************************************************************************/

/****************************************************************************/
/*  excluded Requirements                                                   */
/****************************************************************************/
/* part of other component: SPEC-1597 */
/* Currently no use-case / not supported: SPEC-1601, SPEC-1592, SPEC-1582 */

/****************************************************************************/
/*  MISRA deviations                                                        */
/****************************************************************************/
/* Justification for module-specific MISRA deviations:

MD_Can_Goto:
PRQA message 2001
  Reason: 'goto' statements are used to reduce code complexity.
  Risk: Incorrect jump.
  Prevention: Code inspection and runtime tests.

MD_Can_MultipleReturn:
PRQA message 2889
  Reason: Multiple return paths are used to reduce code complexity.
  Risk: Return a function too soon.
  Prevention: Code inspection and runtime tests.

MD_Can_ParameterName:
PRQA message 0784
  Reason: API parameter that is also used as a macro name is accepted for compatibility reasons.
  Risk: The macro can change the declaration unintentionally.
  Prevention: Relevant inconsistencies in function declarations, definitions and calls are detected by the compiler.

MD_Can_ConstValue:
PRQA message 2741, 2742, 2880, 2985, 2986, 2990, 2991, 2992, 2993, 2994, 2995, 2996
  Reason: Value is constant depending on configuration aspects or platform specific implementation. This leads to constant control expressions, unreachable code or redundant operations.
  Risk: Wrong or missing functionality.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_EmptyFunction:
PRQA message 2987
  Reason: Function is empty depending on configuration aspects and platform specific implementation.
  Risk: Function implementation missing.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_NoneConstParameterPointer:
PRQA message 3673
  Reason: Non-const pointer parameter is required by the internal interface or compatibility reasons but depending on the configuration or specific platform implementation the target may not always be modified.
  Risk: Read only data could be modified without intention.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_ModuleDefine:
PRQA message 0602, 0603
  Reason: Usage of reserved identifiers with leading underscores is accepted for compatibility reasons.
  Risk: Name conflicts.
  Prevention: Compile and link of the different variants in the component and integration test.

MD_Can_RedundantInit:
PRQA message 2981
  Reason: Reduce code complexity by using an explicit variable initializer that may be always modified before being used in some configurations.
  Risk: Unintended change of value.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_GlobalScope:
PRQA message 3218
  Reason: The usage of variables depends on configuration aspects and they may be used only once or defined globally to improve overview.
  Risk: None.
  Prevention: None.

MD_Can_ExternalScope:
PRQA message 1514, 3408, 3447, 3451, 3210
  Reason: The variable is used by other modules and can't be declared static.
  Risk: Name conflicts.
  Prevention: Compile and link of the different variants in the component and integration test.

MD_Can_GenData:
PRQA message 1533
  Reason: These constants are defined in a generated file and cannot be moved to the static source file.
  Risk: None.
  Prevention: None.

MD_Can_Union:
PRQA message 0750, 0759
  Reason: Using union type to handle different data accesses.
  Risk: Misinterpreted data.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_Assertion:
PRQA message 2842
  Reason: Assertion leads to apparent out of bounds indexing.
  Risk: Undefined behaviour.
  Prevention: Code inspection. Assertion itself detects out of bounds indexing.

MD_Can_PointerVoidCast:
PRQA message 0314, 0316
  Reason: API is defined with pointer to void parameter, so pointer has to be casted to or from void.
  Risk: Wrong data access or undefiend behavior for platforms where the byte alignment is not arbitrary.
  Prevention: Code inspection and test with the target compiler/platform in the component test.

MD_Can_PointerCast:
PRQA message 0310
  Reason: Different pointer type is used to access data.
  Risk: Wrong memory is accessed or alignment is incorrect.
  Prevention: Code inspection and test of different variants in the component test.

MD_Can_NoneVolatilePointerCast:
PRQA message 0312
  Reason: Cast to none volatile pointer.
  Risk: Incorrect multiple context access.
  Prevention: Code inspection checks that the value is not multiple accessed.

MD_Can_HwAccess:
PRQA message 0303
  Reason: Hardware access needs cast between a pointer to volatile object and an integral type.
  Risk: Access of unknown memory.
  Prevention: Runtime tests.

MD_Can_MixedSigns:
PRQA message 4393, 4394
  Reason: Casting from signed to unsigned types and vice versa is needed as different types are intentionally used.
  Risk: Value is changed during cast.
  Prevention: Code inspection and test of different variants in the component test.

MD_Can_IntegerCast:
PRQA message 4391, 4398, 4399, 4491
  Reason: Explicit cast to a different integer type.
  Risk: Value is changed during cast.
  Prevention: Code inspection and test of different variants in the component test.

MD_Can_CompilerAbstraction:
PRQA message 0342
  Reason: Glue operator used for compiler abstraction.
  Risk: Only K&R compiler support glue operator.
  Prevention: Compile test show whether compiler accept glue operator.

MD_Can_NoElseAfterIf:
PRQA message 2004
  Reason: No default handling needed for if-else-if here.
  Risk: Missing default handling.
  Prevention: Code inspection and test of different variants in the component test.

MD_Can_IncompleteForLoop:
PRQA message 3418
  Reason: Comma operator in for-loop header is used to get a compact code.
  Risk: Uninitialized variable.
  Prevention: Code inspection and test of different variants in the component test.

MD_Can_HL_UndefTypes:
ID 0841
  Reason: CBD types defined for CAN driver usage but should be not seen in other modules to avoid MSR-CBD mixture problems.
  Risk: None
  Prevention: None

MD_Can_HL_UnusedFunction:
ID 3219
  Reason: depend on platform implementation this function is never called.
  Risk: Function call missing.
  Prevention: Code inspection and component test.
  
MD_Can_HL_UsingIncOrDec:
ID 3387
  Reason: using increment (++) or decrement (--) operator on complex expression.
  Risk: Potential side effects other than that caused by the increment or decrement operator.
  Prevention: Code inspection.
  
MD_Can_HL_DereferencePointerValue:
ID 2843
  Reason: Suspicious: Dereference of an invalid pointer value.
  Risk: Potential dereference access to wrong data.
  Prevention: Code inspection.
  
MD_Can_HL_AdditionalExpressionInForLoop:
ID 2463
  Reason: The variable incremented in the third expression of this for statement is not the variable identified as the loop control variable.
  Risk: Potential side effects.
  Prevention: Code inspection.
  
MD_Can_HL_ExternalDeclaration:
PRQA message 3449, 3451
  Reason: The function or variable is declared as external, because the supporting module is not included or does not declare it depend on configuration aspects.
  Risk: Name conflicts.
  Prevention: Compile and link of the different variants in the component test.

MD_Can_HL_MultipleTypeDeclaration:
PRQA message 1506, 1507, 1508
  Reason: Same Type defined multiple in order to support multiple use cases.
  Risk: Usage of wrong type.
  Prevention: Code inspection.
  

MD_Can_LL_GenericTypeUse:
ID 5209
  Reason: API use explicit generic type and cast is needed here.
  Risk: None
  Prevention: None

*/

/****************************************************************************/
/*  Silent deviations                                                       */
/****************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CAN_HL01
 \DESCRIPTION Write access to CanModeTransitionRequest with controller as index. Controller index is checked against sizeof CanModeTransitionRequest in Can_SetControllerMode(), Can_RamCheckExecute(), Can_MainFunction_Mode(), Can_Init() and CanHL_ReInit().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL02
 \DESCRIPTION Write access to CanLogStatus with controller as index. Controller index is checked against sizeof CanLogStatus in Can_SetControllerMode(), CanHL_ErrorHandling(), Can_RamCheckEnableController(), Can_MainFunction_Mode(), Can_Init() and CanHL_ReInit().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL04
 \DESCRIPTION Write access to CanActiveSendState with activeSendObject as index. activeSendObject is checked against size of ActiveSendObject in Can_CleanUpSendState(), CanHL_WritePrepare(), CanHL_InitTxFullCAN(), CanHL_InitTxBasicCAN(), CanHL_TxTaskCancelationHandling(), CanHL_TxConfirmation() and Can_CancelTx().
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL05
 \DESCRIPTION Write access to CanActiveSendPdu with activeSendObject as index. activeSendObject is checked against size of ActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL06
 \DESCRIPTION Call Can_GenericPrecopy() and CanIf_RxIndication() with parameter RX Queue data pointer. The data pointer is only used for read access and it is always a valid pointer (module local defined).
 \COUNTERMEASURE \R [CM_CAN_HL25]

\ID SBSW_CAN_HL07
 \DESCRIPTION Call Can_GenericPrecopy() and CanIf_RxIndication() with parameter Rx Struct data pointer. The data pointer is only used for read access. Parameter is of type P2CONST in API.
 \COUNTERMEASURE \N [CM_CAN_HL31]

\ID SBSW_CAN_HL08
 \DESCRIPTION Write access to CanIsBusOff with controller as index. Controller index is checked against sizeof CanIsBusOff in Can_SetControllerMode(), CanHL_ErrorHandling() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL09
 \DESCRIPTION Write access to CanIsWakeup with controller as index. Controller index is checked against sizeof CanIsWakeup in CanHL_WakeUpHandling(), Can_CheckWakeup(), reinitialization and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL10
 \DESCRIPTION Write access to VersionInfo pointer within function Can_GetVersionInfo()
 \COUNTERMEASURE \N caller of Can_GetVersionInfo() has to ensure that VersionInfo pointer is valid. (global description available)

\ID SBSW_CAN_HL11
 \DESCRIPTION Write access to CanLastInitObject with controller as index. Controller index is checked against sizeof CanLastInitObject in CanHL_ReInit(), Can_ChangeBaudrate(), Can_SetBaudrate() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL12
 \DESCRIPTION Write access to CanInterruptCounter with controller as index. Controller index is checked against sizeof CanInterruptCounter in Can_Init(), Can_EnableControllerInterrupts() and Can_DisableControllerInterrupts()
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL16
 \DESCRIPTION Call CanLL_CanInterruptDisable() with parameter CanInterruptOldStatus as pointer (compiler secures used type in function declaration fitting to given pointer parameter type).
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL18
 \DESCRIPTION Write access to CanRxQueueBuffer with queue write index. Write index is checked against size of CanRxQueueBuffer.
 \COUNTERMEASURE \R [CM_CAN_HL24]

\ID SBSW_CAN_HL19
 \DESCRIPTION Write access to CanRxQueueBuffer.data with queue write index and data index. Write index is checked against size of CanRxQueueBuffer. (see also generator CM_CAN_HL04 qualified use case CSL02)
 \COUNTERMEASURE \R [CM_CAN_HL24]
                 \R [CM_CAN_HL17]

\ID SBSW_CAN_HL20
 \DESCRIPTION Write access to CanMirrorModeState with controller as index. Controller is checked against size of CanMirrorModeState.
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL22
 \DESCRIPTION Write access to ActiveSendData with activeSendData + length as index. Index is checked against size of table.
 \COUNTERMEASURE \R [CM_CAN_HL20]

\ID SBSW_CAN_HL23
 \DESCRIPTION Write access to ActiveSendObject stuct with activeSendObject as index. activeSendObject is checked against size of CanActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL24
 \DESCRIPTION Call Appl_GenericPreTransmit() with parameter PduInfoPtr_var pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL25
 \DESCRIPTION Call CanIf_CancelTxConfirmation() with parameter CanActiveSendObject.PduInfo[activeSendObject] as pointer with activeSendObject as index. activeSendObject is checked against size of CanActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL26
 \DESCRIPTION Call CanHL_RxBasicMsgReceivedBegin(), CanHL_RxMsgReceivedNotification() and CanHL_RxBasicMsgReceivedEnd() with parameter rxBasicPara pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL27
 \DESCRIPTION Call CanHL_RxFullMsgReceivedBegin(), CanHL_RxFullReceivedNotification() and CanHL_RxFullMsgReceivedEnd() with parameter rxFullPara pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL29
 \DESCRIPTION Write access to rxBasicPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL30
 \DESCRIPTION Write access to rxFullPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL31
 \DESCRIPTION Call Initialization subroutines with initPara pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL32
 \DESCRIPTION Call Appl_GenericConfirmation and CanHL_SetCanPduInfo with canPdu pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL33
 \DESCRIPTION Write access to CanBusOffCounter with controller as index. Controller index is checked against sizeof CanBusOffCounter in Can_SetControllerMode(), CanHL_ErrorHandling() and Can_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL34
 \DESCRIPTION Write access to CanMailboxState with mailbox handle as index. Index secured by ComStackLib [CM_CAN_HL29] and [CM_CAN_HL30] and checked against size of CanMailboxState.
 \COUNTERMEASURE \R [CM_CAN_HL28]

\ID SBSW_CAN_HL35
 \DESCRIPTION Call with pointer to CAN cell register for write and read access.
 \COUNTERMEASURE \N Caller ensures validity of pointer parameter.

\ID SBSW_CAN_HL36
 \DESCRIPTION Write access to initPara structure as pointer (local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL37
 \DESCRIPTION Call GetElapsedValue with timer value pointers as parameter (module local defined).
 \COUNTERMEASURE \N The timer values are static and always valid.

\ID SBSW_CAN_HL38
 \DESCRIPTION Call GetCounterValue with timer value (LoopTimeout) pointers as parameter. Controller index is checked against size of ControllerData, kCanLoopXXX as index in LoopTimeout table is always smaller than the dimension of the table given as static user type in ComStackLib and checked against this value.
 \COUNTERMEASURE \R [CM_CAN_HL01]
                 \R [CM_CAN_HL22]

\ID SBSW_CAN_HL39
 \DESCRIPTION Undefine preprocessor switch to stimulate user defined feature (only test purpose).
 \COUNTERMEASURE \N Redefinement activates a none documented feature.

\ID SBSW_CAN_HL40
 \DESCRIPTION Write access to CanLogStatus with controller as index. Controller is secured by qualified use-case CSL03 of ComStackLib.
 \COUNTERMEASURE \N [CM_CAN_HL02]

\ID SBSW_CAN_HL41
 \DESCRIPTION Write access to CanActiveSendState with activeSendObject as index. activeSendObject is temporary stored in a list of [1,3] elements where each element is initialized to a valid value and always written with size check before.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL42
 \DESCRIPTION Write access to txPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL43
 \DESCRIPTION Write access to taskPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL44
 \DESCRIPTION Call task subroutines with taskPara pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL45
 \DESCRIPTION Call confirmation subroutines with txConfPara pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL46
 \DESCRIPTION Write access to txConfPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL47
 \DESCRIPTION Call cancellation subroutines with txCancellationPara pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL48
 \DESCRIPTION Write access to txCancellationPara (module local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL49
 \DESCRIPTION Call transmit subroutines with txPara pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable)

\ID SBSW_CAN_HL50
 \DESCRIPTION Write access to TxHwFifo member FillCount, WriteIndex and ReadIndex with Mailbox member TxHwFifo as Index. Index is checked against size of TxHwFifo array.
 \COUNTERMEASURE \R [CM_CAN_HL32]

\ID SBSW_CAN_HL51
 \DESCRIPTION Write access to ControllerModePtr pointer within function Can_GetControllerMode()
 \COUNTERMEASURE \N caller of Can_GetControllerMode() has to ensure that ControllerModePtr pointer is valid. (global description available)

\ID SBSW_CAN_HL52
 \DESCRIPTION Write access to ErrorStatePtr pointer within function Can_GetControllerErrorState()
 \COUNTERMEASURE \N caller of Can_GetControllerErrorState() has to ensure that ErrorStatePtr pointer is valid. (global description available)

\ID SBSW_CAN_HL53
 \DESCRIPTION Write access to TxErrorCounterPtr pointer within function Can_GetControllerTxErrorCounter()
 \COUNTERMEASURE \N caller of Can_GetControllerTxErrorCounter() has to ensure that TxErrorCounterPtr pointer is valid. (global description available)

\ID SBSW_CAN_HL54
 \DESCRIPTION Write access to RxErrorCounterPtr pointer within function Can_GetControllerRxErrorCounter()
 \COUNTERMEASURE \N caller of Can_GetControllerRxErrorCounter() has to ensure that RxErrorCounterPtr pointer is valid. (global description available)

\ID SBSW_CAN_HL55
 \DESCRIPTION A ComStackLib generated variable is accessed.
 \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

\ID SBSW_CAN_HL56
 \DESCRIPTION Write access to local canPdu.sdu with canPdu.length as array size.
 \COUNTERMEASURE \R [CM_CAN_HL33]

\ID SBSW_CAN_LL01
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[txObjHandle].
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL02
 \DESCRIPTION Write access to Can_GetgCanController(channel).MsgObj[txObjHandle].mMsg.DataFld.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL03
 \DESCRIPTION Write access to Can_GetRxMsg(channel).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL08
 \DESCRIPTION Write access to Can_GetgCanController(channel).
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL09
 \DESCRIPTION Call CanCell_SendMessage() with parameter txPara member DataFld pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL11
 \DESCRIPTION Call CanLL_RxIndicationMsgCopy() with parameter data pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

\ID SBSW_CAN_LL12
 \DESCRIPTION write access to localInterruptOldFlagPtr data pointer.
 \COUNTERMEASURE \N [CM_CAN_LL01]

SBSW_JUSTIFICATION_END */

/* Counter measurement description

\CM CM_CAN_HL01 A runtime check ensures controller index is always smaller than number of elements in arrays with controller amount as dimension (Qualified use-case CSL01 of ComStackLib size of ControllerData).
\CM CM_CAN_HL02 Qualified use-case CSL03 of ComStackLib (CanMailbox.ControllerConfigIdx is always in range of controller ControllerData)
\CM CM_CAN_HL03 A runtime check ensures activeSendObject index is in valid range for Can_ActiveSendObject table (Qualified use-case CSL01 of ComStackLib - Can_ActiveSendObject size is extended by CAN_MULTIPLEXED_TX_MAX within generator to allow an access to logObjHandle+CAN_MULTIPLEXED_TX_MAX).
\CM CM_CAN_HL17 A runtime check ensures DLC index is always smaller than the dimension of CanRxQueueBuffer.data.
\CM CM_CAN_HL18 A runtime check ensures HRH values is always in range of start stop index of the given mailbox type. (Qualified use-case CSL03 of ComStackLib - start-stop index fit to mailbox entrys)
\CM CM_CAN_HL20 A runtime check ensures size of ActiveSendData (dataByte). For CAN-FD it is checked against the size of the table. For none CAN-FD size is checked against 8 as defined.
\CM CM_CAN_HL22 A runtime check ensures source index for LoopTimeout array is always smaller than the dimension of LoopTimeout.
\CM CM_CAN_HL24 A runtime check ensures RxQueue write index is always smaller than the dimension of RxQueue (Qualified use-case CSL01 of ComStackLib).
\CM CM_CAN_HL25 A runtime check ensures RxQueue read index is always smaller than the dimension of RxQueue (Qualified use-case CSL01 of ComStackLib).
\CM CM_CAN_HL28 A runtime check ensures htrh index is always smaller than the dimension of CanMailboxState (Qualified use-case CSL01 of ComStackLib).
\CM CM_CAN_HL29 Qualified use-case CSL02 of ComStackLib (size of CanMailboxState == size of CanMailbox) 
\CM CM_CAN_HL30 Qualified use-case CSL03 of ComStackLib (CanMailbox - CanController Start/Stop HtrhIndex).
\CM CM_CAN_HL31 valid mailbox data area is assigned to rxStruct.pChipData pointer (Has to be given by LL implementation part).
\CM CM_CAN_HL32 A runtime check ensures TxHwFifo index is always smaller than the dimension of TxHwFifo (Qualified use-case CSL01 of ComStackLib).
\CM CM_CAN_HL33 canPdu.length is limited to size of canPdu.sdu.

\CM CM_CAN_LL01 CANoeEmu is not Silent tested.

*/

/****************************************************************************/
/*  Coverage Justification                                                  */
/****************************************************************************/
/* START_COVERAGE_JUSTIFICATION

\ID COV_CAN_COMPATIBILITY
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX xf xf
  \ACCEPT XF tf tf
  \REASON The condition checks whether a switch is defined and conditionally assigns a default value. This is used to ensure compatibility to older AUTOSAR/Component versions. 

\ID COV_CAN_INTERNAL
  \ACCEPT XF
  \ACCEPT TX
  \REASON This switch is always active within CAN driver context. Secure visibility of internal interface and cannot be stimulated to off while test. The code is verified by code inspection.

\ID COV_CAN_CANCEL_SW_STIMULATION
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT XX
  \ACCEPT TF tf xf
  \REASON It is not possible to stimulate a sw cancellation while the message is already start to send when hw cancellation is active. The code is verified by code inspection.

\ID COV_CAN_AMD_RUNTIME_MEASUREMENT
  \ACCEPT XF
  \REASON The feature AMD Runtime Measurement is not supported for SafeBSW. A MSSV plugin enforces this configuration for SafeBSW.

\ID COV_CAN_FINALMAGICNUMBER
  \ACCEPT TX
  \REASON Switch is always true is OK here no side effect will occur when switch is false verified by review.

\ID COV_CAN_SAFE_BSW_EXCLUSION
  \ACCEPT XF xf xf
  \REASON SafeBsw restricted feature will not be enforced to be deactivated or activated.

\ID COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG
  \ACCEPT X
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TF tf tx
  \ACCEPT TX tx tx
  \REASON BUSOFF not stimulated in each configruation. The behavior does not depend on each configuration variant but TCASE-274 takes long runtime and will not be executed for each configuration.   

\ID COV_CAN_WAKEUP_CHANNEL_NOT_IN_ALL_CONFIG
  \ACCEPT XF
  \ACCEPT TX
  \REASON Wakeup may be always activated or deactivated depend on configuration. Not all configurations contain activated or deactivated wakeup for at least one controller, but is sufficiantly tested in some configuations with both settings.

\ID COV_IMPLICITE_TXCANCEL
  \ACCEPT TX
  \REASON In some configurations there will be no recancellation but for this case there is no active code (empty else), so this is not really a test case.

\ID COV_CAN_TX_CONF_DATA_FAILURE
  \ACCEPT XF
  \REASON Hard to stimulate invalid active send data against configured object length. The code is verified by code inspection.

---------------- LL ------------------

\ID COV_CAN_LL_MODE_CHANGE
  \ACCEPT TX
  \ACCEPT XF
  \REASON CANoeEmu only emulated hardware loops for test purpose. CANoeEmu change states ASAP.

\ID COV_CAN_LL_HIGHVOLTAGE
  \ACCEPT XF
  \REASON Currently explicit not tested in CANoeEmu. Special hardware not available.

\ID COV_CAN_LL_RECEIVE_LOOP
  \ACCEPT XF
  \REASON CANoeEmu only emulated receive loops. No real inconsistency will occur.

\ID COV_CAN_LL_EMULATION
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX tx tx
  \ACCEPT TX tf tx
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \ACCEPT TF tx tf
  \ACCEPT TF tf tx
  \ACCEPT TF tf tx tf
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tx tx
  \ACCEPT TX tx tx tx
  \ACCEPT TF tf tx tf tf
  \REASON CANoeEmu only. no Safe requirements.

\ID COV_CAN_LL_ISR_USAGE
  \ACCEPT X
  \ACCEPT TF
  \ACCEPT TX
  \ACCEPT XF
  \REASON not all ISRs used.

\ID COV_CAN_LL_OS_USAGE
  \ACCEPT TF
  \ACCEPT XF
  \REASON not all ISRs used.

\ID COV_CAN_MULTIPLE_BASICCAN_TX
  \ACCEPT TF
  \ACCEPT TF tf tf
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_SLEEP_SUPPORT
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_WAKEUP_SUPPORT
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TX_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_BASICCAN_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_FULLCAN_POLLING
  \ACCEPT TF
  \REASON The feature configuration specific - fully supported.

\ID COV_CAN_ERROR_POLLING
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_WAKEUP_POLLING
  \ACCEPT XF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_INDIVIDUAL_POLLING
  \ACCEPT TF
  \REASON The feature is platform and Project specific.

\ID COV_CAN_TX_POLLING_OR_CANCEL_IN_HW_TASK
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tf xf
  \REASON The feature is platform specific - C_HL_ENABLE_CANCEL_IN_HW_TASK is always true or false.

\ID COV_CAN_GENERIC_CONFIRMATION_API2
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_FD_SUPPORT
  \ACCEPT XF
  \REASON The feature is platform and Project specific - always off.

\ID COV_CAN_MULTIPLE_BASICCAN
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_TX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HW_TRANSMIT_CANCELLATION
  \ACCEPT TF
  \ACCEPT TX
  \REASON The feature is platform specific - only emulated.

\ID COV_CAN_CANCEL_IN_HW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HARDWARE_CANCELLATION
  \ACCEPT XF
  \ACCEPT TF
  \REASON The feature is platform specific - always off.

\ID COV_CAN_EXTENDED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MIXED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_OVERRUN_NOTIFICATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MULTIPLEXED_TRANSMISSION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TX_HW_FIFO
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_QUEUE
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HW_LAYOUT_TXBASIC_FIRST
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_TX_FULLCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_RX_FULLCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_BASICCAN_HWHANDLE_REVERSE
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_EXTENDED_STATUS
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_OVERRUN_IN_STATUS
  \ACCEPT XF
  \REASON The feature is platform specific - always deactivated.

\ID COV_CAN_RxBasicMsgReceivedBegin
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_RxFullMsgReceivedBegin
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_HW_EXIT_TRANSMIT
  \ACCEPT TX
  \REASON The feature is platform specific - always active.

\ID COV_CAN_HARDWARE_FAILURE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT XF tf xf
  \ACCEPT TF tf xf
  \ACCEPT TF tf tx
  \REASON The condition checks for hardware failures that cannot be stimulated. The code is verified by code inspection.

\ID COV_CAN_LL_HARDWARE_BEHAVIOUR
  \ACCEPT TX
  \ACCEPT XF
  \REASON The condition checks for hardware depended return value. The return value is always true/false because hardware never/ever fail here. The code is verified by code inspection.

\ID COV_CAN_GENDATA_FAILURE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT TF xf tf
  \ACCEPT TF tf xf
  \ACCEPT TF tf xf tf
  \ACCEPT TF xf tf xf tf
  \REASON The condition checks for generated data failures that cannot be simulated. The code is verified by code inspection.

\ID COV_CAN_GENDATA_NOT_IN_ALL_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT X
  \REASON Generated data does not contain all data sets but checked in code. Not encapsulated with preprocessor switch to simplify code.  

\ID COV_CAN_OS_USAGE
  \ACCEPT TX
  \ACCEPT TX xf tx
  \ACCEPT TX tx xf
  \ACCEPT XF
  \ACCEPT XF xf tx
  \REASON Os may be used in component test or not. The code is verified by code inspection.

\ID COV_CAN_LOCK_ISR_BY_APPL
  \ACCEPT TX tf tf
  \ACCEPT TF xf tf 
  \REASON CAN interrupts never locked by application only within tests. It is not part of the component test to check application code. The code is verified by code inspection.

\ID COV_CAN_RAM_CHECK
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RAM_CHECK_EXTENDED
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TIMEOUT_DURATION
  \ACCEPT TF
  \REASON Hardware loop time out is platform specific never reached.

\ID COV_CAN_TRANSITION_REQUEST
  \ACCEPT X
  \ACCEPT TX
  \ACCEPT XF
  \REASON Mode transition is platform specific always true or false.

\ID COV_CAN_BASICCAN_SEARCH_LINEAR
  \ACCEPT TX
  \ACCEPT TF tx tf
  \ACCEPT TX tx tx
  \REASON Received BasicCAN mailbox is always a valid object and found while search.

\ID COV_CAN_FULLCAN_SEARCH_LINEAR
  \ACCEPT TX
  \ACCEPT TF tx tf
  \REASON Received FullCAN mailbox is always a valid object and found while search.

\ID COV_CAN_RAM_CHECK_NO_STIMULATION
  \ACCEPT X
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \REASON some configurations do not stimulate RamCheck by test suite, to check the standard CAN driver behaviour.

\ID COV_CAN_SAFE_BSW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HWOBJINDIVPOLLING
  \ACCEPT TX tf xf
  \ACCEPT TX
  \ACCEPT XF
  \REASON usage of individual hardware object polling is platform specific always true or false.

\ID COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF tf tf tx tf
  \ACCEPT TF tx tf tx tf
  \ACCEPT TF tf tx tx tf
  \ACCEPT TF tx tx tf tf
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tx tx
  \ACCEPT TF tx tx tf
  \REASON some configurations do use all types of TX mailbox objects (like FIFO or MUX).

END_COVERAGE_JUSTIFICATION */ 

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

