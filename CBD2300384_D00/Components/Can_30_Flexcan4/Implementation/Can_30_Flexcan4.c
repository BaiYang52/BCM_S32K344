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

/* \trace SPEC-1570 */
/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
/* ECO_IGNORE_BLOCK_BEGIN */
#define C_DRV_30_FLEXCAN4_INTERNAL /* compatibility */
#define CAN_30_FLEXCAN4_SOURCE     /* testability */
#define __CAN_SRC__       /* compatibility RTM not aware of multi driver instance */ /* PRQA S 0602,0603 */ /* MD_Can_ModuleDefine */
#if !defined(__CAN_30_FLEXCAN4_SRC__) /* driver use ORGANI without infix */ /* COV_CAN_INTERNAL */
# define __CAN_30_FLEXCAN4_SRC__      /* for all MSR module (used by RTM) */ /* PRQA S 0602,0603 */ /* MD_Can_ModuleDefine */
#endif

/* \trace SPEC-1408, SPEC-1590, SPEC-1588 */

/* \trace SPEC-1392 */
#include "CanIf_Cbk.h"   /* for callback declaration */
/* \trace SPEC-1707 */
#include "CanIf.h"       /* for version check */
#include "Can_30_Flexcan4_Local.h"    /* have to be last include due to channel define will lead to problems */


#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
/* \trace SPEC-1596 */
# include "Det.h"
#endif

#if !defined(CAN_30_FLEXCAN4_AMD_RUNTIME_MEASUREMENT) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_AMD_RUNTIME_MEASUREMENT STD_OFF
#endif
#if (CAN_30_FLEXCAN4_AMD_RUNTIME_MEASUREMENT == STD_ON) /* COV_CAN_AMD_RUNTIME_MEASUREMENT */
# include "AmdRtm.h"
#endif

#if !defined(CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT STD_OFF
#endif
#if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
/* AR4-328 */
# define C_30_FLEXCAN4_ENABLE_RUNTIME_MEASUREMENT_SUPPORT
# include "Rtm.h"
#endif

#include "vstdlib.h"

/* ECO_IGNORE_BLOCK_END */

/***************************************************************************/
/* Version Check                                                           */
/***************************************************************************/
/* \trace SPEC-20329 */
/* not the SW version but all file versions that represent the SW version are checked */
#if (CAN_30_FLEXCAN4_COREVERSION           != 0x1000u) /* \trace SPEC-1699 */
# error "Source and Header file are inconsistent!"
#endif
#if (CAN_30_FLEXCAN4_RELEASE_COREVERSION   != 0x03u)
# error "Source and Header file are inconsistent!"
#endif
#if defined(CAN_30_FLEXCAN4_GEN_BASE_CFG5_VERSION) /* COV_CAN_COMPATIBILITY */
# if (CAN_30_FLEXCAN4_GEN_BASE_CFG5_VERSION         != 0x0103u)
#  error "Source and Generated Header file are inconsistent!"
# endif
#else
# error "No CAN_30_FLEXCAN4_GEN_BASE_CFG5_VERSION is defined"
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
/* instead of direct canCanInterruptCounter[] use Can_30_Flexcan4_GetCanInterruptCounter(Index) abstraction for ComStackLib */

#if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
# define CanHookBegin_Can_30_Flexcan4_Init()                              Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_Init)
# define CanHookEnd_Can_30_Flexcan4_Init()                                Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_Init)
# define CanHookBegin_Can_30_Flexcan4_SetControllerMode()                 Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_SetControllerMode)
# define CanHookEnd_Can_30_Flexcan4_SetControllerMode()                   Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_SetControllerMode)
# define CanHookBegin_Can_30_Flexcan4_Write()                             Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_Write)
# define CanHookEnd_Can_30_Flexcan4_Write()                               Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4_Write)

#else /* BRS runtime measurement */
# define CanHookBegin_Can_30_Flexcan4_Init()
# define CanHookEnd_Can_30_Flexcan4_Init()
# define CanHookBegin_Can_30_Flexcan4_SetControllerMode() \

# define CanHookEnd_Can_30_Flexcan4_SetControllerMode() \

# define CanHookBegin_Can_30_Flexcan4_Write() \

# define CanHookEnd_Can_30_Flexcan4_Write() \

#endif

# define CanHookBegin_CanHL_30_Flexcan4_ReInit() \

# define CanHookEnd_CanHL_30_Flexcan4_ReInit() \

#define CanHookBegin_CanHL_30_Flexcan4_TxConfirmation() \

#define CanHookEnd_CanHL_30_Flexcan4_TxConfirmation() \

#define CanHookBegin_CanHL_30_Flexcan4_BasicCanMsgReceived() \

#define CanHookEnd_CanHL_30_Flexcan4_BasicCanMsgReceived() \

#define CanHookBegin_CanHL_30_Flexcan4_FullCanMsgReceived() \

#define CanHookEnd_CanHL_30_Flexcan4_FullCanMsgReceived() \

#define CanHookBegin_Can_30_Flexcan4_MainFunction_Write() \

#define CanHookEnd_Can_30_Flexcan4_MainFunction_Write() \

#define CanHookBegin_Can_30_Flexcan4_MainFunction_Read() \

#define CanHookEnd_Can_30_Flexcan4_MainFunction_Read() \

/* PRQA S 0303,0306,0310,3305 EOF */ /* MD_Can_HwAccess,MD_Can_LL_HwAccess,MD_Can_PointerCast,MD_Can_3305_LL_MsgObjectAccess */
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
/* Define chiphardware                     */
/* Constants concerning can chip registers */
/* Bitmasks of FlexCAN module configuration register CANx_MCR */
#define kFlexCAN_30_Flexcan4_MDIS      ((vuint16)0x8000u)       /*!< Module Disable: shut down FlexCAN clocks */
#define kFlexCAN_30_Flexcan4_FRZ       ((vuint16)0x4000u)       /*!< Freeze Enable: allow entering the freeze mode */
#define kFlexCAN_30_Flexcan4_FEN       ((vuint16)0x2000u)       /*!< FIFO enable */
#define kFlexCAN_30_Flexcan4_HALT      ((vuint16)0x1000u)       /*!< Halt FlexCAN: request entering the freeze mode */
#define kFlexCAN_30_Flexcan4_NOT_RDY   ((vuint16)0x0800u)       /*!< Not Ready: FlexCAN is in DISABLE, DOZE or STOP mode */
#define kFlexCAN_30_Flexcan4_WAK_MSK   ((vuint16)0x0400u)       /*!< Wakeup Interrupt Mask: enable wakeup interrupt generation */
#define kFlexCAN_30_Flexcan4_SOFT_RST  ((vuint16)0x0200u)       /*!< Soft Reset: reset FlexCAN internal state and some memory mapped registers */
#define kFlexCAN_30_Flexcan4_FRZ_ACK   ((vuint16)0x0100u)       /*!< Freeze Mode Acknowledge */
#define kFlexCAN_30_Flexcan4_SUPV      ((vuint16)0x0080u)       /*!< Supervisor Mode */
#define kFlexCAN_30_Flexcan4_SLF_WAK   ((vuint16)0x0040u)       /*!< FlexCAN Self Wakeup by CAN bus activity */
#define kFlexCAN_30_Flexcan4_WRN_EN    ((vuint16)0x0020u)       /*!< Warning Interrupt Enable */
#define kFlexCAN_30_Flexcan4_LPM_ACK   ((vuint16)0x0010u)       /*!< Low Power Mode Acknowledge */
#define kFlexCAN_30_Flexcan4_WAK_SRC   ((vuint16)0x0008u)       /*!< Wakeup Source: enable filtered Rx input signal */
#define kFlexCAN_30_Flexcan4_DOZE      ((vuint16)0x0004u)       /*!< Doze Mode Enable: allow MCU to switch FlexCAN into DOZE low power mode */
#define kFlexCAN_30_Flexcan4_NOT_DOZE  ((vuint16)0xFFFBu)       /*!< Doze Mode Disable: do not allow MCU to switch FlexCAN into DOZE low power mode */
#define kFlexCAN_30_Flexcan4_SRX_DIS   ((vuint16)0x0002u)       /*!< Self Reception Disable */
#define kFlexCAN_30_Flexcan4_IRMQ      ((vuint16)0x0001u)       /*!< Backwards Compatibility Configuration: enable Individual Rx Masks and queue feature */

#define kFlexCAN_30_Flexcan4_LPPRIO_EN ((vuint16)0x2000u)       /*!< Local Priority Enable */
#define kFlexCAN_30_Flexcan4_AEN       ((vuint16)0x1000u)       /*!< Abort Enable: enables transmit abort feature */
#define kFlexCAN_30_Flexcan4_FDEN      ((vuint16)0x0800u)       /*!< CAN FD operation enable */
#define kFlexCAN_30_Flexcan4_IDAM      ((vuint16)0x0300u)       /*!< ID Acceptance Mode */
#define kFlexCAN_30_Flexcan4_MAXMB     ((vuint16)0x003Fu)       /*!< Maximum Number of Message Buffers: maximum number of used message buffers = MAXMB+1 */

/* Bitmasks of FlexCAN module configuration register CANx_CTRL1 */
#define kFlexCAN_30_Flexcan4_BOFF_MSK  ((vuint32)0x00008000u)   /*!< FlexCAN BusOff interrupt mask */
#define kFlexCAN_30_Flexcan4_ERR_MSK   ((vuint32)0x00004000u)   /*!< FlexCAN Error interrupt mask */
#define kFlexCAN_30_Flexcan4_LOM       ((vuint32)0x00000008u)   /*!< FlexCAN Listen Only Mode */
#define kFlexCAN_30_Flexcan4_CLK_SRC   ((vuint32)0x00002000u)   /*!< FlexCAN clock source mask */
#define kFlexCAN_30_Flexcan4_BOFF_REC  ((vuint32)0x00000040u)   /*!< FlexCAN auto BusOff recovery */
#define kFlexCAN_30_Flexcan4_SMP       ((vuint32)0x00000080u)   /*!< FlexCAN sampling mode */
#define kNot_30_Flexcan4FlexCANErrBoff ((vuint32)0xFFFF3FFFu)

/* Bitmasks of FlexCAN module configuration register CANx_ESR */
#define kFlexCAN_30_Flexcan4_WAKE_INT   (vuint32)0x00000001u    /*!< Wake Up interrupt flag */
#define kFlexCAN_30_Flexcan4_ERR_INT    (vuint32)0x00000002u    /*!< Error interrupt flag */
#define kFlexCAN_30_Flexcan4_BOFF_INT   (vuint32)0x00000004u    /*!< Bus Off interrupt flag */
#define kFlexCAN_30_Flexcan4_FCS_BOFF   (vuint32)0x00000020u    /*!< Fault Confinement State value for BusOff */
#define kFlexCAN_30_Flexcan4_FCS_EP     (vuint32)0x00000010u    /*!< Fault Confinement State value for ErrorPassive */
#define kFlexCAN_30_Flexcan4_FCS        (vuint32)0x00000030u    /*!< Fault Confinement State */
#define kFlexCAN_30_Flexcan4_RXTX_WARN  (vuint32)0x00000300u    /*!< RX/TX warning level reached */
#define kFlexCAN_30_Flexcan4_STFERR     (vuint32)0x00000400u    /*!< Stuffing error */
#define kFlexCAN_30_Flexcan4_FRMERR     (vuint32)0x00000800u    /*|< Form error */
#define kFlexCAN_30_Flexcan4_CRCERR     (vuint32)0x00001000u    /*!< CRC error */
#define kFlexCAN_30_Flexcan4_ACKERR     (vuint32)0x00002000u    /*!< ACK error */
#define kFlexCAN_30_Flexcan4_BIT0ERR    (vuint32)0x00004000u    /*!< dominant bit sent as recessive received */
#define kFlexCAN_30_Flexcan4_BIT1ERR    (vuint32)0x00008000u    /*!< recessive bit sent as dominant received */
#define kFlexCAN_30_Flexcan4_OVRERR     (vuint32)0x00200000u    /*!< Error overrun */
#define kFlexCAN_30_Flexcan4_STATUS_INT (kFlexCAN_30_Flexcan4_OVRERR | kFlexCAN_30_Flexcan4_BOFF_INT | kFlexCAN_30_Flexcan4_ERR_INT | kFlexCAN_30_Flexcan4_WAKE_INT) /*!< represents status error interrupt */
#define kFlexCAN_30_Flexcan4_ERR_INT_CLEAR (kFlexCAN_30_Flexcan4_OVRERR | kFlexCAN_30_Flexcan4_ERR_INT)

#define kFlexCAN_30_Flexcan4_MCR_REQ_ONLY_BITS (kFlexCAN_30_Flexcan4_MDIS | kFlexCAN_30_Flexcan4_FRZ | kFlexCAN_30_Flexcan4_HALT)
#define kFlexCAN_30_Flexcan4_MCR               (kFlexCAN_30_Flexcan4_MDIS | kFlexCAN_30_Flexcan4_FRZ | kFlexCAN_30_Flexcan4_HALT | kFlexCAN_30_Flexcan4_NOT_RDY | kFlexCAN_30_Flexcan4_FRZ_ACK | kFlexCAN_30_Flexcan4_LPM_ACK)
#define kFlexCAN_30_Flexcan4_FREEZE_MODE       (kFlexCAN_30_Flexcan4_FRZ | kFlexCAN_30_Flexcan4_HALT | kFlexCAN_30_Flexcan4_NOT_RDY | kFlexCAN_30_Flexcan4_FRZ_ACK) /* FlexCAN in FREEZE mode: FRZ, HALT, NOT_RDY and FRZ_ACK bit are set */
#define kFlexCAN_30_Flexcan4_FREEZE_MODE_REQ   (kFlexCAN_30_Flexcan4_FRZ | kFlexCAN_30_Flexcan4_HALT)
#define kFlexCAN_30_Flexcan4_STOP_MODE         (kFlexCAN_30_Flexcan4_NOT_RDY | kFlexCAN_30_Flexcan4_LPM_ACK) /* FlexCAN in STOP powerdown mode: NOT_RDY and LPM_ACK bits set */
#define kFlexCAN_30_Flexcan4_DISABLE_MODE      (kFlexCAN_30_Flexcan4_MDIS | kFlexCAN_30_Flexcan4_NOT_RDY | kFlexCAN_30_Flexcan4_LPM_ACK) /* FlexCAN in DISABLE powerdown mode: MDIS, NOT_RDY and LPM_ACK bit are set */
#define kFlexCAN_30_Flexcan4_NORMAL_MODE       ((vuint16)0x0000u) /* relevant bits for mode states must all be negated for NORMAL mode */
#define kFlexCAN_30_Flexcan4_NORMAL_MODE_REQ   ((vuint16)0x0000u)
/* FlexCAN is DISABLED, independent from FREEZE mode:
  -> MDIS and LPM_ACK bit are necessary to identify DISABLE mode
  -> NOT_RDY must not be checked because it also influenced by FREEZE mode 
*/
#define kFlexCAN_30_Flexcan4_DISABLE_ONLY_BITS (kFlexCAN_30_Flexcan4_MDIS | kFlexCAN_30_Flexcan4_LPM_ACK)
#define kNotFlexCAN_30_Flexcan4_MCR_CFG_BTIS   (kFlexCAN_30_Flexcan4_MCR | kFlexCAN_30_Flexcan4_SOFT_RST | kFlexCAN_30_Flexcan4_SUPV) /* user configurable bits are '0', not changeable bits are '1' */

#define kFlexCAN_30_Flexcan4_SFR_CLEAR  ((vuint32)0x00000000u)
#define kFlexCAN_30_Flexcan4_SFR_SET    ((vuint32)0xFFFFFFFFu)

/* Macros to access the CODE-bits in the control/status word ------------------*/
#define kCode_30_Flexcan4_Mask         ((vuint16)0x0F00u)     /*!< Mask to access the CODE in the control/status word */
#define kNot_30_Flexcan4DlcMask       ((vuint16)0xFF00u)
#define kNot_30_Flexcan4CodeMask      ((vuint16)0xF0FFu)

/* Code definitions for receive objects */
#define kRx_30_Flexcan4CodeEmpty      ((vuint16)0x0400u)    /*!< Message buffer is active and empty */
#define kRx_30_Flexcan4CodeClear      ((vuint16)0x00F0u)    /*!< Mask to clear control register but leave the ID type */
#define kRx_30_Flexcan4CodeOverrun    ((vuint16)0x0600u)    /*!< Second frame was received into a full buffer */
#define kRx_30_Flexcan4CodeBusy       ((vuint16)0x0100u)    /*!< Receive buffer locked */

/* Code definitions for transmit objects */
#define kTx_30_Flexcan4CodeTransmit     ((vuint16)0x0C40u)     /*!< transmit request in nominal bit rate  */
#define kTx_30_Flexcan4CodeTransmitFD   ((vuint16)0xCC40u)     /*!< transmit request for CAN-FD: EDL (extended data length) and BRS (bit rate switch) bit must be set */
#define kNot_30_Flexcan4IDEMask         ((vuint16)0xFF0Fu)
#define kTx_30_Flexcan4DlcMask          ((vuint16)0x006Fu)     /*!< Mask to access the DLC in the control/status word */
#define kTx_30_Flexcan4CodeFree         ((vuint16)0x0800u)     /*!< Transmit object free */
#define kTx_30_Flexcan4CodeAbort        ((vuint16)0x0900u)     /*!< Abort message transmission */

#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define kTx_30_Flexcan4CodeInactive  ((vuint16)0x0820u)       /*!< Transmit object inactive for extended or mixed IDs */
#else
# define kTx_30_Flexcan4CodeInactive  ((vuint16)0x0800u)       /*!< Transmit object inactive for standard IDs */
#endif

/* Code definitions for Rx FiFo ---------------------------- */
#define kRx_30_Flexcan4FIFO_OVERRUN     (vuint32)0x00000080u  /*!< RxFIFO overrun (iflag1) */
#define kRx_30_Flexcan4FIFO_WARN        (vuint32)0x00000040u  /*!< RxFIFO warning (iflag1) */
#define kRx_30_Flexcan4FIFO_NEWMSG      (vuint32)0x00000020u  /*!< RxFIFO new message available (iflag1) */
#define kRx_30_Flexcan4FIFO_EXT         (vuint32)0x40000000u  /*!< RxFIFO extended ID (ID tab element) */
#define kRx_30_Flexcan4FIFO_REM         (vuint32)0x80000000u  /*!< RxFIFO remote frame (ID tab element) */
#define kRx_30_Flexcan4FIFO_MASK        (vuint32)0xC0000000u  /*!< RxFIFO mask for ID tab element */

# define C_30_FLEXCAN4_FLEXCAN_RXFIFO_MAXLOOP  6u  /*!< 6 RX Fifo elements available */

/* Code definitions for ctrl2 register ------------------- */
#define kFlexCAN_30_Flexcan4_ECRWRE         (vuint32)0x20000000u  /*!< Enables write access to MECR register */
#define kFlexCAN_30_Flexcan4_MRP            (vuint32)0x00040000u  /*!< Matching process starts from mailboxes and continues on Rx FIFO */
#define kFlexCAN_30_Flexcan4_RRS            (vuint32)0x00020000u  /*!< Remote request frame is stored */
#define kFlexCAN_30_Flexcan4_EACEN          (vuint32)0x00010000u  /*!< Enables the comparison of IDE and RTR bit */
#define kFlexCAN_30_Flexcan4_TASD_DEFAULT   (vuint32)0x00800000u  /*!< TASD default value */
#define kFlexCAN_30_Flexcan4_BTE            (vuint32)0x00002000u  /*!< Enables enhanced bit timing */
#define kNotFlexCAN_30_Flexcan4_BTE         (vuint32)0xFFFFDFFFu  /*!< Disables enhanced bit timing */
#define kFlexCAN_30_Flexcan4_STFCNTEN       (vuint32)0x00001000u  /*!< Enables ISO CAN FD */

/* Code definitions for cbt register ------------------- */
#define kNotFlexCAN_30_Flexcan4_BTF         (vuint32)0x7FFFFFFFu  /*!< Disables extended bit timing */

/* Code definitions for mecr register ------------------- */
#define kFlexCAN_30_Flexcan4_ECRWRDIS       (vuint32)0x80000000u  /*!< Disable MECR write */
#define kFlexCAN_30_Flexcan4_RERRDIS        (vuint32)0x00000200u  /*!< Disable Error Report */
#define kFlexCAN_30_Flexcan4_ECCDIS         (vuint32)0x00000100u  /*!< Disable Error Correction */
#define kFlexCAN_30_Flexcan4_Disable_ParityCheck (kFlexCAN_30_Flexcan4_RERRDIS | kFlexCAN_30_Flexcan4_ECCDIS) /* Disable Parity Check */
#define kFlexCAN_30_Flexcan4_Clear_Parity_Status (vuint32)0x000D000Du /* Clear Status register */

#define kExtIDBit               (vuint32)0x80000000u

/* Code definitions for erfcr register */
#define kFlexCAN_30_Flexcan4_ERFEN          (vuint32)0x80000000u   /*!< Enhanced Rx Fifo enable */
#define kFlexCAN_30_Flexcan4_ERFEN_DISABLE  (vuint32)0x00000000u   /*!< Enhanced Rx Fifo disable */

/* Code definitions for erfier register */
#define kFlexCAN_30_Flexcan4_ERFUFWIE       (vuint32)0x80000000u   /*!< Enhanced Rx Fifo underflow interrupt enable */
#define kFlexCAN_30_Flexcan4_ERFOVFIE       (vuint32)0x40000000u   /*!< Enhanced Rx Fifo overflow interrupt enable */
#define kFlexCAN_30_Flexcan4_ERWFMIIE       (vuint32)0x20000000u   /*!< Enhanced Rx Fifo watermark indication interrupt enable */
#define kFlexCAN_30_Flexcan4_ERFDAIE        (vuint32)0x10000000u   /*!< Enhanced Rx Fifo data available interrupt enable */

/* Code definitions for erfsr register */
#define kFlexCAN_30_Flexcan4_ERFUFW         (vuint32)0x80000000u   /*!< Enhanced Rx Fifo underflow */
#define kFlexCAN_30_Flexcan4_ERFOVF         (vuint32)0x40000000u   /*!< Enhanced Rx Fifo overflow */
#define kFlexCAN_30_Flexcan4_ERWFMI         (vuint32)0x20000000u   /*!< Enhanced Rx Fifo watermark indication */
#define kFlexCAN_30_Flexcan4_ERFDA          (vuint32)0x10000000u   /*!< Enhanced Rx Fifo data available */
#define kFlexCAN_30_Flexcan4_ERFCLR         (vuint32)0x08000000u   /*!< Enhanced Rx Fifo clear */
#define kFlexCAN_30_Flexcan4_ERFE           (vuint32)0x00020000u   /*!< Enhanced Rx Fifo empty */
#define kFlexCAN_30_Flexcan4_ERFF           (vuint32)0x00010000u   /*!< Enhanced Rx Fifo full */
#define kFlexCAN_30_Flexcan4_ERFEL_MASK     (vuint32)0x0000003Fu   /*!< Enhanced Rx Fifo number of stored messages */
#define kFlexCAN_30_Flexcan4_ERF_CLEAR_STATUS (kFlexCAN_30_Flexcan4_ERFUFW | kFlexCAN_30_Flexcan4_ERFOVF | kFlexCAN_30_Flexcan4_ERWFMI | kFlexCAN_30_Flexcan4_ERFDA)



#define kCan_30_Flexcan4RxMaskStd     ((vuint32)0xDFFC0000u)
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
#  define kCan_30_Flexcan4RxMaskExt   ((vuint32)0xDFFFFFFFu)
#endif

#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
# define CAN_30_FLEXCAN4_MSGID(x) (vuint32)(x) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define CAN_30_FLEXCAN4_MSGID(x) ((vuint32)(x) << 16) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

#define kCan_30_Flexcan4RxFifoIntUnmask   ((vuint32)0xFFFFFF00u)

#define kCan_30_Flexcan4MaskAll32 0xFFFFFFFFu
#define kCan_30_Flexcan4MaskAll16 0xFFFFu

/* all bits of CAN[MCR].MCR except mode and reserved bits */
#if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_USER_MODE )
# define kCan_30_Flexcan4RamCheckMaskMCR         0x20A3u
#else
# define kCan_30_Flexcan4RamCheckMaskMCR         0x2023u
#endif

#if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) || defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
# define kCan_30_Flexcan4MaxmbMask                  0x007Fu
#else
# define kCan_30_Flexcan4MaxmbMask                  0x003Fu
#endif

/* all bits of CAN[MCR].MAXMB except unused/reserved bits */  
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
# define kCan_30_Flexcan4RamCheckMaskMAXMB      (0x3300u | kFlexCAN_30_Flexcan4_FDEN | kCan_30_Flexcan4MaxmbMask)
#else
# define kCan_30_Flexcan4RamCheckMaskMAXMB      (0x3300u | kCan_30_Flexcan4MaxmbMask)
#endif

#define kCan_30_Flexcan4RamCheckMailboxControl  0x0F7Fu

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )

/* mask for CAN_30_FLEXCAN4_CTRL1 register check */
#  define kCan_30_Flexcan4RamCheckMaskCONTROL1             0x0000DCF8u

/* mask for CAN_30_FLEXCAN4_ECR register check */
# define kCan_30_Flexcan4RamCheckMaskECR                   0x0000FFFFu

/* mask for CAN_30_FLEXCAN4_CBT register check */
# define kCan_30_Flexcan4RamCheckMaskCBT                   0xFFEFFFFFu


# if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
/* mask for CAN_30_FLEXCAN4_CTRL2 register check */
#  define kCan_30_Flexcan4RamCheckMaskCTRL2                0x3FFF0000u
/* mask for CAN_30_FLEXCAN4_MECR register check */
#  define kCan_30_Flexcan4RamCheckMaskMECR                 0x0001E380u
# else
/* mask for CAN_30_FLEXCAN4_CTRL2 register check */
#  define kCan_30_Flexcan4RamCheckMaskCTRL2                0x0FFF0000u
# endif

/* mask for CAN_30_FLEXCAN4_EFifoFilter chseck */
# define kCan_30_Flexcan4RamCheckMaskEFifoFilter           0x0FFF0FFFu

/* configuration check */
# if !defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )
#  error "C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK has to be defined if C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED is set."
# endif

#endif /* C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED */

#define Can_30_Flexcan4Bswap32(x)                   (vuint32)  (( ((vuint32)((x) & 0xFF000000u)) >> 24) | /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */ \
                                                    ( ((vuint32)((x) & 0x00FF0000u)) >>  8) | \
                                                    ( ((vuint32)((x) & 0x0000FF00u)) <<  8) | \
                                                    ( ((vuint32)((x) & 0x000000FFu)) << 24))      /* swap b4-b3-b2-b1 to b1-b2-b3-b4 */

/* FlexCAN LL Transition States */
#define kCan_30_Flexcan4LLStateStart                        0u
#define kCan_30_Flexcan4LLStateRequested                    1u

#if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
# define FLEXCAN4_CONTROLLER_MEMORY_SIZE ((vuint16)(0x3200u))
#endif

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/
/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */

/* for LL compatibility */
/* hw handles */
#define CAN_30_FLEXCAN4_HL_HW_TX_NORMAL_INDEX(hwch)     (Can_30_Flexcan4_GetTxBasicHwStart(hwch)) 
#if defined(CAN_30_FLEXCAN4_HL_HW_LAYOUT_TXBASIC_FIRST) /* COV_CAN_HW_LAYOUT_TXBASIC_FIRST */
# define CAN_30_FLEXCAN4_HL_HW_TX_STARTINDEX(hwch)      (Can_30_Flexcan4_GetTxBasicHwStart(hwch))
#else
# define CAN_30_FLEXCAN4_HL_HW_TX_STARTINDEX(hwch)      (Can_30_Flexcan4_GetTxFullHwStart(hwch))
#endif
#define CAN_30_FLEXCAN4_HL_HW_TX_FULL_STARTINDEX(hwch)  (Can_30_Flexcan4_GetTxFullHwStart(hwch))
#define CAN_30_FLEXCAN4_HL_HW_UNUSED_STARTINDEX(hwch)   (Can_30_Flexcan4_GetUnusedHwStart(hwch))
#define CAN_30_FLEXCAN4_HL_HW_RX_FULL_STARTINDEX(hwch)  (Can_30_Flexcan4_GetRxFullHwStart(hwch))
#define CAN_30_FLEXCAN4_HL_HW_RX_BASIC_STARTINDEX(hwch) (Can_30_Flexcan4_GetRxBasicHwStart(hwch))
#if defined(CAN_30_FLEXCAN4_HL_HW_LAYOUT_TXBASIC_FIRST) /* COV_CAN_HW_LAYOUT_TXBASIC_FIRST */
# define CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(hwch)       (Can_30_Flexcan4_GetTxFullHwStop(hwch))
#else
# define CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(hwch)       (Can_30_Flexcan4_GetTxBasicHwStop(hwch))
#endif
#define CAN_30_FLEXCAN4_HL_HW_TX_FULL_STOPINDEX(hwch)   (Can_30_Flexcan4_GetTxFullHwStop(hwch))
#define CAN_30_FLEXCAN4_HL_HW_UNUSED_STOPINDEX(hwch)    (Can_30_Flexcan4_GetUnusedHwStop(hwch))
#define CAN_30_FLEXCAN4_HL_HW_RX_FULL_STOPINDEX(hwch)   (Can_30_Flexcan4_GetRxFullHwStop(hwch))
#define CAN_30_FLEXCAN4_HL_HW_RX_BASIC_STOPINDEX(hwch)  (Can_30_Flexcan4_GetRxBasicHwStop(hwch))
/* mailbox handles */
#define CAN_30_FLEXCAN4_HL_MB_RX_FULL_STARTINDEX(hwch)  (Can_30_Flexcan4_GetRxFullHandleStart(hwch))
#define CAN_30_FLEXCAN4_HL_MB_RX_BASIC_STARTINDEX(hwch) (Can_30_Flexcan4_GetRxBasicHandleStart(hwch))
#define CAN_30_FLEXCAN4_HL_MB_TX_FULL_STARTINDEX(hwch)  (Can_30_Flexcan4_GetTxFullHandleStart(hwch))
#define CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX(hwch) (Can_30_Flexcan4_GetTxBasicHandleStart(hwch))
#define CAN_30_FLEXCAN4_HL_MB_RX_FULL_STOPINDEX(hwch)   (Can_30_Flexcan4_GetRxFullHandleStop(hwch))
#define CAN_30_FLEXCAN4_HL_MB_RX_BASIC_STOPINDEX(hwch)  (Can_30_Flexcan4_GetRxBasicHandleStop(hwch))
#define CAN_30_FLEXCAN4_HL_MB_TX_FULL_STOPINDEX(hwch)   (Can_30_Flexcan4_GetTxFullHandleStop(hwch))
#define CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STOPINDEX(hwch)  (Can_30_Flexcan4_GetTxBasicHandleStop(hwch))

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
# if !defined(CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT) /* May be defined by test suite to stimulate RAM_CHECK failure */
#  if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#   define CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitIsMailboxCorrupt((initParaPtr))
#  else
#   define CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitIsMailboxCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_30_FLEXCAN4_RAM_CHECK_FINISHED_CTP) /* May be defined by test suite to check RAM_CHECK finished / executed */
#  define CAN_30_FLEXCAN4_RAM_CHECK_FINISHED_CTP(ch)
# endif
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
# if !defined(CAN_30_FLEXCAN4_RAM_CHECK_BEGIN_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#   define CAN_30_FLEXCAN4_RAM_CHECK_BEGIN_REG_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt((initParaPtr))
#  else
#   define CAN_30_FLEXCAN4_RAM_CHECK_BEGIN_REG_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_30_FLEXCAN4_RAM_CHECK_END_REG_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#   define CAN_30_FLEXCAN4_RAM_CHECK_END_REG_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitEndIsRegisterCorrupt((initParaPtr))
#  else
#   define CAN_30_FLEXCAN4_RAM_CHECK_END_REG_RESULT(ch, initParaPtr) CanLL_30_Flexcan4_InitEndIsRegisterCorrupt((ch), (initParaPtr))
#  endif
# endif
# if !defined(CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT) /* May be defined by test suite to stimulate RAM_CHECK_EXTENDED failure */
#  define CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(ch)
# endif
#endif

#define CanHL_30_Flexcan4_IsStart(ch) ((Can_30_Flexcan4_GetLogStatus(ch) & CAN_30_FLEXCAN4_STATUS_START) == CAN_30_FLEXCAN4_STATUS_START)
#define CanHL_30_Flexcan4_IsSleep(ch) ((Can_30_Flexcan4_GetLogStatus(ch) & CAN_30_FLEXCAN4_STATUS_SLEEP) == CAN_30_FLEXCAN4_STATUS_SLEEP)
#define CanHL_30_Flexcan4_IsStop(ch) ((Can_30_Flexcan4_GetLogStatus(ch) & CAN_30_FLEXCAN4_STATUS_STOP) == CAN_30_FLEXCAN4_STATUS_STOP)
#define CanHL_30_Flexcan4_IsBusOff(ch) (Can_30_Flexcan4_IsIsBusOff(ch))
#define CanHL_30_Flexcan4_IsControllerInit(ch) ((Can_30_Flexcan4_GetLogStatus(ch) & CAN_30_FLEXCAN4_STATUS_INIT) == CAN_30_FLEXCAN4_STATUS_INIT)

#if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED) /* also for BRS because DLC may be bigger than 8 and will be checked by CanIf */
# define CAN_30_FLEXCAN4_DLC2LEN(dlc) (Can_30_Flexcan4_DlcToFrameLenght[((uint8)(dlc)) & ((uint8)0xFu)])
# define CAN_30_FLEXCAN4_LEN2DLC(length) ((uint8)Can_30_Flexcan4_MessageLengthToDlc[(length)])
#else
# define CAN_30_FLEXCAN4_DLC2LEN(dlc) (dlc)
# define CAN_30_FLEXCAN4_LEN2DLC(length) (length)
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_FULL)
# define CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(Index) Can_30_Flexcan4_GetMailboxDataLen(Index)
#else
# define CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(Index) 8u
#endif
#define CanHL_30_Flexcan4_IsFdMessage(id) (((id) & (Can_IdType)CAN_30_FLEXCAN4_ID_FD_MASK) == (Can_IdType)CAN_30_FLEXCAN4_ID_FD_MASK) /* \trace SPEC-60432, SPEC-50586 */
#define CanHL_30_Flexcan4_IsFdTxBrs(ch)   (Can_30_Flexcan4_GetInitObjectFdBrsConfig(Can_30_Flexcan4_GetLastInitObject(ch)) == CAN_30_FLEXCAN4_FD_RXTX) /* \trace SPEC-60432, SPEC-50586 */

/* PRQA L:FctLikeMacroLabel */

/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */

# define CANDRV_SET_CODE_TEST_POINT(x)

#if defined( C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER )
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#  define   CanLL_30_Flexcan4_ApplCanTimerStart(loop)   Appl_30_Flexcan4CanTimerStart(loop)
#  define   CanLL_30_Flexcan4_ApplCanTimerLoop(loop)    Appl_30_Flexcan4CanTimerLoop(loop)
#  define   CanLL_30_Flexcan4_ApplCanTimerEnd(loop)     Appl_30_Flexcan4CanTimerEnd(loop)
# else
#  define   CanLL_30_Flexcan4_ApplCanTimerStart(loop)   Appl_30_Flexcan4CanTimerStart(channel, (loop))
#  define   CanLL_30_Flexcan4_ApplCanTimerLoop(loop)    Appl_30_Flexcan4CanTimerLoop(channel, (loop))
#  define   CanLL_30_Flexcan4_ApplCanTimerEnd(loop)     Appl_30_Flexcan4CanTimerEnd(channel, (loop))
# endif
#else
# define   CanLL_30_Flexcan4_ApplCanTimerStart(loop)
# define   CanLL_30_Flexcan4_ApplCanTimerLoop(loop)     CAN_OK
# define   CanLL_30_Flexcan4_ApplCanTimerEnd(loop)
#endif

#if defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_OS )
# define CAN_30_FLEXCAN4_AREA(ch) Can_30_Flexcan4_GetProtectedAreaId(ch)
#else
  /* dummy area */
# define CAN_30_FLEXCAN4_AREA(ch) 0
#endif

#if defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_OS )
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG(reg, val)              osWritePeripheral16(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (val))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(reg, bits)       osModifyPeripheral16(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (vuint16)(~(bits)), (vuint16)0x0000u)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(reg, bits)         osModifyPeripheral16(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (vuint16)0xFFFFu, (bits))
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG(reg)                    osReadPeripheral16(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_32BIT(reg, val)        osWritePeripheral32(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (val))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(reg, bits) osModifyPeripheral32(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (vuint32)(~(bits)), (vuint32)0x00000000u)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(reg, bits)   osModifyPeripheral32(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg), (vuint32)0xFFFFFFFFu, (bits))
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(reg)              osReadPeripheral32(CAN_30_FLEXCAN4_AREA(canHwChannel), (vuint32)&(reg))
#elif defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_APPL )
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG(reg, val)              Appl_30_Flexcan4CanWriteProtectedRegister16(&(reg), (vuint16)0xFFFFu, (val))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(reg, bits)       Appl_30_Flexcan4CanWriteProtectedRegister16(&(reg), (bits), (vuint16)0x0000u)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(reg, bits)         Appl_30_Flexcan4CanWriteProtectedRegister16(&(reg), (bits), (bits))
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG(reg)                    Appl_30_Flexcan4CanReadProtectedRegister16(&(reg))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(reg, bits) Appl_30_Flexcan4CanWriteProtectedRegister32(&(reg), (bits), (vuint32)0x00000000u)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(reg, bits)   Appl_30_Flexcan4CanWriteProtectedRegister32(&(reg), (bits), (bits))
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(reg)              Appl_30_Flexcan4CanReadProtectedRegister32(&(reg))
#else
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG(reg, val)              (reg) = (val)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(reg, bits)       (reg) &= (vuint16)(~(bits))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(reg, bits)         (reg) |= (bits)
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG(reg)                    (reg)
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(reg, bits) (reg) &= (vuint32)(~(bits))
# define CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(reg, bits)   (reg) |= (bits)
# define CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(reg)              (reg)
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
# define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_0TO31(canFlagPtr) (canFlagPtr)->flags1 = pFlex_30_Flexcan4can->imask1; pFlex_30_Flexcan4can->imask1 = 0u
# define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_0TO31(canFlag)    pFlex_30_Flexcan4can->imask1 = (canFlag).flags1
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_32TO63(canFlagPtr) (canFlagPtr)->flags2 = pFlex_30_Flexcan4can->imask2; pFlex_30_Flexcan4can->imask2 = 0u
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_32TO63(canFlag)    pFlex_30_Flexcan4can->imask2 = (canFlag).flags2
# else
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_32TO63(canFlagPtr)
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_32TO63(canFlag)
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_64TO95(canFlagPtr) (canFlagPtr)->flags3 = pFlex_30_Flexcan4can->imask3; pFlex_30_Flexcan4can->imask3 = 0u
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_64TO95(canFlag)    pFlex_30_Flexcan4can->imask3 = (canFlag).flags3
# else
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_64TO95(canFlagPtr)
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_64TO95(canFlag)
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_96TO127(canFlagPtr) (canFlagPtr)->flags4 = pFlex_30_Flexcan4can->imask4; pFlex_30_Flexcan4can->imask4 = 0u
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_96TO127(canFlag)    pFlex_30_Flexcan4can->imask4 = (canFlag).flags4
# else
#  define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_96TO127(canFlagPtr)
#  define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_96TO127(canFlag)
# endif
#else
# define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_0TO31(canFlagPtr)
# define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_0TO31(canFlag)
# define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_32TO63(canFlagPtr)
# define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_32TO63(canFlag)
# define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_64TO95(canFlagPtr)
# define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_64TO95(canFlag)
# define   CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_96TO127(canFlagPtr)
# define   CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_96TO127(canFlag)
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
# define CAN_30_FLEXCAN4_DISABLE_BUSOFF_INTERRUPT(canFlagPtr)  (canFlagPtr)->canctrl1 = pFlex_30_Flexcan4can->control1; pFlex_30_Flexcan4can->control1 &= kNot_30_Flexcan4FlexCANErrBoff
# define CAN_30_FLEXCAN4_RESTORE_BUSOFF_INTERRUPT(canFlag)     pFlex_30_Flexcan4can->control1 |= (vuint32)((canFlag).canctrl1 & (vuint32)(~kNot_30_Flexcan4FlexCANErrBoff))
#else
# define CAN_30_FLEXCAN4_DISABLE_BUSOFF_INTERRUPT(canFlagPtr)
# define CAN_30_FLEXCAN4_RESTORE_BUSOFF_INTERRUPT(canFlag)
#endif

# define CAN_30_FLEXCAN4_DISABLE_WAKEUP_INTERRUPT(canFlagPtr)
# define CAN_30_FLEXCAN4_RESTORE_WAKEUP_INTERRUPT(canFlag)

/*
|<DataModelStart>| CanLL_30_Flexcan4_TxIsObjFree
Relation_Context:
Relation:
ChannelAmount, TxBasicAmount
ChannelAmount, TxFullAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_TxIsObjFree(hwCh, txObjHandle) ((CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS((hwCh), (txObjHandle), control) & kCode_30_Flexcan4_Mask) == kTx_30_Flexcan4CodeFree)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsSleep
Relation_Context:
Relation:
Wakeup, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
# define CanLL_30_Flexcan4_HwIsSleep(hwCh)  (kCan_30_Flexcan4False != kCan_30_Flexcan4False)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsStop
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsStop(hwCh) ((CAN_30_FLEXCAN4_READ_PROTECTED_REG((pFlex_30_Flexcan4CAN(hwCh))->canmcr) & kFlexCAN_30_Flexcan4_MCR) == kFlexCAN_30_Flexcan4_FREEZE_MODE)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsStopRequested
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsStopRequested(hwCh) ((CAN_30_FLEXCAN4_READ_PROTECTED_REG((pFlex_30_Flexcan4CAN(hwCh))->canmcr) & kFlexCAN_30_Flexcan4_MCR_REQ_ONLY_BITS) == kFlexCAN_30_Flexcan4_FREEZE_MODE_REQ)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsStart
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsStart(hwCh) ((CAN_30_FLEXCAN4_READ_PROTECTED_REG((pFlex_30_Flexcan4CAN(hwCh))->canmcr) & kFlexCAN_30_Flexcan4_MCR) == kFlexCAN_30_Flexcan4_NORMAL_MODE)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsStartRequested
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsStartRequested(hwCh) ((CAN_30_FLEXCAN4_READ_PROTECTED_REG((pFlex_30_Flexcan4CAN(hwCh))->canmcr) & kFlexCAN_30_Flexcan4_MCR_REQ_ONLY_BITS) == kFlexCAN_30_Flexcan4_NORMAL_MODE_REQ)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsBusOff
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsBusOff(hwCh) (((pFlex_30_Flexcan4CAN(hwCh))->estat & kFlexCAN_30_Flexcan4_FCS_BOFF) != 0u)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsListenOnlyMode
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsAutoRecoveryActive(hwCh) (((pFlex_30_Flexcan4CAN(hwCh))->control1 &  kFlexCAN_30_Flexcan4_BOFF_REC) == 0u)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsListenOnlyMode
Relation_Context:
Relation:
OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#define CanLL_30_Flexcan4_HwIsListenOnlyMode(hwCh) (((pFlex_30_Flexcan4CAN(hwCh))->control1 & kFlexCAN_30_Flexcan4_LOM) != 0u)

#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS )
/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsPassive
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#  define CanLL_30_Flexcan4_HwIsPassive(hwCh) (((pFlex_30_Flexcan4CAN(hwCh))->estat & kFlexCAN_30_Flexcan4_FCS) == kFlexCAN_30_Flexcan4_FCS_EP)

/*
|<DataModelStart>| CanLL_30_Flexcan4_HwIsWarning
Relation_Context:
Relation:
CanGetStatus, OneChOpt, ChannelAmount
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
#  if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#   define CanLL_30_Flexcan4_HwIsWarning(hwCh) ((Can_30_Flexcan4RxActualErrorCounter() >= 96u) || (Can_30_Flexcan4TxActualErrorCounter() >= 96u))
#  else
#   define CanLL_30_Flexcan4_HwIsWarning(hwCh) ((Can_30_Flexcan4RxActualErrorCounter(hwCh) >= 96u) || (Can_30_Flexcan4TxActualErrorCounter(hwCh) >= 96u))
#  endif
#endif /* C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS */



#define Can_30_Flexcan4BitMask(x)      ((vuint32)((vuint32)0x01u << ((x) & (0x1Fu))))
#define Can_30_Flexcan4InvBitMask(x)   ((vuint32)~((vuint32)0x01u << ((x) & (0x1Fu))))

/* PRQA L:FctLikeMacroLabel */

/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* BR:012 */
V_DEF_30_FLEXCAN4_CONST(V_NONE, uint8, CONST) Can_30_Flexcan4_MainVersion         = (uint8)CAN_30_FLEXCAN4_SW_MAJOR_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
V_DEF_30_FLEXCAN4_CONST(V_NONE, uint8, CONST) Can_30_Flexcan4_SubVersion          = (uint8)CAN_30_FLEXCAN4_SW_MINOR_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
V_DEF_30_FLEXCAN4_CONST(V_NONE, uint8, CONST) Can_30_Flexcan4_ReleaseVersion      = (uint8)CAN_30_FLEXCAN4_SW_PATCH_VERSION; /* PRQA S 3408 */ /* MD_Can_ExternalScope */


#if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
V_DEF_30_FLEXCAN4_CONST(CAN_STATIC, uint8, CONST) Can_30_Flexcan4_DlcToFrameLenght[16] = /* PRQA S 3218,3408 */ /* MD_Can_GlobalScope */
{
  0u,  1u,  2u,  3u,
  4u,  5u,  6u,  7u,
  8u, 12u, 16u, 20u,
 24u, 32u, 48u, 64u
};
V_DEF_30_FLEXCAN4_CONST(CAN_STATIC, uint8, CONST) Can_30_Flexcan4_MessageLengthToDlc[65] = /* PRQA S 3218,3408 */ /* MD_Can_GlobalScope */
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

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )
/* ROM CATEGORY 4 START */
/* test bit patterns for (Extended) RAM check */
V_DEF_30_FLEXCAN4_CONST(CAN_STATIC, vuint32, CONST) Can_30_Flexcan4RamCheckPattern32[3] = /* PRQA S 3218 */ /* MD_Can_GlobalScope */
{
  0xAAAAAAAAu, 0x55555555u, 0x00000000u
};
/* ROM CATEGORY 4 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
/* ROM CATEGORY 4 START */
/* additional test bit patterns for Extended RAM check */
V_DEF_30_FLEXCAN4_CONST(CAN_STATIC, vuint16, CONST) Can_30_Flexcan4RamCheckPattern16[3] = /* PRQA S 3218 */ /* MD_Can_GlobalScope */
{
  0xAAAAu, 0x5555u, 0x0000u
};
/* ROM CATEGORY 4 END */
#endif


#define CAN_30_FLEXCAN4_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_VAR_NOINIT_UNSPECIFIED  /*---------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***************************************************************************************/
/* local data definitions   (need also INFIX because STATIC may be defined to nothing) */
/***************************************************************************************/
/* \trace SPEC-1585 */
/*! Semaphore to block reentrancy for Can_30_Flexcan4_MainFunction_Read() */
V_DEF_30_FLEXCAN4_VAR(CAN_STATIC, uint8, VAR_NOINIT) can_30_Flexcan4IsRxTaskLocked; /* PRQA S 3408 */ /* MD_Can_GlobalScope */

/* Can LL Init State variable */


#if ( CAN_30_FLEXCAN4_REINIT_START == STD_ON )
/* RAM CATEGORY 1 START */
V_DEF_30_FLEXCAN4_VAR(CAN_STATIC volatile, vuint8, VAR_NOINIT) canLL_30_Flexcan4_canStartReinitState[kCan_30_Flexcan4NumberOfChannels];
/* RAM CATEGORY 1 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
/* RAM CATEGORY 1 START */
V_DEF_30_FLEXCAN4_P2VAR(V_NONE, vuint32, VAR_NOINIT, REG_CANCELL) canLL_30_Flexcan4_VirtualPtrField[kCan_30_Flexcan4NumberOfHwChannels]; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
/* RAM CATEGORY 1 END */
#endif

/* RAM CATEGORY 1 START */
V_DEF_30_FLEXCAN4_VAR(CAN_STATIC volatile, vuint32, VAR_NOINIT) canLL_30_Flexcan4_Estat[kCan_30_Flexcan4NumberOfHwChannels];
/* RAM CATEGORY 1 END */

#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
/* RAM CATEGORY 1 START */
V_DEF_30_FLEXCAN4_VAR(CAN_STATIC volatile, vuint32, VAR_NOINIT) canLL_30_Flexcan4_PassiveReportFlag[kCan_30_Flexcan4NumberOfHwChannels];
/* RAM CATEGORY 1 END */
#endif

#define CAN_30_FLEXCAN4_STOP_SEC_VAR_NOINIT_UNSPECIFIED  /*----------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CAN_30_FLEXCAN4_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ! Mark module to be initialized (used to check double/none initialization) */
V_DEF_30_FLEXCAN4_VAR(CAN_STATIC, uint8, VAR_INIT) can_30_Flexcan4ConfigInitFlag = CAN_30_FLEXCAN4_STATUS_UNINIT; /* PRQA S 3408 */ /* MD_Can_ExternalScope */
# define CAN_30_FLEXCAN4_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* not static because of debug/test usage */
#if defined(CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER)
# define CAN_30_FLEXCAN4_START_SEC_VAR_INIT_UNSPECIFIED  /*-----------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Pointer to the current configuration */
V_DEF_30_FLEXCAN4_P2CONST (V_NONE, Can_30_Flexcan4_ConfigType, VAR_INIT, CONST_PBCFG) Can_30_Flexcan4_ConfigDataPtr = NULL_PTR; /* UREQ00035484 */ /* PRQA S 3408 */ /* MD_Can_ExternalScope */
# define CAN_30_FLEXCAN4_STOP_SEC_VAR_INIT_UNSPECIFIED  /*------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#else
# define CAN_30_FLEXCAN4_START_SEC_CONST_UNSPECIFIED  /*--------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Pointer to the current configuration */
V_DEF_30_FLEXCAN4_CONSTP2CONST(V_NONE, Can_30_Flexcan4_ConfigType, CONST, CONST_PBCFG) Can_30_Flexcan4_ConfigDataPtr = NULL_PTR; /* PRQA S 3408,1514 */ /* MD_Can_ExternalScope */
# define CAN_30_FLEXCAN4_STOP_SEC_CONST_UNSPECIFIED  /*---------------------------------*/
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/***************************************************************************/
/*  Safe Check                                                             */
/***************************************************************************/
#if !defined(CAN_30_FLEXCAN4_SAFE_BSW) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_SAFE_BSW   STD_OFF
#else
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
#  if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_OFF) || (CAN_30_FLEXCAN4_AMD_RUNTIME_MEASUREMENT == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW_EXCLUSION */
#   error "Unsupported Feature activated for SafeBSW"
#  endif
# endif
#endif

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/
#if (CAN_30_FLEXCAN4_TRANSMIT_BUFFER == STD_OFF) && defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
# error "C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW need CAN_30_FLEXCAN4_TRANSMIT_BUFFER == STD_ON (no FIFO buffering but PRIO_BY_CANID in CanIf) -> check Generated data"
#endif
# if (CAN_30_FLEXCAN4_SLEEP_SUPPORT == STD_ON) && (CAN_30_FLEXCAN4_WAKEUP_SUPPORT == STD_ON)
#  error "C_30_FLEXCAN4_ENABLE_SLEEP_WAKEUP organified but generated like supported"
# endif
#if (CAN_30_FLEXCAN4_WAKEUP_SUPPORT == STD_ON) && (CAN_30_FLEXCAN4_SLEEP_SUPPORT == STD_OFF)
# error "activated CAN_30_FLEXCAN4_WAKEUP_SUPPORT need activated CAN_30_FLEXCAN4_SLEEP_SUPPORT"
#endif
#if defined(C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER)
# if (CAN_30_FLEXCAN4_LOOP_MAX == 0)
#  error "CAN_30_FLEXCAN4_LOOP_MAX should not be generated as 0"
# endif
#else
# if (CAN_30_FLEXCAN4_HARDWARE_CANCELLATION == STD_ON)
#  error "C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER organified but generated like supported"
# endif
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
#else
# if (CAN_30_FLEXCAN4_HW_TRANSMIT_CANCELLATION == STD_ON)
#  error "C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW organified but generated like supported"
# endif
#endif
#if (CAN_30_FLEXCAN4_MIRROR_MODE == STD_ON) && (!defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2) || (CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_OFF))
# error "CAN_30_FLEXCAN4_MIRROR_MODE need CAN_30_FLEXCAN4_GENERIC_CONFIRMATION with CAN_30_FLEXCAN4_API2 and CAN_30_FLEXCAN4_GENERIC_PRECOPY as STD_ON"
#endif
#if (CAN_30_FLEXCAN4_MULTIPLE_BASICCAN_TX == STD_ON)
# if (CAN_30_FLEXCAN4_HW_TRANSMIT_CANCELLATION == STD_ON)
/* cancellation does not work in CanHL_30_Flexcan4_WritePrepareCancelTx for multiple BasicCAN messages, and is not useful for FIFO queue in CanIf (normally combined with multiple BCAN)*/
# error "CAN_30_FLEXCAN4_MULTIPLE_BASICCAN_TX and CAN_30_FLEXCAN4_HW_TRANSMIT_CANCELLATION are STD_ON but this combination is not supported"
# endif
#endif


/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_STATIC_CODE  /*--------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#  define CanHL_30_Flexcan4_ApplCanTimerStart( timerIdx )  Appl_30_Flexcan4CanTimerStart( timerIdx );
#  define CanHL_30_Flexcan4_ApplCanTimerEnd( timerIdx )    Appl_30_Flexcan4CanTimerEnd( timerIdx );
# else
#  define CanHL_30_Flexcan4_ApplCanTimerStart( timerIdx )  Appl_30_Flexcan4CanTimerStart( channel, (timerIdx) );
#  define CanHL_30_Flexcan4_ApplCanTimerEnd( timerIdx )    Appl_30_Flexcan4CanTimerEnd( channel, (timerIdx) );
# endif
# define CanHL_30_Flexcan4_ApplCanTimerLoop( timerIdx )   if (Appl_30_Flexcan4CanTimerLoop(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST (timerIdx)) != CAN_OK) \
                                              { \
                                                break; \
                                              }

# if (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_OFF)
/* Internal handling of HW loops (like AutoSar) - otherwise ApplCanTimer... is defined an implemented by Application */
/****************************************************************************
| NAME:             Appl_30_Flexcan4CanTimerStart
****************************************************************************/
/* Used as macro to save runtime and code */
#  if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
/* \trace SPEC-1640 */
#    define Appl_30_Flexcan4CanTimerStart(source)     ((void)GetCounterValue( CAN_30_FLEXCAN4_OS_COUNTER_ID, &Can_30_Flexcan4_GetCanLoopTimeout(0, (source)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#    define Appl_30_Flexcan4CanTimerStart(ch, source) ((void)GetCounterValue( CAN_30_FLEXCAN4_OS_COUNTER_ID, &Can_30_Flexcan4_GetCanLoopTimeout((ch), (source)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif
/****************************************************************************
| NAME:             Appl_30_Flexcan4CanTimerLoop
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) Appl_30_Flexcan4CanTimerLoop( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 source )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  uint8 errorId;
  uint8 apiId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  apiId = CAN_30_FLEXCAN4_HW_ACCESS_ID;
  retval = CAN_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and source is valid (no INIT check because may be used inside power on INIT) */
  if (source > CAN_30_FLEXCAN4_LOOP_MAX) /* CM_CAN_HL22 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
#   if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
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
    startTime = Can_30_Flexcan4_GetCanLoopTimeout(channel, source);
    /* Get elapsed time - from start time to now */
    /* #120 Throw DET error when GetElapsedValue() failed */
    if ( GetElapsedValue( CAN_30_FLEXCAN4_OS_COUNTER_ID, &startTime, &elapsedTime ) != (StatusType)E_OK ) /* SBSW_CAN_HL37 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_30_FLEXCAN4_E_TIMEOUT_DET;
    }
    else
    {
      /* #130 Check if time out occurs for HW loop checks (none mode transition) \trace SPEC-1594 */
      if ( (TickType)CAN_30_FLEXCAN4_OS_TICK2MS( elapsedTime ) < (TickType)CAN_30_FLEXCAN4_TIMEOUT_DURATION ) /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 1840, 2985 */ /* MD_Can_IntegerCast, MD_Can_ConstValue */
      { /* no time out occur return OK */
        retval = CAN_OK;
      }
      else /* only used in case C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER but removed because of coverage TX for some organiy cases */
      { /* Mode Change synchronous & HW loop check */
        /* #140 Throw DET error and set hardware cancellation flag */
        if (source < CAN_30_FLEXCAN4_LOOP_MAX) /* COV_CAN_HARDWARE_FAILURE */
        { 
          errorId = CAN_30_FLEXCAN4_E_TIMEOUT_DET;
        }
      }
    }
    CAN_30_FLEXCAN4_DUMMY_STATEMENT(startTime); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)  
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(apiId, errorId);
  }
#  else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(apiId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
/****************************************************************************
| NAME:             Appl_30_Flexcan4CanTimerEnd
****************************************************************************/
/* Used as macro to save runtime and code */
/* because of possible nested timers .. timer has to be reset */
#  define Appl_30_Flexcan4CanTimerEnd Appl_30_Flexcan4CanTimerStart
# endif /* (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_OFF) */


/****************************************************************************
| NAME:             CanHL_30_Flexcan4_CleanUpSendState
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_CleanUpSendState
Relation_Context:
# from CanLL_30_Flexcan4_ModeTransition() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_CleanUpSendState( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Flexcan4ObjectHandle activeSendObject;
  Can_30_Flexcan4ObjectHandle mailboxHandle;
  Can_HwHandleType mailboxElement;
  /* ----- Implementation ----------------------------------------------- */
  /* parameter channel is already checked by caller */
  /* #10 set all BasicCAN TX Objects to state FREE to allow a new transmission */
  mailboxHandle = Can_30_Flexcan4_GetTxBasicHandleStart(canHwChannel);
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; mailboxHandle < Can_30_Flexcan4_GetTxBasicHandleStop(canHwChannel); mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
# endif
  {
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) 
    for (mailboxElement = 0u; mailboxElement < Can_30_Flexcan4_GetMailboxSize(mailboxHandle); mailboxElement++) /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
# else
    mailboxElement = 0u; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
# endif
    {
      activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(mailboxHandle, mailboxElement);
      Can_30_Flexcan4_SetActiveSendState(activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
    }
  }
# if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
  /* #30 set all FullCAN TX Objects to state FREE to allow a new transmission */
  for (mailboxHandle = Can_30_Flexcan4_GetTxFullHandleStart(canHwChannel); mailboxHandle < Can_30_Flexcan4_GetTxFullHandleStop(canHwChannel); mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
  {
    activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(mailboxHandle, 0u); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    Can_30_Flexcan4_SetActiveSendState(activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */ 
  }
# endif
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_ReInit
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_ReInit
Relation_Context:
# from CanHL_30_Flexcan4_ModeTransition() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, uint8, STATIC_CODE) CanHL_30_Flexcan4_ReInit( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST uint8 doRamCheck )
{ /* \trace SPEC-1719, SPEC-1589, SPEC-1346, SPEC-1565 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCan_30_Flexcan4InitParaStruct initPara;
  CanHookBegin_CanHL_30_Flexcan4_ReInit();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  initPara.isInitOk = kCan_30_Flexcan4Failed; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Config pointer is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* \trace SPEC-1718 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1713 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* \trace SPEC-1713 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 initialization controller states and initialization objects --- */
    initPara.initObject = Can_30_Flexcan4_GetLastInitObject(channel);
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    initPara.doRamCheck = doRamCheck;
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
    initPara.isChRamCheckFail = kCan_30_Flexcan4False;
#endif
    /* #110 clean up status (do not activate communication and let SLEEP and initialization to detect ram-check request) */
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK) && !defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
    if (doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    { /* #112 standard RAM check will be performed so delete old status CAN_DEACTIVATE_CONTROLLER but leave CAN_STATUS_INIT (info about PowerOn) */
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) & (uint8)CAN_30_FLEXCAN4_STATUS_INIT);  /* SBSW_CAN_HL02 */
    }
    else
#endif
    { /* #114 extended RAM check may be performed so delete old status except CAN_DEACTIVATE_CONTROLLER and CAN_STATUS_INIT */
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) & ((uint8)(CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) | (uint8)((uint8)CAN_30_FLEXCAN4_STATUS_INIT)));  /* SBSW_CAN_HL02 */
    }
    /* #125 begin initialization CanHL_30_Flexcan4_InitBegin() */
    initPara.isInitOk = CanHL_30_Flexcan4_InitBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */
    /* #130 block invalid hardware access (silent check) */
    if (initPara.isInitOk == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
    {
#if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
      /* #140 do TX FullCAN initialization CanHL_30_Flexcan4_InitTxFullCAN()*/
      initPara.isInitOk &= CanHL_30_Flexcan4_InitTxFullCAN(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
      /* #150 do TX BasicCAN initialization CanHL_30_Flexcan4_InitTxBasicCAN() */
      initPara.isInitOk &= CanHL_30_Flexcan4_InitTxBasicCAN(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
      /* #170 do RX FullCAN initialization CanHL_30_Flexcan4_InitRxFullCAN() */
      initPara.isInitOk &= CanHL_30_Flexcan4_InitRxFullCAN(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
      /* #180 do RX BasicCAN initialization CanHL_30_Flexcan4_InitRxBasicCAN() */
      initPara.isInitOk &= CanHL_30_Flexcan4_InitRxBasicCAN(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#endif
      /* #190 end initialization CanHL_30_Flexcan4_InitEnd_InitMode() */
      initPara.isInitOk &= CanHL_30_Flexcan4_InitEnd_InitMode(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
      if (doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
      { 
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
        /* #200 extended RamChecks new issue occur - notify corrupt register / deactivate controller */
        if (initPara.isChRamCheckFail == kCan_30_Flexcan4True) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        {
          CanIf_30_Flexcan4_RamCheckCorruptController((uint8)Can_30_Flexcan4_GetCanToCanIfChannelMapping(channel));
          Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */
        }
# else
        if( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) == CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        { /* #210 standard RAM check issue occur - call Appl_CanRamCheckFailed() (any mailbox fail) */
          if (Appl_30_Flexcan4_CanRamCheckFailed((uint8)channel) != CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER)
          { /* #220 let the application decide if communication stay disabled as set by RAM check or not */
            Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) & (uint8)(~(CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER))); /* Application decide to activate communication in case RAM_CHECK failed */  /* SBSW_CAN_HL02 */
          }
        }
# endif
        CAN_30_FLEXCAN4_RAM_CHECK_FINISHED_CTP(channel)
      }
#endif
    }
    if (initPara.isInitOk == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
    { /* #280 set controller init flag when no issue occure before */
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | (CAN_30_FLEXCAN4_STATUS_INIT)); /* SBSW_CAN_HL02 */
    }
    else
    { /* #275 set error when issue occure before */
      Can_30_Flexcan4_SetLogStatus(channel, /* CAN_30_FLEXCAN4_STATUS_UNINIT | */ (Can_30_Flexcan4_GetLogStatus(channel) & (uint8)(CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER))); /* SBSW_CAN_HL02 */
      errorId = CAN_30_FLEXCAN4_E_UNINIT;
    }
  } /* controller active in multiple ECU configuration */

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_INITCTR_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if !defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(doRamCheck); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_CanHL_30_Flexcan4_ReInit();
  return initPara.isInitOk;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_ModeTransition
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_ModeTransition
Relation_Context:
# from Can_30_Flexcan4_Init #
ChannelAmount
# from Can_30_Flexcan4_MainFunction_BusOff, CanHL_30_Flexcan4_ErrorHandling #
StatusPolling, ChannelAmount
# from Can_30_Flexcan4_ChangeBaudrate #
ChannelAmount
# from Can_30_Flexcan4_SetBaudrate #
ChannelAmount
# from Can_30_Flexcan4_SetControllerMode #
Wakeup
RamCheck
# from Can_30_Flexcan4_RamCheckExecute #
RamCheck
# from Can_30_Flexcan4_RamCheckEnableController #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_ModeTransition( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 transitionRequest, uint8 busOffRecovery, uint8 doRamCheck )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 transitionState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 request transition in LL */
  transitionState = CanLL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
  if(transitionState != kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */
  { /* #20 first request fail so start Loop that wait short time for transition \trace SPEC-1642, SPEC-1635, SPEC-1630 */
    CanHL_30_Flexcan4_ApplCanTimerStart((uint8)CAN_30_FLEXCAN4_LOOP_MAX); /* SBSW_CAN_HL38 */
    do
    { /* #30 HW check mode reached (LL) */
      transitionState = CanLL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
      CanHL_30_Flexcan4_ApplCanTimerLoop((uint8)CAN_30_FLEXCAN4_LOOP_MAX); /* COV_CAN_TRANSITION_REQUEST */
    } while ( transitionState != kCan_30_Flexcan4Ok ); /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2990,2995 */ /* MD_Can_ConstValue */
    CanHL_30_Flexcan4_ApplCanTimerEnd ((uint8)CAN_30_FLEXCAN4_LOOP_MAX); /* SBSW_CAN_HL38 */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */
  }
  if ( transitionState == kCan_30_Flexcan4Requested ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_TRANSITION_REQUEST */
  {
    /* #50 transition requested so remember request to repeat it asynchronous */
    Can_30_Flexcan4_SetModeTransitionRequest(channel, transitionRequest); /* SBSW_CAN_HL01 */
    Can_30_Flexcan4_SetBusOffTransitionRequest(channel, busOffRecovery); /* SBSW_CAN_HL01 */
    Can_30_Flexcan4_SetRamCheckTransitionRequest(channel, doRamCheck); /* SBSW_CAN_HL01 */
  }
  return transitionState;
}

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_NotifyTransition
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_NotifyTransition
Relation_Context:
# from Can_30_Flexcan4_SetControllerMode() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_NotifyTransition( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 canState, CanIf_ControllerModeType canIfState, uint8 transitionState )
{
  /* #10 if mode changed successful */
  if ( transitionState == kCan_30_Flexcan4Ok ) /* COV_CAN_TRANSITION_REQUEST */
  {
    /* #20 transition finished change internal state */
    Can_30_Flexcan4_SetLogStatus(channel, (uint8)((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_MASK_NOMODE) | canState)); /* SBSW_CAN_HL02 */
    /* #30 remove transition request */
    Can_30_Flexcan4_SetModeTransitionRequest(channel, kCan_30_Flexcan4ModeNone); /* SBSW_CAN_HL01 */
    /* #40 call notification CanIf_ControllerModeIndication() \trace SPEC-1726, SPEC-1644, SPEC-60446 */
    CanIf_30_Flexcan4_ControllerModeIndication( (uint8)Can_30_Flexcan4_GetCanToCanIfChannelMapping(channel), canIfState );
  }
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(transitionState); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_IsHigherCanIdPrio
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_IsHigherCanIdPrio
Relation_Context:
# from CanHL_30_Flexcan4_WritePrepare #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_30_Flexcan4_IsHigherCanIdPrio( Can_IdType CanId1, Can_IdType CanId2 )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  Can_IdType tmpCanId1, tmpCanId2;
  tmpCanId1 = CanId1;
  tmpCanId2 = CanId2;
  /* ----- Implementation ----------------------------------------------- */
# if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
  /* #10 take care about mixed Extended and Standard ID format (shift Standard ID to match Extended format) */
  if (((tmpCanId1) & CAN_30_FLEXCAN4_ID_IDE_MASK) == CAN_30_FLEXCAN4_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    tmpCanId1 = tmpCanId1 & (Can_IdType)(CAN_30_FLEXCAN4_ID_MASK);
  } else {
    tmpCanId1 = (Can_IdType)(((tmpCanId1) & CAN_30_FLEXCAN4_ID_MASK_STD) << 18);
  }
  if (((tmpCanId2) & CAN_30_FLEXCAN4_ID_IDE_MASK) == CAN_30_FLEXCAN4_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    tmpCanId2 = tmpCanId2 & (Can_IdType)(CAN_30_FLEXCAN4_ID_MASK);
  } else {
    tmpCanId2 = (Can_IdType)(((tmpCanId2) & CAN_30_FLEXCAN4_ID_MASK_STD) << 18);
  }
# else
  /* #20 mask out unused bits */
  tmpCanId1 = tmpCanId1 & (Can_IdType)CAN_30_FLEXCAN4_ID_MASK;
  tmpCanId2 = tmpCanId2 & (Can_IdType)CAN_30_FLEXCAN4_ID_MASK;
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
| NAME:             CanHL_30_Flexcan4_WritePrepare
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_WritePrepare
Relation_Context:
# from Can_30_Flexcan4_Write() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WritePrepare( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 calculate logical TX mailbox handle for FullCAN or for BasicCAN (Multiplexed TX) as start value */
  txPara->mailboxElement = 0u; /* PRQA S 2982 */ /* MD_MSR_RetVal */ /* SBSW_CAN_HL42 */
  txPara->activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
#if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
  if ( Can_30_Flexcan4_GetMailboxType(txPara->mailboxHandle) != CAN_30_FLEXCAN4_TX_FULLCAN_TYPE ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
#endif
  { /* #20 BasicCAN preparation - search for hardware and logical transmit handle and do cancellation */
#if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) || defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
    Can_30_Flexcan4ObjectHandle mailboxElement_max;
    mailboxElement_max = Can_30_Flexcan4_GetMailboxSize(txPara->mailboxHandle); /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    /* \trace SPEC-1677, SPEC-1672, SPEC-1679, SPEC-1673, SPEC-1671 */
    { /* #27 if normal or Mux Tx search from first to last element 0..1 or 0..3 for free object or one to be cancelled */
      Can_30_Flexcan4ObjectHandle mailboxElementFound;
      Can_30_Flexcan4ObjectHandle activeSendObjectFound;
      mailboxElementFound = mailboxElement_max; /* PRQA S 2982 */ /* MD_MSR_RetVal */
      activeSendObjectFound = mailboxElement_max; /* PRQA S 2982 */ /* MD_MSR_RetVal */
      for (txPara->mailboxElement = 0u; txPara->mailboxElement < mailboxElement_max; txPara->mailboxElement++) /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
      { /* #30 over all multiplexed TX objects: find free mailbox or mailbox with identical ID */
# if (CAN_30_FLEXCAN4_IDENTICAL_ID_CANCELLATION == STD_ON) && defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
        if ( ((txPara->pdu.id) == Can_30_Flexcan4_GetActiveSendId(txPara->activeSendObject) ) && (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) != kCan_30_Flexcan4BufferFree) )
        { /* #40 Identical ID: priority 1 - cancel identical (also when free is available) */
          mailboxElementFound = txPara->mailboxElement;
          activeSendObjectFound = txPara->activeSendObject;
          break;
        }
        if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferFree)
        { /* #50 Identical ID: priority 2 - find free mailbox (no cancellation) */
          mailboxElementFound = txPara->mailboxElement; /* do not break ... next may be identical ID */
          activeSendObjectFound = txPara->activeSendObject;
        }
# else
        if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferFree)
        { /* #60 None Identical ID: else priority 1 - find free mailbox (no cancellation) */
          mailboxElementFound = txPara->mailboxElement;
          activeSendObjectFound = txPara->activeSendObject;
          break;
        }
# endif
        txPara->activeSendObject++; /* SBSW_CAN_HL42 */
      }
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
      if (mailboxElementFound == mailboxElement_max)
      { /* #70 No free nor identical ID found but hardware cancellation is on - search for lowest prio message to cancel */
        Can_IdType lowestPrioId = txPara->pdu.id;
        txPara->activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
        for (txPara->mailboxElement = 0u; txPara->mailboxElement < mailboxElement_max; txPara->mailboxElement++) /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
        {
          if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferSend)
          { /* #80 mailbox with send request found: check for priority and remember the lowest that could be cancelled */
            if (CanHL_30_Flexcan4_IsHigherCanIdPrio(lowestPrioId, Can_30_Flexcan4_GetActiveSendId(txPara->activeSendObject)) == CAN_OK)
            {
              lowestPrioId = Can_30_Flexcan4_GetActiveSendId(txPara->activeSendObject);
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
        txPara->activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(txPara->mailboxHandle, 0u); /* SBSW_CAN_HL42 */
      }
      else
      {
        txPara->mailboxElement = mailboxElementFound; /* SBSW_CAN_HL42 */
        txPara->activeSendObject = activeSendObjectFound; /* SBSW_CAN_HL42 */
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) /* \trace SPEC-1674 */
        CanHL_30_Flexcan4_WritePrepareCancelTx(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
# endif
      }
    } /* normal or mux tx */
#endif
  } /* end BasicCAN handling */
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  /* #200 no special FullCAN handling needed */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) 
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_WriteSavePduInfo
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_WriteSavePduInfo
Relation_Context:
# from CanHL_30_Flexcan4_WritePrepareCancelTx #
HwCancelTx
# from CanHL_30_Flexcan4_WriteStart #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WriteSavePduInfo( CanTxTransmissionParaStructPtr txPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
    /* #10 Get buffer to store data */
    uint16 activeSendData;
    activeSendData = CanHL_30_Flexcan4_GetActiveSendData(txPara->mailboxHandle, txPara->mailboxElement);
    /* #20 SDU pointer may be a null pointer in case the DLC is 0 - so do not copy data in this case */
    if(txPara->pdu.sdu != NULL_PTR)
    {
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
      if(Can_30_Flexcan4_GetSizeOfActiveSendData() >= (activeSendData + txPara->pdu.length)) /* CM_CAN_HL20 */ /* COV_CAN_GENDATA_FAILURE */
# endif
      {
        /* #25 copy data with VStdLib optimized copy routine */
        VStdMemCpy((void*)&Can_30_Flexcan4_GetActiveSendData(activeSendData), (void*)txPara->pdu.sdu, txPara->pdu.length); /* SBSW_CAN_HL22 */ /* PRQA S 0314,0316 */ /* MD_Can_PointerVoidCast */
      }
    }
    /* #30 copy ID, DLC and pointer to data (also pdu as part of PduInfo struct) */
    Can_30_Flexcan4_SetActiveSendId(txPara->activeSendObject, txPara->pdu.id); /* SBSW_CAN_HL23 */
    Can_30_Flexcan4_SetActiveSendLength(txPara->activeSendObject, txPara->pdu.length); /* SBSW_CAN_HL23 */
    Can_30_Flexcan4_SetActiveSendSdu(txPara->activeSendObject, &Can_30_Flexcan4_GetActiveSendData(activeSendData)); /* SBSW_CAN_HL23 */
    Can_30_Flexcan4_SetActiveSendSwPduHandle(txPara->activeSendObject, txPara->pdu.swPduHandle); /* SBSW_CAN_HL23 */
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_WritePrepareCancelTx
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_WritePrepareCancelTx
Relation_Context:
# from CanHL_30_Flexcan4_WritePrepare #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_WritePrepareCancelTx( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferFree)
  { /* #10 free buffer found: backup ID, DLC, data, PDU for possible cancellation - as parameter to IF */
    CanHL_30_Flexcan4_WriteSavePduInfo(txPara); /* SBSW_CAN_HL49 */
  }
  else if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferSend) /* COV_IMPLICITE_TXCANCEL */
  { /* #20 occupied buffer found (lower ID or Identical - found before): cancel it */
    /* \trace SPEC-1747, SPEC-15133, SPEC-1685, SPEC-1690, SPEC-1692, SPEC-1676, SPEC-1683, SPEC-15126 */
    tCan_30_Flexcan4TxCancellationParaStruct txCancellationPara;
    txCancellationPara.mailboxHandle = txPara->mailboxHandle;
    txCancellationPara.hwObjHandle = txPara->hwObjHandle; 
    txCancellationPara.mailboxElement = txPara->mailboxElement;
    txCancellationPara.activeSendObject = txPara->activeSendObject;
    txCancellationPara.canHandleCurTxObj = kCan_30_Flexcan4BufferCancel;
    /* #30 HW cancel mailbox (LL) */
    CanLL_30_Flexcan4_TxCancelInHw(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
    Can_30_Flexcan4_SetActiveSendState(txPara->activeSendObject, kCan_30_Flexcan4BufferCancel); /* SBSW_CAN_HL41 */
    /* #40 do not notify IF about successful cancellation because of recursion will not work! Hardware support not possible in this case */
  }
  else
  { /* avoid MISRA warning (if else if without ending else) */
    /* \trace SPEC-1754 */
  }
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_WriteStart
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_WriteStart
Relation_Context:
# from Can_30_Flexcan4_Write #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_ReturnType, STATIC_CODE) CanHL_30_Flexcan4_WriteStart( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 free TX mailbox found so start transmission \trace SPEC-1744 */
  if (Can_30_Flexcan4_GetActiveSendState(txPara->activeSendObject) == kCan_30_Flexcan4BufferFree)
  {
    uint8 llretval;
    /* #20 remember PDU and send state to use it in Confirmation function \trace SPEC-1675 */
    Can_30_Flexcan4_SetActiveSendPdu(txPara->activeSendObject, txPara->pdu.swPduHandle); /* SBSW_CAN_HL05 */
    Can_30_Flexcan4_SetActiveSendState(txPara->activeSendObject, kCan_30_Flexcan4BufferSend); /* SBSW_CAN_HL04 */
#if defined(C_30_FLEXCAN4_ENABLE_PRETRANSMIT_FCT)
    /* call pretransmit function ----------------------------------------------- */
    Appl_30_Flexcan4_GenericPreTransmit( (uint8)channel, &(txPara->pdu) );  /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL24 */
#endif
    /* #50 HW prepare transmission */
    CanLL_30_Flexcan4_TxBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    /* #60 Set ID, ID type and DLC in HW register depend on ID type */
    txPara->dlcRaw = MK_TX_DLC(CAN_30_FLEXCAN4_LEN2DLC(txPara->pdu.length)); /* SBSW_CAN_HL42 */
#if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
# if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
    if ( ((txPara->pdu.id) & CAN_30_FLEXCAN4_ID_IDE_MASK) != CAN_30_FLEXCAN4_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      txPara->idType = kCan_30_Flexcan4IdTypeStd; /* SBSW_CAN_HL42 */
      SET_PARASTRUCT_IDRAW_TX_STD(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
    }
    else
# endif
    {
# if defined( C_30_FLEXCAN4_ENABLE_MIXED_ID ) 
      txPara->idType = kCan_30_Flexcan4IdTypeExt; /* SBSW_CAN_HL42 */
# endif
      SET_PARASTRUCT_IDRAW_TX_EXT(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */
    }
#else
    {
      SET_PARASTRUCT_IDRAW_TX_STD(txPara, txPara->pdu.id); /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* SBSW_CAN_HL42 */ /* PRQA S 4491 */ /* MD_Can_IntegerCast */
    }
#endif
    /* set id, dlc in hardware */
    CanLL_30_Flexcan4_TxSetMailbox(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    if(txPara->pdu.sdu != NULL_PTR)
    { /* #70 Set CAN data in HW register (LL) - only when data pointer is not null \trace SPEC-1680, SPEC-1572, SPEC-1678, SPEC-60421 */
      txPara->CanMemCopySrcPtr = txPara->pdu.sdu; /* SBSW_CAN_HL42 */
      CanLL_30_Flexcan4_TxCopyToCan(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    }
    /* #80 HW start transmission (LL) */
    llretval = CanLL_30_Flexcan4_TxStart(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    if (llretval == kCan_30_Flexcan4Failed) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
    { /* #90 transmit request wasn't successful in LL part - free mailbox */
      retval = CAN_NOT_OK; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
      Can_30_Flexcan4_SetActiveSendState(txPara->activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
    }
    /* HW finish transmission (LL) */
    CanLL_30_Flexcan4_TxEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST txPara); /* SBSW_CAN_HL49 */
    CAN_30_FLEXCAN4_DUMMY_STATEMENT(llretval); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }
  else
  { /* #200 no free TX mailbox found so return busy - also in case cancellation (BasicCAN) was possible \trace SPEC-1754, SPEC-1764, SPEC-1747 */
    retval = CAN_BUSY;
  }
  return retval;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_GetActiveSendObject
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_GetActiveSendObject
Relation_Context:
# from CanHL_30_Flexcan4_InitXXX() #
Wakeup, OneChOpt, ChannelAmount
# from CanHL_30_Flexcan4_WritePrepare #
# from CanHL_30_Flexcan4_TxTaskCancelationHandling #
HwCancelTx
# from CanHL_30_Flexcan4_CleanUpSendState #
# from Can_30_Flexcan4_CancelTx #
TransmitCancellationAPI
# from CanHL_30_Flexcan4_TxConfirmation #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, Can_30_Flexcan4ObjectHandle, STATIC_CODE) CanHL_30_Flexcan4_GetActiveSendObject( Can_HwHandleType mailboxHandle, Can_HwHandleType mailboxElement )
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Flexcan4ObjectHandle activeSendObject;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 get logical handle from generated indirection table */
  activeSendObject = (Can_30_Flexcan4ObjectHandle) Can_30_Flexcan4_GetMailboxActiveSendObject(mailboxHandle) + mailboxElement;
#if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if (activeSendObject >= (Can_HwHandleType)Can_30_Flexcan4_GetSizeOfActiveSendObject()) /* CM_CAN_HL03 */ /* COV_CAN_GENDATA_FAILURE */
  { /* #20 secure SilentBSW */
    activeSendObject = ((Can_HwHandleType)Can_30_Flexcan4_GetSizeOfActiveSendObject() - (Can_HwHandleType)1u);
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)  
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_WRITE_ID, CAN_30_FLEXCAN4_E_PARAM_HANDLE);
# endif
  }
#endif
  return activeSendObject;
}

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) 
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_GetActiveSendData
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_GetActiveSendData
Relation_Context:
HwCancelTx, TxHwFifo
# from CanHL_30_Flexcan4_WritePrepare #
# from CanHL_30_Flexcan4_TxTaskCancelationHandling #
HwCancelTx
# from Can_30_Flexcan4_CancelTx #
TransmitCancellationAPI
# from CanHL_30_Flexcan4_TxConfirmation #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint16, STATIC_CODE) CanHL_30_Flexcan4_GetActiveSendData( Can_HwHandleType mailboxHandle, uint8 mailboxElement )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 activeSendData;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 get logical handle from generated indirection table */
  activeSendData = (uint16) (Can_30_Flexcan4_GetMailboxActiveSendData(mailboxHandle) + ((uint16)mailboxElement * (uint16)Can_30_Flexcan4_GetMailboxDataLen(mailboxHandle)));
#if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if ((activeSendData + Can_30_Flexcan4_GetMailboxDataLen(mailboxHandle)) > Can_30_Flexcan4_GetSizeOfActiveSendData()) /* CM_CAN_HL20 */ /* COV_CAN_GENDATA_FAILURE */
  { /* #20 secure SilentBSW */
    activeSendData = 0u;
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)  
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_WRITE_ID, CAN_30_FLEXCAN4_E_PARAM_HANDLE);
# endif
  }
#endif
  return activeSendData;
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_RamCheckMailboxNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_RamCheckMailboxNotification
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RamCheckMailboxNotification( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */ /* COV_CAN_RAM_CHECK_NO_STIMULATION */
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 deactivate mailbox */
  Can_30_Flexcan4_SetMailboxState(initPara->mailboxHandle, kCan_30_Flexcan4Failed); /* SBSW_CAN_HL34 */
  /* #20 deactivate controller */
  Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL02 */
  /* #30 notify application about corrupt mailbox */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
  CanIf_30_Flexcan4_RamCheckCorruptMailbox((uint8)Can_30_Flexcan4_GetCanToCanIfChannelMapping(channel), initPara->mailboxHandle);
# else
#  if defined(C_30_FLEXCAN4_ENABLE_NOTIFY_CORRUPT_MAILBOX) /* with Mailbox Notification */
  Appl_30_Flexcan4_CanCorruptMailbox((uint8)channel, (initPara->hwObjHandle));
#  endif
# endif
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitBegin
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitBegin
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitBegin( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Failed; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  if (CanLL_30_Flexcan4_InitBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara) == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL31 */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
    /* #20 Execute pattern RamCheck */
    if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    {
      initPara->isChRamCheckFail |= CAN_30_FLEXCAN4_RAM_CHECK_BEGIN_REG_RESULT(channel, initPara); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL31 */ /* COV_CAN_HARDWARE_FAILURE */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
    } 
#endif
    /* #30 Initialize controller registers (Begin section) */
    initOk = CanLL_30_Flexcan4_InitBeginSetRegisters(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
  }
  return initOk;
}

#if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitTxFullCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitTxFullCAN
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitTxFullCAN( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
  Can_30_Flexcan4ObjectHandle activeSendObject;
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif
  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Ok;
  /* #10 iterate over all FullCAN TX */
  for (initPara->mailboxHandle = Can_30_Flexcan4_GetTxFullHandleStart(canHwChannel); initPara->mailboxHandle < Can_30_Flexcan4_GetTxFullHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #20 initialize FullCAN TX mailboxes */
    initPara->hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
    activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(initPara->mailboxHandle, 0u);
    Can_30_Flexcan4_SetActiveSendState(activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #30 RAM check for FullCAN TX mailboxes */
    isMbRamCheckFail = kCan_30_Flexcan4False;
    if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* SBSW_CAN_HL31 */ /* COV_CAN_HARDWARE_FAILURE */
    } 
# endif
    /* TX initialization */
    mailboxState = CanLL_30_Flexcan4_InitMailboxTx(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCan_30_Flexcan4Failed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCan_30_Flexcan4True; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_INIT) != CAN_30_FLEXCAN4_STATUS_INIT )
    { /* PowerOn */
      Can_30_Flexcan4_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCan_30_Flexcan4True) || (Can_30_Flexcan4_GetMailboxState(initPara->mailboxHandle) == kCan_30_Flexcan4Failed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_30_Flexcan4_InitMailboxTxDeactivate(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCan_30_Flexcan4True)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_30_Flexcan4_RamCheckMailboxNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitTxBasicCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitTxBasicCAN
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitTxBasicCAN( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
  Can_30_Flexcan4ObjectHandle activeSendObject;
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
#endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Ok;
  initPara->mailboxHandle = Can_30_Flexcan4_GetTxBasicHandleStart(canHwChannel); /* SBSW_CAN_HL36 */
#if defined(C_30_FLEXCAN4_ENABLE_MULTIPLE_BASICCAN_TX)
  for (; initPara->mailboxHandle < Can_30_Flexcan4_GetTxBasicHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */
#endif
  { /* #10 iterate over all logical BasicCAN TX */
#if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) 
    Can_30_Flexcan4ObjectHandle activeSendObjectStop;
#endif
    initPara->hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
    activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(initPara->mailboxHandle, 0u);
#if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) 
    activeSendObjectStop = activeSendObject + Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle);
    for (; activeSendObject < activeSendObjectStop; activeSendObject++)
#endif
    { /* #20 initialize BasicCAN TX mailboxes */
      Can_30_Flexcan4_SetActiveSendState(activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
    }
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #30 RAM check for BasicCAN TX mailboxes */
    isMbRamCheckFail = kCan_30_Flexcan4False;
    if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
#endif
    /* TX initialization */
    mailboxState = CanLL_30_Flexcan4_InitMailboxTx(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985,2986 */ /* MD_Can_ConstValue */
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCan_30_Flexcan4Failed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCan_30_Flexcan4True; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_INIT) != CAN_30_FLEXCAN4_STATUS_INIT )
    { /* PowerOn */
      Can_30_Flexcan4_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCan_30_Flexcan4True) || (Can_30_Flexcan4_GetMailboxState(initPara->mailboxHandle) == kCan_30_Flexcan4Failed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_30_Flexcan4_InitMailboxTxDeactivate(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCan_30_Flexcan4True)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_30_Flexcan4_RamCheckMailboxNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
#endif
  }
  return initOk;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */


#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitRxFullCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitRxFullCAN
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitRxFullCAN( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Ok;
  for (initPara->mailboxHandle = Can_30_Flexcan4_GetRxFullHandleStart(canHwChannel); initPara->mailboxHandle < Can_30_Flexcan4_GetRxFullHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #10 iterate over all FullCAN RX mailboxes */
    initPara->hwObjHandle=Can_30_Flexcan4_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #20 RAM check for FullCAN RX mailboxes */
    isMbRamCheckFail = kCan_30_Flexcan4False;
    if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
# endif
    /* #30 set ID for FullCAN RX mailboxes (LL) */
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
#  if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
    if ( ((Can_IdType)Can_30_Flexcan4_GetMailboxIDValue(initPara->mailboxHandle) & CAN_30_FLEXCAN4_ID_IDE_MASK) != CAN_30_FLEXCAN4_ID_IDE_MASK) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
      initPara->idType = kCan_30_Flexcan4IdTypeStd; /* SBSW_CAN_HL36 */
      SET_PARASTRUCT_IDRAW_RX_STD(initPara, Can_30_Flexcan4_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* PRQA S 4491 */ /* MD_Can_IntegerCast */
    }
    else
#  endif
    {
      initPara->idType = kCan_30_Flexcan4IdTypeExt; /* SBSW_CAN_HL36 */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      SET_PARASTRUCT_IDRAW_RX_EXT(initPara, Can_30_Flexcan4_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
    }
# else
    initPara->idType = kCan_30_Flexcan4IdTypeStd; /* SBSW_CAN_HL36 */
    SET_PARASTRUCT_IDRAW_RX_STD(initPara, Can_30_Flexcan4_GetMailboxIDValue(initPara->mailboxHandle)); /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */ /* PRQA S 4491,2985 */ /* MD_Can_IntegerCast */
# endif
    /* #40 initialize FullCAN RX mailboxes */
    mailboxState = CanLL_30_Flexcan4_InitMailboxRxFullCan(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCan_30_Flexcan4Failed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCan_30_Flexcan4True; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_INIT) != CAN_30_FLEXCAN4_STATUS_INIT )
    { /* PowerOn */
      Can_30_Flexcan4_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCan_30_Flexcan4True) || (Can_30_Flexcan4_GetMailboxState(initPara->mailboxHandle) == kCan_30_Flexcan4Failed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCan_30_Flexcan4True)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_30_Flexcan4_RamCheckMailboxNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitRxBasicCAN
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitRxBasicCAN
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitRxBasicCAN( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;
  uint8 mailboxState;
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
  uint8 isMbRamCheckFail;
# endif

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Ok;
  for (initPara->mailboxHandle = Can_30_Flexcan4_GetRxBasicHandleStart(canHwChannel); initPara->mailboxHandle < Can_30_Flexcan4_GetRxBasicHandleStop(canHwChannel); initPara->mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */ /* SBSW_CAN_HL36 */ /* SBSW_CAN_HL36 */
  { /* #10 iterate over all BasicCAN RX mailboxes */
    initPara->hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(initPara->mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_HL36 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #20 RAM check for BasicCAN RX mailboxes */
    isMbRamCheckFail = kCan_30_Flexcan4False;
    if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
    {
      isMbRamCheckFail = CAN_30_FLEXCAN4_RAM_CHECK_MAILBOX_RESULT(channel, initPara); /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_HL31 */
    } 
# endif
    /* #30 initialize BasicCAN RX mailboxes */
    mailboxState = CanLL_30_Flexcan4_InitMailboxRxBasicCan(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
    initOk &= mailboxState; /* all mailboxes OK - otherwise return fail */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    if(mailboxState == kCan_30_Flexcan4Failed) { /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
      isMbRamCheckFail = kCan_30_Flexcan4True; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    if( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_INIT) != CAN_30_FLEXCAN4_STATUS_INIT )
    { /* PowerOn */
      Can_30_Flexcan4_SetMailboxState(initPara->mailboxHandle, mailboxState); /* SBSW_CAN_HL34 */
    }
    if( (isMbRamCheckFail == kCan_30_Flexcan4True) || (Can_30_Flexcan4_GetMailboxState(initPara->mailboxHandle) == kCan_30_Flexcan4Failed) ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    { /* #45 deactivate corrupt mailbox */
      CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      if (isMbRamCheckFail == kCan_30_Flexcan4True)
      {  /* #50 notify application about corrupt mailbox and deactivate controller */
        CanHL_30_Flexcan4_RamCheckMailboxNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */
      }
    }
# endif
  }
  return initOk;
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_InitEnd_InitMode
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_InitEnd_InitMode
Relation_Context:
# CanHL_30_Flexcan4_ReInit() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_InitEnd_InitMode( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 initOk;

  /* ----- Implementation ----------------------------------------------- */
  initOk = kCan_30_Flexcan4Ok;

  /* #20 HW specific initialization end */
  initOk &= CanLL_30_Flexcan4_InitEndSetRegisters(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara); /* SBSW_CAN_HL31 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  initOk &= CanLL_30_Flexcan4_InitEnd( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara ); /* SBSW_CAN_HL31 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  return initOk;
}

#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_RxBasicCanPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_RxBasicCanPolling
Relation_Context:
# from Can_30_Flexcan4_MainFunction_Read #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxBasicCanPolling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  tCan_30_Flexcan4TaskParaStruct taskPara;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  if(CanLL_30_Flexcan4_RxBasicIsGlobalIndPending(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
    /* #20 loop over all BasicCAN mailboxes */
    for (taskPara.mailboxHandle = Can_30_Flexcan4_GetRxBasicHandleStart(canHwChannel); taskPara.mailboxHandle < Can_30_Flexcan4_GetRxBasicHandleStop(canHwChannel); taskPara.mailboxHandle++) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
    { /* loop over all BasicCAN mailboxes with gap */
      taskPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      /* #30 check individual polling flag */
      if ( Can_30_Flexcan4_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
      {
        /* #40 call LL handling */
        CanLL_30_Flexcan4_RxBasicProcessPendings(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
      }
    }
  }
}
# endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_RxFullCanPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_RxFullCanPolling
Relation_Context:
# from Can_30_Flexcan4_MainFunction_Read #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxFullCanPolling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  tCan_30_Flexcan4TaskParaStruct taskPara;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 check global pending flag (over all mailboxes - if available) */
  if(CanLL_30_Flexcan4_RxFullIsGlobalIndPending(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
  {
    /* #20 loop over all FullCAN mailboxes */
    for (taskPara.mailboxHandle = Can_30_Flexcan4_GetRxFullHandleStart(canHwChannel); taskPara.mailboxHandle < Can_30_Flexcan4_GetRxFullHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
    {
      taskPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
      /* #30 check individual polling flag */
      if ( Can_30_Flexcan4_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
      {
        /* #40 call LL handling */
        CanLL_30_Flexcan4_RxFullProcessPendings(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
      }
    }
  }
}
# endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS) || defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_RxMsgReceivedNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_RxMsgReceivedNotification
Relation_Context:
# from CanHL_30_Flexcan4_FullCanMsgReceived #
RxFullCANSupport
# from CanHL_30_Flexcan4_BasicCanMsgReceived #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, uint8, STATIC_CODE) CanHL_30_Flexcan4_RxMsgReceivedNotification( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST CanRxInfoStructPtr rxStructPtr ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* ----- Local Variables ---------------------------------------------- */
# if !defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE) && (CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_ON)
  Can_ReturnType generic_retval;
# endif
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 RX queue */
# if defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE)
  /**************************** RxQueue handling **********************************/
    /* #20 EXCLUSIVE_AREA_4 secure RxQueue data handling */
  SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_4();
  if (Can_30_Flexcan4_GetRxQueueInfo().Count < Can_30_Flexcan4_GetSizeOfRxQueueBuffer()) /* COV_CAN_GENDATA_FAILURE */
  { /* #30 Queue not full: */
    /* #45 copy HRH, ID, DLC and data to RX queue */
    Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().WriteIndex).Hrh = rxStructPtr->localMailboxHandle; /* SBSW_CAN_HL18 */
    Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().WriteIndex).id  = rxStructPtr->localId; /* SBSW_CAN_HL18 */
    Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().WriteIndex).dlc = rxStructPtr->localDlc; /* SBSW_CAN_HL18 */

#  if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
    if (((uint8) (sizeof(Can_30_Flexcan4_GetRxQueueBuffer(0).data) / sizeof(Can_30_Flexcan4_GetRxQueueBuffer(0).data[0]))) < rxStructPtr->localDlc) /* CM_CAN_HL17 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
    }
    else
#  endif
    {
      /* #46 copy data with VStdLib optimized copy routine */
      VStdMemCpy((void*)Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().WriteIndex).data, (void*)rxStructPtr->pChipData, rxStructPtr->localDlc); /* SBSW_CAN_HL19 */ /* PRQA S 0312,0314,0316 */ /* MD_Can_NoneVolatilePointerCast,MD_Can_PointerVoidCast,MD_Can_PointerVoidCast */
    }
      /* #47 increase pointer to next queue element */ 
    Can_30_Flexcan4_GetRxQueueInfo().Count++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    if (Can_30_Flexcan4_GetRxQueueInfo().WriteIndex < ((uint16)Can_30_Flexcan4_GetSizeOfRxQueueBuffer() - (uint16)1u) )
    {
      Can_30_Flexcan4_GetRxQueueInfo().WriteIndex++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    }
    else
    {
      Can_30_Flexcan4_GetRxQueueInfo().WriteIndex = 0u; /* CM_CAN_HL24 */ /* SBSW_CAN_HL55 */
    }
  }
#  if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  else 
  { /* #50 Queue full: last message will be lost -> overrun in queue do not change ReadIndex to override latest message because inconsistency problem (currently reading queue) */
    errorId = CAN_30_FLEXCAN4_E_RXQUEUE;
  }
#  endif
  SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_4();
# else /* C_30_FLEXCAN4_ENABLE_RX_QUEUE */
  /* #100 no RX queue */
  /* -------------------------- Interface + APPL Notifications --- */
    /* #110 call Appl_GenericPrecopy() depend on configuration and if mirror mode is active */
#  if (CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_ON)
#   if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
  if (Can_30_Flexcan4_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
  { /* do not call Appl_GenericPrecopy() in case mirror mode was disabled */
    generic_retval = CAN_OK;
  }
  else
#   endif
  {
    generic_retval = Appl_30_Flexcan4_GenericPrecopy((uint8)channel, (rxStructPtr->localId & CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT), rxStructPtr->localDlc, (Can_30_Flexcan4_DataPtrType)rxStructPtr->pChipData);  /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL07 */
  }
  if (generic_retval == CAN_OK) /* \trace SPEC-15119 */
#  endif
  { /* #120 call CanIf_RxIndication() if mirror mode is no active or Appl_GenericPrecopy() return OK \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
    CanIf_30_Flexcan4_RxIndication(rxStructPtr->localMailboxHandle, rxStructPtr->localId, rxStructPtr->localDlc, (Can_30_Flexcan4_DataPtrType)rxStructPtr->pChipData); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL07 */
  }
# endif
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return errorId;
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_RxQueueExecution
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
|<DataModelStart>| CanHL_30_Flexcan4_RxQueueExecution
Relation_Context:
# from Can_30_Flexcan4_MainFunction_Read #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_RxQueueExecution( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute RxQueue (over all channels) */
  while ( Can_30_Flexcan4_GetRxQueueInfo().Count != (uint16)0 )
  { /* #20 over all filled RxQueue elements */
# if (CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_ON)
    Can_ReturnType generic_retval;
    CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
    generic_retval = CAN_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_30_Flexcan4_GetMailboxController(Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).Hrh);
#   if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
    if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #30 Check controller is valid */
      errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
    }
    else
#   endif
#  endif
#  if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
    if (Can_30_Flexcan4_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
    { /* #40 MirrorMode: only when mirror is activated for this controller */
      generic_retval = CAN_OK;
    }
    else
#  endif
    { /* #50 call Appl_GenericPrecopy() */
      generic_retval = Appl_30_Flexcan4_GenericPrecopy( (uint8)channel,
                (Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).id & CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT),
                Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).dlc,
                (Can_30_Flexcan4_DataPtrType)Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).data ); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL06 */
    }
    if (generic_retval == CAN_OK)
# endif
    { /* #60 call CanIf_RxIndication() if Appl_GenericPrecopy() return OK or no generic precopy is used \trace SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687 */
      CanIf_30_Flexcan4_RxIndication( Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).Hrh, /* SBSW_CAN_HL06 */
              Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).id,
              Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).dlc,
              (Can_30_Flexcan4_DataPtrType)Can_30_Flexcan4_GetRxQueueBuffer(Can_30_Flexcan4_GetRxQueueInfo().ReadIndex).data ); /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */
    }
    /* #70 EXCLUSIVE_AREA_4 secure RxQueue handling */
    SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_4();
    /* #80 get next RxQueue element pointer */
    if (Can_30_Flexcan4_GetRxQueueInfo().ReadIndex >= ((uint16)Can_30_Flexcan4_GetSizeOfRxQueueBuffer() - (uint16)1u) ) /* CM_CAN_HL25 */
    {
      Can_30_Flexcan4_GetRxQueueInfo().ReadIndex = 0u; /* CM_CAN_HL25 */ /* SBSW_CAN_HL55 */
    }
    else
    {
      Can_30_Flexcan4_GetRxQueueInfo().ReadIndex++; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    }
    Can_30_Flexcan4_GetRxQueueInfo().Count--; /* SBSW_CAN_HL55 */ /* PRQA S 3387 */ /* MD_Can_HL_UsingIncOrDec */
    SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_4();
  }
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_MAINFCT_READ_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW) 
#endif


/****************************************************************************
| NAME:             CanLL_30_Flexcan4_InitMailboxWriteReg16
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxWriteReg16
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxWriteReg16( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr16 regPtr, vuint16 value, vuint16 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 result = kCan_30_Flexcan4False;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_LL03 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED) 
  if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
  {
    volatile vuint16 readBack = *regPtr;
    /* #20 read back value for RamCheck */
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    }
    CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_LL04 */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return result;
}

/****************************************************************************
| NAME:             CanLL_30_Flexcan4_WriteReg32
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_WriteReg32
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_WriteReg32( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr32 regPtr, vuint32 value, vuint32 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_LL03 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED) 
  if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
  {
    vuint8 result = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    volatile vuint32 readBack = *regPtr;
    /* #20 read back value for RamCheck */
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    }
    CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_LL04 */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/****************************************************************************
| NAME:             CanLL_30_Flexcan4_InitMailboxWriteReg32
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxWriteReg32
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ChipMsgPtr32 regPtr, vuint32 value, vuint32 readMask, CanInitParaStructPtr initPara ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 result = kCan_30_Flexcan4False;
  /* #10 write register value */
  *regPtr = value; /* SBSW_CAN_LL03 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED) 
  if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
  {
    volatile vuint32 readBack = *regPtr;
    /* #20 read back value for RamCheck */
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    }
    CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_LL04 */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return result;
}


/****************************************************************************
| NAME:             CanLL_30_Flexcan4_WriteProtectedReg16
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_WriteProtectedReg16
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_WriteProtectedReg16( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST vuint16 area, Can_30_Flexcan4ChipMsgPtr16 regPtr, vuint16 value, vuint16 readMask, CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 write register value */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG16(area, regPtr, value); /* SBSW_CAN_LL01 */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED) 
  if (initPara->doRamCheck == kCan_30_Flexcan4ExecuteRamCheck)
  {
    vuint8 result = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    volatile vuint16 readBack = CAN_30_FLEXCAN4_READ_PROTECTED_REG16(area, regPtr); /* SBSW_CAN_LL01 */
    /* #20 read back value for RamCheck */
    if ((readBack & readMask) != (value & readMask)) /* COV_CAN_HARDWARE_FAILURE */
    {
      /* #30 return issue when read back value differs */
      result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
    }
    CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(channel)
    initPara->isChRamCheckFail |= result; /* SBSW_CAN_LL04 */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(readMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(area); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* PRQA S 6060 */ /* MD_MSR_STPAR */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitBegin
Relation_Context:
# CanHL_30_Flexcan4_InitBegin() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBegin( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 returnValue;


  /* #10 Set FlexCAN to Init Mode */
  returnValue = CanLL_30_Flexcan4_SetFlexCANToInitMode(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST initPara->initObject);
  if (returnValue == kCan_30_Flexcan4Ok) /* COV_CAN_LL_HARDWARE_BEHAVIOUR */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
  {
    /* #20 Perform a soft reset */
    returnValue = CanLL_30_Flexcan4_ExecuteSoftReset(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
  }

  return(returnValue);
}
/* CODE CATEGORY 4 END */

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt
Relation_Context:
# CanHL_30_Flexcan4_InitBegin() #
OneChOpt, ChannelAmount, RamCheck
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */

{
    /* ----- Local Variables ---------------------------------------------- */
  vuint16 tmpCANmaxmb;
  vuint8 loopRamCheckCounter;
  vuint8 result;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  vuint32 tmpRamCheckPattern;
# if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
  tCan_30_Flexcan4ParityCheckPtr pParityCheckLocal = pCan_30_Flexcan4ParityCheck(canHwChannel);
# endif
  
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Load MAXMB Pattern and remove FD enable bit if FD is not supported on this channel */
  tmpCANmaxmb = kCan_30_Flexcan4RamCheckMaskMAXMB;
# if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
  if (!Can_30_Flexcan4_IsHasCANFDBaudrateOfControllerConfig(canHwChannel)) /* COV_CAN_FD_BAUDRATE_AVAILABILITY */
  {
    /* Remove FD Flag in case of HwChannel does not support CANFD */
    tmpCANmaxmb &= ((vuint16) ~kFlexCAN_30_Flexcan4_FDEN);
  }
# endif
  /* #20 Check CLKSRC bit in control1 register */
# if defined( C_30_FLEXCAN4_ENABLE_CLKSRC_SELECTION ) /* COV_CAN_RAMCHECK_EXTENDED_CLKSRC_AVAILABILITY */
  result = CanLL_30_Flexcan4_CheckClockSource(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# else
  result = kCan_30_Flexcan4False;
# endif

  /* #30 Iterate test patterns */
  for (loopRamCheckCounter = 0u; loopRamCheckCounter < 3u; loopRamCheckCounter++)
  {
    /* #40 Write MCR and MAXMB register */
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kCan_30_Flexcan4RamCheckMaskMCR); /* SBSW_CAN_LL01 */
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, (Can_30_Flexcan4RamCheckPattern16[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskMCR)); /* SBSW_CAN_LL01 */
    
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmaxmb, tmpCANmaxmb); /* SBSW_CAN_LL01 */
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmaxmb, (Can_30_Flexcan4RamCheckPattern16[loopRamCheckCounter] & tmpCANmaxmb)); /* SBSW_CAN_LL01 */
    
    /* #50 Check the register contents */
    if(  ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kCan_30_Flexcan4RamCheckMaskMCR) != (Can_30_Flexcan4RamCheckPattern16[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskMCR))
      || ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmaxmb) & tmpCANmaxmb) != (Can_30_Flexcan4RamCheckPattern16[loopRamCheckCounter] & tmpCANmaxmb))) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL01 */ /* SBSW_CAN_LL01 */  /* PRQA S 3415 */ /* MD_Can_RuntimeOptimization_LL */
    {
      result = kCan_30_Flexcan4True;
      break; /* no need to carry on if check already failed */
    }
  }

# if defined C_30_FLEXCAN4_ENABLE_FLEXCAN_USER_MODE
  /* #60 Set all CAN register except the MCR register in unrestricted memory space */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_SUPV); /* SBSW_CAN_LL01 */
# endif

  /* #70 Set WRNEN flag in CAN_30_FLEXCAN4_MCR register to check TWRNMSK and RWRNMASK in CTRL1 register */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_WRN_EN); /* SBSW_CAN_LL01 */

  /* #80 Iterate test patterns */
  for (loopRamCheckCounter = 0u; loopRamCheckCounter < 3u; loopRamCheckCounter++)
  {
    /* #90 Write CTRL1 register */
    pFlex_30_Flexcan4CANLocal->control1 &= ~kCan_30_Flexcan4RamCheckMaskCONTROL1; /* SBSW_CAN_LL01 */
    pFlex_30_Flexcan4CANLocal->control1 |= Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCONTROL1; /* SBSW_CAN_LL01 */
    
# if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
    /* enable ECRWRE flag in CTRL2 register to get write access for MECR register */
    pFlex_30_Flexcan4CANLocal->ctrl2 |= kFlexCAN_30_Flexcan4_ECRWRE; /* SBSW_CAN_LL01 */
    /* #100 Write MECR register if available */
    pParityCheckLocal->mecr &= (vuint32)(~kFlexCAN_30_Flexcan4_ECRWRDIS); /* SBSW_CAN_LL09 */
    pParityCheckLocal->mecr = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskMECR; /* SBSW_CAN_LL09 */
# endif

    /* #110 Write CTRL2 register */
    pFlex_30_Flexcan4CANLocal->ctrl2 = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCTRL2; /* SBSW_CAN_LL01 */
    
    /* #120 Write ECR register */
    pFlex_30_Flexcan4CANLocal->ecr = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskECR; /* SBSW_CAN_LL01 */
    
    /* #130 Write IMASK1 register */
    tmpRamCheckPattern = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter];

# if defined (C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) < 32u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    {
      tmpRamCheckPattern = (vuint32) Can_30_Flexcan4RamCheckPattern16[loopRamCheckCounter]; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
# endif
    pFlex_30_Flexcan4CANLocal->imask1 = tmpRamCheckPattern; /* SBSW_CAN_LL01 */
    
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #140 Write IMASK2 register */
    pFlex_30_Flexcan4CANLocal->imask2 = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter]; /* SBSW_CAN_LL01 */
# endif
  
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #150 Write IMASK3 register */
#  if defined (C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
    {
      pFlex_30_Flexcan4CANLocal->imask3 = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter]; /* SBSW_CAN_LL01 */
    }
# endif
    
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #160 Write IMASK4 register */
    pFlex_30_Flexcan4CANLocal->imask4 = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter]; /* SBSW_CAN_LL01 */
# endif

# if !defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
    /* #170 Write CBT register */
    pFlex_30_Flexcan4CANLocal->cbt = Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCBT; /* SBSW_CAN_LL01 */
# endif

    /* #190 Check the register contents  */
    if( ((pFlex_30_Flexcan4CANLocal->control1 & kCan_30_Flexcan4RamCheckMaskCONTROL1) != (Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCONTROL1))      ||
        ((pFlex_30_Flexcan4CANLocal->ecr & kCan_30_Flexcan4RamCheckMaskECR) != (Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskECR))                     ||
        ((pFlex_30_Flexcan4CANLocal->ctrl2 & kCan_30_Flexcan4RamCheckMaskCTRL2) != (Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCTRL2))               ||
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
         (pFlex_30_Flexcan4CANLocal->imask2 != Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter])                                                                 ||
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
         (pFlex_30_Flexcan4CANLocal->imask4 != Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter])                                                                 ||
# endif
# if !defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
        ((pFlex_30_Flexcan4CANLocal->cbt & kCan_30_Flexcan4RamCheckMaskCBT) != (Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskCBT))                     ||
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
        ((pParityCheckLocal->mecr & kCan_30_Flexcan4RamCheckMaskMECR) != (Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter] & kCan_30_Flexcan4RamCheckMaskMECR))  /* SBSW_CAN_LL09 */ ||
# endif
        (pFlex_30_Flexcan4CANLocal->imask1 != tmpRamCheckPattern)
      ) /* COV_CAN_HARDWARE_FAILURE */
    {
      result = kCan_30_Flexcan4True;
    }
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    else
    {
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
      if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
      {
        if(pFlex_30_Flexcan4CANLocal->imask3 != Can_30_Flexcan4RamCheckPattern32[loopRamCheckCounter]) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
        {
          result = kCan_30_Flexcan4True;
        }
      }
    }
# endif
    if (result == kCan_30_Flexcan4True) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
       break; /* no need to carry on if check already failed */
    }
  }
  CAN_30_FLEXCAN4_RAM_CHECK_READ_BACK_RESULT(channel);
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return result;
}
#endif /* C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitBeginSetRegisters
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitBeginSetRegisters
Relation_Context:
# CanHL_30_Flexcan4_InitBegin() #
OneChOpt, ChannelAmount
Relation:
HwCancelTx
CanFdSupport
StatusPolling
Tasd
SamplingMode
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitBeginSetRegisters( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  vuint16 tmpCANmaxmb, tmpCANmcr;
  vuint32 tmpCANctrl2, tmpCANcontrol1;
  vuint32 tmpErfcr;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);
#if defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
  tCan_30_Flexcan4EBTRegisterPtr pEBTRegister =  pCan_30_Flexcan4EBTRegister(canHwChannel);
#endif
#if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
  tCan_30_Flexcan4ParityCheckPtr pParityCheckLocal = pCan_30_Flexcan4ParityCheck(canHwChannel);
#endif

  /* #10 Configure CAN[MCR].MAXMB register value */
  tmpCANmaxmb = ((vuint16)CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(canHwChannel) - (vuint16)1u)
#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW )
  /* #11 Set AEN bit to enable Abort feature (Cancel in Hardware) */
  | kFlexCAN_30_Flexcan4_AEN
#endif
  ;
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
  if (Can_30_Flexcan4_IsHasCANFDBaudrateOfControllerConfig(channel)) /* COV_CAN_FD_BAUDRATE_AVAILABILITY */
  {
    /* #12 Set FDEN bit to enable CAN FD */
    tmpCANmaxmb |= kFlexCAN_30_Flexcan4_FDEN;
  }
#endif 
  /* #15 Write configured value to register */
  CanLL_30_Flexcan4_WriteProtectedReg16(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_AREA(canHwChannel), &pFlex_30_Flexcan4CANLocal->canmaxmb, tmpCANmaxmb, kCan_30_Flexcan4RamCheckMaskMAXMB, initPara); /* SBSW_CAN_LL01 */
  
  /* #20 Configure CAN[MCR].MCR register value */
  tmpCANmcr = CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kNotFlexCAN_30_Flexcan4_MCR_CFG_BTIS; /* SBSW_CAN_LL01 */
  /* #21 Set IRMQ bit to allow individual buffer masking configuration */
  tmpCANmcr |= kFlexCAN_30_Flexcan4_IRMQ
  /* #22 Set SRXDIS bit to disable self-reception feature */
  | kFlexCAN_30_Flexcan4_SRX_DIS
  ;
  /* #29 Write configured value to register */
  CanLL_30_Flexcan4_WriteProtectedReg16(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_AREA(canHwChannel), &pFlex_30_Flexcan4CANLocal->canmcr, tmpCANmcr, kCan_30_Flexcan4RamCheckMaskMCR, initPara); /* SBSW_CAN_LL01 */

  /* #30 Disable enhanced and extended bit timing */
  pFlex_30_Flexcan4CANLocal->ctrl2 = pFlex_30_Flexcan4CANLocal->ctrl2 & kNotFlexCAN_30_Flexcan4_BTE; /* SBSW_CAN_LL01 */
  pFlex_30_Flexcan4CANLocal->cbt = pFlex_30_Flexcan4CANLocal->cbt & kNotFlexCAN_30_Flexcan4_BTF; /* SBSW_CAN_LL01 */

  /* #31 Configure CAN[CTRL1] register value */
  tmpCANcontrol1 = Can_30_Flexcan4_GetControl1OfInitObject(initPara->initObject) & (kFlexCAN_30_Flexcan4_CLK_SRC | kFlexCAN_30_Flexcan4_SMP);
  /* #33 Disable BusOff Auto Recovery */
  tmpCANcontrol1 |= kFlexCAN_30_Flexcan4_BOFF_REC;
# if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )  
  /* #34 Enable BusOff interrupt */
  if(Can_30_Flexcan4_GetCanInterruptCounter(channel) == 0u)
  {
    tmpCANcontrol1 |= kFlexCAN_30_Flexcan4_BOFF_MSK;

#  if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
    /* #35 Enable Error interrupt */
    tmpCANcontrol1 |= kFlexCAN_30_Flexcan4_ERR_MSK;
#  endif
  }
# endif
  /* #39 Write configured value to register */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->control1, tmpCANcontrol1, kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */

#if !defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
  /* #40 Write CAN[CBT] register value (extended bit timing) */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->cbt, Can_30_Flexcan4_GetCBTOfInitObject(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
  if (Can_30_Flexcan4_IsHasCANFDBaudrateOfControllerConfig(canHwChannel)) /* COV_CAN_FD_BAUDRATE_AVAILABILITY */
  {
    tCan_30_Flexcan4FDRegisterPtr pCan_30_Flexcan4FDRegisterLocal = pCan_30_Flexcan4FDRegister(canHwChannel);
    /* #50 Write CAN[FDCTRL] register value (CAN FD control) */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pCan_30_Flexcan4FDRegisterLocal->fdctrl, Can_30_Flexcan4_GetFDCTRLOfInitObjectFD(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
# if !defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
    /* #60 Write CAN[FDCBT] register value (CAN FD bit timing) */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pCan_30_Flexcan4FDRegisterLocal->fdcbt, Can_30_Flexcan4_GetFDCBTOfInitObjectFD(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
# endif
  }
#endif

  /* #71 Configure CAN[CTRL2] register value */
  tmpCANctrl2 = kFlexCAN_30_Flexcan4_MRP | kFlexCAN_30_Flexcan4_RRS | kFlexCAN_30_Flexcan4_EACEN
  /* #72 Set ECR Write Enable */
#if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
  | kFlexCAN_30_Flexcan4_ECRWRE
#endif
  /* #73 Set TASD Value */
#if defined( C_30_FLEXCAN4_ENABLE_TASD )
  | Can_30_Flexcan4_GetTASDOfInitObject(initPara->initObject)
#else
  | kFlexCAN_30_Flexcan4_TASD_DEFAULT
#endif
#if defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
  | kFlexCAN_30_Flexcan4_BTE
#endif
  ;
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
# if defined (C_30_FLEXCAN4_ENABLE_ISO_CANFD)
  /* #74 Set CAN ISO FD */
  if (Can_30_Flexcan4_IsHasCANFDBaudrateOfControllerConfig(channel)) /* COV_CAN_FD_BAUDRATE_AVAILABILITY */
  {
    tmpCANctrl2 |= kFlexCAN_30_Flexcan4_STFCNTEN;
  }
# endif
#endif
  /* #78 Write configured value to register */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->ctrl2, tmpCANctrl2, kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */

#if defined ( C_30_FLEXCAN4_ENABLE_ENHANCED_BITTIMING )
  /* #79 Write Enhanced Bit Timing Registers */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pEBTRegister->eprs, Can_30_Flexcan4_GetEPRSOfInitObjectEBT(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pEBTRegister->encbt, Can_30_Flexcan4_GetENCBTOfInitObjectEBT(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pEBTRegister->edcbt, Can_30_Flexcan4_GetEDCBTOfInitObjectEBT(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pEBTRegister->etdc, Can_30_Flexcan4_GetETDCOfInitObjectEBT(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
#endif

#if defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK_AVAILABLE )
  /* #80 Enable write on MECR register */
  pParityCheckLocal->mecr &= (vuint32)(~kFlexCAN_30_Flexcan4_ECRWRDIS); /* SBSW_CAN_LL09 */
  /* #81 Configuration of MECR register */
# if !defined ( C_30_FLEXCAN4_ENABLE_FLEXCAN_PARITY_CHECK )
  /* #82 Disable parity check functionality (not supported but enabled by default) */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pParityCheckLocal->mecr, kFlexCAN_30_Flexcan4_Disable_ParityCheck, kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL09 */
# endif
  /* #83 Disable write on MECR register */
  pParityCheckLocal->mecr |= kFlexCAN_30_Flexcan4_ECRWRDIS; /* SBSW_CAN_LL09 */
  /* #84 Clear Error Status register */
  pParityCheckLocal->errStatusR = kFlexCAN_30_Flexcan4_Clear_Parity_Status; /* SBSW_CAN_LL09 */
#endif

  /* #90 Configure enhanced Rx Fifo */
  tmpErfcr = Can_30_Flexcan4_GetERFCR(initPara->initObject) & kFlexCAN_30_Flexcan4_ERFEN;
  /* #91 Enable Enhanced Rx Fifo */
  CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pERxFifoRegister->erfcr, tmpErfcr, kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
  if (tmpErfcr == kFlexCAN_30_Flexcan4_ERFEN) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
  {
    /* #92 Clear Enhanced Rx Fifo erfsr register */
    pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERFCLR; /* SBSW_CAN_LL01 */
    pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERF_CLEAR_STATUS; /* SBSW_CAN_LL01 */
    /* #93 Configure Enhanced Rx Fifo erfcr register */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pERxFifoRegister->erfcr, Can_30_Flexcan4_GetERFCR(initPara->initObject), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
  }


  return(kCan_30_Flexcan4Ok);
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxTx
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxTx
Relation_Context:
# CanHL_30_Flexcan4_InitTxFullCAN() #
OneChOpt, ChannelAmount
# CanHL_30_Flexcan4_InitTxBasicCAN() #
OneChOpt, ChannelAmount
Relation:
TxFullCANSupport
MultiplexedTx,DevErrorDetect
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxTx( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  vuint8 retVal = kCan_30_Flexcan4Failed; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
#if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
#endif


  /* #10 Configure TX mailbox to INACTIVE */
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  if ( (initPara->mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX(canHwChannel)) && (initPara->mailboxHandle < CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STOPINDEX(canHwChannel)) ) /* COV_CAN_MAILBOX_ORDER */
  {
    Can_30_Flexcan4ObjectHandle mailboxElement;
#  if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
    /* #20 Check for valid hwObject handle (multiplexed transmission) */
    if (CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(canHwChannel, (initPara->hwObjHandle + Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle)) - 1u) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#  endif
    {
      retVal = kCan_30_Flexcan4Ok;
      for (mailboxElement = 0u; mailboxElement < Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle); mailboxElement++)
      {
        retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg16( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, (vuint16)(initPara->hwObjHandle) + (vuint16)mailboxElement, control), kTx_30_Flexcan4CodeInactive, kCan_30_Flexcan4RamCheckMailboxControl, initPara ); /* PRQA S 2985 */ /* MD_Can_ConstValue */ /* SBSW_CAN_LL02 */
      }
    }
  }
  else
# endif
  {
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
    /* #30 Check for valid hwObject handle */
    if (CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(canHwChannel, initPara->hwObjHandle) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
# endif
    {
      retVal = kCan_30_Flexcan4Ok;
      retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg16( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, control), kTx_30_Flexcan4CodeInactive, kCan_30_Flexcan4RamCheckMailboxControl, initPara ); /* SBSW_CAN_LL02 */
    }
  }

  return(retVal);
}
/* CODE CATEGORY 4 END */

# if (defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK ))
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxTxDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxTxDeactivate
Relation_Context:
# CanHL_30_Flexcan4_InitTxFullCAN() #
OneChOpt, ChannelAmount
# CanHL_30_Flexcan4_InitTxBasicCAN() #
OneChOpt, ChannelAmount
Relation:
TxFullCANSupport, RamCheck
MultiplexedTx, RamCheck
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
*/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxTxDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
#  if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
#  endif

  /* #10 Deactivate transmit mailbox */
#  if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  if ( (initPara->mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX(canHwChannel)) && (initPara->mailboxHandle < CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STOPINDEX(canHwChannel)) ) /* COV_CAN_MAILBOX_ORDER */
  {
    Can_30_Flexcan4ObjectHandle mailboxElement;
    for (mailboxElement = 0u; mailboxElement < Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle); mailboxElement++)
    {
      CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, (vuint16)initPara->hwObjHandle + (vuint16)mailboxElement, control) = 0u; /* set all mailboxes inactive */ /* SBSW_CAN_LL02 */
    }
  }
  else
#  endif
  {
    CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, control) = 0u; /* SBSW_CAN_LL02 */
  }
}
/* CODE CATEGORY 4 END */
# endif


#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxFullCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxRxFullCan
Relation_Context:
# CanHL_30_Flexcan4_InitRxFullCAN() #
OneChOpt, ChannelAmount, RxFullCANSupport
Relation:
CanFdSupport
DevErrorDetect
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
*/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxFullCan( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara )
{
  vuint8 retVal = kCan_30_Flexcan4Failed; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
# if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
# endif
  
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  /* #10 Check for valid hwObject handle */
  if (CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(canHwChannel, initPara->hwObjHandle) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
# endif
  {
    tCan_30_Flexcan4RxMaskPtr pIndivRxMask = pCan_30_Flexcan4RxMask(canHwChannel);
    retVal = kCan_30_Flexcan4Ok;

    /* #20 Set message ID */
    retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, msgID), CAN_30_FLEXCAN4_MSGID(initPara->idRaw0), kCan_30_Flexcan4MaskAll32, initPara ); /* SBSW_CAN_LL02 */
    /* #30 set ID type and set mailbox ready for receive */
    retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg16( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, control), kRx_30_Flexcan4CodeEmpty | (vuint16)(initPara->idType), kCan_30_Flexcan4RamCheckMailboxControl, initPara ); /* SBSW_CAN_LL02 */

    /* #40 Initialize individual mask values for each FullCAN mailbox */
# if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
#  if defined( C_30_FLEXCAN4_ENABLE_MIXED_ID )
    if((CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, control) & (vuint16)kCan_30_Flexcan4IdTypeExt) == 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* SBSW_CAN_LL02 */
    {
      retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pIndivRxMask->indivRxMask[initPara->hwObjHandle], kCan_30_Flexcan4RxMaskStd, kCan_30_Flexcan4MaskAll32, initPara ); /* SBSW_CAN_LL09 */
    }
    else
#  endif
    {
      retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pIndivRxMask->indivRxMask[initPara->hwObjHandle], kCan_30_Flexcan4RxMaskExt, kCan_30_Flexcan4MaskAll32, initPara ); /* SBSW_CAN_LL09 */
    }
# else
    retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pIndivRxMask->indivRxMask[initPara->hwObjHandle], kCan_30_Flexcan4RxMaskStd, kCan_30_Flexcan4MaskAll32, initPara ); /* SBSW_CAN_LL09 */
# endif
  }

  return(retVal);
}
/* CODE CATEGORY 4 END */

# if ((defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )))
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate
Relation_Context:
# CanHL_30_Flexcan4_InitRxFullCAN() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxFullCanDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 Deactivate receive mailbox */
  CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, initPara->hwObjHandle, control) = 0u; /* SBSW_CAN_LL02 */
}
/* CODE CATEGORY 4 END */
# endif

#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

#if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS )  /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxBasicCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxRxBasicCan
Relation_Context:
# CanHL_30_Flexcan4_InitRxBasicCAN() #
Wakeup, OneChOpt, ChannelAmount, RxBasicCANSupport
Relation:
CanFdSupport,DevErrorDetect,IDType
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxBasicCan( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara)
{
  vuint8 retVal = kCan_30_Flexcan4Failed; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */

#  if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
    /* #10 Check for valid Enhanced Rx Fifo Filter configuration */
    if (CanLL_30_Flexcan4_CheckEnhancedRxFifoFilter(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST initPara->initObject) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#  endif
    {
      vuint16 filterIndex;
      vuint16 idx;
      vuint16 idxStdStart = Can_30_Flexcan4_GetInitBasicCanIndexStd(initPara->initObject);
      vuint16 idxStdStop = Can_30_Flexcan4_GetInitBasicCanIndexStd(initPara->initObject + 1u);
      vuint16 idxExtStart = Can_30_Flexcan4_GetInitBasicCanIndexExt(initPara->initObject);
      vuint16 idxExtStop = Can_30_Flexcan4_GetInitBasicCanIndexExt(initPara->initObject + 1u);
      tCan_30_Flexcan4ERxFifoFilterPtr pERxFifoFilter = pCan_30_Flexcan4ERxFifoFilter(canHwChannel);
      retVal = kCan_30_Flexcan4Ok;

      /* Add extended ID filter first */
      filterIndex = 0;
      for (idx = idxExtStart; idx < idxExtStop; idx++) /* COV_CAN_ENHANCED_FILTERTABLE_GENERATION */
      {
        retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pERxFifoFilter->filterElement[filterIndex], Can_30_Flexcan4_GetInitCodeExtOfInitBasicCanExt(idx), kCan_30_Flexcan4MaskAll32, initPara ); /* PRQA S 2985 */ /* MD_Can_ConstValue */ /* SBSW_CAN_LL01 */
        filterIndex++;
        retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pERxFifoFilter->filterElement[filterIndex], Can_30_Flexcan4_GetInitMaskExtOfInitBasicCanExt(idx), kCan_30_Flexcan4MaskAll32, initPara ); /* PRQA S 2985 */ /* MD_Can_ConstValue */ /* SBSW_CAN_LL01 */
        filterIndex++;
      }

      /* #30 Add standard ID filter second */
      for (idx = idxStdStart; idx < idxStdStop; idx++) /* COV_CAN_ENHANCED_FILTERTABLE_GENERATION */
      {
        retVal &= ~CanLL_30_Flexcan4_InitMailboxWriteReg32( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &pERxFifoFilter->filterElement[filterIndex], Can_30_Flexcan4_GetInitCodeMaskStdOfInitBasicCanStd(idx), kCan_30_Flexcan4MaskAll32, initPara ); /* PRQA S 2985 */ /* MD_Can_ConstValue */ /* SBSW_CAN_LL01 */
        filterIndex++;
      }
  }

  return(retVal);
}
/* CODE CATEGORY 4 END */

# if ((defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )))
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate
Relation_Context:
# CanHL_30_Flexcan4_InitRxBasicCAN() #
Wakeup, OneChOpt, ChannelAmount, RxBasicCANSupport, RamCheck
Relation:
CanFdSupport, RxFullCANSupport
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMailboxRxBasicCanDeactivate( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);

  if ((pERxFifoRegister->erfcr & kFlexCAN_30_Flexcan4_ERFEN) != 0u) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
  {
    /* #10 Clear Enhanced Rx Fifo */
    pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERFCLR; /* SBSW_CAN_LL01 */
    pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERF_CLEAR_STATUS; /* SBSW_CAN_LL01 */
    /* #20 Disable Enhanced Rx Fifo */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pERxFifoRegister->erfcr, kFlexCAN_30_Flexcan4_ERFEN_DISABLE, kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
  }

  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 4 END */
# endif
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitEndSetRegisters
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitEndSetRegisters
Relation_Context:
# CanHL_30_Flexcan4_InitEnd_InitMode() #
OneChOpt, ChannelAmount
Relation:
CanFdSupport,RxPolling,IndividualPolling
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitEndSetRegisters( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);

  if (Can_30_Flexcan4_GetCanInterruptCounter(channel) == 0u)
  {
    /* #5 Configure CAN[ERFIER] register value */
    if ((pERxFifoRegister->erfcr & kFlexCAN_30_Flexcan4_ERFEN) != 0u) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
    {
      CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pERxFifoRegister->erfier, Can_30_Flexcan4_GetErfdaInterruptMaskOfControllerConfig(canHwChannel), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
    }

    /* #10 Configure CAN[IMASK1] register value */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->imask1, CAN_30_FLEXCAN4_INT_MASK1(canHwChannel), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #20 Configure CAN[IMASK2] register value */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->imask2, CAN_30_FLEXCAN4_INT_MASK2(canHwChannel), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
# endif  
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
    {
      /* #30 Configure CAN[IMASK3] register value */
      CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->imask3, CAN_30_FLEXCAN4_INT_MASK3(canHwChannel), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
    }
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #40 Configure CAN[IMASK4] register value */
    CanLL_30_Flexcan4_WriteReg32(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &pFlex_30_Flexcan4CANLocal->imask4, CAN_30_FLEXCAN4_INT_MASK4(canHwChannel), kCan_30_Flexcan4MaskAll32, initPara); /* SBSW_CAN_LL01 */
# endif
  }

  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return(kCan_30_Flexcan4Ok);
}
/* CODE CATEGORY 4 END */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitEnd
Relation_Context:
# CanHL_30_Flexcan4_InitEnd_InitMode() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitEnd( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 Nothing to do */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(initPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return(kCan_30_Flexcan4Ok);
}
/* CODE CATEGORY 4 END */

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitIsMailboxCorrupt
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitIsMailboxCorrupt
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
Wakeup, OneChOpt, ChannelAmount
Relation:
RamCheck
DevErrorDetect
CanFdSupport
RxBasicCANSupport
RxFullCANSupport
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitIsMailboxCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanInitParaStructPtr initPara ) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
   /* ----- Local Variables ---------------------------------------------- */
  vuint8    result, idx, wordLength, i;

  Can_30_Flexcan4ObjectHandle hwObjHandle;
  Can_30_Flexcan4ObjectHandle hwObjCount;

# if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
#  if defined (C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
  tCan_30_Flexcan4RxMaskPtr pIndivRxMask = pCan_30_Flexcan4RxMask(canHwChannel);
#  endif
# endif

# if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
# endif

  /* ----- Implementation ----------------------------------------------- */
# if defined (C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
  /* #5 Check enhanced Rx Fifo */
  if ( (initPara->mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_RX_BASIC_STARTINDEX(canHwChannel)) && (initPara->mailboxHandle < CAN_30_FLEXCAN4_HL_MB_RX_BASIC_STOPINDEX(canHwChannel)) ) /* COV_CAN_MAILBOX_ORDER */
  {
    result = CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY);
  }
  else
# endif
  {
    result = kCan_30_Flexcan4False;

# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
    /* #10 Check for valid hwObject handle */
    if (CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(canHwChannel, (initPara->hwObjHandle + Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle)) - 1u) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
# endif
    {
      hwObjHandle = initPara->hwObjHandle;

      for (hwObjCount = 0u; hwObjCount < Can_30_Flexcan4_GetMailboxSize(initPara->mailboxHandle); hwObjCount++)
      {
        /* #20 DLC and IDE register check */
        CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = (vuint16)0x0055u; /* SBSW_CAN_LL02 */
        if((CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) & (vuint16)0x007Fu) != (vuint16)0x0055u) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL02 */
        {
          result = kCan_30_Flexcan4True;
        }
        CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = (vuint16)0x002Au; /* SBSW_CAN_LL02 */
        if((CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) & (vuint16)0x007Fu) != (vuint16)0x002Au) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL02 */
        {
          result = kCan_30_Flexcan4True;
        }
        CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = (vuint16)0x0000u; /* SBSW_CAN_LL02 */
        if((CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) & (vuint16)0x007Fu) != (vuint16)0x0000u) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL02 */
        {
          result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
        }

       /* #30 Iterate test patterns */
        for(i = 0u; i < 3u; i++)
        {
          /* #40 ID register check */
          CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, msgID) = Can_30_Flexcan4RamCheckPattern32[i]; /* SBSW_CAN_LL02 */
          if((CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, msgID) != Can_30_Flexcan4RamCheckPattern32[i])) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL02 */
          {
            result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
          }
          /* #50 Data register check */
# if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
          wordLength = CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(canHwChannel);
# else
          wordLength = 2u; /* check 8 data bytes */
# endif
          for (idx = 0; idx < wordLength; idx++)
          {
            CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, data[idx]) = Can_30_Flexcan4RamCheckPattern32[i]; /* SBSW_CAN_LL02 */
            if(CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, data[idx]) != Can_30_Flexcan4RamCheckPattern32[i]) /* COV_CAN_HARDWARE_FAILURE */ /* SBSW_CAN_LL02 */
            {
              result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
            }
          }
        }
        hwObjHandle++;
      }

# if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
      if (result == kCan_30_Flexcan4False) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
      {
#  if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
        /* #80 Check RX FullCAN objects */
        if ( (initPara->mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_RX_FULL_STARTINDEX(canHwChannel)) && (initPara->mailboxHandle < CAN_30_FLEXCAN4_HL_MB_RX_FULL_STOPINDEX(canHwChannel)) ) /* COV_CAN_MAILBOX_ORDER */
        {
          for(i = 0u; i < 3u; i++)
          {
            pIndivRxMask->indivRxMask[initPara->hwObjHandle] = Can_30_Flexcan4RamCheckPattern32[i]; /* SBSW_CAN_LL09 */
            if(pIndivRxMask->indivRxMask[initPara->hwObjHandle] != Can_30_Flexcan4RamCheckPattern32[i]) /* COV_CAN_HARDWARE_FAILURE */
            {
              result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
            }
          }
        }
#  endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */
      }
# endif
    }
  }
  return result;
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
/* CODE CATEGORY 4 END */
#endif /* C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitPowerOn
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitPowerOn
Relation_Context:
# from Can_30_Flexcan4_Init #
Relation:
DevErrorDetect,CanFdSupport
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitPowerOn(void)
{
  vuint8 retVal = kCan_30_Flexcan4Ok; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
#if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  Can_30_Flexcan4ChannelHandle chan;

  /* #10 SafeBSW check for maximum number of loops */
  if (kCan_30_Flexcan4LoopResetInit >= (vuint8)CAN_30_FLEXCAN4_LOOP_MAX)/* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL02 */ /* PRQA S 2742 */ /* MD_Can_ConstValue */
  {
    retVal = kCan_30_Flexcan4False; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }

  for (chan = 0u; chan < kCan_30_Flexcan4NumberOfChannels; chan++)
  {
# if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
    if ( CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(chan) > 16u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL10 */
    {
      retVal = kCan_30_Flexcan4False;
    }
    if ( sizeof( Can_30_Flexcan4_GetRxMsgBufferOfControllerData(chan).data ) < 64u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL01 */ /* PRQA S 2742 */ /* MD_Can_ConstValue */
    {
      retVal = kCan_30_Flexcan4False; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
# else
    if ( sizeof( Can_30_Flexcan4_GetRxMsgBufferOfControllerData(chan).data ) < 8u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL01 */ /* PRQA S 2742 */ /* MD_Can_ConstValue */
    {
      retVal = kCan_30_Flexcan4False; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
# endif
  }
#endif

  return (retVal);
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitPowerOnChannelSpecific
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitPowerOnChannelSpecific
Relation_Context:
# from Can_30_Flexcan4_Init #
Relation:
ChannelAmount, OneChOpt
VirtualAdressing
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitPowerOnChannelSpecific(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
  /* #20 Get virtual base address of FlexCAN controller */
# if defined (C_30_FLEXCAN4_MULTIPLE_RECEIVE_CHANNEL)
  canLL_30_Flexcan4_VirtualPtrField[channel] = (vuint32 *)(Appl_30_Flexcan4CanPowerOnGetBaseAddress((vuint32)(Can_30_Flexcan4_GetBaseAddressOfControllerConfig(channel)), FLEXCAN4_CONTROLLER_MEMORY_SIZE)); /* SBSW_CAN_LL06 */ /* PRQA S 0316 */ /* MD_Can_PointerVoidCast */
# else
  canLL_30_Flexcan4_VirtualPtrField[0] = (vuint32 *)(Appl_30_Flexcan4CanPowerOnGetBaseAddress((vuint32)(Can_30_Flexcan4_GetBaseAddressOfControllerConfig(0)), FLEXCAN4_CONTROLLER_MEMORY_SIZE)); /* SBSW_CAN_LL06 */ /* PRQA S 0316 */ /* MD_Can_PointerVoidCast */
# endif
#endif /* QWRAP / MDWRAP */


#if ( CAN_30_FLEXCAN4_REINIT_START == STD_ON )
  canLL_30_Flexcan4_canStartReinitState[channel] = kCan_30_Flexcan4LLStateStart; /* SBSW_CAN_LL14 */
#endif

#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  /* #30 initialize passive report flag */
  canLL_30_Flexcan4_PassiveReportFlag[channel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
#endif

  CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return (kCan_30_Flexcan4Ok);
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitMemoryPowerOn
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitMemoryPowerOn
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_InitMemoryPowerOn(void)
{
  /* #10 CanLL_30_Flexcan4_InitMemoryPowerOn: nothing to do here */
}
/* CODE CATEGORY 4 END */


#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxCancelInHw
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxCancelInHw
Relation_Context:
# from Can_30_Flexcan4_CancelTx #
TransmitCancellationAPI, TxFullCANSupport, HwCancelTx
# from CanHL_30_Flexcan4_WritePrepareCancelTx #
Relation:
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
*/
/* CODE CATEGORY 3 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxCancelInHw(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxCancellationParaStructPtr txCancellationPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 calculate mailbox to abort and issue ABORT to it */
#  if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  txCancellationPara->hwObjHandle += txCancellationPara->mailboxElement; /* SBSW_CAN_LL04 */
#  endif
  CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txCancellationPara->hwObjHandle, control) = kTx_30_Flexcan4CodeAbort; /* SBSW_CAN_LL02 */
}
/* CODE CATEGORY 3 END */
#endif



/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxBegin
Relation_Context:
# from Can_30_Flexcan4_Write #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_TxBegin: nothing to do here */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(txPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxSetMailbox
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxSetMailbox
Relation_Context:
# from Can_30_Flexcan4_Write #
Relation:
CanFdSupport
MultiplexedTx
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
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxSetMailbox(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
# if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
# endif
  vuint16 tmpControlVal;
  Can_30_Flexcan4ObjectHandle hwObjHandle;

  /* #10 Determine mailbox to be transmitted */
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  hwObjHandle = txPara->hwObjHandle + txPara->mailboxElement;
# else
  hwObjHandle = txPara->hwObjHandle;
# endif
  /* #20 Configure message ID */
# if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
  CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, msgID) = (vuint32)(txPara->idRaw0); /* SBSW_CAN_LL02 */
# else
  CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, msgID) = (vuint32)(txPara->idRaw0) << 16; /* SBSW_CAN_LL02 */
# endif
  /* #30 Configure control word */
  tmpControlVal = (vuint16)(((vuint16)txPara->dlcRaw & kTx_30_Flexcan4DlcMask) | kTx_30_Flexcan4CodeFree); /* write IDE,SRR and DLC */

# if defined( C_30_FLEXCAN4_ENABLE_MIXED_ID ) 
  if (txPara->idType == kCan_30_Flexcan4IdTypeExt) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    tmpControlVal |= (vuint16)(kCan_30_Flexcan4IdTypeExt | kCan_30_Flexcan4IdTypeSRR);
  }
# endif

# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
  tmpControlVal |= (vuint16)(txPara->fdType | txPara->fdBrsType);
# endif
  
  CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = tmpControlVal; /* SBSW_CAN_LL02 */
}
/* CODE CATEGORY 1 END */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxCopyToCan
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxCopyToCan
Relation_Context:
# from Can_30_Flexcan4_Write #
Relation:
CanFdSupport,DevErrorDetect
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
 *
 *
 *
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxCopyToCan(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
#  if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  vuint32     canFDTxBuf[16];
  vuint8 idx, idxByte, idxWord, dataWordLen, sduWordLen, sduResLen;
  vuint32 paddingWordValue;
#  else
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  vuint32     canTxBuf[2];
#  endif
  Can_30_Flexcan4ObjectHandle hwObjHandle;

  /* #10 Determine mailbox to be used */
#  if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  hwObjHandle = txPara->hwObjHandle + txPara->mailboxElement;
#  else
  hwObjHandle = txPara->hwObjHandle;
#  endif
#  if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_FULL)
  /* #20 Preparation for FD data copying */
#   if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if ((txPara->messageLen <= 64u) && (txPara->frameLen <= 64u)) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL11 */
#   endif
  {
    sduWordLen = txPara->messageLen >> 2;
    sduResLen = txPara->messageLen & 0x03u;
    Can_30_Flexcan4CopyFDPadding2Uint32(paddingWordValue, txPara->paddingVal);

    /* #30 Copy FD data into tx message object */
    idxByte = 0u;
    /* #40 Copy four bytes into one word */
    for (idxWord = 0u; idxWord < sduWordLen; idxWord++)
    {
      Can_30_Flexcan4CopyFDDataBytes2Uint32(canFDTxBuf[idxWord], &(txPara->CanMemCopySrcPtr[idxByte])); /* SBSW_CAN_LL11 */
      idxByte += 4u;
    }
    /* #50 Data bytes do not fit into a whole word, mix data bytes and padding value */
    if (sduResLen != 0u)
    {
      canFDTxBuf[idxWord] = 0u; /* SBSW_CAN_LL11 */
      for (idx = idxByte; idx < txPara->messageLen; idx++)
      {
        Can_30_Flexcan4Add1FDByte2Uint32(canFDTxBuf[idxWord], txPara->CanMemCopySrcPtr[idx]); /* SBSW_CAN_LL11 */ /* SBSW_CAN_LL11 */
      }
      for (idx = txPara->messageLen; idx < (txPara->messageLen + (4u - sduResLen)); idx++)
      {
        Can_30_Flexcan4Add1FDByte2Uint32(canFDTxBuf[idxWord], txPara->paddingVal); /* SBSW_CAN_LL11 */ /* SBSW_CAN_LL11 */
      }
      idxWord++;
    }
    /* #60 Add padding */
    dataWordLen = (txPara->frameLen + 3u) >> 2;
    for (idx = idxWord; idx < dataWordLen; idx++) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      canFDTxBuf[idx] = paddingWordValue; /* SBSW_CAN_LL11 */
    }
    /* #70 Copy prepared FD message to mailbox */
    for (idx = 0u; idx < dataWordLen; idx++)
    {
      CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, data[idx]) = canFDTxBuf[idx]; /* SBSW_CAN_LL02 */
    }
  }
#  else
  /* #80 Prepare and copy non-FD data into tx message object */
  Can_30_Flexcan4CopyDataBytes2Uint32(canTxBuf, txPara->CanMemCopySrcPtr); /* SBSW_CAN_LL13 */  /* SBSW_CAN_LL13 */
  CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, data[0]) = canTxBuf[0]; /* SBSW_CAN_LL02 */
  CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, data[1]) = canTxBuf[1]; /* SBSW_CAN_LL02 */
#  endif
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxStart
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxStart
Relation_Context:
# from Can_30_Flexcan4_Write #
Relation:
MultiplexedTx
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
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxStart(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
# if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
# endif
  Can_30_Flexcan4ObjectHandle hwObjHandle;

  /* #10 Determine mailbox to be used */
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  hwObjHandle = txPara->hwObjHandle + txPara->mailboxElement;
# else
  hwObjHandle = txPara->hwObjHandle;
# endif

  {
    /* #20 Request transmission */
    CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) |= kTx_30_Flexcan4CodeTransmit; /* start tx of the message */ /* SBSW_CAN_LL02 */
  }
  
  return(kCan_30_Flexcan4Ok);
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxEnd
Relation_Context:
# from Can_30_Flexcan4_Write #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxTransmissionParaStructPtr txPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 Check for error active transition */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(txPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

#if defined( C_30_FLEXCAN4_ENABLE_TX_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxIsGlobalConfPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxIsGlobalConfPending
Relation_Context:
# from Can_30_Flexcan4_MainFunction_Write #
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
 *
 *
 *
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxIsGlobalConfPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint32 iFlags;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  vuint8 retVal = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */

  /* #10 Retrieve IFLAG1 */
  iFlags = pFlex_30_Flexcan4CANLocal->iflag1 & CAN_30_FLEXCAN4_TX_POLL_MASK1(canHwChannel);
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  /* #20 Retrieve IFLAG2 */
  iFlags |= pFlex_30_Flexcan4CANLocal->iflag2 & CAN_30_FLEXCAN4_TX_POLL_MASK2(canHwChannel);
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
  {
    /* #30 Retrieve IFLAG3 */
    iFlags |= pFlex_30_Flexcan4CANLocal->iflag3 & CAN_30_FLEXCAN4_TX_POLL_MASK3(canHwChannel);
  }
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  /* #40 Retrieve IFLAG4 */
  iFlags |= pFlex_30_Flexcan4CANLocal->iflag4 & CAN_30_FLEXCAN4_TX_POLL_MASK4(canHwChannel);
# endif

  /* #50 Check for pending confirmation */
  if (iFlags != 0u)
  {
    retVal = kCan_30_Flexcan4True;
  }

  /* #60 Check for error active transition */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif

  return retVal;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxProcessPendings
Relation_Context:
# from Can_30_Flexcan4_MainFunction_Write #
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
*/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  Can_30_Flexcan4ObjectHandle mailboxElement = 0u;
  Can_30_Flexcan4ObjectHandle hwObjHandle = taskPara->hwObjHandle;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
  for (; mailboxElement < Can_30_Flexcan4_GetMailboxSize(taskPara->mailboxHandle); mailboxElement++)
# endif
  {
    /* #10 Check for confirmation condition */
    if(((CanLL_30_Flexcan4_GetInterruptFlag(pFlex_30_Flexcan4CANLocal, hwObjHandle) & Can_30_Flexcan4BitMask(hwObjHandle)) != 0u)) /* SBSW_CAN_LL01 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */ /* COV_CAN_FULLCAN_LL_TX_POLLING */
    {
      /* #20 Perform TX confirmation */
      CanHL_30_Flexcan4_TxConfirmationPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST  taskPara->mailboxHandle, mailboxElement, hwObjHandle);
    }
    hwObjHandle++; /* PRQA S 2983 */ /* MD_MSR_DummyStmt */
  }
}
/* CODE CATEGORY 2 END */
#endif /* C_30_FLEXCAN4_ENABLE_TX_POLLING */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxConfBegin
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmation #
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
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 Clear pending interrupt flag */
  CanLL_30_Flexcan4_ClearPendingInterrupt(pFlex_30_Flexcan4CAN(canHwChannel), txConfPara->hwObjHandle); /* SBSW_CAN_LL01 */ /* PRQA S 2987,0303,0306 */ /* MD_Can_EmptyFunction, MD_Can_HwAccess, MD_Can_LL_HwAccess */
}
/* CODE CATEGORY 1 END */

#if defined( C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfIsMsgTransmitted
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxConfIsMsgTransmitted
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmation #
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
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_TxConfIsMsgTransmitted(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint8 isMsgTransmitted = kCan_30_Flexcan4True;

  /* #10 Check for transmitted message */
  if (!CanLL_30_Flexcan4_TxIsObjFree(canHwChannel, txConfPara->hwObjHandle))
  {
    /* #20 Set mailbox to free if message was cancelled */
    CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txConfPara->hwObjHandle, control) = kTx_30_Flexcan4CodeFree; /* SBSW_CAN_LL02 */
    isMsgTransmitted = kCan_30_Flexcan4False;
  }

  return isMsgTransmitted;
}
/* CODE CATEGORY 1 END */
#endif

#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfSetTxConfStruct
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxConfSetTxConfStruct
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmation #
GenericConfirmation
Relation:
TxBasicAmount
TxFullAmount
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
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfSetTxConfStruct(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara)
{
  /* # 10 Get Confirmation information from mailbox */
#  if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  {
    vuint32 tmpData; /* avoid IAR compiler warning */
#   if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
    vuint8 idx;

    for (idx = 0u; idx < CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(canHwChannel); idx++)
    {
      tmpData = CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txConfPara->hwObjHandle, data[idx]); /* SBSW_CAN_LL02 */
      txConfPara->canDataBuffer[idx] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL12 */
    }
    txConfPara->txStructConf->pChipData = (Can_30_Flexcan4ChipDataPtr)&(txConfPara->canDataBuffer[0]); /* SBSW_CAN_LL04 */
#   else
    tmpData = CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txConfPara->hwObjHandle, data[0]); /* SBSW_CAN_LL02 */
    txConfPara->canDataBuffer[0] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL13 */
    tmpData = CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txConfPara->hwObjHandle, data[1]); /* SBSW_CAN_LL02 */
    txConfPara->canDataBuffer[1] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL13 */
    txConfPara->txStructConf->pChipData = (Can_30_Flexcan4ChipDataPtr)&(txConfPara->canDataBuffer[0]); /* SBSW_CAN_LL04 */
#   endif
    txConfPara->txStructConf->pChipMsgObj = (Can_30_Flexcan4ChipMsgPtr)&(CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(canHwChannel, txConfPara->hwObjHandle, timestamp)); /* SBSW_CAN_LL04 */
  }
#  endif
}
/* CODE CATEGORY 1 END */
#endif

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_TxConfEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_TxConfEnd
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmation #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_TxConfEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTxConfirmationParaStructPtr txConfPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_TxConfEnd: nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(txConfPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */



#if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicMsgReceivedBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxBasicMsgReceivedBegin
Relation_Context:
# from CanHL_30_Flexcan4_BasicCanMsgReceived #
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
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxBasicMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara)
{
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);
  tCan_30_Flexcan4ERxFifoObjPtr pERxFifoObj = pCan_30_Flexcan4ERxFifoObj(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  vuint8 retVal = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  vuint8 idx;
  vuint8 idxStop;

  /* #10 Copy control word from RxFifo */
  Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control = pERxFifoObj->control; /* SBSW_CAN_LL06 */
  /* #20 Copy id to RAM buffer */
  Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).msgID = pERxFifoObj->msgID; /* SBSW_CAN_LL06 */

  /* #30 Copy data to RAM buffer */
  idxStop = (CAN_30_FLEXCAN4_DLC2LEN((vuint8)(Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control) & (vuint8)0x0Fu) + 3u) >> 2u; /* PRQA S 2985 */ /* MD_Can_ConstValue */
# if !defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  idxStop = (idxStop > 2u) ? 2u : idxStop;
# endif
# if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  {
    vuint32 tmpData;
    for (idx = 0u; idx < idxStop; idx++)
    {
      tmpData = pERxFifoObj->data[idx]; /* SBSW_CAN_LL02 */
      Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[idx] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL06 */
    }
  }
# endif

  /* #40 Clear enhanced Rx Fifo pending interrupt flag after message is stored */
  pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERFDA; /* SBSW_CAN_LL01 */

  /* #50 Save pointer of control word and data */
  rxBasicPara->rxStruct.pChipMsgObj = (Can_30_Flexcan4ChipMsgPtr)&(Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control); /* SBSW_CAN_LL04 */
  rxBasicPara->rxStruct.pChipData = (Can_30_Flexcan4ChipDataPtr)&(Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[0]); /* SBSW_CAN_LL04 */

# if defined( C_30_FLEXCAN4_ENABLE_OVERRUN )
  /* #60 Save overrun status */
  if((pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFOVF) != 0u)
  {
    rxBasicPara->isOverrun = (vuint8)kCan_30_Flexcan4True; /* SBSW_CAN_LL04 */
    pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERFOVF; /* SBSW_CAN_LL01 */
  }
  else
  {
    rxBasicPara->isOverrun = (vuint8)kCan_30_Flexcan4False; /* SBSW_CAN_LL04 */
  }
# endif

  return (retVal);
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicReleaseObj
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxBasicReleaseObj
Relation_Context:
# from CanHL_30_Flexcan4_BasicCanMsgReceived #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicReleaseObj(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_RxBasicReleaseObj(): nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(rxBasicPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicMsgReceivedEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxBasicMsgReceivedEnd
Relation_Context:
# from CanHL_30_Flexcan4_BasicCanMsgReceived #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxBasicParaStructPtr rxBasicPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_RxBasicMsgReceivedEnd(): nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(rxBasicPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

# if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicIsGlobalIndPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxBasicIsGlobalIndPending
Relation_Context:
# from CanHL_30_Flexcan4_RxBasicCanPolling #
RxBasicCANSupport, RxPolling
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
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxBasicIsGlobalIndPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint8 retVal = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */

  /* #10 Check for pending RX BesicCAN message */
  if ((pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFDA) != 0u)
  {
    retVal = kCan_30_Flexcan4True;
  }

  /* #20 Check for error active transition */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif

  return retVal;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxBasicProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxBasicProcessPendings
Relation_Context:
# from CanHL_30_Flexcan4_RxBasicCanPolling #
RxBasicCANSupport, RxPolling
Relation:
CanFdSupport
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
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxBasicProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  vuint32 msgCount;

  /* #40 Check for RxFifo message */
  msgCount = pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFEL_MASK;
  while(((pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFE) == 0u) && (msgCount > 0u)) /* COV_CAN_RXFIFO_HANDLING */
  {
    msgCount--;
    CanHL_30_Flexcan4_BasicCanMsgReceivedPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST taskPara->mailboxHandle, taskPara->hwObjHandle);
  }
}
/* CODE CATEGORY 2 END */
# endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */

#if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
/**********************************************************************************************************************
*  CanLL_30_Flexcan4_RxFullMsgReceivedBegin
*********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxFullMsgReceivedBegin
Relation_Context:
# from CanHL_30_Flexcan4_FullCanMsgReceived #
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
 *
 *
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxFullMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara)
{
# if defined( C_30_FLEXCAN4_ENABLE_PROTECTED_RX_PROCESS )
  Can_30_Flexcan4DeclareGlobalInterruptOldStatus
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  vuint8 idx;
# endif
  vuint8 retVal = kCan_30_Flexcan4Ok; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  Can_ReturnType loopResult = CAN_OK; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);

# if defined( C_30_FLEXCAN4_ENABLE_PROTECTED_RX_PROCESS )
  Can_30_Flexcan4NestedGlobalInterruptDisable();
# endif
  /* #10 Request access to FullCAN mailbox and copy control word */
  Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, control); /* SBSW_CAN_LL06 */
  if ((Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control & kRx_30_Flexcan4CodeBusy) == kRx_30_Flexcan4CodeBusy) /* COV_CAN_HARDWARE_PROCESSING_TIME */
  {
    CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopMsgReception); /* start hw loop timer */ /* SBSW_CAN_LL08 */
    do
    { /* Check busy state of receive object */
      Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, control); /* SBSW_CAN_LL06 */
      loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopMsgReception);
    } while(((Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control & kRx_30_Flexcan4CodeBusy) == kRx_30_Flexcan4CodeBusy) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* PRQA S 2995, 2843 */ /* MD_Can_ConstValue, MD_Can_HL_DereferencePointerValue */
    CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopMsgReception); /* stop hw loop timer */ /* SBSW_CAN_LL08 */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */
  }

  /* #20 Copy id to RAM buffer */
  Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).msgID = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, msgID); /* SBSW_CAN_LL06 */
  /* #30 Copy data to RAM buffer */
# if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  {
    vuint32 tmpData; /* avoid IAR compiler warning */
#  if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
    for (idx = 0u; idx < CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(canHwChannel); idx++)
    {
      tmpData = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, data[idx]); /* SBSW_CAN_LL02 */
      Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[idx] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL06 */
    }
#  else
    tmpData = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, data[0]); /* SBSW_CAN_LL02 */
    Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[0] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL06 */
    tmpData = CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, rxFullPara->hwObjHandle, data[1]); /* SBSW_CAN_LL02 */
    Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[1] = Can_30_Flexcan4Bswap32(tmpData); /* SBSW_CAN_LL06 */
#  endif
  }
# endif

  /* #40 Clear pending interrupt flag for FullCAN */
  CanLL_30_Flexcan4_ClearPendingInterrupt(pFlex_30_Flexcan4CANLocal, rxFullPara->hwObjHandle); /* SBSW_CAN_LL01 */

  /* #50 Unlock FullCAN mailbox */
  Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).timestamp = (vuint16)pFlex_30_Flexcan4CANLocal->timer; /* unlock current receive buffer */ /* SBSW_CAN_LL01 */

# if defined( C_30_FLEXCAN4_ENABLE_PROTECTED_RX_PROCESS )
  Can_30_Flexcan4NestedGlobalInterruptRestore();
# endif

# if defined( C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER )
  if (loopResult != CAN_OK) /* COV_CAN_TIMEOUT_DURATION */
  {    
    retVal = kCan_30_Flexcan4Failed;
  }
# endif

  /* #60 Save pointer of control word and data */
  rxFullPara->rxStruct.pChipMsgObj = (Can_30_Flexcan4ChipMsgPtr) &(Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control); /* SBSW_CAN_LL04 */
  rxFullPara->rxStruct.pChipData = (Can_30_Flexcan4ChipDataPtr) &(Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).data[0]); /* SBSW_CAN_LL04 */

# if defined( C_30_FLEXCAN4_ENABLE_FULLCAN_OVERRUN )
  /* #70 Save overrun status */
  if ((Can_30_Flexcan4_GetRxMsgBufferOfControllerData(canHwChannel).control & kCode_30_Flexcan4_Mask) == kRx_30_Flexcan4CodeOverrun) /* COV_CAN_FULL_OVERRUN_HANDLING */
  {
    rxFullPara->isOverrun = kCan_30_Flexcan4True; /* SBSW_CAN_LL04 */
  }
  else
  {
    rxFullPara->isOverrun = kCan_30_Flexcan4False; /* SBSW_CAN_LL04 */
  }
# endif

  return (retVal);
}
/* CODE CATEGORY 1 END */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullReleaseObj
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxFullReleaseObj
Relation_Context:
# from CanHL_30_Flexcan4_FullCanMsgReceived #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullReleaseObj(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_RxFullReleaseObj(): nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(rxFullPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullMsgReceivedEnd
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxFullMsgReceivedEnd
Relation_Context:
# from CanHL_30_Flexcan4_FullCanMsgReceived #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanRxFullParaStructPtr rxFullPara) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 CanLL_30_Flexcan4_RxFullMsgReceivedEnd(): nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(rxFullPara); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 1 END */


# if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullIsGlobalIndPending
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxFullIsGlobalIndPending
Relation_Context:
# from CanHL_30_Flexcan4_RxFullCanPolling #
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
 *
 *
 *
 *
 *
*/
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_RxFullIsGlobalIndPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint32 iFlags;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  vuint8 retVal = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */

  /* #10 Retrieve IFLAG1 of Rx FullCAN */
  iFlags = pFlex_30_Flexcan4CANLocal->iflag1 & CAN_30_FLEXCAN4_RXFULL_POLL_MASK1(canHwChannel);
#if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  /* #20 Retrieve IFLAG2 of Rx FullCAN */
  iFlags |= pFlex_30_Flexcan4CANLocal->iflag2 & CAN_30_FLEXCAN4_RXFULL_POLL_MASK2(canHwChannel);
#endif
#if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  /* #30 Retrieve IFLAG3 of Rx FullCAN */
# if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
# endif
  {
    iFlags |= pFlex_30_Flexcan4CANLocal->iflag3 & CAN_30_FLEXCAN4_RXFULL_POLL_MASK3(canHwChannel);
  }
#endif
#if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  /* #40 Retrieve IFLAG of Rx FullCAN */
  iFlags |= pFlex_30_Flexcan4CANLocal->iflag4 & CAN_30_FLEXCAN4_RXFULL_POLL_MASK4(canHwChannel);
#endif
  /* #50 Check for received FullCAN message */
  if (iFlags != 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    retVal = kCan_30_Flexcan4True;
  }

  /* #60 Check for error active transition */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif

  return retVal;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_RxFullProcessPendings
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_RxFullProcessPendings
Relation_Context:
# from CanHL_30_Flexcan4_RxFullCanPolling #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_RxFullProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST CanTaskParaStructPtr taskPara) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint32 iFlags;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);

  /* #10 Get iflag of corresponding hwObject */
  iFlags = CanLL_30_Flexcan4_GetInterruptFlag(pFlex_30_Flexcan4CANLocal, taskPara->hwObjHandle); /* SBSW_CAN_LL01 */
  /* #20 Check for received FullCAN message */
  if((iFlags & Can_30_Flexcan4BitMask(taskPara->hwObjHandle)) != 0u) /* COV_CAN_FULLCAN_LL_RX_POLLING */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  {
    CanHL_30_Flexcan4_FullCanMsgReceivedPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST Can_30_Flexcan4_GetHwToMbHandle(canHwChannel, taskPara->hwObjHandle), taskPara->hwObjHandle); /* PRQA S 2986 */ /* MD_Can_ConstValue */
  }
}
/* CODE CATEGORY 2 END */
# endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING */
#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */




#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
StatusPolling
Relation:
OneChOpt, ChannelAmount
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint8 retVal = kCan_30_Flexcan4False;

  /* #10 Check if a transition to error state passive has occurred */
  if ((canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] == kCan_30_Flexcan4True) && ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_FCS) == kFlexCAN_30_Flexcan4_FCS_EP))
  {
    canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4False; /* SBSW_CAN_LL14 */
    retVal = kCan_30_Flexcan4True;
  }

  return retVal;
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorNotificationHandling
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ErrorNotificationHandling
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
StatusPolling
Relation:
OneChOpt, ChannelAmount
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
/* CODE CATEGORY 2 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorNotificationHandling( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);

  /* #10 A 0 was transmitted and a 1 was read back */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_BIT0ERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_BIT_MONITORING1);
  }

  /* #20 A 1 was transmitted and a 0 was read back */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_BIT1ERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_BIT_MONITORING0);
  }

  /* #30 Acknowledgement check failed */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_ACKERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_CHECK_ACK_FAILED);
  }

  /* #40 Violations of the fixed frame format */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_FRMERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_CHECK_FORM_FAILED);
  }

  /* #50 Stuffing bits not as expected */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_STFERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_CHECK_STUFFING_FAILED);
  }

  /* #60 CRC failed */
  if ((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_CRCERR) != 0u)
  {
    CanHL_30_Flexcan4_ErrorNotification(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST CAN_ERROR_CHECK_CRC_FAILED);
  }

  /* #70 Clear Error interrupt flag */
  pFlex_30_Flexcan4CANLocal->estat = kFlexCAN_30_Flexcan4_ERR_INT_CLEAR; /* SBSW_CAN_LL01 */
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
StatusPolling
Relation:
OneChOpt, ChannelAmount
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  /* #10 Check if a transition to error state active has occurred */
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if ((Can_30_Flexcan4RxActualErrorCounter() < 128u) && (Can_30_Flexcan4TxActualErrorCounter() < 128u)) /* COV_CAN_ACTIVE_TRANSITION_STIMULATION */
# else
  if ((Can_30_Flexcan4RxActualErrorCounter(canHwChannel) < 128u) && (Can_30_Flexcan4TxActualErrorCounter(canHwChannel) < 128u)) /* COV_CAN_ACTIVE_TRANSITION_STIMULATION */
# endif
  {
    canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
  }
}
/* CODE CATEGORY 2 END */
#endif /* CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorHandlingBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ErrorHandlingBegin
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorHandlingBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */

  /* #10 Save FlexCAN status */
  canLL_30_Flexcan4_Estat[canHwChannel] = pFlex_30_Flexcan4CANLocal->estat; /* SBSW_CAN_LL14 */
}
/* CODE CATEGORY 2 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_BusOffOccured
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_BusOffOccured
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
StatusPolling
Relation:
OneChOpt
ChannelAmount
BusOffRecoveryDefine
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_BusOffOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  vuint8 retVal = kCan_30_Flexcan4False; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  
  /* #10 Check for BusOff */
  if((canLL_30_Flexcan4_Estat[canHwChannel] & kFlexCAN_30_Flexcan4_BOFF_INT) != 0u) /* COV_CAN_BUSOFF_HANDLING */
  {
    /* #20 clear busoff and error interrupt flags */
    vuint32 busOffErrFlags;
#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
    busOffErrFlags = (vuint32)(kFlexCAN_30_Flexcan4_BOFF_INT);
#else
    busOffErrFlags = (vuint32)(kFlexCAN_30_Flexcan4_BOFF_INT | kFlexCAN_30_Flexcan4_ERR_INT);
#endif

    pFlex_30_Flexcan4CANLocal->estat = (vuint32)(busOffErrFlags); /* clear busoff and error interrupt flags */ /* SBSW_CAN_LL01 */

#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
    /* #30 Delete pending transmit requests */
    {
      Can_30_Flexcan4ObjectHandle hwObjHandle;
      for (hwObjHandle = CAN_30_FLEXCAN4_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle < CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++)
      {
        CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = kTx_30_Flexcan4CodeInactive; /* set all mailboxes inactive */ /* SBSW_CAN_LL02 */
      }
    }
#endif
    retVal = kCan_30_Flexcan4True;
  }
  
  return retVal;
}
/* CODE CATEGORY 2 END */


/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ErrorHandlingEnd
 *********************************************************************************************************************/
 /*
 |<DataModelStart>| CanLL_30_Flexcan4_ErrorHandlingEnd
 Relation_Context:
 # from CanHL_30_Flexcan4_ErrorHandling() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ErrorHandlingEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  /* #10 Clear saved FlexCAN status */
  canLL_30_Flexcan4_Estat[canHwChannel] = 0x0uL; /* SBSW_CAN_LL14 */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 2 END */

#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS )
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_GetStatusBegin
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_GetStatusBegin
Relation_Context:
# from Can_30_Flexcan4_GetStatus
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_GetStatusBegin(CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY)
{
  /* #10 CanLL_30_Flexcan4_GetStatusBegin(): nothing to do here */
  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 3 END */
#endif



/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_ModeTransition
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ModeTransition
Relation_Context:
# from CanHL_30_Flexcan4_ModeTransition() #
Wakeup, OneChOpt, HardwareLoopCheck, ChannelAmount
RamCheck, OneChOpt, HardwareLoopCheck, ChannelAmount
# from Can_30_Flexcan4_Mainfunction_Mode() #
ChannelAmount
Relation:
Wakeup
SilentMode
ReInitStart
BusOffRecoveryDefine
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 busOffRecovery, vuint8 ramCheck)
{
  vuint8 retVal = kCan_30_Flexcan4Failed; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
#if ( (CAN_30_FLEXCAN4_REINIT_START == STD_ON) )
  if ( (mode != kCan_30_Flexcan4ModeStartReinit)
     )
  {
    canLL_30_Flexcan4_canStartReinitState[channel] = kCan_30_Flexcan4LLStateStart; /* SBSW_CAN_LL14 */
  }
#endif
  switch(mode)
  {

    /* #400 STOP reinit request */
    case kCan_30_Flexcan4ModeStopReinit:
      retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST ramCheck);
      break;

    /* #500 STOP reinit fast request */
    case kCan_30_Flexcan4ModeStopReinitFast:
      retVal = CanLL_30_Flexcan4_StopTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST busOffRecovery, ramCheck);
      break;

    /* #600 START request */
#if ( (CAN_30_FLEXCAN4_REINIT_START == STD_OFF) )
    case kCan_30_Flexcan4ModeStart:
#endif
#if ( (CAN_30_FLEXCAN4_REINIT_START == STD_ON) )
    /* #700 START reinit request */
    case kCan_30_Flexcan4ModeStartReinit:
#endif
#if defined ( C_30_FLEXCAN4_ENABLE_SILENT_MODE )
    /* #800 SILENT mode request */
    case kCan_30_Flexcan4ModeSilent:
#endif
      retVal = CanLL_30_Flexcan4_StartTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST mode, ramCheck);
      break;

    /* #1000 BUSOFF start request */
    case kCan_30_Flexcan4ModeResetBusOffStart:
#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
      retVal = kCan_30_Flexcan4Ok;
#else
      retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST ramCheck);
#endif
      break;

    /* #1100 BUSOFF end request */
    case kCan_30_Flexcan4ModeResetBusOffEnd:
#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
      retVal = CanLL_30_Flexcan4_BusOffEndTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST ramCheck);
#else
      retVal = CanLL_30_Flexcan4_StartTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST mode, ramCheck);
#endif
      break;

    default:
      /* retVal is kCan_30_Flexcan4Failed */
      break;
  }

  return retVal;
}
/* CODE CATEGORY 4 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_StopReinit
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_StopReinit
Relation_Context:
# from CanLL_30_Flexcan4_StopTransition #
OneChOpt,ChannelAmount
Relation:
TxFullCANSupport
RxFullCANSupport
RxBasicCANSupport,CanFdSupport
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
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_StopReinit(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY)
{
  Can_30_Flexcan4ObjectHandle hwObjHandle;
  Can_30_Flexcan4ObjectHandle mailboxHandle = 0u; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  tFlex_30_Flexcan4CANPtr     pFlex_30_Flexcan4CANLocal;

  /* #10 Clear all pending Rx/Tx interrupts */
  pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  pFlex_30_Flexcan4CANLocal->iflag1 = kFlexCAN_30_Flexcan4_SFR_SET; /* clear pending Rx/Tx interrupts */ /* SBSW_CAN_LL01 */

# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  pFlex_30_Flexcan4CANLocal->iflag2 = kFlexCAN_30_Flexcan4_SFR_SET; /* clear pending Rx/Tx interrupts */ /* SBSW_CAN_LL01 */
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
  {
    pFlex_30_Flexcan4CANLocal->iflag3 = kFlexCAN_30_Flexcan4_SFR_SET; /* clear pending Rx/Tx interrupts */ /* SBSW_CAN_LL01 */
  }
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  pFlex_30_Flexcan4CANLocal->iflag4 = kFlexCAN_30_Flexcan4_SFR_SET; /* clear pending Rx/Tx interrupts */ /* SBSW_CAN_LL01 */
# endif

  /* #20 Clear pending BusOff and error interrupts */
  pFlex_30_Flexcan4CANLocal->estat = (vuint32)kFlexCAN_30_Flexcan4_STATUS_INT; /* SBSW_CAN_LL01 */

  /* #30 Activate BusOff auto recovery */
  pFlex_30_Flexcan4CANLocal->control1 &= (vuint32)(~kFlexCAN_30_Flexcan4_BOFF_REC); /* SBSW_CAN_LL01 */

  /* #40 Inactivate all Tx mailboxes */

  for (hwObjHandle = CAN_30_FLEXCAN4_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle < CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++)
  {
    CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) = kTx_30_Flexcan4CodeInactive; /* set all mailboxes inactive */ /* SBSW_CAN_LL02 */
  }

# if defined ( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
  /* #50 Disable all Rx FullCAN mailboxes */
  for (mailboxHandle = CAN_30_FLEXCAN4_HL_MB_RX_FULL_STARTINDEX(canHwChannel); mailboxHandle < CAN_30_FLEXCAN4_HL_MB_RX_FULL_STOPINDEX(canHwChannel); mailboxHandle++)
  {
    hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(mailboxHandle);
    CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) &= kRx_30_Flexcan4CodeClear; /* clear control register and do not change the ID type */ /* SBSW_CAN_LL02 */
    CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(canHwChannel, hwObjHandle, control) |= kRx_30_Flexcan4CodeEmpty; /* set mailbox ready for receive */ /* SBSW_CAN_LL02 */
  }
# endif

# if defined ( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) /* COV_CAN_MULTIPLE_BASICCAN */
  /* #70 Clear Enhanced Rx Fifo */
  {
    tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);
    if ((pERxFifoRegister->erfcr & kFlexCAN_30_Flexcan4_ERFEN) != 0u) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
    {
      pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERFCLR; /* SBSW_CAN_LL01 */
      pERxFifoRegister->erfsr = kFlexCAN_30_Flexcan4_ERF_CLEAR_STATUS; /* SBSW_CAN_LL01 */
    }
  }
# endif

  CAN_30_FLEXCAN4_DUMMY_STATEMENT(mailboxHandle); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
/* CODE CATEGORY 4 END */


#if (( CAN_30_FLEXCAN4_INTLOCK != CAN_30_FLEXCAN4_APPL) ) /* COV_CAN_LOCK_ISR_BY_APPL */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_CanInterruptDisable
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CanInterruptDisable
Relation_Context:
# from Can_30_Flexcan4_DisableControllerInterrupts #
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
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanInterruptDisable(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST tCan_30_Flexcan4LLCanIntOldPtr localInterruptOldFlagPtr) /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
#if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
    defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
  {
    /* ----- Local Variables ---------------------------------------------- */
    tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4can = pFlex_30_Flexcan4CAN(canHwChannel);

    /* ----- Implementation ----------------------------------------------- */
    /* #10 Disable RxTx interrupts */
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
    {
      tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);
      if ((pERxFifoRegister->erfcr & kFlexCAN_30_Flexcan4_ERFEN) != 0u) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
      {
        pERxFifoRegister->erfier = 0u; /* SBSW_CAN_LL07 */
      }
    }
# endif
    CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_0TO31(localInterruptOldFlagPtr); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
    CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_32TO63(localInterruptOldFlagPtr); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
# if defined (C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
# endif
    {
      CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_64TO95(localInterruptOldFlagPtr); /* PRQA S 2880 */ /* MD_Can_ConstValue */ /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
    }
    CAN_30_FLEXCAN4_DISABLE_RXTX_INTERRUPT_96TO127(localInterruptOldFlagPtr); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */

    /* #20 Disable BusOff interrupts */
    CAN_30_FLEXCAN4_DISABLE_BUSOFF_INTERRUPT(localInterruptOldFlagPtr); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */

    /* #30 Disable WakeUp interrupts */
    CAN_30_FLEXCAN4_DISABLE_WAKEUP_INTERRUPT(localInterruptOldFlagPtr); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
  }
#else
  CAN_30_FLEXCAN4_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(localInterruptOldFlagPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}
/* CODE CATEGORY 1 END */

/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_CanInterruptRestore
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CanInterruptRestore
Relation_Context:
# from Can_30_Flexcan4_EnableControllerInterrupts #
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
 *
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanInterruptRestore(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST tCan_30_Flexcan4LLCanIntOld localInterruptOldFlag)
{
#if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
    defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
  {
    /* ----- Local Variables ---------------------------------------------- */
    tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4can = pFlex_30_Flexcan4CAN(canHwChannel);

    /* ----- Implementation ----------------------------------------------- */
    /* #10 Restore RxTx interrupts */
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
    {
      tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel);
      if ((pERxFifoRegister->erfcr & kFlexCAN_30_Flexcan4_ERFEN) != 0u) /* COV_CAN_ENHANCED_RXFIFO_SUPPORT */
      {
        pERxFifoRegister->erfier = Can_30_Flexcan4_GetErfdaInterruptMaskOfControllerConfig(canHwChannel); /* SBSW_CAN_LL07 */
      }
    }
# endif
    CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_0TO31(localInterruptOldFlag); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
    CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_32TO63(localInterruptOldFlag); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
# if defined (C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
# endif
    {
      CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_64TO95(localInterruptOldFlag); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */ /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
    }
    CAN_30_FLEXCAN4_RESTORE_RXTX_INTERRUPT_96TO127(localInterruptOldFlag); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */

    /* #20 Restore BusOff interrupts */
    CAN_30_FLEXCAN4_RESTORE_BUSOFF_INTERRUPT(localInterruptOldFlag); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */

    /* #30 Restore WakeUp interrupts */
    CAN_30_FLEXCAN4_RESTORE_WAKEUP_INTERRUPT(localInterruptOldFlag); /* SBSW_CAN_LL07 */ /* SBSW_CAN_LL01 */
  }
#else
  CAN_30_FLEXCAN4_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(localInterruptOldFlag); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}
/* CODE CATEGORY 1 END */
#endif


/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_SetFlexCANToInitMode
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_SetFlexCANToInitMode
Relation_Context:
# from CanLL_30_Flexcan4_InitBegin #
OneChOpt, ChannelAmount
Relation:
HardwareLoopCheck
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_SetFlexCANToInitMode(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4InitHandle initObject)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType loopResult = CAN_OK; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  vuint8 returnCode = kCan_30_Flexcan4Ok; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if FlexCAN is DISABLED or in a NON_BUSOFF state (and in NON_LOM mode) */
  if (((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_DISABLE_MODE) == kFlexCAN_30_Flexcan4_DISABLE_MODE)
    || (((CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(pFlex_30_Flexcan4CANLocal->estat) & kFlexCAN_30_Flexcan4_FCS_BOFF) == 0u) /* PRQA S 3415 */ /* MD_Can_RuntimeOptimization_LL */
#if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
    && ((CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(pFlex_30_Flexcan4CANLocal->control1) & kFlexCAN_30_Flexcan4_LOM) == 0u) /* PRQA S 3415 */ /* MD_Can_RuntimeOptimization_LL */
#endif
  )) /* COV_CAN_INIT_STIMULATION */ /* SBSW_CAN_LL01 */ /* SBSW_CAN_LL01 */ /* SBSW_CAN_LL01 */
  {
    /* #20 Check if FlexCAN is ENABLED */
    if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_DISABLE_ONLY_BITS) == 0u) /* COV_CAN_INIT_STIMULATION */ /* SBSW_CAN_LL01 */
    {
      /* #30 Enter FREEZE mode (INIT mode) of FlexCAN */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ); /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* SBSW_CAN_LL01 */
      if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_MCR) != kFlexCAN_30_Flexcan4_FREEZE_MODE) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
      {
        CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopEnterFreezeModeInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
        do  
        { /* wait for FRZACK */  
          loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopEnterFreezeModeInit);
        } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_MCR) != kFlexCAN_30_Flexcan4_FREEZE_MODE) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* SBSW_CAN_LL01 */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
        CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopEnterFreezeModeInit); /* stop hw loop timer */ /* SBSW_CAN_LL08 */
#if defined( C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER )
        if (loopResult != CAN_OK) /* COV_CAN_TIMEOUT_DURATION */
        {
          returnCode = kCan_30_Flexcan4Failed;
        }
#endif
      }
      
      /* #40 Enter DISABLE mode of FlexCAN for clock selection */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* disable FlexCAN module before clock selection */ /* SBSW_CAN_LL01 */
      if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) != kFlexCAN_30_Flexcan4_LPM_ACK) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
      {
        CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopEnterDisableModeInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
        do
        { /* wait for FlexCAN is disabled */
          loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopEnterDisableModeInit);
        } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) != kFlexCAN_30_Flexcan4_LPM_ACK) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* SBSW_CAN_LL01 */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
        CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopEnterDisableModeInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
#if defined( C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER )
        if (loopResult != CAN_OK) /* COV_CAN_TIMEOUT_DURATION */
        {
          returnCode = kCan_30_Flexcan4Failed;
        }
#endif
      }
    }
    
    /* #50 Set FlexCAN clock source */
    if((Can_30_Flexcan4_GetControl1OfInitObject(initObject) & kFlexCAN_30_Flexcan4_CLK_SRC) != 0u) /* COV_CAN_CLK_SRC_GENERATION */ /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */
    {
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC); /* SBSW_CAN_LL01 */ /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }
    else
    {
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC); /* SBSW_CAN_LL01 */ /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    }

    /* #60 Leave DISABLE mode of FlexCAN */
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
    if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) == kFlexCAN_30_Flexcan4_LPM_ACK) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
    {
      CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopLeaveDisableModeInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
      do
      { /* wait for FlexCAN is enabled */
        loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopLeaveDisableModeInit);
      } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) == kFlexCAN_30_Flexcan4_LPM_ACK) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* SBSW_CAN_LL01 */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
      CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopLeaveDisableModeInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
#if defined( C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER )
      if (loopResult != CAN_OK) /* COV_CAN_TIMEOUT_DURATION */
      {
        returnCode = kCan_30_Flexcan4Failed;
      }
#endif
    }
  }

  return (returnCode);
} /* PRQA S 6010,6030 */ /* MD_MSR_STPTH,MD_MSR_STCYC */
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_ExecuteSoftReset
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ExecuteSoftReset
Relation_Context:
# from CanLL_30_Flexcan4_InitBegin #
OneChOpt, ChannelAmount
Relation:
CanFdSupport,WorkaroundErrata010368
HardwareLoopCheck,CanFdSupport,WorkaroundErrata010368
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_ExecuteSoftReset(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY)
{
  Can_ReturnType loopResult1 = CAN_OK; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED ) && defined( C_30_FLEXCAN4_ENABLE_WORKAROUND_ERR010368 )
  Can_ReturnType loopResult2 = CAN_OK; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
#endif
  vuint8 returnCode = kCan_30_Flexcan4Ok; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
  
  /* ----- Implementation ----------------------------------------------- */     
  /* *********************************************************************** */
  /* Perform FlexCAN soft reset.                                             */
  /* Please note: the soft reset cannot be applied while clocks are          */
  /* shut down: low power mode or clocks are not enabled.                    */
  /* Please check clock settings if soft reset cannot be finalized.          */
  /* Errata e10368: Perform a second soft reset in case of FD enabled        */
  /* *********************************************************************** */
  /* #10 Execute SoftReset of FlexCAN */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_SOFT_RST); /* SBSW_CAN_LL01 */
  if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_SOFT_RST) == kFlexCAN_30_Flexcan4_SOFT_RST) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
  {
    /* #20 Wait for soft reset confirmation from hardware */
    CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopResetInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
    do
    { /* wait for SOFT_RST */
      loopResult1 = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopResetInit);
    } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_SOFT_RST) == kFlexCAN_30_Flexcan4_SOFT_RST) && (loopResult1 == CAN_OK)); /* SBSW_CAN_LL01 */ /* COV_CAN_TIMEOUT_DURATION */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
    CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopResetInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
  }
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED ) && defined( C_30_FLEXCAN4_ENABLE_WORKAROUND_ERR010368 )
  /* #30 Execute SoftReset of FlexCAN second time (errata e10368) if CAN FD is enabled */
  if (Can_30_Flexcan4_IsHasCANFDBaudrateOfControllerConfig(canHwChannel)) /* COV_CAN_FD_BAUDRATE_AVAILABILITY */
  {
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_SOFT_RST); /* SBSW_CAN_LL01 */
    if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_SOFT_RST) == kFlexCAN_30_Flexcan4_SOFT_RST) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
    {
      /* #40 Wait for soft reset confirmation from hardware second time (errata e10368) if CAN FD is enabled */
      CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopResetInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
      do
      { /* wait for SOFT_RST */
        loopResult2 = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopResetInit);
      } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_SOFT_RST) == kFlexCAN_30_Flexcan4_SOFT_RST) && (loopResult2 == CAN_OK)); /* SBSW_CAN_LL01 */ /* COV_CAN_TIMEOUT_DURATION */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
      CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopResetInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
    }
  }
#endif

#if defined(C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER)
  if ((loopResult1 != CAN_OK)
# if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED ) && defined( C_30_FLEXCAN4_ENABLE_WORKAROUND_ERR010368 )
      || (loopResult2 != CAN_OK)
# endif
      ) /* COV_CAN_TIMEOUT_DURATION */
  {
    returnCode = kCan_30_Flexcan4Failed;
  }
#endif

#if defined C_30_FLEXCAN4_ENABLE_FLEXCAN_USER_MODE
  /* #50 Reset SUPV bit to grant access to CAN register (except MCR register) in User Mode */
  /* set all CAN register except the MCR register in unrestricted memory space directly after soft reset */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_SUPV); /* SBSW_CAN_LL01 */
#endif

  return (returnCode);
}
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_StopTransition
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_StopTransition
Relation_Context:
# from CanLL_30_Flexcan4_ModeTransition #
OneChOpt,ChannelAmount
Relation:
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
 *
 *
 */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_StopTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 busOffRecovery, vuint8 doRamCheck)
{
  vuint8 retVal;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */

  /* ----- Implementation ----------------------------------------------- */
  /* *********************************************************************** */
  /* Handle stop transition of FlexCAN                                       */
  /* *********************************************************************** */
  if (CanLL_30_Flexcan4_HwIsBusOff(canHwChannel)) /* COV_CAN_STOP_BOFF_STIMULATION */ /* SBSW_CAN_LL01 */
  { /* #10 Check for BusOff state */
    if (busOffRecovery == kCan_30_Flexcan4ContinueBusOffRecovery) /* COV_CAN_STOP_BOFF_STIMULATION */
    { /* #20 Stop Transition in case of kCan_30_Flexcan4ContinueBusOffRecovery */
      retVal = kCan_30_Flexcan4Failed;
    }
    else
    { /* #30 Stop Transition in case of kCan_30_Flexcan4FinishBusOffRecovery */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
      canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
# endif
      retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST doRamCheck);
    }
  }
# if defined (C_30_FLEXCAN4_ENABLE_SILENT_MODE)
  else if (CanLL_30_Flexcan4_HwIsListenOnlyMode(canHwChannel)) /* SBSW_CAN_LL01 */
  { /* #40 Stop Transition in case of silent mode active */
      retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST doRamCheck);
  }
# endif
  else
  { /* #50 Fast Re-Init Stop Transition */
    if (CanLL_30_Flexcan4_HwIsStop(canHwChannel)) /* SBSW_CAN_LL01 */
    { /* #60 Check for stop transition finished */
      CanLL_30_Flexcan4_StopReinit(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY);
      CanHL_30_Flexcan4_CleanUpSendState(CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
      canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
# endif
      retVal = kCan_30_Flexcan4Ok;
    }
    else if (CanLL_30_Flexcan4_HwIsStopRequested(canHwChannel)) /* SBSW_CAN_LL01 */ /* COV_CAN_HARDWARE_PROCESSING_TIME */
    { /* #70 Check for stop mode requested */
      retVal = kCan_30_Flexcan4Requested;
    }
    else
    { /* #80 Request stop mode for all other cases */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ);  /* set FRZ bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* set HALT bit */ /* SBSW_CAN_LL01 */
      retVal = kCan_30_Flexcan4Requested;
    }
  }

  return (retVal);
}
/* CODE CATEGORY 4 END */

/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_StartTransition
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_StartTransition
Relation_Context:
# from CanLL_30_Flexcan4_ModeTransition #
OneChOpt,ChannelAmount
Relation:
SilentMode
ReInitStart
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_StartTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 mode, vuint8 doRamCheck)
{
  vuint8 retVal;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  /* ----- Implementation ----------------------------------------------- */
  /* *********************************************************************** */
  /* Handle start transition of FlexCAN                                      */
  /* *********************************************************************** */
#if (CAN_30_FLEXCAN4_REINIT_START == STD_ON)
  retVal = kCan_30_Flexcan4Ok;
  if ( (mode == kCan_30_Flexcan4ModeStartReinit)
     )  /* COV_CAN_REINITSTART_STIMULATION */
  { /* #10 Check for kCan_30_Flexcan4ModeStartReinit */
    if (canLL_30_Flexcan4_canStartReinitState[canHwChannel] == kCan_30_Flexcan4LLStateStart)
    { /* #20 Check for reinitialization execution */
      retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST doRamCheck);
      canLL_30_Flexcan4_canStartReinitState[canHwChannel] = kCan_30_Flexcan4LLStateRequested; /* SBSW_CAN_LL14 */
    }
  }
  if (retVal == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
#endif
  {
    if (CanLL_30_Flexcan4_HwIsStart(canHwChannel)) /* SBSW_CAN_LL01 */
    { /* #30 Check for start transition finished */
#if (CAN_30_FLEXCAN4_REINIT_START == STD_ON)
      canLL_30_Flexcan4_canStartReinitState[canHwChannel] = kCan_30_Flexcan4LLStateStart; /* SBSW_CAN_LL14 */
#endif
#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
      canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
#endif
      retVal = kCan_30_Flexcan4Ok;
    }
    else if (CanLL_30_Flexcan4_HwIsStartRequested(canHwChannel)) /* SBSW_CAN_LL01 */ /* COV_CAN_START_STIMULATION */
    { /* #40 Check for start mode requested */
      retVal = kCan_30_Flexcan4Requested;
    }
    else
    { /* #50 Process start mode */
      {
#if defined ( C_30_FLEXCAN4_ENABLE_SILENT_MODE )
        if (mode == kCan_30_Flexcan4ModeSilent)
        { /* #60 Check for kCan_30_Flexcan4ModeSilent */
          pFlex_30_Flexcan4CANLocal->control1 |= kFlexCAN_30_Flexcan4_LOM; /* SBSW_CAN_LL01 */
        }
        else
        {
          pFlex_30_Flexcan4CANLocal->control1 &= (vuint32)(~kFlexCAN_30_Flexcan4_LOM); /* SBSW_CAN_LL01 */
        }
#endif
        /* #70 Request start mode */
        /* request NORMAL mode - considered as Can_30_Flexcan4Start mode */
        CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
        CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* clear HALT bit */ /* SBSW_CAN_LL01 */
        CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ);  /* clear FRZ bit  */ /* SBSW_CAN_LL01 */

        /* #80 Deactivate BusOff auto recovery mode */
        /* the transition Can_30_Flexcan4Stop - Can_30_Flexcan4Start does also leave a potential BusOff state */
        pFlex_30_Flexcan4CANLocal->control1 |= kFlexCAN_30_Flexcan4_BOFF_REC; /* SBSW_CAN_LL01 */

        retVal = kCan_30_Flexcan4Requested;
      }
    }
  }
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(doRamCheck); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return (retVal);
}
/* CODE CATEGORY 4 END */

#if defined ( C_30_FLEXCAN4_ENABLE_BUSOFF_RECOVERY_COMPLIANT )
/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_BusOffEndTransition
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_BusOffEndTransition
Relation_Context:
# from CanLL_30_Flexcan4_ModeTransition #
BusOffRecoveryDefine
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
 *
 *
 *
 *
 *
*/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_BusOffEndTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST vuint8 doRamCheck)
{
  vuint8 retVal;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);

  /* ----- Implementation ----------------------------------------------- */
  /* *********************************************************************** */
  /* Handle busoff end transition of FlexCAN                                      */
  /* *********************************************************************** */
  if (CanLL_30_Flexcan4_HwIsBusOff(canHwChannel)) /* SBSW_CAN_LL01 */
  { /* #10 Check for busoff state */
    if (!CanLL_30_Flexcan4_HwIsAutoRecoveryActive(canHwChannel)) /* SBSW_CAN_LL01 */ /* COV_CAN_BUSOFFEND_STIMULATION */
    { /* #20 Activate busoff auto recovery */
      pFlex_30_Flexcan4CANLocal->control1 &= (vuint32)(~kFlexCAN_30_Flexcan4_BOFF_REC); /* SBSW_CAN_LL01 */
    }
    if (!CanLL_30_Flexcan4_HwIsStartRequested(canHwChannel)) /* COV_CAN_BUSOFFEND_STIMULATION */ /* SBSW_CAN_LL01 */
    { /* #30 Start auto recovery process in case of CAN controler is not started */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* clear HALT bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ);  /* clear FRZ bit  */ /* SBSW_CAN_LL01 */
    }
    retVal = kCan_30_Flexcan4Requested;
  }
  else
  { /* #40 Check for busoff state left */
    if (CanLL_30_Flexcan4_HwIsStart(canHwChannel)) /* COV_CAN_BUSOFFEND_STIMULATION */ /* SBSW_CAN_LL01 */
    { /* #50 Check for start transition finished */
      if (CanLL_30_Flexcan4_HwIsAutoRecoveryActive(canHwChannel)) /* SBSW_CAN_LL01 */
      { /* #60 In case of autorecovery finished: Controller re-initialization */
        retVal = CanHL_30_Flexcan4_ReInit(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST doRamCheck);
        if (retVal == kCan_30_Flexcan4Ok) /* COV_CAN_BUSOFFEND_STIMULATION */
        {
          CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
          CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* clear HALT bit */ /* SBSW_CAN_LL01 */
          CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ);  /* clear FRZ bit  */ /* SBSW_CAN_LL01 */

          pFlex_30_Flexcan4CANLocal->control1 |= kFlexCAN_30_Flexcan4_BOFF_REC; /* SBSW_CAN_LL01 */
          retVal = kCan_30_Flexcan4Requested;
        }
      }
      else
      { /* #70 Start transition finished */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
        canLL_30_Flexcan4_PassiveReportFlag[canHwChannel] = kCan_30_Flexcan4True; /* SBSW_CAN_LL14 */
# endif
        retVal = kCan_30_Flexcan4Ok;
      }
    }
    else if (CanLL_30_Flexcan4_HwIsStartRequested(canHwChannel)) /* COV_CAN_BUSOFFEND_STIMULATION */ /* SBSW_CAN_LL01 */
    { /* #80 Check for start mode requested */
       retVal = kCan_30_Flexcan4Requested;
    }
    else
    { /* #90 Request start mode in case of busoff end due to a controller re-initialization */
      /* request NORMAL mode - considered as Can_30_Flexcan4Start mode */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_HALT); /* clear HALT bit */ /* SBSW_CAN_LL01 */
      CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_FRZ);  /* clear FRZ bit  */ /* SBSW_CAN_LL01 */

      /* the transition Can_30_Flexcan4Stop - Can_30_Flexcan4Start does also leave a potential BusOff state */
      pFlex_30_Flexcan4CANLocal->control1 |= kFlexCAN_30_Flexcan4_BOFF_REC; /* SBSW_CAN_LL01 */

      retVal = kCan_30_Flexcan4Requested;
    }
  }

  return (retVal);
}
/* CODE CATEGORY 4 END */
#endif


#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED ) && defined( C_30_FLEXCAN4_ENABLE_CLKSRC_SELECTION ) /* COV_CAN_RAMCHECK_EXTENDED_CLKSRC_AVAILABILITY */
/* CODE CATEGORY 4 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_CheckClockSource
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CheckClockSource
Relation_Context:
# from CanLL_30_Flexcan4_InitBeginIsRegisterCorrupt #
RamCheck
Relation:
HardwareLoopCheck
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
*/
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckClockSource(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY)
{
  vuint32 control1OldVal;
  Can_ReturnType loopResult = CAN_OK; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
  vuint8 clkSrcCheckFailed = kCan_30_Flexcan4False;
  tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel);
  
  /* #10 Enter DISABLE mode of FlexCAN */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* disable FlexCAN module before clock selection */ /* SBSW_CAN_LL01 */
  if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) != kFlexCAN_30_Flexcan4_LPM_ACK) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
  {
    CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopEnterDisableModeInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
    do
    { /* wait for FlexCAN is disabled */
      loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopEnterDisableModeInit);
    } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) != kFlexCAN_30_Flexcan4_LPM_ACK) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* SBSW_CAN_LL01 */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
    CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopEnterDisableModeInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
  }
      
  /* #20 Save configured value of control1 register */
  control1OldVal = CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(pFlex_30_Flexcan4CANLocal->control1); /* SBSW_CAN_LL01 */
      
  /* #30 Perform check of Clock Source */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC);  /* SBSW_CAN_LL01 */
  if((CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(pFlex_30_Flexcan4CANLocal->control1) & kFlexCAN_30_Flexcan4_CLK_SRC) != kFlexCAN_30_Flexcan4_CLK_SRC) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* SBSW_CAN_LL01 */
  {
    clkSrcCheckFailed = kCan_30_Flexcan4True;
  }

  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC); /* SBSW_CAN_LL01 */
  if((CAN_30_FLEXCAN4_READ_PROTECTED_REG_32BIT(pFlex_30_Flexcan4CANLocal->control1) & (kFlexCAN_30_Flexcan4_CLK_SRC)) != 0x00u) /* COV_CAN_RAM_CHECK_NO_STIMULATION */ /* SBSW_CAN_LL01 */
  {
    clkSrcCheckFailed = kCan_30_Flexcan4True;
  }
  
  /* #40 Reset FlexCAN clock source to configured value */
  if((control1OldVal & kFlexCAN_30_Flexcan4_CLK_SRC) != 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_SET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC); /* SBSW_CAN_LL01 */ /* SBSW_CAN_LL01 */
  }
  else
  {
    CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET_32BIT(pFlex_30_Flexcan4CANLocal->control1, kFlexCAN_30_Flexcan4_CLK_SRC); /* SBSW_CAN_LL01 */
  }
  
  /* #50 Leave DISABLE mode of FlexCAN */
  CAN_30_FLEXCAN4_WRITE_PROTECTED_REG_RESET(pFlex_30_Flexcan4CANLocal->canmcr, kFlexCAN_30_Flexcan4_MDIS); /* clear MDIS bit */ /* SBSW_CAN_LL01 */
  if ((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) == kFlexCAN_30_Flexcan4_LPM_ACK) /* COV_CAN_HARDWARE_PROCESSING_TIME */ /* SBSW_CAN_LL01 */
  {
    CanLL_30_Flexcan4_ApplCanTimerStart(kCan_30_Flexcan4LoopLeaveDisableModeInit); /* start hw loop timer */ /* SBSW_CAN_LL08 */
    do
    { /* wait for FlexCAN is enabled */
      loopResult = CanLL_30_Flexcan4_ApplCanTimerLoop(kCan_30_Flexcan4LoopLeaveDisableModeInit);
    } while(((CAN_30_FLEXCAN4_READ_PROTECTED_REG(pFlex_30_Flexcan4CANLocal->canmcr) & kFlexCAN_30_Flexcan4_LPM_ACK) == kFlexCAN_30_Flexcan4_LPM_ACK) && (loopResult == CAN_OK)); /* COV_CAN_TIMEOUT_DURATION */ /* SBSW_CAN_LL01 */ /* PRQA S 2995,0303 */ /* MD_Can_ConstValue */ /* MD_Can_HwAccess */
    CanLL_30_Flexcan4_ApplCanTimerEnd(kCan_30_Flexcan4LoopLeaveDisableModeInit);  /* stop hw loop timer */ /* SBSW_CAN_LL08 */
  }
  
  return clkSrcCheckFailed;
}
/* CODE CATEGORY 4 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_ComputeMailboxInterrupt
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ComputeMailboxInterrupt
Relation_Context:
# from CanLL_30_Flexcan4_CanMailboxInterrupt #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts,
Relation:
OneChOpt
DevErrorDetect,CanFdSupport
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST vuint32 iFlags, vuint32 iMask, Can_30_Flexcan4ObjectHandle startIndex) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
{
  vuint32 activeInterrupts;
  Can_30_Flexcan4ObjectHandle index;
  
  activeInterrupts = iFlags & iMask;
  index = startIndex;
  /* #10 For all active pending interrupts */
  while(activeInterrupts != 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    /* #20 Compute index of active interrupt */
    while((activeInterrupts & (vuint32)0xFFu) == 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      activeInterrupts >>= 8;
      index += (Can_30_Flexcan4ObjectHandle)8u;
    }    
    if((activeInterrupts & (vuint32)0x0Fu) == 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      activeInterrupts >>= 4;
      index += (Can_30_Flexcan4ObjectHandle)4u;
    }        
    while((activeInterrupts & (vuint32)0x01u) == 0u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      activeInterrupts >>= 1;
      index++;
    }

    /* #30 Call computation of interrupt type */
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
#  if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
    if ( Can_30_Flexcan4_GetSizeOfFDMailboxMapping() > (Can_30_Flexcan4_GetFDHwChannelIndirectionOfControllerConfig((canHwChannel)) + index) ) /* COV_CAN_GENDATA_FAILURE */
#  endif
# endif
    {
      CanLL_30_Flexcan4_ComputeInterruptType(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST index);
    }

    index++;
    activeInterrupts = (iFlags & iMask) >> (index-startIndex);
  }
}
/* CODE CATEGORY 1 END */

/* CODE CATEGORY 1 START */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_ComputeInterruptType
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ComputeMailboxInterrupt
Relation_Context:
# from CanLL_30_Flexcan4_ComputeMailboxInterrupt #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts,
Relation:
OneChOpt
RxFullCANSupport,RxPolling,IndividualPolling,DevErrorDetect
CanFdSupport,RxBasicCANSupport,RxPolling,IndividualPolling,DevErrorDetect
TxPolling,IndividualPolling,TxFullCANSupport,DevErrorDetect,MultiplexedTx
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ComputeInterruptType(CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle index) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
{

  {
    Can_30_Flexcan4ObjectHandle mailboxHandle = Can_30_Flexcan4_GetHwToMbHandle(canHwChannel, index); /* PRQA S 2981,2982*/ /* MD_MSR_RetVal */ /* PRQA S 2843 */ /* MD_Can_HL_DereferencePointerValue */

#  if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS )
#   if !defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING ) || defined( C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING )
    /* #10 Check if pending interrupt is caused due to a FullCAN reception */
    if((index >= CAN_30_FLEXCAN4_HL_HW_RX_FULL_STARTINDEX(canHwChannel)) && (index < CAN_30_FLEXCAN4_HL_HW_RX_FULL_STOPINDEX(canHwChannel))) /* COV_CAN_MAILBOX_ORDER */
    {
#    if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
      if ((mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_RX_FULL_STARTINDEX(canHwChannel)) && (mailboxHandle < CAN_30_FLEXCAN4_HL_MB_RX_FULL_STOPINDEX(canHwChannel))) /* COV_CAN_GENDATA_FAILURE */
#    endif
      {
        CanHL_30_Flexcan4_FullCanMsgReceived(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST mailboxHandle, index);
      }
    }
#   endif
#  endif

#  if !defined( C_30_FLEXCAN4_ENABLE_TX_POLLING ) || defined( C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING )
    /* #30 Check if pending interrupt is caused due to a successful transmission */
    if((index >= CAN_30_FLEXCAN4_HL_HW_TX_STARTINDEX(canHwChannel)) && (index < CAN_30_FLEXCAN4_HL_HW_TX_STOPINDEX(canHwChannel))) /* COV_CAN_MAILBOX_ORDER */
    {
#   if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
      if (
#    if defined ( C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS )
          ((mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_TX_FULL_STARTINDEX(canHwChannel)) && (mailboxHandle < CAN_30_FLEXCAN4_HL_MB_TX_FULL_STOPINDEX(canHwChannel))) ||
#    endif
          ((mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX(canHwChannel)) && (mailboxHandle < CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STOPINDEX(canHwChannel)))
      ) /* COV_CAN_GENDATA_FAILURE */
#   endif
      {
        Can_30_Flexcan4ObjectHandle mailboxElement = 0u;
#   if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION)
        if ((mailboxHandle >= CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STARTINDEX(canHwChannel)) && (mailboxHandle < CAN_30_FLEXCAN4_HL_MB_TX_BASIC_STOPINDEX(canHwChannel))) /* COV_CAN_MAILBOX_ORDER */
        {
          mailboxElement = index - Can_30_Flexcan4_GetMailboxHwHandle(mailboxHandle);
        }
#    if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
        if (mailboxElement < Can_30_Flexcan4_GetMailboxSize(mailboxHandle)) /* COV_CAN_GENDATA_FAILURE */
#    endif
#   endif
        {
          CanHL_30_Flexcan4_TxConfirmation(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST mailboxHandle, mailboxElement, index);
        }
      }
    }
#  endif
    CAN_30_FLEXCAN4_DUMMY_STATEMENT(mailboxHandle); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }
}
/* CODE CATEGORY 1 END */

/****************************************************************************
| NAME:             CanLL_30_Flexcan4_CanMailboxInterrupt
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CanMailboxInterrupt
Relation_Context:
# from Can_30_Flexcan4MailboxInterrupt, Can_30_Flexcan4Interrupt #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
CanFdSupport,RxPolling,IndividualPolling
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
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_CanMailboxInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
{
#  if !defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING ) || defined( C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING )
  /* #10 Check if pending interrupt is caused due to a BasicCAN reception (Enhanced Rx Fifo) */
#   if defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING)
  /* additional check is necessary to ensure only objects configured for interrupt are processed here */
  if (!Can_30_Flexcan4_IsMailboxIndivPolling(Can_30_Flexcan4_GetHwToERxFifoHandle(canHwChannel, 0u))) /* COV_CAN_HWOBJINDIVPOLLING */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
#   endif
  {
    tCan_30_Flexcan4ERxFifoRegisterPtr pERxFifoRegister = pCan_30_Flexcan4ERxFifoRegister(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */
    vuint32 msgCount = pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFEL_MASK;
    while(((pERxFifoRegister->erfsr & kFlexCAN_30_Flexcan4_ERFE) == 0u) && (msgCount > 0u)) /* COV_CAN_RXFIFO_HANDLING */
    {
      msgCount--;;
      CanHL_30_Flexcan4_BasicCanMsgReceived(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST Can_30_Flexcan4_GetHwToERxFifoHandle(canHwChannel, 0u), 0u); /* PRQA S 2985 */ /* MD_Can_ConstValue */
    }
  }
#  endif
  {
    vuint32 iflag;
    vuint32 imask;
    tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal = pFlex_30_Flexcan4CAN(canHwChannel); /* PRQA S 3679 */ /* MD_Can_NoneConstParameterPointer */

    /* #20 Call mailbox interrupt computation function for Mailboxes 0 to 31 */
    iflag = pFlex_30_Flexcan4CANLocal->iflag1;
    imask = pFlex_30_Flexcan4CANLocal->imask1;
    CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST iflag, imask, 0u); /* PRQA S 0404 */ /* MD_Can_LL_VolatileAccess */
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #30 Call mailbox interrupt computation function for Mailboxes 32 to 63 */
    iflag = pFlex_30_Flexcan4CANLocal->iflag2;
    imask = pFlex_30_Flexcan4CANLocal->imask2;
    CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST iflag, imask, 32u); /* PRQA S 0404 */ /* MD_Can_LL_VolatileAccess */
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
    if (CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(canHwChannel) > 64u) /* PRQA S 2741,2742 */ /* MD_Can_ConstValue */ /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  endif
    {
      /* #40 Call mailbox interrupt computation function for Mailboxes 64 to 95 */
      iflag = pFlex_30_Flexcan4CANLocal->iflag3;
      imask = pFlex_30_Flexcan4CANLocal->imask3;
      CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST iflag, imask, 64u); /* PRQA S 0404 */ /* MD_Can_LL_VolatileAccess */
    }
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    /* #50 Call mailbox interrupt computation function for Mailboxes 96 to 127 */
    iflag = pFlex_30_Flexcan4CANLocal->iflag4;
    imask = pFlex_30_Flexcan4CANLocal->imask4;
    CanLL_30_Flexcan4_ComputeMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST iflag, imask, 96u); /* PRQA S 0404 */ /* MD_Can_LL_VolatileAccess */
# endif
  }
  /* #60 Check for error active transition */
# if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) 
  CanLL_30_Flexcan4_ErrorStateActiveTransitionOccured(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif
}
/* CODE CATEGORY 1 END */
#endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */

/**********************************************************************************************************************
*  CanLL_30_Flexcan4_ClearPendingInterrupt
*********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_ClearPendingInterrupt
Relation_Context:
# from CanLL_30_Flexcan4_TxConfBegin #
Relation:
TxBasicAmount
TxFullAmount
RxFullAmount
RxBasicAmount
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
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanLL_30_Flexcan4_ClearPendingInterrupt( tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  if (hwObjHandle < 32u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* #10 Clear pending interrupt for mailbox 0 to 31 */
    pFlex_30_Flexcan4CANLocal->iflag1 = Can_30_Flexcan4BitMask(hwObjHandle); /* clear pending interrupt flag  */ /* SBSW_CAN_LL01 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
  }
#if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  else
  { /* #20 Clear pending interrupt for mailbox 32 to 63 */
    if (hwObjHandle < 64u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    {
      pFlex_30_Flexcan4CANLocal->iflag2 = Can_30_Flexcan4BitMask(hwObjHandle); /* clear pending interrupt flag  */ /* SBSW_CAN_LL01 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
    }
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    else
    { /* #30 Clear pending interrupt for mailbox 64 to 95 */
      if (hwObjHandle < 96u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      {
        pFlex_30_Flexcan4CANLocal->iflag3 = Can_30_Flexcan4BitMask(hwObjHandle); /* clear pending interrupt flag  */ /* SBSW_CAN_LL01 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
      }
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
      else
      { /* #40 Clear pending interrupt for mailbox 96 to 128 */
        pFlex_30_Flexcan4CANLocal->iflag4 = Can_30_Flexcan4BitMask(hwObjHandle); /* clear pending interrupt flag  */ /* SBSW_CAN_LL01 */ /* PRQA S 2985 */ /* MD_Can_ConstValue */
      }
#  endif
    }
# endif
  }
#endif
}
/* CODE CATEGORY 1 END */

#if defined (C_30_FLEXCAN4_ENABLE_TX_POLLING) || ( defined (C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) && defined (C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS) )
/**********************************************************************************************************************
*  CanLL_30_Flexcan4_GetInterruptFlag
*********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_GetInterruptFlag
Relation_Context:
# from CanLL_30_Flexcan4_RxFullProcessPendings #
RxFullCANSupport, RxPolling
# from CanLL_30_Flexcan4_RxBasicProcessPendings #
RxBasicCANSupport, RxPolling
# from CanLL_30_Flexcan4_TxProcessPendings #
TxFullCANSupport, TxPolling
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
 *
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint32, STATIC_CODE) CanLL_30_Flexcan4_GetInterruptFlag( tFlex_30_Flexcan4CANPtr pFlex_30_Flexcan4CANLocal, Can_30_Flexcan4ObjectHandle hwObjHandle ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  vuint32 iFlags = 0u; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */

  if (hwObjHandle < 32u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* #10 Get iflag for mailboxes 0 to 31 */
    iFlags = pFlex_30_Flexcan4CANLocal->iflag1;
  }
#if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  else
  {
    if (hwObjHandle < 64u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* #20 Get iflag for mailboxes 32 to 63 */
      iFlags = pFlex_30_Flexcan4CANLocal->iflag2;
    }
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
    else
    {
      if (hwObjHandle < 96u) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* #30 Get iflag for mailboxes 64 to 95 */
        iFlags = pFlex_30_Flexcan4CANLocal->iflag3;
      }
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
      else
      { /* #40 Get iflag for mailboxes 96 to 128 */
        iFlags = pFlex_30_Flexcan4CANLocal->iflag4;
      }
#  endif
    }
# endif
  }
#endif

  return (iFlags);
}
/* CODE CATEGORY 1 END */
#endif

#if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK ) && defined (C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
/**********************************************************************************************************************
 *  CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt
Relation_Context:
# CanHL_30_Flexcan4_InitXXX() #
Wakeup, OneChOpt, ChannelAmount
Relation:
RamCheck
DevErrorDetect
CanFdSupport
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
*/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_InitIsEnhancedRxFifoCorrupt( CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  vuint8 result;
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
  vuint8 idx;
  vuint8 i;
  tCan_30_Flexcan4ERxFifoFilterPtr pERxFifoFilter = pCan_30_Flexcan4ERxFifoFilter(canHwChannel);
  vuint8 numOfMaxFilterAvailable = Can_30_Flexcan4_GetNumberOfMaxStdFilterOfControllerConfig(canHwChannel);
# endif

  /* ----- Implementation ----------------------------------------------- */
  result = kCan_30_Flexcan4False;

# if defined( C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED )
  /* #10 Check filter table */
  for(i = 0u; i < 3u; i++)
  {
    for (idx = 0u; idx < numOfMaxFilterAvailable; idx++)
    {
      pERxFifoFilter->filterElement[idx] = Can_30_Flexcan4RamCheckPattern32[i] & kCan_30_Flexcan4RamCheckMaskEFifoFilter; /* SBSW_CAN_LL09 */
      if(pERxFifoFilter->filterElement[idx] != (Can_30_Flexcan4RamCheckPattern32[i] & kCan_30_Flexcan4RamCheckMaskEFifoFilter)) /* COV_CAN_HARDWARE_FAILURE */
      {
        result = kCan_30_Flexcan4True; /* PRQA S 2981,2982 */ /* MD_MSR_RetVal */
      }
    }
  }
# endif

  CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return result;
}
/* CODE CATEGORY 4 END */
#endif


#if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_CheckMaxHwMailboxHandle
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CheckMaxHwMailboxHandle
Relation_Context:
# from CanLL_30_Flexcan4_InitMailboxTx #
TxFullCANSupport,DevErrorDetect
# from CanLL_30_Flexcan4_InitMailboxRxFullCan #
RxFullCANSupport,DevErrorDetect
# from CanLL_30_Flexcan4_InitMailboxRxBasicCan #
RxBasicCANSupport,DevErrorDetect
Relation:
CanFdSupport
ChannelAmount
TxBasicAmount
TxFullAmount
RxFullAmount
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
*/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckMaxHwMailboxHandle(Can_30_Flexcan4ChannelHandle hwChannel, Can_HwHandleType hwObjHandle)
{
  vuint8 retVal = kCan_30_Flexcan4Ok;

  /* #10 Check for maximum value of HW mailbox handle */
# if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  if ( Can_30_Flexcan4_GetSizeOfFDMailboxMapping() <= (Can_30_Flexcan4_GetFDHwChannelIndirectionOfControllerConfig((hwChannel)) + hwObjHandle) ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
  {
    retVal = kCan_30_Flexcan4False;
  }
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(hwChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# else
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  if ( hwObjHandle >= 128u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#  elif defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  if (hwObjHandle >= CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(hwChannel)) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#   else
  if ( hwObjHandle >= 96u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#   endif
#  elif defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  if ( hwObjHandle >= 64u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#  else
#   if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  if (hwObjHandle >= CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(hwChannel)) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#   else
  if ( hwObjHandle >= 32u ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL04 */
#   endif
#  endif
  {
    retVal = kCan_30_Flexcan4False;
  }
#  if !defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(hwChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
# endif

  return ( retVal );
}
/* CODE CATEGORY 4 END */

/****************************************************************************
| NAME:             CanLL_30_Flexcan4_CheckEnhancedRxFifoFilter
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CheckMaxHwMailboxHandle
Relation_Context:
# from CanLL_30_Flexcan4_InitMailboxRxBasicCan #
RxBasicCANSupport,DevErrorDetect
Relation:
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
*/
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckEnhancedRxFifoFilter(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4InitHandle initObject)
{
  vuint8 retVal = kCan_30_Flexcan4Ok;

  /* #10 Check for maximum value of filter index tables */
  if ((Can_30_Flexcan4_GetSizeOfInitBasicCanIndexStd() <= (initObject + 1u)) || (Can_30_Flexcan4_GetSizeOfInitBasicCanIndexExt() <= (initObject + 1u))) /* PRQA S 2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  {
    retVal = kCan_30_Flexcan4False;
  }
  else
  {
    vuint16 idxStdStart = Can_30_Flexcan4_GetInitBasicCanIndexStd(initObject);
    vuint16 idxStdStop = Can_30_Flexcan4_GetInitBasicCanIndexStd(initObject + 1u);
    vuint16 idxExtStart = Can_30_Flexcan4_GetInitBasicCanIndexExt(initObject);
    vuint16 idxExtStop = Can_30_Flexcan4_GetInitBasicCanIndexExt(initObject + 1u);
    vuint16 numOfStd = idxStdStop - idxStdStart;
    vuint16 numOfExt = (idxExtStop - idxExtStart) << 1u;
    vuint16 numOfFilterElements = numOfStd + numOfExt;
    vuint16 numOfMaxFilterAvailable = (vuint16)Can_30_Flexcan4_GetNumberOfMaxStdFilterOfControllerConfig(canHwChannel);

    /* #20 Check for maximum value of filter tables */
    if ((idxStdStart > (vuint16)(Can_30_Flexcan4_GetSizeOfInitBasicCanStd())) || (idxStdStop > (vuint16)(Can_30_Flexcan4_GetSizeOfInitBasicCanStd() + 1u)) || /* PRQA S 4391 */ /* MD_Can_IntegerCast */
        (idxExtStart > (vuint16)(Can_30_Flexcan4_GetSizeOfInitBasicCanExt())) || (idxExtStop > (vuint16)(Can_30_Flexcan4_GetSizeOfInitBasicCanExt() + 1u))) /* COV_CAN_GENDATA_FAILURE */ /* PRQA S 4391 */ /* MD_Can_IntegerCast */
    {
      retVal = kCan_30_Flexcan4False;
    }

    /* #30 Check for maximum number of filter elements */
    if (numOfFilterElements > numOfMaxFilterAvailable) /* COV_CAN_GENDATA_FAILURE */
    {
      retVal = kCan_30_Flexcan4False;
    }
  }

  return ( retVal );
}
/* CODE CATEGORY 4 END */




# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
     defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/****************************************************************************
| NAME:             CanLL_30_Flexcan4_CheckSizeOfHwChannelData
****************************************************************************/
/*
|<DataModelStart>| CanLL_30_Flexcan4_CheckSizeOfHwChannelData
Relation_Context:
# from Can_30_Flexcan4Interrupt #
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxInterrupt #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusoffInterrupt #
StatusPolling,UseNestedCANInterrupts
Relation:
OneChOpt
DevErrorDetect,TxPolling,RxPolling,IndividualPolling
DevErrorDetect,StatusPolling,WakeupPolling
DevErrorDetect,StatusPolling
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, vuint8, STATIC_CODE) CanLL_30_Flexcan4_CheckSizeOfHwChannelData(Can_30_Flexcan4ChannelHandle hwChannel) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
{
  vuint8 retVal = kCan_30_Flexcan4Ok;

  /* #10 Check size of canHwChannel against size of HwChannelData */
  if ( hwChannel >= kCan_30_Flexcan4NumberOfChannels ) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL07 */
  {
    retVal = kCan_30_Flexcan4Failed;
  }
  else
  {
    /* #20 Check if controller is initialized */
    if (!(CanHL_30_Flexcan4_IsControllerInit(hwChannel))) /* COV_CAN_CONTROLLER_INIT_STATE  */
    {
      retVal = kCan_30_Flexcan4Failed;
    }
  }

  return ( retVal );
}
/* CODE CATEGORY 4 END */
# endif
#endif

#if defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_TxConfirmationPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_TxConfirmationPolling
Relation_Context:
# from CanLL_30_Flexcan4_TxProcessPendings #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_TxConfirmationPolling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle mailboxHandle, Can_30_Flexcan4ObjectHandle mailboxElement, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. (avoid nested call of confirmation (individual polling) and CancelTx interrupt confirmation out of Can interrupt like TP) */
  Can_30_Flexcan4_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_30_Flexcan4_TxConfirmation() for handling */
  CanHL_30_Flexcan4_TxConfirmation(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST mailboxHandle, mailboxElement, hwObjHandle);
  Can_30_Flexcan4_EnableControllerInterrupts((uint8)channel);
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_TxConfirmation
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_TxConfirmation
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmationPolling #
# from Tx Interrupt #
TxPolling, IndividualPolling
Relation:
TxHwFifo, GenericConfirmation
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_TxConfirmation( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle mailboxHandle, Can_30_Flexcan4ObjectHandle mailboxElement, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  /* \trace SPEC-1574 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCan_30_Flexcan4TxConfirmationParaStruct txConfPara;
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
  tCan_30_Flexcan4TxConfInfoStruct txConfInfoStruct = {0}; /* init see TAR-171 */
#endif
  CanHookBegin_CanHL_30_Flexcan4_TxConfirmation();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  txConfPara.mailboxHandle = mailboxHandle;
  txConfPara.mailboxElement = mailboxElement; /* PRQA S 2983 */ /* MD_MSR_DummyStmt */
  txConfPara.hwObjHandle = hwObjHandle; /* PRQA S 2983 */ /* MD_MSR_DummyStmt */
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
  txConfPara.txStructConf = &txConfInfoStruct;
#endif
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  /* #10 Check if parameter controller and hwObjHandle is valid (only for Interrupt system, polling do this by caller) */
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( 
#  if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
       ((txConfPara.mailboxHandle < Can_30_Flexcan4_GetTxFullHandleStart(canHwChannel)) || (txConfPara.mailboxHandle >= Can_30_Flexcan4_GetTxFullHandleStop(canHwChannel))) &&  /* PRQA S 2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
#  endif
       ((txConfPara.mailboxHandle < Can_30_Flexcan4_GetTxBasicHandleStart(canHwChannel)) || (txConfPara.mailboxHandle >= Can_30_Flexcan4_GetTxBasicHandleStop(canHwChannel))) ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    PduIdType tmp_pdu;
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
    uint8_least tmp_state;
#endif
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
    Can_ReturnType generic_retval;
#endif
    txConfPara.activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(txConfPara.mailboxHandle, txConfPara.mailboxElement); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
    {
      /* #120 Pre Release HW transmit object (LL) */
      CanLL_30_Flexcan4_TxConfBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
      /* #130 Remember PDU for confirmation parameter */
      tmp_pdu   = Can_30_Flexcan4_GetActiveSendPdu(txConfPara.activeSendObject);
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
      /* #140 Remember SEND STATE which may be modified due to new transmit call in indication */
      tmp_state = (uint8_least)Can_30_Flexcan4_GetActiveSendState(txConfPara.activeSendObject);
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW) /* cancel interrupt occur */
      /* #150 If the message was successfully transmitted (without cancellation) (LL): */
      if (CanLL_30_Flexcan4_TxConfIsMsgTransmitted(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara) == kCan_30_Flexcan4True) /* SBSW_CAN_HL45 */ /* COV_CAN_HW_TRANSMIT_CANCELLATION */
#endif
      { /* #200 Set send state to free to allow next TX out of confirmation or direct (reload queue) */
        /* #210 Notify the application by call Appl_GenericConfirmation() and CanIf_TxConfirmation() */
        /*      - Appl_GenericConfirmation() if configured and mirror mode is active
                and afterwards depend of return value
                - CanIf_TxConfirmation() if generic confirmation return OK, or no generic is used, and TX is not cancelled */
#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API1)
        Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
        if (Appl_30_Flexcan4_GenericConfirmation( tmp_pdu ) == CAN_OK)
#elif defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
# if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
        if (Can_30_Flexcan4_GetMirrorModeState(channel) == CDDMIRROR_INACTIVE)
        { /* MirrorMode is inactive so call the following CanIf_TxConfirmation() */
          Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
          generic_retval = CAN_OK;
        }
        else
# endif
        { /* #230 For generic confirmation with API2 copy PDU data as parameter for Appl_GenericConfirmation() */
          { /* #250 in case of none FIFO get data out of CAN cell buffer (LL txStructConf) */
            Can_PduType canPdu;
            /* #260 get confirmation data out of hardware */
            CanLL_30_Flexcan4_TxConfSetTxConfStruct(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
            CanHL_30_Flexcan4_SetCanPduInfo(&canPdu, &txConfPara); /* SBSW_CAN_HL32 */ /* SBSW_CAN_HL45 */
            canPdu.swPduHandle = tmp_pdu;
            Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */ /* release after copy data to avoid data inconsistency */
            /* call Appl_GenericConfirmation() and in case of CAN_OK also the following CanIf_TxConfirmation() */
            generic_retval = Appl_30_Flexcan4_GenericConfirmation( (uint8)channel, &canPdu ); /* SBSW_CAN_HL32 */
          }
        }
        /* #300 Appl_GenericConfirmation() decide to call Indication */
        if (generic_retval == CAN_OK)
#else /* no generic confirmation */
        Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
#endif
        {
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
          /* #310 If TX is SW cancelled call CanIf_CancelTxNotification() */
          if (tmp_state == kCan_30_Flexcan4BufferCancelSw) /* COV_CAN_CANCEL_SW_STIMULATION */
          {
            CanIf_30_Flexcan4_CancelTxNotification(tmp_pdu, FALSE);
          }
          else
#endif  /* #320 otherwise call CanIf_TxConfirmation() */
          {
            CanIf_30_Flexcan4_TxConfirmation(tmp_pdu); /* \trace SPEC-1726, SPEC-1571 */
          }
        }
#if (defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API1) || defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)) && (CAN_30_FLEXCAN4_TRANSMIT_BUFFER == STD_ON)
        /* #350 otherwise no CanIf_TxConfirmation() will be called so call CanIf_CancelTxNotification() just to notify IF to restart sending out of Queue */
        else
        {
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
          CanIf_30_Flexcan4_CancelTxNotification( tmp_pdu, FALSE );
# else
#  error "When using Generic Confirmation and Transmit buffer (If) the Cancel-support-api (if) has to be activated"
# endif
        }
#endif
      }
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW)
      /* #400 otherwise mailbox is cancelled successful */
      else
      {
#  if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
        if (tmp_state == kCan_30_Flexcan4BufferCancelSw) /* COV_CAN_CANCEL_SW_STIMULATION */
        { /* #410 free active PDU to allow next TX out of confirmation or direct (reload queue) */
          Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree); /* SBSW_CAN_HL04 */
          /* #420 SW cancellation: just call CanIf_CancelTxNotification() (no interrupt lock used here! - worst case Can_Write() came in between and a unnecessary cancellation appear) */
          CanIf_30_Flexcan4_CancelTxNotification( tmp_pdu, TRUE );
        }
        else
#  endif
        {
#  if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) /* COV_CAN_CANCEL_IN_HW TX */
          /* always CancelSW when only FullCAN TX is cancelled so never called */
#   if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
          /* #440 Check send state is not free now, to secure that ActiveSendPduInfo is already set before used */
          if (Can_30_Flexcan4_GetActiveSendState(txConfPara.activeSendObject) == kCan_30_Flexcan4BufferFree) /* COV_CAN_HARDWARE_FAILURE */
          {
            errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
          }
          else
#   endif
          { /* #450 EXCLUSIVE_AREA_3 (needed to avoid another Can_Write() while interface send out of queue (first send out of queue avoid inversion)) -> no call to upper layer here */
            SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_3();
            /* #460 free active PDU to allow next TX out of confirmation or direct (reload queue) */
            Can_30_Flexcan4_SetActiveSendState(txConfPara.activeSendObject, kCan_30_Flexcan4BufferFree);  /* SBSW_CAN_HL04 */          
            /* #470 call CanIf_CancelTxConfirmation() to re trigger queue \trace SPEC-1726, SPEC-1725, SPEC-1684, SPEC-1689 */
            CanIf_30_Flexcan4_CancelTxConfirmation( Can_30_Flexcan4_GetActiveSendSwPduHandle(txConfPara.activeSendObject), &Can_30_Flexcan4_GetActiveSendPduInfo(txConfPara.activeSendObject) ); /* SBSW_CAN_HL25 */
            SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_3();
          }
#  endif
        }
      }
#endif
      /* #500 Post release HW transmit object (LL) */
      CanLL_30_Flexcan4_TxConfEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txConfPara); /* SBSW_CAN_HL45 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
# if !defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING */
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_TXCNF_ID, errorId);
  }
# endif
#endif
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CanHookEnd_CanHL_30_Flexcan4_TxConfirmation();
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */


#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_SetCanPduInfo
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_SetCanPduInfo
Relation_Context:
# from CanHL_30_Flexcan4_TxConfirmation #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_SetCanPduInfo(Can_30_Flexcan4_PduInfoPtrType_var PduInfo, CanTxConfirmationParaStructPtr txConfPara) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */ /* PRQA S 3673 */ /* MD_Can_NoneConstParameterPointer */
{
  /* #10 set data pointer */
  PduInfo->sdu = (Can_SduPtrType) txConfPara->txStructConf->pChipData; /* PRQA S 0312 */ /* MD_Can_NoneVolatilePointerCast */ /* SBSW_CAN_HL46 */
  /* #20 set data length */
  PduInfo->length = Can_30_Flexcan4TxActualDLC(txConfPara->txStructConf); /* SBSW_CAN_HL46 */
  /* #30 set ID */
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
#  if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
  if ( Can_30_Flexcan4TxActualIdType(txConfPara->txStructConf) == kCan_30_Flexcan4IdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { /* Mixed ID */
    PduInfo->id = (Can_IdType)Can_30_Flexcan4TxActualStdId(txConfPara->txStructConf); /* PRQA S 4391 */ /* MD_Can_IntegerCast */ /* SBSW_CAN_HL46 */
  }
  else
#  endif  /* pure Extended ID */
  { /* Extended ID */
    PduInfo->id = (Can_IdType)(Can_30_Flexcan4TxActualExtId(txConfPara->txStructConf) | CAN_30_FLEXCAN4_ID_IDE_MASK); /* SBSW_CAN_HL46 */
  }
# else    /* Standard ID */
  PduInfo->id = Can_30_Flexcan4TxActualStdId(txConfPara->txStructConf); /* SBSW_CAN_HL46 */
# endif
# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
  if ( Can_30_Flexcan4TxActualFdType(txConfPara->txStructConf) == kCan_30_Flexcan4FdTypeFd )
  {
    PduInfo->id |= (Can_IdType)CAN_30_FLEXCAN4_ID_FD_MASK; /* SBSW_CAN_HL46 */
  }
# endif
  PduInfo->id &= CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT; /* SBSW_CAN_HL46 */
}
#endif


#if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
# if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING) /* COV_CAN_RX_BASICCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_BasicCanMsgReceivedPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_BasicCanMsgReceivedPolling
Relation_Context:
# from CanLL_30_Flexcan4_RxBasicProcessPendings #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_BasicCanMsgReceivedPolling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. */
  Can_30_Flexcan4_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_30_Flexcan4_BasicCanMsgReceived() for handling */
  CanHL_30_Flexcan4_BasicCanMsgReceived( CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST rxMailboxHandle, hwObjHandle );
  Can_30_Flexcan4_EnableControllerInterrupts((uint8)channel);
}
#endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_BasicCanMsgReceived
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_BasicCanMsgReceived
Relation_Context:
# from CanHL_30_Flexcan4_BasicCanMsgReceivedPolling #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_BasicCanMsgReceived( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCan_30_Flexcan4RxBasicParaStruct rxBasicPara;
  CanHookBegin_CanHL_30_Flexcan4_BasicCanMsgReceived();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  rxBasicPara.mailboxHandle = rxMailboxHandle;
  rxBasicPara.hwObjHandle = hwObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON) && !defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING)
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif 
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanLL_30_Flexcan4_RxBasicMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanLL_30_Flexcan4_RxBasicMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &rxBasicPara) == kCan_30_Flexcan4Ok) /* SBSW_CAN_HL26 */ /* COV_CAN_RxBasicMsgReceivedBegin */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
      /* -------------------------- RAM check failed --- */
      if ((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) != CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
# if defined(C_30_FLEXCAN4_ENABLE_OVERRUN)
        /* -------------------------- Overrun --- */
        if (rxBasicPara.isOverrun == kCan_30_Flexcan4True)
        { /* #40 Overrun occur so notify upper layer \trace SPEC-1686 */
#   if (CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_DET)
          Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_RXINDI_ID, CAN_30_FLEXCAN4_E_DATALOST); /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL */
          Appl_30_Flexcan4_CanOverrun( (uint8)channel );
#   endif
        }
# endif
        /* \trace SPEC-1346 */
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxBasicPara.rxStruct.localDlc = Can_30_Flexcan4RxActualDLC((&(rxBasicPara.rxStruct)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
#  if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
          if ( Can_30_Flexcan4RxActualIdType((&(rxBasicPara.rxStruct))) == kCan_30_Flexcan4IdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID */
            rxBasicPara.rxStruct.localId = (Can_IdType)Can_30_Flexcan4RxActualStdId((&(rxBasicPara.rxStruct)));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxBasicPara.rxStruct.localId = (Can_IdType)(Can_30_Flexcan4RxActualExtId((&(rxBasicPara.rxStruct))) | CAN_30_FLEXCAN4_ID_IDE_MASK);
          }
          {
# else 
          { /* pure - standard ID */
            rxBasicPara.rxStruct.localId = Can_30_Flexcan4RxActualStdId((&(rxBasicPara.rxStruct)));
# endif
# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
            if ( Can_30_Flexcan4RxActualFdType((&(rxBasicPara.rxStruct))) == kCan_30_Flexcan4FdTypeFd )
            { /* \trace SPEC-60428 */
              rxBasicPara.rxStruct.localId |= (Can_IdType)CAN_30_FLEXCAN4_ID_FD_MASK;
            }
            /* DLC is not bigger than expected length (8) for CLASSIC CAN msg (additional to HashTag 145) */
            if ((Can_30_Flexcan4RxActualFdType((&(rxBasicPara.rxStruct))) != kCan_30_Flexcan4FdTypeFd) && (rxBasicPara.rxStruct.localDlc > 8u)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
            }
            else
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING) || defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING))
            /* #140 Check HRH is BasicCAN (already checked in polling task - do it only for interrupt) */
            if ( (rxBasicPara.mailboxHandle < Can_30_Flexcan4_GetRxBasicHandleStart(canHwChannel)) ||
                 (rxBasicPara.mailboxHandle >= Can_30_Flexcan4_GetRxBasicHandleStop(canHwChannel)) ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
            } 
            else
#  endif
# endif
            /* #145 DLC is not bigger than expected length from mailbox (MAX over all PDU for this mailbox) */
            if (rxBasicPara.rxStruct.localDlc > CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(rxBasicPara.mailboxHandle)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
            }
            else
            {
              /* -------------------------- Notification --- */
              /* #150 RX queue and notification handling CanHL_30_Flexcan4_RxMsgReceivedNotification() */
              rxBasicPara.rxStruct.localMailboxHandle = rxBasicPara.mailboxHandle;
              errorId = CanHL_30_Flexcan4_RxMsgReceivedNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &(rxBasicPara.rxStruct)); /* SBSW_CAN_HL26 */
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_30_Flexcan4_RxBasicMsgReceivedEnd() */
    CanLL_30_Flexcan4_RxBasicReleaseObj(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &rxBasicPara); /* SBSW_CAN_HL26 */
    CanLL_30_Flexcan4_RxBasicMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &rxBasicPara); /* SBSW_CAN_HL26 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_RXINDI_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CanHookEnd_CanHL_30_Flexcan4_BasicCanMsgReceived();
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS */

#if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
# if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_FullCanMsgReceivedPolling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_FullCanMsgReceivedPolling
Relation_Context:
# from CanLL_30_Flexcan4_RxFullProcessPendings #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_FullCanMsgReceivedPolling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle )
{
  /* #10 Lock CAN controller interrupts. */
  Can_30_Flexcan4_DisableControllerInterrupts((uint8)channel);
  /* #20 call CanHL_30_Flexcan4_FullCanMsgReceived() for handling */
  CanHL_30_Flexcan4_FullCanMsgReceived( CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST rxMailboxHandle, hwObjHandle );
  Can_30_Flexcan4_EnableControllerInterrupts((uint8)channel);
}
# endif

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_FullCanMsgReceived
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_FullCanMsgReceived
Relation_Context:
# from CanLL_30_Flexcan4_FullCanMsgReceivedPolling #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_FullCanMsgReceived( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST Can_30_Flexcan4ObjectHandle rxMailboxHandle, Can_30_Flexcan4ObjectHandle hwObjHandle ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  tCan_30_Flexcan4RxFullParaStruct rxFullPara;
  CanHookBegin_CanHL_30_Flexcan4_FullCanMsgReceived();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  rxFullPara.mailboxHandle = rxMailboxHandle;
  rxFullPara.hwObjHandle = hwObjHandle;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
#  if !defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) /* COV_CAN_RX_FULLCAN_POLLING */
#   if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit (already checked in polling task - do it only for interrupt) */
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#   endif
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #25 receive handling begin CanLL_30_Flexcan4_RxFullMsgReceivedBegin() \trace SPEC-1682, SPEC-1694 */
    if (CanLL_30_Flexcan4_RxFullMsgReceivedBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara) == kCan_30_Flexcan4Ok) /* SBSW_CAN_HL27 */ /* COV_CAN_RxFullMsgReceivedBegin */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
    {
  /* -------------------------- RAM check failed --- */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
      if ((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) != CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) /* COV_CAN_HARDWARE_FAILURE */
# endif
      { /* #30 RAM CHECK pass so continue reception */
        /* -------------------------- Overrun --- */
# if defined(C_30_FLEXCAN4_ENABLE_FULLCAN_OVERRUN)
        if (rxFullPara.isOverrun == kCan_30_Flexcan4True)
        {
          /* #40 Overrun occur so notify upper layer */
#   if (CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_DET)
          Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_RXINDI_ID, CAN_30_FLEXCAN4_E_DATALOST); /* \trace SPEC-1686 */ /* Call DET direct because Overrun will be notified additional to following DETs */
#   else /* CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL */
          Appl_30_Flexcan4_CanFullCanOverrun( (uint8)channel );
#   endif
        }
# endif
        {
          /* -------------------------- Get DLC and Data Pointer --- */
          /* #60 get ID, DLC and data from HW \trace SPEC-1691, SPEC-1693 */
          rxFullPara.rxStruct.localDlc = Can_30_Flexcan4RxActualDLC((&(rxFullPara.rxStruct)));
          /* -------------------------- Get ID  &  reject unwanted ID type --- */
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_ID)
#  if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
          if ( Can_30_Flexcan4RxActualIdType((&(rxFullPara.rxStruct))) == kCan_30_Flexcan4IdTypeStd) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
          { /* mixed - standard ID */
            rxFullPara.rxStruct.localId = (Can_IdType)Can_30_Flexcan4RxActualStdId((&(rxFullPara.rxStruct)));
          }
          else
#  endif 
          { /* mixed or pure - extended ID */
            rxFullPara.rxStruct.localId = (Can_IdType)(Can_30_Flexcan4RxActualExtId((&(rxFullPara.rxStruct))) | CAN_30_FLEXCAN4_ID_IDE_MASK);
          }
# else 
          rxFullPara.rxStruct.localId = Can_30_Flexcan4RxActualStdId((&(rxFullPara.rxStruct)));
# endif
          { /* ID has valid IdType */
# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
            if ( Can_30_Flexcan4RxActualFdType((&(rxFullPara.rxStruct))) == kCan_30_Flexcan4FdTypeFd )
            { /* \trace SPEC-60428 */
              rxFullPara.rxStruct.localId |= (Can_IdType)CAN_30_FLEXCAN4_ID_FD_MASK;
            }
            /* DLC is not bigger than expected length (8) for CLASSIC CAN msg (additional to HashTag 145) */
            if ((Can_30_Flexcan4RxActualFdType((&(rxFullPara.rxStruct))) != kCan_30_Flexcan4FdTypeFd) && (rxFullPara.rxStruct.localDlc > 8u)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
            }
            else
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
#  if (!defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING) || defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING)) /* COV_CAN_RX_FULLCAN_POLLING */
            /* #140 Check HRH is FullCAN (already checked in polling task - do it only for interrupt) */
            if ( (rxFullPara.mailboxHandle < Can_30_Flexcan4_GetRxFullHandleStart(canHwChannel)) ||
                 (rxFullPara.mailboxHandle >= Can_30_Flexcan4_GetRxFullHandleStop(canHwChannel)) )  /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* CM_CAN_HL18 */ /* COV_CAN_GENDATA_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
            }
            else
#  endif
# endif
            /* #145 DLC is not bigger than expected length from mailbox */
            if (rxFullPara.rxStruct.localDlc > CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(rxFullPara.mailboxHandle)) /* ESCAN00084263 */ /* COV_CAN_HARDWARE_FAILURE */
            {
              errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
            }
            else
            {
              /* -------------------------- Notification --- */
              /* #150 RX queue and notification handling CanHL_30_Flexcan4_RxMsgReceivedNotification() */
              rxFullPara.rxStruct.localMailboxHandle = rxFullPara.mailboxHandle;
              errorId = CanHL_30_Flexcan4_RxMsgReceivedNotification(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &(rxFullPara.rxStruct)); /* SBSW_CAN_HL27 */
            }
          }
        }
      }
    }
    /* #160 receive handling end CanHL_30_Flexcan4_RxFullMsgReceivedEnd() */
    CanLL_30_Flexcan4_RxFullReleaseObj(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara); /* SBSW_CAN_HL27 */
    CanLL_30_Flexcan4_RxFullMsgReceivedEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &rxFullPara); /* SBSW_CAN_HL27 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_RXINDI_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CanHookEnd_CanHL_30_Flexcan4_FullCanMsgReceived();
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */
#endif /* C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_ErrorHandling
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_ErrorHandling
Relation_Context:
# from Can_30_Flexcan4_MainFunction_BusOff() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_STATIC, void, STATIC_CODE) CanHL_30_Flexcan4_ErrorHandling( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
# if !defined(C_30_FLEXCAN4_ENABLE_ERROR_POLLING) /* COV_CAN_ERROR_POLLING */
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  /* #10 Check if parameter controller is in expected limit */
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Pre Error handling (LL) */
    CanLL_30_Flexcan4_ErrorHandlingBegin(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
    /* #30 Check if Transition to error state passive occured (LL) */
    if (CanLL_30_Flexcan4_ErrorStatePassiveTransitionOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True)
    {
      uint16 rxErrorCount; /* ESCAN00110429 */
      uint16 txErrorCount;
      /* #35 Report the Rx/Tx error counters */
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
      rxErrorCount = Can_30_Flexcan4RxActualErrorCounter();
      txErrorCount = Can_30_Flexcan4TxActualErrorCounter();
# else
      rxErrorCount = Can_30_Flexcan4RxActualErrorCounter(channel);
      txErrorCount = Can_30_Flexcan4TxActualErrorCounter(channel);
# endif
      CanHL_30_Flexcan4_ControllerErrorStatePassive(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST rxErrorCount, txErrorCount);
    }
    /* #40 Error Notification Handling (LL) */
    CanLL_30_Flexcan4_ErrorNotificationHandling(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#endif
    if ( (CanLL_30_Flexcan4_BusOffOccured(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True) && (!Can_30_Flexcan4_IsIsBusOff(channel) /* avoid repeated call */)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
    {
      /* #90 EXCLUSIVE_AREA_6 secure mode changes */
      SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
      /* #100 BUSOFF occur -> ECU Manager restart Controller (no ResetBusOffStart/End needed) */
      Can_30_Flexcan4_SetBusOffCounter(channel, 3); /* SBSW_CAN_HL33 */
      Can_30_Flexcan4_SetIsBusOff(channel, TRUE); /* SBSW_CAN_HL08 */
      /* #110 ResetBusOffStart need when BUSOFF handled by Application */
      (void)CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeResetBusOffStart, kCan_30_Flexcan4ContinueBusOffRecovery, kCan_30_Flexcan4SuppressRamCheck);
      /* #115 Controller is in STOP mode after ResetBusOffStart \trace SPEC-1578, SPEC-1664, SPEC-1663 */
      /* #120 Status changed to STOP - no more call of STOP from CAN Interface */
      Can_30_Flexcan4_SetLogStatus(channel, (uint8)((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_MASK_NOMODE) | CAN_30_FLEXCAN4_STATUS_STOP)); /* SBSW_CAN_HL02 */
      SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
      /* #130 notify higher layer by call CanIf_ControllerBusOff() \trace SPEC-1726, SPEC-1578 */
      CanIf_30_Flexcan4_ControllerBusOff((uint8)Can_30_Flexcan4_GetCanToCanIfChannelMapping(channel));
    }
    /* #200 Post Error handling (LL) */
    CanLL_30_Flexcan4_ErrorHandlingEnd(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_FAILURE */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_CTRBUSOFF_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
/****************************************************************************
| NAME:             CanHL_30_Flexcan4_ControllerErrorStatePassive
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_ControllerErrorStatePassive
Relation_Context:
# from CanHL_30_Flexcan4_ErrorHandling() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_ControllerErrorStatePassive( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint16 RxErrorCounter, uint16 TxErrorCounter )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Notify passive state to the CanIf and pass on the error counters */
  CanIf_ControllerErrorStatePassive(channel, RxErrorCounter, TxErrorCounter);
}

/****************************************************************************
| NAME:             CanHL_30_Flexcan4_ErrorNotification
****************************************************************************/
/*
|<DataModelStart>| CanHL_30_Flexcan4_ErrorNotification
Relation_Context:
# from CanLL_30_Flexcan4_ErrorNotificationHandling() #
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
V_DEF_30_FLEXCAN4_FUNC(CAN_LOCAL_INLINE, void, STATIC_CODE) CanHL_30_Flexcan4_ErrorNotification( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST Can_ErrorType CanError ) /* PRQA S 3219 */ /* MD_Can_HL_UnusedFunction */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Notify the error to CanIf */
  CanIf_ErrorNotification(channel, CanError);
}
#endif /* CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING */


#define CAN_30_FLEXCAN4_STOP_SEC_STATIC_CODE  /*---------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*-------------------------------------------------------------------------*/

#define CAN_30_FLEXCAN4_START_SEC_CODE  /*---------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CAN_30_FLEXCAN4_VERSION_INFO_API == STD_ON) /* \trace SPEC-1716 */
/****************************************************************************
| NAME:             Can_30_Flexcan4_GetVersionInfo
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetVersionInfo
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_GetVersionInfo( Can_30_Flexcan4_VersionInfoPtrType VersionInfo )
{ /* \trace SPEC-1723 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter versionInfo is an illegal null pointer \trace SPEC-1721 */
  if (VersionInfo == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
# endif
  { /* #100 copy version info to given pointer parameter \trace SPEC-1717 */
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID   = CAN_30_FLEXCAN4_VENDOR_ID; /* SBSW_CAN_HL10 */
    VersionInfo->moduleID   = CAN_30_FLEXCAN4_MODULE_ID; /* SBSW_CAN_HL10 */
    VersionInfo->sw_major_version = (uint8)CAN_30_FLEXCAN4_SW_MAJOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_minor_version = (uint8)CAN_30_FLEXCAN4_SW_MINOR_VERSION; /* SBSW_CAN_HL10 */
    VersionInfo->sw_patch_version = (uint8)CAN_30_FLEXCAN4_SW_PATCH_VERSION; /* SBSW_CAN_HL10 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_VERSION_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif

/****************************************************************************
| NAME:             Can_30_Flexcan4_InitMemory
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_InitMemory
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_InitMemory( void )
{ /* BR:011 */
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 mark driver as uninitialized \trace SPEC-1650 */
  can_30_Flexcan4ConfigInitFlag = CAN_30_FLEXCAN4_STATUS_UNINIT;
#if defined(CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER)
  /* #20 reset global configuration pointer */
  Can_30_Flexcan4_ConfigDataPtr = NULL_PTR;
#endif /* otherwise constant pointer is used */
  /* #30 HW reset memory */
  CanLL_30_Flexcan4_InitMemoryPowerOn(); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
#if(CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_OFF)
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(can_30_Flexcan4ConfigInitFlag); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}


#if ((CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON) || (CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_OFF))
/****************************************************************************
| NAME:             Can_30_Flexcan4_ChangeBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_ChangeBaudrate
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_ChangeBaudrate( uint8 Controller, V_DEF_30_FLEXCAN4_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20314 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  retval = E_NOT_OK;
  errorId = CAN_30_FLEXCAN4_E_PARAM_BAUDRATE; /* explicit default as Issue - remove when baud rate fit */ /* \trace SPEC-20321 */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-20338 */
  if ( can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT )
  {
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCan_30_Flexcan4NumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-20331 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_STOP) != CAN_30_FLEXCAN4_STATUS_STOP )
  { /* \trace SPEC-1655, SPEC-20312 */
    errorId = CAN_30_FLEXCAN4_E_TRANSITION;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_30_Flexcan4_GetInitObjectStartIndex(channel); baudrateIndex < Can_30_Flexcan4_GetInitObjectStartIndex(channel + 1u); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_30_Flexcan4_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 set baud rate and reinitialize controller to activate baud rate \trace SPEC-1669 */
        uint8 transitionState;
        Can_30_Flexcan4_SetLastInitObject(channel, (uint8)baudrateIndex); /* SBSW_CAN_HL11 */
        transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4SuppressRamCheck);
        if (transitionState == kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */
        { /* #50 check transition STOP (Reinit) is successful */
          Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
        }
        retval = E_OK;
        errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_CHANGE_BR_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_30_Flexcan4_CheckBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_CheckBaudrate
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_CheckBaudrate( uint8 Controller, V_DEF_30_FLEXCAN4_CONST(V_NONE, uint16, APPL_CONST) Baudrate )
{ /* \trace SPEC-20311 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  retval = E_NOT_OK;
  errorId = CAN_30_FLEXCAN4_E_PARAM_BAUDRATE; /* \trace SPEC-20317 */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT \trace SPEC-20328, SPEC-20318 */
  if ( can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT )
  {
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCan_30_Flexcan4NumberOfChannels )
  { /* \trace SPEC-20335 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex;
    for (baudrateIndex = Can_30_Flexcan4_GetInitObjectStartIndex(channel); baudrateIndex < Can_30_Flexcan4_GetInitObjectStartIndex(channel + 1u); baudrateIndex++)
    { /* #30 search for baud rate configuration */
      if (Can_30_Flexcan4_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #40 requested baud rate is set - return OK */
        retval = E_OK;
        errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_CHECK_BR_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON */

#if (CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_ON)
/****************************************************************************
| NAME:             Can_30_Flexcan4_SetBaudrate
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_SetBaudrate
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )
{ /* \trace SPEC-50605 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval;
  uint8 errorId;
  /* #5 return E_NOT_OK in case no fitting baudrate is found */
  retval = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT and STOP \trace SPEC-50595 */
  if ( can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT )
  {
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if ( channel >= kCan_30_Flexcan4NumberOfChannels ) /* CM_CAN_HL01 */
  { /* \trace SPEC-50587 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if ( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_STATUS_STOP) != CAN_30_FLEXCAN4_STATUS_STOP )
  { /* \trace SPEC-50584 */
    errorId = CAN_30_FLEXCAN4_E_TRANSITION;
  }
  else
  if ( BaudRateConfigID >= (uint16)(Can_30_Flexcan4_GetInitObjectStartIndex(channel + 1u) - Can_30_Flexcan4_GetInitObjectStartIndex(channel)) ) /* PRQA S 4391 */ /* MD_Can_IntegerCast */
  { /* \trace SPEC-50625, SPEC-50563 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_BAUDRATE;
  }
  else
# endif
  { /* #50 set baud rate and reinitialize controller to activate baud rate */
    uint8 transitionState;
    /* ----- Implementation ----------------------------------------------- */
    Can_30_Flexcan4_SetLastInitObject(channel, (uint8)(Can_30_Flexcan4_GetInitObjectStartIndex(channel) + BaudRateConfigID)); /* SBSW_CAN_HL11 */
    transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4SuppressRamCheck);
    /* #60 check if hardware cancellation appear while mode change, so for the baud rate set is not successful \trace SPEC-60434 */
    if (transitionState == kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */
    {
      retval = E_OK;
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_SET_BR_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_ON */

/****************************************************************************
| NAME:             Can_30_Flexcan4_Init
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_Init
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_Init( Can_30_Flexcan4_ConfigPtrType ConfigPtr ) /* PRQA S 3206 */ /* MD_MSR_DummyStmt */
{ /* \trace SPEC-1587, SPEC-1708 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  CanHookBegin_Can_30_Flexcan4_Init();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
#if defined(CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER) /* PostBuild (load+sel) */
  Can_30_Flexcan4_ConfigDataPtr = ConfigPtr; /* \trace SPEC-1394, SPEC-1575 */
# if defined(CAN_30_FLEXCAN4_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* PostBuildLoadable */
  if (ConfigPtr == NULL_PTR)
  { /* #10 EcuM null pointer configuration check and notification */
    EcuM_BswErrorHook((uint16) CAN_30_FLEXCAN4_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
  }
  else
#  if (CAN_30_FLEXCAN4_FINALMAGICNUMBER == STD_ON) /* COV_CAN_FINALMAGICNUMBER */
  if (Can_30_Flexcan4_GetFinalMagicNumber() != 20510u) /* COV_CAN_GENDATA_FAILURE */
  { /* #20 EcuM wrong magic number check and notification */
    EcuM_BswErrorHook((uint16) CAN_30_FLEXCAN4_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
#  endif
# else
  
  /* ----- Development Error Checks ------------------------------------- */
#  if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #30 Check if all parameters are valid \trace SPEC-1724 */
  if (ConfigPtr == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
#  endif
# endif
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(ConfigPtr);  /* PRQA S 1338, 2982, 2983, 3112 */ /* MD_MSR_DummyStmt */
# if defined(V_ENABLE_USE_DUMMY_STATEMENT) /* COV_CAN_COMPATIBILITY */
  ConfigPtr = Can_30_Flexcan4_ConfigDataPtr;  /* PRQA S 2983,1338 */ /* MD_MSR_DummyStmt */
# endif
#endif /* CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag != CAN_30_FLEXCAN4_STATUS_UNINIT)
  {
    /* #60 Check already initialized -> 2nd call is not allowed \trace SPEC-1712, SPEC-1722 */
    errorId = CAN_30_FLEXCAN4_E_TRANSITION;
  }
  else
#endif
  if (  ( Can_30_Flexcan4_GetBaseDll_GeneratorVersion()     != (uint16)CAN_30_FLEXCAN4_GEN_BASE_CFG5_VERSION ) ||  /* PRQA S 2742 */ /* MD_Can_ConstValue */
        ( Can_30_Flexcan4_GetPlatformDll_GeneratorVersion() != (uint16)CAN_30_FLEXCAN4_GEN_Arm32Flexcan4Asr_VERSION ) ) /* COV_CAN_GENDATA_FAILURE */
  {
    /* #70 EcuM generator and compatibility version check and notification */
#if defined(CAN_30_FLEXCAN4_ENABLE_USE_ECUM_BSW_ERROR_HOOK) /* not for PostBuildSelectable */
    EcuM_BswErrorHook((uint16) CAN_30_FLEXCAN4_MODULE_ID, (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#else
    errorId = CAN_30_FLEXCAN4_E_UNINIT; /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#endif
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    {
      can_30_Flexcan4IsRxTaskLocked = kCan_30_Flexcan4False;
#if defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE)
      /* #110 delete RxQueue (all channels) */
      Can_30_Flexcan4_GetRxQueueInfo().WriteIndex = 0u;  /* CM_CAN_HL24 */ /* SBSW_CAN_HL55 */
      Can_30_Flexcan4_GetRxQueueInfo().ReadIndex = 0u; /* SBSW_CAN_HL55 */
      Can_30_Flexcan4_GetRxQueueInfo().Count = 0u; /* SBSW_CAN_HL55 */
#endif
      /* #120 Set active Identity */
      /* #130 HW specific power on (LL) */
      if (CanLL_30_Flexcan4_InitPowerOn() == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
      {
        can_30_Flexcan4ConfigInitFlag = CAN_30_FLEXCAN4_STATUS_INIT; /* \trace SPEC-1648 */ /* before Controller initialization begin */
#if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
        for (channel = 0u; channel < kCan_30_Flexcan4NumberOfChannels; channel++) /* CM_CAN_HL01 */
#endif
        { /* #132 for each controller */
#if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
          if ( Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed ) /* COV_CAN_GENDATA_FAILURE */
          { /* #134 not for inactive controller */
            continue;
          }
#endif
          /* #140 initialize start up values for each controllers */
          Can_30_Flexcan4_SetLogStatus(channel, CAN_30_FLEXCAN4_STATUS_UNINIT); /* PowerOn mark */ /* SBSW_CAN_HL02 */
          Can_30_Flexcan4_SetModeTransitionRequest(channel, kCan_30_Flexcan4ModeNone); /* SBSW_CAN_HL01 */
          Can_30_Flexcan4_SetBusOffTransitionRequest(channel, kCan_30_Flexcan4FinishBusOffRecovery); /* SBSW_CAN_HL01 */
          Can_30_Flexcan4_SetRamCheckTransitionRequest(channel, kCan_30_Flexcan4SuppressRamCheck); /* SBSW_CAN_HL01 */
          Can_30_Flexcan4_SetBusOffCounter(channel, 0u); /* SBSW_CAN_HL33 */
          Can_30_Flexcan4_SetIsBusOff(channel, FALSE);  /* start up: no BUSOFF */ /* SBSW_CAN_HL08 */
          Can_30_Flexcan4_SetCanInterruptCounter(channel, 0u); /* SBSW_CAN_HL12 */
#if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
          Can_30_Flexcan4_SetMirrorModeState(channel, CDDMIRROR_INACTIVE); /* SBSW_CAN_HL20 */
#endif
#if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
          Can_30_Flexcan4_SetSilentModeState(channel, CAN_SILENT_INACTIVE); /* SBSW_CAN_HL20 */
#endif
          /* #155 HW channel specific power on */
          if (CanLL_30_Flexcan4_InitPowerOnChannelSpecific(CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4Ok) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_HARDWARE_FAILURE */
          {
            uint8 transitionState;
            /* #160 set baud rate and initialize all CAN controllers \trace SPEC-1587, SPEC-1656 */
            Can_30_Flexcan4_SetLastInitObject(channel, (uint8)(Can_30_Flexcan4_GetInitObjectStartIndex(channel) + Can_30_Flexcan4_GetCanControllerDefaultBaudrateIdx(CAN_30_FLEXCAN4_HL_HW_CHANNEL_STARTINDEX(channel)))); /* SBSW_CAN_HL11 */
            transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4ExecuteRamCheck);
            if (transitionState == kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */
            { /* #170 check transition STOP (Reinit) is successful */
              Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_STATUS_STOP); /* STOP mode reached or issue */ /* SBSW_CAN_HL02 */
            }
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
            else
            {              
              errorId = CAN_30_FLEXCAN4_E_TRANSITION;
            }
#endif
          }
        }
      } /* CanLL_30_Flexcan4_InitPowerOn */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_INIT_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_30_Flexcan4_Init();
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_SetControllerMode
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_SetControllerMode
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_SetControllerMode( uint8 Controller, Can_StateTransitionType Transition )
{ /* \trace SPEC-1715 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Can_ReturnType retval;
  uint8 transitionRequest;
  uint8 busOffRecovery;
  uint8 doRamCheck;
  uint8 transitionState;
  CanHookBegin_Can_30_Flexcan4_SetControllerMode();
  retval = CAN_NOT_OK;  /* \trace SPEC-1407 */ /* For successful transition it is set explicit to CAN_OK */
  transitionState = kCan_30_Flexcan4Failed;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller and Transition is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* \trace SPEC-1731 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1732 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  { 
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER; /* \trace SPEC3874 */
  }
  else
# endif
#endif
  { /* #50 second level check (modes and transitions) only valid when controller is in active ECU and valid */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
    if (!CanHL_30_Flexcan4_IsControllerInit(channel))
    { /* #60 Check controller is initialized */
      errorId = CAN_30_FLEXCAN4_E_UNINIT;
    }
    else
    if ((!CanHL_30_Flexcan4_IsStop(channel)) && (Transition == CAN_T_START))
    { /* #70 Check No-STOP -> START-Req is invalid \trace SPEC-1637 */ /* ESCAN00073272 */
      errorId = CAN_30_FLEXCAN4_E_TRANSITION;
    }
    else
# if (CAN_30_FLEXCAN4_TRANSITION_CHECK == STD_ON)  /* SREQ00000291 */
    if ( ((!CanHL_30_Flexcan4_IsStop(channel)) && (!CanHL_30_Flexcan4_IsSleep(channel)) && ((Transition == CAN_T_WAKEUP) || (Transition == CAN_T_SLEEP))) ||
         ((CanHL_30_Flexcan4_IsSleep(channel)) && (Transition == CAN_T_STOP)) )
    { /* #80 Check  No STOP Nor SLEEP -> SLEEP-Req   or   No SLEEP Nor STOP --> WAKEUP-Req   is invalid \trace SPEC-1654, SPEC-1649 */
      /* #90 Check  No START Nor STOP -> STOP-Req is invalid \trace SPEC-1652 */
      errorId = CAN_30_FLEXCAN4_E_TRANSITION;
    }
    else
# endif
#endif
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
    /* #95 controller is active (no RAM check issue): */
    if ((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) != CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */ /* COV_CAN_RAM_CHECK_NO_STIMULATION */
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
      SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
      /* #125 HW begin mode transition */
      switch(Transition)
      {
      case CAN_T_START:
        /* #200 Handle --- START --- transition */
        if (Can_30_Flexcan4_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
        { /* #210 BUSOFF occur so finish BUSOFF handling and do START transition inside */
          transitionRequest = kCan_30_Flexcan4ModeResetBusOffEnd;
        } 
        else
        {
#if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
          if (Can_30_Flexcan4_GetSilentModeState(channel) == CAN_SILENT_ACTIVE)
          {
            transitionRequest = kCan_30_Flexcan4ModeSilent;
          }
          else
#endif
          {
#if (CAN_30_FLEXCAN4_REINIT_START == STD_ON)
            transitionRequest = kCan_30_Flexcan4ModeStartReinit;
#else
            transitionRequest = kCan_30_Flexcan4ModeStart;
#endif
          }
        }
        busOffRecovery = kCan_30_Flexcan4ContinueBusOffRecovery;
        doRamCheck = kCan_30_Flexcan4SuppressRamCheck;
        transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        if ( transitionState == kCan_30_Flexcan4Ok ) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */
        {
          Can_30_Flexcan4_SetIsBusOff(channel, FALSE); /* SBSW_CAN_HL08 */
        }
        CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_START, CANIF_CS_STARTED, transitionState);
        break;

      case CAN_T_STOP:
        /* #300 Handle --- STOP --- transition */
        transitionRequest = kCan_30_Flexcan4ModeStopReinitFast;
        if (Can_30_Flexcan4_GetBusOffCounter(channel) == 0u)  /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
        {
          busOffRecovery = kCan_30_Flexcan4FinishBusOffRecovery;
        }
        else
        {
          Can_30_Flexcan4_DecBusOffCounter(channel); /* SBSW_CAN_HL33 */
          busOffRecovery = kCan_30_Flexcan4ContinueBusOffRecovery;
        }
        doRamCheck = kCan_30_Flexcan4SuppressRamCheck;
        transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
        break;

      case CAN_T_WAKEUP:
        /* #400 Handle --- WAKEUP --- transition */
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK) && !defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
        if (!CanHL_30_Flexcan4_IsStop(channel)) { /* do not call RAM_CHECK again in case CanIf call wakeup a second time (buswakeup + stop transition afterwards) */
          /* #415 do standard RAM check CanHL_30_Flexcan4_DoRamCheck() if controller is not in STOP mode */
          transitionRequest = kCan_30_Flexcan4ModeStopReinit;
          doRamCheck = kCan_30_Flexcan4ExecuteRamCheck;
        }
        else
#endif
        {
          transitionRequest = kCan_30_Flexcan4ModeStopReinitFast;
          doRamCheck = kCan_30_Flexcan4SuppressRamCheck;
        }
        {
          busOffRecovery = kCan_30_Flexcan4FinishBusOffRecovery;
        }
        transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
        CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
        break;

      case CAN_T_SLEEP: /* \trace SPEC-1639 */
        /* #500 Handle --- SLEEP --- transition */
        { /* #520 no WAKEUP-source-ref: do emulated SLEEP mode \trace SPEC-1629, SPEC-1641, SPEC-1645 */
          transitionState = kCan_30_Flexcan4Ok;
        }
        CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_SLEEP, CANIF_CS_SLEEP, transitionState);
        break;

      default:
        /* #600 check unknown transition, return NOT_OK \trace SPEC-1573, SPEC-1402, SPEC-1403 */
        errorId = CAN_30_FLEXCAN4_E_TRANSITION; /* \trace SPEC-1733, SPEC-1407 */
        break;
      } /* switch */
      SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
    }
  }
  if ( transitionState != kCan_30_Flexcan4Failed ) /* COV_CAN_TRANSITION_REQUEST */ /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */
  {
    retval = CAN_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_SETCTR_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_30_Flexcan4_SetControllerMode();
  return retval;
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_DisableControllerInterrupts
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_DisableControllerInterrupts
Relation_Context:
# from Can_30_Flexcan4_MainFunction_BusOff #
StatusPolling, ChannelAmount
# from CanHL_30_Flexcan4_TxTaskCancelationHandling #
HwCancelTx, IndividualPolling
# from CanHL_30_Flexcan4_TxConfirmationPolling #
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_DisableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1746 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* \trace SPEC-1750 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1742 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1750 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_1();
    if (Can_30_Flexcan4_GetCanInterruptCounter(channel) == 0u)
    { /* #110 Disable only when not disabled before (count nesting) \trace SPEC-1735, SPEC-1745 */
#if (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_DRIVER) || (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      { /* #120 Disable CAN interrupt for each HW channel */
        CanLL_30_Flexcan4_CanInterruptDisable(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST  &Can_30_Flexcan4_GetCanInterruptOldStatus(canHwChannel)); /* SBSW_CAN_HL16 */
      }
#endif
#if (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_APPL) || (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
      /* #130 Let application Disable CAN interrupt ApplCanInterruptDisable() */
      Appl_30_Flexcan4CanInterruptDisable((uint8)channel);
#endif
    }
    Can_30_Flexcan4_IncCanInterruptCounter(channel); /* \trace SPEC-1748 */ /* SBSW_CAN_HL12 */
    SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_1();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_DIINT_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_EnableControllerInterrupts
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_EnableControllerInterrupts
Relation_Context:
# from Can_30_Flexcan4_MainFunction_BusOff #
StatusPolling, ChannelAmount
# from CanHL_30_Flexcan4_TxTaskCancelationHandling #
HwCancelTx, IndividualPolling
# from CanHL_30_Flexcan4_TxConfirmationPolling #
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_EnableControllerInterrupts( uint8 Controller )
{ /* \trace SPEC-1741 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* \trace SPEC-1739 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* \trace SPEC-1752 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
# endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* #40 Check controller is initialized \trace SPEC-1739 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#endif
  { /* ----- Implementation ----------------------------------------------- */
    /* #100 EXCLUSIVE_AREA_1 secure interrupt lock handling */
    SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_1();
    if (Can_30_Flexcan4_GetCanInterruptCounter(channel) != 0u)
    { /* #110 Enable only when disabled before (count nesting) \trace SPEC-1749, SPEC-1748 */
      Can_30_Flexcan4_DecCanInterruptCounter(channel); /* SBSW_CAN_HL12 */
      if (Can_30_Flexcan4_GetCanInterruptCounter(channel) == 0u) /* \trace SPEC-1736, SPEC-1756 */
      {
#if (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_DRIVER) || (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        { /* #120 Disable CAN interrupt for each HW channel */
          CanLL_30_Flexcan4_CanInterruptRestore(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST Can_30_Flexcan4_GetCanInterruptOldStatus(canHwChannel));
        }
#endif
#if (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_APPL) || (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH) /* COV_CAN_LOCK_ISR_BY_APPL */
        /* #130 Let application Enable CAN interrupt ApplCanInterruptRestore() */
        Appl_30_Flexcan4CanInterruptRestore((uint8)channel);
#endif
      }
    }
    SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_1();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_ENINT_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
#if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_Write
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_Write
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_Write( Can_HwHandleType Hth, Can_30_Flexcan4_PduInfoPtrType PduInfo )
{ /* \trace SPEC-1751, SPEC-1760, SPEC-1406 */
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval;
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  CanHookBegin_Can_30_Flexcan4_Write();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  retval = CAN_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* \trace SPEC-1759 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (Hth >= Can_30_Flexcan4_GetSizeOfMailbox()) {
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
#endif
  {
#if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_30_Flexcan4_GetMailboxController(Hth);
#endif
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
    }
    else
# endif
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_FAILURE */
    {
      errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
    }
    else
# endif
    if (!CanHL_30_Flexcan4_IsControllerInit(channel))
    { /* #40 Check controller is initialized \trace SPEC-1407 */
      errorId = CAN_30_FLEXCAN4_E_UNINIT;
    }
    else
# if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
    if ((Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_MUX_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_FIFO_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_FULLCAN_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
# else
    if ((Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_MUX_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_FIFO_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
# endif
    { /* \trace SPEC-1763 */
      errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
    }
    else
    if ((PduInfo == NULL_PTR) || ((PduInfo->sdu == NULL_PTR) && (PduInfo->length != 0u)))
    { /* \trace SPEC-1761 */
      errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
    }
    else
# if !defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
    if ( CanHL_30_Flexcan4_IsFdMessage(PduInfo->id) )
    { /* no CAN_FD: Check parameter PduInfo->id should not have FD bit in ID */
      errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
    }
    else
# endif
# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_FULL)
    if ( ((PduInfo->length > 8u) && (Can_30_Flexcan4_GetInitObjectFdBrsConfig(Can_30_Flexcan4_GetLastInitObject(channel)) == CAN_30_FLEXCAN4_NONE)) ||
         (PduInfo->length > CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(Hth)) ) /* COV_CAN_GENDATA_FAILURE */
         { /* \trace SPEC-1758 */
      /* \trace SPEC-60433 */ /* CAN_FD: Check parameter PduInfo->length is smaller than 9 for none FD configuration
         If development error detection for the CAN module is enabled and the CAN Controller is not in CAN FD mode (no CanControllerFdBaudrateConfig):
         The function Can_30_Flexcan4_Write shall raise the error CAN_E_PARAM_DLC and shall return CAN_NOT_OK if the length is more than 8 byte.() */
      /* CAN_FD: Check parameter PduInfo->length against PDU maximum size */
      errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
    }
    else
# else
    if ( PduInfo->length > CAN_30_FLEXCAN4_MAX_DATALEN_OBJ(Hth) ) /* CM_CAN_HL20 */
    { /* \trace SPEC-1758 */ /* Check parameter PduInfo->length against maximum buffer size */
      errorId = CAN_30_FLEXCAN4_E_PARAM_DLC;
    }
    else
# endif
# if defined(C_30_FLEXCAN4_ENABLE_MIXED_ID)
    if ( ((PduInfo->id & (Can_IdType)CAN_30_FLEXCAN4_ID_IDE_MASK) != (Can_IdType)CAN_30_FLEXCAN4_ID_IDE_MASK ) && ( (PduInfo->id & (Can_IdType)CAN_30_FLEXCAN4_ID_MASK) > (Can_IdType)CAN_30_FLEXCAN4_ID_MASK_STD ) ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
    { /* Check parameter PduInfo->id for STD ID is in STD range (EXT is always in range) */
      errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
    }
    else
# endif
    if ( (PduInfo->id & (Can_IdType)CAN_30_FLEXCAN4_ID_UNUSED_MASK) != (Can_IdType)0UL )
    { /* Check parameter PduInfo->id contains illegal bits (e.g. bigger ID value than MAX for given type) */
      errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
    }
    else
#endif
    /* ----- Implementation ----------------------------------------------- */
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #150 reject deactivated mailbox (by RamCheck) */
    if (Can_30_Flexcan4_GetMailboxState(Hth) == kCan_30_Flexcan4Failed) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      retval = CAN_NOT_OK;
    }
    else
#endif
    /* #180 check Hardware is in BUSOFF recovery state and is not able to handle a request: return BUSY \trace SPEC-1764 */
    if (Can_30_Flexcan4_IsIsBusOff(channel)) /* COV_CAN_BUSOFF_NOT_IN_ALL_CONFIG */
    {
      retval = CAN_NOT_OK; /* ESCAN00096369 */
    }
    else
#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
    /* #200 controller is active (no RAM check issue): */
    if ((Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) != CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */ /* COV_CAN_HARDWARE_FAILURE */
#endif
    {
      /* #205 temporary pdu buffer to avoid data modification on constant data (modification in generic pretransmit) */
      tCan_30_Flexcan4TxTransmissionParaStruct txPara;
      /* #210 EXCLUSIVE_AREA_2: Interrupts may not be locked and re-entrant call may occur. */
      /*                        Time in between ID search and lock of mailbox is critical 
                                (only when no Vector Interface used otherwise CanIf EXCLUSIVE_AREA is used instead) 
                                Can_30_Flexcan4_Write() could be called re-entrant, there for also for polling systems the interrupts have to be locked  
                                this is essential while check for mailbox is free (in between if (free) and set to not free) and save PDU handle */
#if !defined(CAN_USE_VECTOR_IF)
      SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_2();
#endif
      txPara.mailboxHandle = Hth;
      txPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(Hth); 
      txPara.pdu.id = PduInfo->id;
      txPara.pdu.length = PduInfo->length;
      txPara.pdu.sdu = PduInfo->sdu;
      txPara.pdu.swPduHandle = PduInfo->swPduHandle;
#if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_USED)
      if (txPara.pdu.length <= 8u) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      {
        if (Can_30_Flexcan4_GetInitObjectFdBrsConfig(Can_30_Flexcan4_GetLastInitObject(channel)) == CAN_30_FLEXCAN4_NONE)
        { /* #220 remove FD flag when no FD support is activated in this initialization structure and DLC not greater than 8 */
          /* CAN FD also support an extended payload which allows the transmission of up to 64 bytes.
          This feature also depends on the CAN FD configuration (see CanControllerFdBaudrateConfig).
          Therefore, if the CAN FD feature is enabled and the CAN FD flag is set in CanId passed to
           Can_30_Flexcan4_Write(), CanDrv supports the transmission of PDUs with a length up to 64 bytes.
          If there is a request to transmit a CAN FD frame without the CAN FD feature enabled the
          frame is sent as conventional CAN frame as long as the PDU length <= 8 bytes. */
          txPara.pdu.id = txPara.pdu.id & ((Can_IdType)(CAN_30_FLEXCAN4_ID_IDE_MASK | CAN_30_FLEXCAN4_ID_MASK)); /* CAN_FD_   mask out the FD bit */
        }
      }
      txPara.fdBrsType = kCan_30_Flexcan4FdBrsTypeFalse;
      if (CanHL_30_Flexcan4_IsFdMessage(txPara.pdu.id))
      {
        txPara.fdType = kCan_30_Flexcan4FdTypeFd;
        if (CanHL_30_Flexcan4_IsFdTxBrs(channel))
        {
          txPara.fdBrsType = kCan_30_Flexcan4FdBrsTypeTrue;
        }
      } else {
        txPara.fdType = kCan_30_Flexcan4FdTypeClassic;
      }
      txPara.messageLen = txPara.pdu.length;
      txPara.frameLen = CAN_30_FLEXCAN4_DLC2LEN(CAN_30_FLEXCAN4_LEN2DLC(txPara.messageLen));
# if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_FULL)
      txPara.paddingVal = Can_30_Flexcan4_GetMailboxFdPadding(txPara.mailboxHandle);
# endif
#endif
      /* #230 search for BasicCAN object handle to transmit (Multiplexed TX) and backup data for cancel TX object if necessary */
      CanHL_30_Flexcan4_WritePrepare(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &txPara); /* SBSW_CAN_HL49 */
      /* #240 start transmission for given handle (if free) and return state */
      retval = CanHL_30_Flexcan4_WriteStart(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST &txPara); /* SBSW_CAN_HL49 */

#if !defined(CAN_USE_VECTOR_IF) /* No Vector Interface used, Interrupts may not be locked */
      /* avoid change of PDU information due to TX interrupt while changing */
      SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_2();
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_WRITE_ID, errorId);
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_30_Flexcan4_Write();
  return retval;
} /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             ASR4.x - Can_30_Flexcan4_CheckWakeup
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_CheckWakeup
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
#if defined(C_30_FLEXCAN4_ENABLE_CHECK_WAKEUP_CAN_RET_TYPE) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_CHECKWAKEUP_RETTYPE Can_ReturnType
# define CAN_CHECKWAKEUP_RETVAL_OK     CAN_OK
# define CAN_CHECKWAKEUP_RETVAL_NOT_OK CAN_NOT_OK
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_CheckWakeup( uint8 Controller )
#else
# define CAN_30_FLEXCAN4_CHECKWAKEUP_RETTYPE Std_ReturnType
# define CAN_CHECKWAKEUP_RETVAL_OK     E_OK
# define CAN_CHECKWAKEUP_RETVAL_NOT_OK E_NOT_OK
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_CheckWakeup( uint8 Controller )
#endif
{ /* \trace SPEC-1740 */
  /* ----- Local Variables ---------------------------------------------- */
  CAN_30_FLEXCAN4_CHECKWAKEUP_RETTYPE retval;
  retval = CAN_CHECKWAKEUP_RETVAL_NOT_OK; /* \trace SPEC-1407 */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_MainFunction_Write
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_MainFunction_Write
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_MainFunction_Write( void )
{ /* \trace SPEC-1766 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) 
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  CanHookBegin_Can_30_Flexcan4_MainFunction_Write();
  /* canSendSemaphor no more needed because of ControllerInterrupts disabled while Can_30_Flexcan4_Write() */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1767 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCan_30_Flexcan4NumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON) /* \trace SPEC-1573, SPEC-1402, SPEC-1403 */
      if (!CanHL_30_Flexcan4_IsControllerInit(channel))
      { /* #50 Check controller is initialized */
        errorId = CAN_30_FLEXCAN4_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_30_Flexcan4_IsSleep(channel) )
        { /* #60 do not access CAN hardware in SLEEP */
          tCan_30_Flexcan4TaskParaStruct taskPara;
# if defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */ /* always true when no hw-cancel is supported */
          /* #100 do confirmation handling for pending mailboxes */
          if ( CanLL_30_Flexcan4_TxIsGlobalConfPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True ) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
          { /* #110 is at least one mailbox pending */
            for ( taskPara.mailboxHandle = Can_30_Flexcan4_GetTxBasicHandleStart(canHwChannel); taskPara.mailboxHandle < Can_30_Flexcan4_GetTxBasicHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
            { /* #120 iterate over all TxBasicCAN */
              taskPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#  if defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
              if ( Can_30_Flexcan4_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#  endif
              { /* #130 is mailbox handled by polling (individual) */
                /* #140 call LL confirmation handling */
                CanLL_30_Flexcan4_TxProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
              }
            }
          }
# endif
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW) 
# endif
# if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
#  if defined(C_30_FLEXCAN4_ENABLE_TX_POLLING) /* COV_CAN_TX_POLLING TX */ /* always true when no hw-cancel is supported */
          /* #200 do confirmation handling for pending mailboxes */
          if ( CanLL_30_Flexcan4_TxIsGlobalConfPending(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY) == kCan_30_Flexcan4True ) /* PRQA S 2991,2995 */ /* MD_Can_ConstValue */ /* COV_CAN_LL_HARDWARE_BEHAVIOUR */
          { /* #210 is at least one mailbox pending */
            for ( taskPara.mailboxHandle = Can_30_Flexcan4_GetTxFullHandleStart(canHwChannel); taskPara.mailboxHandle < Can_30_Flexcan4_GetTxFullHandleStop(canHwChannel); taskPara.mailboxHandle++ ) /* PRQA S 2994,2996 */ /* MD_Can_ConstValue */
            { /* #220 iterate over all TxFullCAN */
              taskPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(taskPara.mailboxHandle); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
#   if defined(C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING) /* COV_CAN_HWOBJINDIVPOLLING */
              if ( Can_30_Flexcan4_IsMailboxIndivPolling(taskPara.mailboxHandle)) /* COV_CAN_HWOBJINDIVPOLLING */
#   endif
              { /* #230 is mailbox handled by polling (individual) */
                /* #240 call LL confirmation handling */
                CanLL_30_Flexcan4_TxProcessPendings(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &taskPara); /* SBSW_CAN_HL44 */
              }
            }
          }
#  endif
#  if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW) 
#  endif
# endif
        } /* !IsSleep */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_MAINFCT_WRITE_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

#else
  CanHookBegin_Can_30_Flexcan4_MainFunction_Write();
#endif /* C_30_FLEXCAN4_ENABLE_TX_POLLING */
  CanHookEnd_Can_30_Flexcan4_MainFunction_Write();
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_MainFunction_Read
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_MainFunction_Read
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_MainFunction_Read( void )
{ /* \trace SPEC-1776 */
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  CanHookBegin_Can_30_Flexcan4_MainFunction_Read();
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check Driver is initialized \trace SPEC-1784 */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  {
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#endif
  /* ----- Implementation ----------------------------------------------- */
  /* #20 semaphore to secure parallel access to RX buffers interrupt or polling \trace SPEC-1569 */
  if ( can_30_Flexcan4IsRxTaskLocked == kCan_30_Flexcan4False )  /* PRQA S 2004 */ /* MD_Can_NoElseAfterIf */
  {
#if defined(C_30_FLEXCAN4_ENABLE_RX_POLLING) /* COV_CAN_RX_POLLING */
    CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
#endif
    can_30_Flexcan4IsRxTaskLocked = kCan_30_Flexcan4True; /* PRQA S 2982 */ /* MD_MSR_RetVal */
#if defined(C_30_FLEXCAN4_ENABLE_RX_POLLING) /* \trace SPEC-1782 */ /* COV_CAN_RX_POLLING */
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCan_30_Flexcan4NumberOfChannels; channel++)
# endif
    { /* #30 iterate over all active controller */
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_30_Flexcan4_IsControllerInit(channel))
      { /* #60 Check controller is initialized */
        errorId = CAN_30_FLEXCAN4_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_30_Flexcan4_IsSleep(channel) )
        { /* #70 do not access CAN hardware in SLEEP */
          /* #80 over all HW channel \trace SPEC-1774 */
# if defined(C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS)
          /* #90 search for all pending FullCAN mailbox objects and call notification */
          CanHL_30_Flexcan4_RxFullCanPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif
# if defined(C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS) /* COV_CAN_MULTIPLE_BASICCAN */
          /* #100 search for all pending BasicCAN mailbox objects and call notification */
          CanHL_30_Flexcan4_RxBasicCanPolling(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
# endif
        }
      }
    }
#endif /* C_30_FLEXCAN4_ENABLE_RX_POLLING */
#if defined(C_30_FLEXCAN4_ENABLE_RX_QUEUE)
    /* #200 execute RxQueue CanHL_30_Flexcan4_RxQueueExecution() */
    CanHL_30_Flexcan4_RxQueueExecution(); 
#endif
    can_30_Flexcan4IsRxTaskLocked = kCan_30_Flexcan4False;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_Can_ConstValue */
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_MAINFCT_READ_ID, errorId); /* PRQA S 2880 */ /*  MD_MSR_Unreachable */
  }
#else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CanHookEnd_Can_30_Flexcan4_MainFunction_Read();
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_MainFunction_BusOff
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_MainFunction_BusOff
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_MainFunction_BusOff( void )
{ /* \trace SPEC-1783 */
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_30_FLEXCAN4_ENABLE_ERROR_POLLING) /* \trace SPEC-1778 */ /* COV_CAN_ERROR_POLLING */
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1780 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# endif
  {  
    /* ----- Implementation ----------------------------------------------- */
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCan_30_Flexcan4NumberOfChannels; channel++)
# endif
    { /* #20 over all active controller */
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_30_Flexcan4_IsControllerInit(channel))
      { /* #50 Check controller is initialized */
        errorId = CAN_30_FLEXCAN4_E_UNINIT;
      }
      else
# endif
      {
        if ( !CanHL_30_Flexcan4_IsSleep(channel) )
        { /* #60 do not access CAN hardware in SLEEP */
          /* #80 disable CAN interrupts */
          Can_30_Flexcan4_DisableControllerInterrupts((uint8)channel);
          /* #90 call CanHL_30_Flexcan4_ErrorHandling */
          CanHL_30_Flexcan4_ErrorHandling( CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY );
          Can_30_Flexcan4_EnableControllerInterrupts((uint8)channel);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_MAINFCT_BO_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
#endif /* C_30_FLEXCAN4_ENABLE_ERROR_POLLING */
}


/****************************************************************************
| NAME:             Can_30_Flexcan4_MainFunction_Wakeup
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_MainFunction_Wakeup
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_MainFunction_Wakeup( void )
{ /* \trace SPEC-1770 */
  /* ----- Local Variables ---------------------------------------------- */
}

/****************************************************************************
| NAME:             Can_30_Flexcan4_MainFunction_Mode
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_MainFunction_Mode
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_MainFunction_Mode( void )
{ /* \trace SPEC-1775 */
  /* ----- Local Variables ---------------------------------------------- */
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized \trace SPEC-1779 */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# endif
  { /* #15 EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    for (channel = 0u; channel < kCan_30_Flexcan4NumberOfChannels; channel++) /* CM_CAN_HL01 */
# endif
    { /* #20 over all active controller */
# if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON) /* one controller optimization is always off */
      if ( Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
      { /* not for inactive controller */
        continue;
      }
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
      if (!CanHL_30_Flexcan4_IsControllerInit(channel))
      { /* #50 Check controller is initialized */ /* \trace SPEC-1779 */
        errorId = CAN_30_FLEXCAN4_E_UNINIT;
      }
      else
# endif
      {
        uint8 transitionRequest;
        uint8 busOffRecovery;
        uint8 doRamCheck;
        uint8 transitionState;
        transitionRequest = Can_30_Flexcan4_GetModeTransitionRequest(channel);
        busOffRecovery = Can_30_Flexcan4_GetBusOffTransitionRequest(channel);
        doRamCheck = Can_30_Flexcan4_GetRamCheckTransitionRequest(channel);
        /* #55 only one transition request at one time is possible - execute it \trace SPEC-1771 */
        if(transitionRequest != kCan_30_Flexcan4ModeNone)
        {
          transitionState = CanLL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST transitionRequest, busOffRecovery, doRamCheck);
          switch(transitionRequest)
          {
            case kCan_30_Flexcan4ModeStopReinitFast: /* COV_CAN_TRANSITION_REQUEST */
            case kCan_30_Flexcan4ModeStopReinit: /* COV_CAN_TRANSITION_REQUEST */
              CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
              break;
# if (CAN_30_FLEXCAN4_REINIT_START == STD_ON)
            case kCan_30_Flexcan4ModeStartReinit: /* COV_CAN_TRANSITION_REQUEST */
# else
            case kCan_30_Flexcan4ModeStart: /* COV_CAN_TRANSITION_REQUEST */
# endif
            case kCan_30_Flexcan4ModeResetBusOffEnd: /* COV_CAN_TRANSITION_REQUEST */
# if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
            case kCan_30_Flexcan4ModeSilent: /* COV_CAN_TRANSITION_REQUEST */
# endif
              CanHL_30_Flexcan4_NotifyTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST CAN_30_FLEXCAN4_STATUS_START, CANIF_CS_STARTED, transitionState);
              if ( transitionState == kCan_30_Flexcan4Ok ) /* PRQA S 2991,2992,2995,2996 */ /* MD_Can_ConstValue */ /* COV_CAN_TRANSITION_REQUEST */
              {
                Can_30_Flexcan4_SetIsBusOff(channel, FALSE); /* SBSW_CAN_HL08 */
              }
              break;
            case kCan_30_Flexcan4ModeResetBusOffStart: /* COV_CAN_TRANSITION_REQUEST */ /* not expected, no repetition request */
            default: /* COV_CAN_TRANSITION_REQUEST */
              break;
          }
        }
      }
    } /* over all channels */
    SchM_Exit_Can_30_Flexcan4_CAN_30_FLEXCAN4_EXCLUSIVE_AREA_6();
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_MAINFCT_MODE_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */


/****************************************************************************
| NAME:             Can_30_Flexcan4_CancelTx
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_CancelTx
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_CancelTx( Can_HwHandleType Hth, PduIdType PduId )
{
  /* ----- Local Variables ---------------------------------------------- */
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API)
  CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
  tCan_30_Flexcan4TxCancellationParaStruct txCancellationPara;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (Hth >= Can_30_Flexcan4_GetSizeOfMailbox())
  { /* #20 Check Hth is in range */
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
#  if defined(C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
  if ((Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_MUX_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_FIFO_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_FULLCAN_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
#  else
  if ((Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_MUX_TYPE) && (Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_BASICCAN_FIFO_TYPE)) /* COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG */
#  endif
  { /* #30 Check Hth is FullCAN or BasicCAN */
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
# endif
  {
# if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL ) /* otherwise 'channel' is a define */
    channel = Can_30_Flexcan4_GetMailboxController(Hth);
# endif
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */ /* COV_CAN_GENDATA_FAILURE */
    { /* #40 Check parameter controller is in range */
      errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
    }
    else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
    if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_FAILURE */
    { /* #60 Check parameter controller is valid */
      errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
    }
    else
#  endif
    if (!CanHL_30_Flexcan4_IsControllerInit(channel))
    { /* #70 Check controller is initialized */
      errorId = CAN_30_FLEXCAN4_E_UNINIT;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      txCancellationPara.mailboxHandle = Hth;
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW)
      txCancellationPara.hwObjHandle = Can_30_Flexcan4_GetMailboxHwHandle(Hth);
# endif
      txCancellationPara.mailboxElement = 0u;
      txCancellationPara.activeSendObject = CanHL_30_Flexcan4_GetActiveSendObject(txCancellationPara.mailboxHandle, txCancellationPara.mailboxElement);

# if defined (C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
      if ( Can_30_Flexcan4_GetMailboxType(Hth) != CAN_30_FLEXCAN4_TX_FULLCAN_TYPE ) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
# endif
      { /* #100 is BasicCAN */
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) 
        for (; txCancellationPara.mailboxElement < Can_30_Flexcan4_GetMailboxSize(Hth); txCancellationPara.mailboxElement++, txCancellationPara.activeSendObject++) /* PRQA S 2463,3387,3418 */ /* MD_Can_HL_AdditionalExpressionInForLoop,MD_Can_HL_UsingIncOrDec,MD_Can_IncompleteForLoop */
# endif
        { /* #110 over all Multiplexed TX objects - search for handle */
          if (Can_30_Flexcan4_GetActiveSendPdu(txCancellationPara.activeSendObject) == PduId)
          {
            if ((Can_30_Flexcan4_GetActiveSendState(txCancellationPara.activeSendObject) == kCan_30_Flexcan4BufferSend)
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW)
                || (Can_30_Flexcan4_GetActiveSendState(txCancellationPara.activeSendObject) == kCan_30_Flexcan4BufferCancel)
# endif
                )  /* COV_CAN_CANCEL_SW_STIMULATION */
            { /* #120 PDU found in active send list - handle found */
              /* #130 CANCEL_IN_HW is active - just suppress TX confirmation */
# if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW)
              { /* #140 LL TX cancel in HW */ /* \trace SPEC-1690 */
                /* #150 mark as to be cancelled by hardware */
                txCancellationPara.canHandleCurTxObj = kCan_30_Flexcan4BufferCancelSw;
                CanLL_30_Flexcan4_TxCancelInHw(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
                Can_30_Flexcan4_SetActiveSendState(txCancellationPara.activeSendObject, kCan_30_Flexcan4BufferCancelSw); /* SBSW_CAN_HL04 */
              }
# else
              Can_30_Flexcan4_SetActiveSendState(txCancellationPara.activeSendObject, kCan_30_Flexcan4BufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
# endif
# if defined(C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION) 
              break;
# endif
            }
          }
        }
      }
# if defined (C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS)
      else
      { /* #200 is FullCAN */
        if (Can_30_Flexcan4_GetActiveSendPdu(txCancellationPara.activeSendObject) == PduId)
        {
          if ((Can_30_Flexcan4_GetActiveSendState(txCancellationPara.activeSendObject) == kCan_30_Flexcan4BufferSend)
#  if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW)
              || (Can_30_Flexcan4_GetActiveSendState(txCancellationPara.activeSendObject) == kCan_30_Flexcan4BufferCancel)
#  endif
             )  /* COV_CAN_CANCEL_SW_STIMULATION */
          { /* #210 mailbox to cancel is valid (PDU, handle, send state OK) */
            /* #220 CANCEL_FULLCAN_IN_HW is active - just suppress TX confirmation */
#  if defined(C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW)
            { /* #230 LL TX cancel in HW */ /* \trace SPEC-1690 */
              /* #150 mark as to be cancelled by hardware */
              txCancellationPara.canHandleCurTxObj = kCan_30_Flexcan4BufferCancelSw;
              CanLL_30_Flexcan4_TxCancelInHw(CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST &txCancellationPara); /* SBSW_CAN_HL47 */
              Can_30_Flexcan4_SetActiveSendState(txCancellationPara.activeSendObject, kCan_30_Flexcan4BufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
            }
#  endif
            Can_30_Flexcan4_SetActiveSendState(txCancellationPara.activeSendObject, kCan_30_Flexcan4BufferCancelSw); /* mark as cancelled by SW */ /* SBSW_CAN_HL04 */
          }
        }
      }
# endif
    }
    CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT; /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_WRITE_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
#else /* CAN_ENABLE_CANCEL_SUPPORT_API */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Hth); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(PduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* CAN_ENABLE_CANCEL_SUPPORT_API */
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

#if (CAN_30_FLEXCAN4_GET_STATUS == STD_ON)
/****************************************************************************
| NAME:             Can_30_Flexcan4_GetStatus
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetStatus
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, uint8, CODE) Can_30_Flexcan4_GetStatus( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 canReturnCode;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  canReturnCode = CAN_30_FLEXCAN4_STATUS_INCONSISTENT; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #20 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* #50 Check controller is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CanLL_30_Flexcan4_GetStatusBegin(CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
    /* #100 set logical state as return value */
    canReturnCode = (uint8)Can_30_Flexcan4_GetLogStatus(channel);
    if ( CanLL_30_Flexcan4_HwIsBusOff(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #150 set BUSOFF - BUSOFF can only happen on first HW channel (with TX objects) */
      canReturnCode |= CAN_30_FLEXCAN4_STATUS_BUSOFF;
    }
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS) /* COV_CAN_EXTENDED_STATUS */
    {    
      if ( CanLL_30_Flexcan4_HwIsPassive(channel) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #160 set Error Passive */
        canReturnCode |= CAN_30_FLEXCAN4_STATUS_PASSIVE;
      }    
      if ( CanLL_30_Flexcan4_HwIsWarning(channel) ) /* COV_CAN_HARDWARE_FAILURE */
      { /* #170 set Error Warning */
        canReturnCode |= CAN_30_FLEXCAN4_STATUS_WARNING;
      }
    }
# endif /* C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return (uint8)canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_GetControllerMode
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetControllerMode
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerMode( uint8 Controller, Can_30_Flexcan4_ControllerStatePtrType ControllerModePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameters are valid */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #20 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (ControllerModePtr == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 get logical controller mode from state machine variable */
    if (!CanHL_30_Flexcan4_IsControllerInit(channel))
    {
      *ControllerModePtr = CAN_CS_UNINIT; /* SBSW_CAN_HL51 */
    }
    else if (CanHL_30_Flexcan4_IsStart(channel))
    {
      *ControllerModePtr = CAN_CS_STARTED; /* SBSW_CAN_HL51 */
    } 
    else if (CanHL_30_Flexcan4_IsSleep(channel))
    {
      *ControllerModePtr = CAN_CS_SLEEP; /* SBSW_CAN_HL51 */
    }
    else /* default is:   if (CanHL_30_Flexcan4_IsStop(channel)) */
    {
      *ControllerModePtr = CAN_CS_STOPPED; /* SBSW_CAN_HL51 */
    }
    canReturnCode = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_GETCTR_MODE_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_GetControllerErrorState
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetControllerErrorState
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerErrorState( uint8 Controller, Can_30_Flexcan4_ErrorStatePtrType ErrorStatePtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (ErrorStatePtr == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CanLL_30_Flexcan4_GetStatusBegin(CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY); /* PRQA S 2987 */ /* MD_Can_EmptyFunction */
    if ( CanLL_30_Flexcan4_HwIsBusOff(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #40 set BUSOFF */
      *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF; /* SBSW_CAN_HL52 */
    } 
# if defined(C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS) /* COV_CAN_EXTENDED_STATUS */
    else if ( CanLL_30_Flexcan4_HwIsPassive(channel) ) /* COV_CAN_HARDWARE_FAILURE */
    { /* #50 set Error Passive */
      *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /* SBSW_CAN_HL52 */
    }
# endif /* C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS */
    else
    { /* #60 default return is ACTIVE */
      *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE; /* SBSW_CAN_HL52 */
    }
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_GETCTR_STATE_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_GetControllerTxErrorCounter
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetControllerTxErrorCounter
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerTxErrorCounter( uint8 Controller, Can_30_Flexcan4_ErrorCounterPtrType TxErrorCounterPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (TxErrorCounterPtr == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 get error counter from hardware */
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    *TxErrorCounterPtr = Can_30_Flexcan4TxActualErrorCounter(); /* SBSW_CAN_HL53 */
# else
    *TxErrorCounterPtr = Can_30_Flexcan4TxActualErrorCounter(channel); /* SBSW_CAN_HL53 */
# endif
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_GETCTR_TXCNT_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/****************************************************************************
| NAME:             Can_30_Flexcan4_GetControllerRxErrorCounter
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_GetControllerRxErrorCounter
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerRxErrorCounter( uint8 Controller, Can_30_Flexcan4_ErrorCounterPtrType RxErrorCounterPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType canReturnCode;
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  canReturnCode = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter controller is valid and state is INIT */
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
#  if (CAN_30_FLEXCAN4_POSTBUILD_VARIANT_SUPPORT == STD_ON)
  if (Can_30_Flexcan4_GetCanIfChannelId(channel) == kCan_30_Flexcan4ChannelNotUsed) /* COV_CAN_GENDATA_NOT_IN_ALL_CONFIG */
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
  if (!CanHL_30_Flexcan4_IsControllerInit(channel))
  { /* Check controller is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (RxErrorCounterPtr == NULL_PTR)
  {
    errorId = CAN_30_FLEXCAN4_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 get error counter from hardware */
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
    *RxErrorCounterPtr = Can_30_Flexcan4RxActualErrorCounter(); /* SBSW_CAN_HL54 */
# else
    *RxErrorCounterPtr = Can_30_Flexcan4RxActualErrorCounter(channel); /* SBSW_CAN_HL54 */
# endif
    canReturnCode = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_GETCTR_RXCNT_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return canReturnCode;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#endif

#if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
/****************************************************************************
| NAME:             Can_30_Flexcan4_SetMirrorMode
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_SetMirrorMode
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_SetMirrorMode( uint8 Controller, CddMirror_MirrorModeType mirrorMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set mirror mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_30_Flexcan4_SetMirrorModeState(channel, mirrorMode); /* SBSW_CAN_HL20 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
/****************************************************************************
| NAME:             Can_30_Flexcan4_SetSilentMode
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_SetSilentMode
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_SetSilentMode( uint8 Controller, Can_30_Flexcan4_SilentModeType silentMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  Std_ReturnType retval;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  retval = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 set silent mode for given controller */
    /* ----- Implementation ----------------------------------------------- */
    Can_30_Flexcan4_SetSilentModeState(channel, silentMode); /* SBSW_CAN_HL20 */
    retval = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
/****************************************************************************
| NAME:             Can_30_Flexcan4_RamCheckExecute
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_RamCheckExecute
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckExecute( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  { /* #30 Do RamCheck by execute reinitialization with activated check */
    uint8 transitionState;
    /* ----- Implementation ----------------------------------------------- */
    transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4ExecuteRamCheck);
    if (transitionState == kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */
    { /* #40 check transition STOP (Reinit) is successful */
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/****************************************************************************
| NAME:             Can_30_Flexcan4_RamCheckEnableMailbox
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_RamCheckEnableMailbox
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckEnableMailbox( Can_HwHandleType htrh )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
  if (htrh >= Can_30_Flexcan4_GetSizeOfMailbox()) { /* CM_CAN_HL28 */
    errorId = CAN_30_FLEXCAN4_E_PARAM_HANDLE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #15 disable controller to force reinitialization afterwards in Can_30_Flexcan4_RamCheckEnableController() */
    Can_30_Flexcan4_SetLogStatus(Can_30_Flexcan4_GetMailboxController(htrh), CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER); /* SBSW_CAN_HL40 */
    /* #20 reactivate mailbox */
    Can_30_Flexcan4_SetMailboxState(htrh, kCan_30_Flexcan4Ok); /* SBSW_CAN_HL34 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/****************************************************************************
| NAME:             Can_30_Flexcan4_RamCheckEnableController
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4_RamCheckEnableController
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckEnableController( uint8 Controller )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  errorId = CAN_30_FLEXCAN4_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_DETECT == STD_ON)
  if (can_30_Flexcan4ConfigInitFlag == CAN_30_FLEXCAN4_STATUS_UNINIT)
  { /* #10 Check Driver is initialized */
    errorId = CAN_30_FLEXCAN4_E_UNINIT;
  }
  else
#  if !defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  if (channel >= kCan_30_Flexcan4NumberOfChannels) /* CM_CAN_HL01 */
  { /* #20 Check parameter controller is in range */
    errorId = CAN_30_FLEXCAN4_E_PARAM_CONTROLLER;
  }
  else
#  endif
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 activate controller only when currently disabled */
    if ( (Can_30_Flexcan4_GetLogStatus(channel) & CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER) == CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER ) /* COV_CAN_RAM_CHECK_NO_STIMULATION */
    {
      uint8 transitionState;
      /* #40 re-initialize to activate mailboxes and controller after RAM check issue */
      Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) & (uint8)(~(CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER))); /* SBSW_CAN_HL02 */
      transitionState = CanHL_30_Flexcan4_ModeTransition(CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST kCan_30_Flexcan4ModeStopReinit, kCan_30_Flexcan4FinishBusOffRecovery, kCan_30_Flexcan4SuppressRamCheck);
      if (transitionState == kCan_30_Flexcan4Ok) /* COV_CAN_TRANSITION_REQUEST */
      { /* #40 check transition STOP (Reinit) is successful */
        Can_30_Flexcan4_SetLogStatus(channel, Can_30_Flexcan4_GetLogStatus(channel) | CAN_30_FLEXCAN4_STATUS_STOP); /* STOP mode reached */ /* SBSW_CAN_HL02 */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CAN_30_FLEXCAN4_E_NO_ERROR)
  { /* Throw DET if an error detected before */
    Can_30_Flexcan4_CallDetReportError(CAN_30_FLEXCAN4_HW_ACCESS_ID, errorId);
  }
# else
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
# if defined( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif


#if defined( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_SINGLE */
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
     defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* **************************************************************************
| NAME:             Can_30_Flexcan4Interrupt<Name>
| CALLED BY:        Can_30_Flexcan4Isr<Name>_<physicalChannelIndex>()
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
/*
|<DataModelStart>| Can_30_Flexcan4Interrupt
Relation_Context:
# from Can_30_Flexcan4Isr_0 #
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4Isr_1 #
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4Isr_2 #
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4Isr_3 #
TxPolling,RxPolling,IndividualPolling,StatusPolling,WakeupPolling,UseNestedCANInterrupts
Relation:
OneChOpt
DevErrorDetect
StatusPolling,WakeupPolling
TxPolling,RxPolling,IndividualPolling
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4Interrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
{
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if (CanLL_30_Flexcan4_CheckSizeOfHwChannelData( canHwChannel ) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */  /* CM_CAN_LL07 */
# endif
  {
  
#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
    Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4Interrupt);
#   endif


#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
    {
      CanHL_30_Flexcan4_ErrorHandling( CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY );
    }
#  endif

#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
    /*  #30 Process mailbox interrupt */
    CanLL_30_Flexcan4_CanMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
#  endif /* C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT */

#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
    Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4Interrupt);
#   endif
  }
} /* end of Can_30_Flexcan4Interrupt */
/* CODE CATEGORY 1 END */
# endif
#endif /* C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE */

#if defined ( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE */
# if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4BusOffInterrupt
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4BusOffInterrupt
Relation_Context:
# from Can_30_Flexcan4BusOffIsr_0 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_1 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_2 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_3 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_4 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_5 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_6 #
StatusPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4BusOffIsr_7 #
StatusPolling,UseNestedCANInterrupts
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
*/
/* CODE CATEGORY 1 START */
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4BusOffInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY )
{
#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4BusOffInterrupt);
#   endif

  /* #10 Call internal indication function if BUSOFF occurred */
#  if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if (CanLL_30_Flexcan4_CheckSizeOfHwChannelData( canHwChannel ) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL07 */
#  endif
  {
    CanHL_30_Flexcan4_ErrorHandling( CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY );
  }

#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4BusOffInterrupt);
#   endif
}
/* CODE CATEGORY 1 END */
# endif
# if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/****************************************************************************
| NAME:             Can_30_Flexcan4MailboxInterrupt
****************************************************************************/
/*
|<DataModelStart>| Can_30_Flexcan4MailboxInterrupt
Relation_Context:
# from Can_30_Flexcan4MailboxIsr_0 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_1 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_2 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_3 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_4 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_5 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_6 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
# from Can_30_Flexcan4MailboxIsr_7 #
TxPolling,RxPolling,IndividualPolling,UseNestedCANInterrupts
Relation:
OneChOpt
DevErrorDetect
TxPolling,RxPolling,IndividualPolling
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
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4MailboxInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY ) /* COV_CAN_MAILBOX_INT_INDIVPOLLING */
{
#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Start(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4MailboxInterrupt);
#   endif

  /* #10 Process mailbox interrupt */
# if (CAN_30_FLEXCAN4_SAFE_BSW == STD_ON) /* COV_CAN_30_FLEXCAN4_SAFE_BSW */
  if (CanLL_30_Flexcan4_CheckSizeOfHwChannelData( canHwChannel ) == kCan_30_Flexcan4Ok) /* COV_CAN_GENDATA_FAILURE */ /* CM_CAN_LL07 */
# endif
  {
    CanLL_30_Flexcan4_CanMailboxInterrupt(CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY);
  }

#   if (CAN_30_FLEXCAN4_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
  Rtm_Stop(RtmConf_RtmMeasurementPoint_Can_30_Flexcan4MailboxInterrupt);
#   endif
}
# endif
#endif /* C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE */

#define CAN_30_FLEXCAN4_STOP_SEC_CODE  /*----------------------------------------------*/
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
PRQA message 0784, 0788
  Reason: A parameter that is also used as a macro name is accepted for compatibility reasons.
  Risk: The macro can change the declaration unintentionally and readability is reduced.
  Prevention: Code inspection and test of the different variants in the component test.

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
PRQA message 3673, 3679
  Reason: Non-const pointer parameter or variable is required by the internal interface or compatibility reasons but depending on the configuration or specific platform implementation the target may not always be modified.
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
PRQA message 1533, 3408
  Reason: These constants are defined in a generated file and cannot be moved to the static source file. Visibility might be reduced by stricter switches for the declaration (kept for compatibility reasons).
  Risk: Compile or link errors.
  Prevention: Code inspection and compile-link of the different variants in the component and integration test.

MD_Can_Union:
PRQA message 0750, 0759
  Reason: Using union type to handle different data accesses.
  Risk: Misinterpreted data.
  Prevention: Code inspection and test of the different variants in the component test.

MD_Can_Assertion:
PRQA message 2842, 2897
  Reason: Assertion leads to apparent out of bounds indexing or casting an apparent negative value to an unsigned type.
  Risk: Undefined behaviour.
  Prevention: Code inspection. The assertion itself prevents the apparent anomaly.

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
  
MD_Can_3305_LL_MsgObjectAccess
PRQA message 3305
  Reason: Pointer keeps either a RX message buffer object or a HW message buffer object. Both are stored as a uint8 pointer. For access the appropriate structure is used.
  Risk: Wrong object structure might be accessed
  Prevention: Code review and runtime tests

MD_Can_LL_HwAccess:
PRQA message 0306
  Reason: Hardware access needs cast between a pointer object and an integral type in case of Virtual Addressing.
  Risk: Access of unknown memory.
  Prevention: Runtime tests.
  
MD_Can_ArraySizeUnknown:
PRQA message 3684
  Reason: Arrays declared without size, because size depends on configuration and is unknown here, especially for linktime tables.
  Risk: Data access outside table.
  Prevention: Code inspection and test of the different variants in the component test.
  
MD_Can_FlexibleStructMember:
PRQA message 	1039
  Reason: Array struct member is of size one and at the end of the struct.
  Risk: None.
  Prevention: None.

MD_Can_LL_VolatileAccess
PRQA message 0404
  Reason: Two volatile parameter values in a function call. One of the parameter is a register value to be configured during initialization and is not changed by hardware.
  Risk: None
  Prevention: None

MD_Can_RuntimeOptimization_LL:
PRQA message 3415
  Reason: Runtime optimization by explicitly assigning a function call to the right hand of the && or || operator.
  Risk: Side effects of function may always be expected.
  Prevention: Code inspection and test of different variants in the component test.
*/

/****************************************************************************/
/*  Silent deviations                                                       */
/****************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CAN_HL01
 \DESCRIPTION Write access to Can_30_Flexcan4ModeTransitionRequest with controller as index. Controller index is checked against sizeof Can_30_Flexcan4ModeTransitionRequest in Can_30_Flexcan4_SetControllerMode(), Can_30_Flexcan4_RamCheckExecute(), Can_30_Flexcan4_MainFunction_Mode(), Can_30_Flexcan4_Init() and CanHL_30_Flexcan4_ReInit().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL02
 \DESCRIPTION Write access to Can_30_Flexcan4LogStatus with controller as index. Controller index is checked against sizeof CanLogStatus in Can_30_Flexcan4_SetControllerMode(), CanHL_30_Flexcan4_ErrorHandling(), Can_30_Flexcan4_RamCheckEnableController(), Can_30_Flexcan4_MainFunction_Mode(), Can_30_Flexcan4_Init() and CanHL_30_Flexcan4_ReInit().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL04
 \DESCRIPTION Write access to Can_30_Flexcan4ActiveSendState with activeSendObject as index. activeSendObject is checked against size of ActiveSendObject in Can_30_Flexcan4_CleanUpSendState(), CanHL_30_Flexcan4_WritePrepare(), CanHL_30_Flexcan4_InitTxFullCAN(), CanHL_30_Flexcan4_InitTxBasicCAN(), CanHL_30_Flexcan4_TxTaskCancelationHandling(), CanHL_30_Flexcan4_TxConfirmation() and Can_30_Flexcan4_CancelTx().
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL05
 \DESCRIPTION Write access to CanActiveSendPdu with activeSendObject as index. activeSendObject is checked against size of ActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL06
 \DESCRIPTION Call Can_30_Flexcan4_GenericPrecopy() and CanIf_RxIndication() with parameter RX Queue data pointer. The data pointer is only used for read access and it is always a valid pointer (module local defined).
 \COUNTERMEASURE \R [CM_CAN_HL25]

\ID SBSW_CAN_HL07
 \DESCRIPTION Call Can_30_Flexcan4_GenericPrecopy() and CanIf_RxIndication() with parameter Rx Struct data pointer. The data pointer is only used for read access. Parameter is of type P2CONST in API.
 \COUNTERMEASURE \N [CM_CAN_HL31]

\ID SBSW_CAN_HL08
 \DESCRIPTION Write access to Can_30_Flexcan4IsBusOff with controller as index. Controller index is checked against sizeof CanIsBusOff in Can_30_Flexcan4_SetControllerMode(), CanHL_30_Flexcan4_ErrorHandling() and Can_30_Flexcan4_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL09
 \DESCRIPTION Write access to Can_30_Flexcan4IsWakeup with controller as index. Controller index is checked against sizeof CanIsWakeup in CanHL_30_Flexcan4_WakeUpHandling(), Can_30_Flexcan4_CheckWakeup(), reinitialization and Can_30_Flexcan4_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL10
 \DESCRIPTION Write access to VersionInfo pointer within function Can_30_Flexcan4_GetVersionInfo()
 \COUNTERMEASURE \N caller of Can_30_Flexcan4_GetVersionInfo() has to ensure that VersionInfo pointer is valid. (global description available)

\ID SBSW_CAN_HL11
 \DESCRIPTION Write access to Can_30_Flexcan4LastInitObject with controller as index. Controller index is checked against sizeof Can_30_Flexcan4LastInitObject in CanHL_30_Flexcan4_ReInit(), Can_30_Flexcan4_ChangeBaudrate(), Can_30_Flexcan4_SetBaudrate() and Can_30_Flexcan4_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL12
 \DESCRIPTION Write access to Can_30_Flexcan4InterruptCounter with controller as index. Controller index is checked against sizeof Can_30_Flexcan4InterruptCounter in Can_30_Flexcan4_Init(), Can_30_Flexcan4_EnableControllerInterrupts() and Can_30_Flexcan4_DisableControllerInterrupts()
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL16
 \DESCRIPTION Call CanLL_30_Flexcan4_CanInterruptDisable() with parameter Can_30_Flexcan4InterruptOldStatus as pointer (compiler secures used type in function declaration fitting to given pointer parameter type).
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL18
 \DESCRIPTION Write access to Can_30_Flexcan4RxQueueBuffer with queue write index. Write index is checked against size of CanRxQueueBuffer.
 \COUNTERMEASURE \R [CM_CAN_HL24]

\ID SBSW_CAN_HL19
 \DESCRIPTION Write access to Can_30_Flexcan4RxQueueBuffer.data with queue write index and data index. Write index is checked against size of Can_30_Flexcan4RxQueueBuffer. (see also generator CM_CAN_HL04 qualified use case CSL02)
 \COUNTERMEASURE \R [CM_CAN_HL24]
                 \R [CM_CAN_HL17]

\ID SBSW_CAN_HL20
 \DESCRIPTION Write access to CanMirrorModeState with controller as index. Controller is checked against size of CanMirrorModeState.
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL22
 \DESCRIPTION Write access to ActiveSendData with activeSendData + length as index. Index is checked against size of table.
 \COUNTERMEASURE \R [CM_CAN_HL20]

\ID SBSW_CAN_HL23
 \DESCRIPTION Write access to ActiveSendObject stuct with activeSendObject as index. activeSendObject is checked against size of Can_30_Flexcan4ActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL24
 \DESCRIPTION Call Appl_30_Flexcan4_GenericPreTransmit() with parameter PduInfoPtr_var pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL25
 \DESCRIPTION Call CanIf_CancelTxConfirmation() with parameter Can_30_Flexcan4ActiveSendObject.PduInfo[activeSendObject] as pointer with activeSendObject as index. activeSendObject is checked against size of Can_30_Flexcan4ActiveSendObject.
 \COUNTERMEASURE \R [CM_CAN_HL03]

\ID SBSW_CAN_HL26
 \DESCRIPTION Call CanHL_30_Flexcan4_RxBasicMsgReceivedBegin(), CanHL_30_Flexcan4_RxMsgReceivedNotification() and CanHL_30_Flexcan4_RxBasicMsgReceivedEnd() with parameter rxBasicPara pointer (module local defined).
 \COUNTERMEASURE \N The pointer is static and always valid.

\ID SBSW_CAN_HL27
 \DESCRIPTION Call CanHL_30_Flexcan4_RxFullMsgReceivedBegin(), CanHL_30_Flexcan4_RxFullReceivedNotification() and CanHL_30_Flexcan4_RxFullMsgReceivedEnd() with parameter rxFullPara pointer (module local defined).
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
 \DESCRIPTION Call Appl_30_Flexcan4_GenericConfirmation and CanHL_30_Flexcan4_SetCanPduInfo with canPdu pointer as parameter (local defined).
 \COUNTERMEASURE \N The structure is static and always valid.

\ID SBSW_CAN_HL33
 \DESCRIPTION Write access to Can_30_Flexcan4BusOffCounter with controller as index. Controller index is checked against sizeof CanBusOffCounter in Can_30_Flexcan4_SetControllerMode(), CanHL_30_Flexcan4_ErrorHandling() and Can_30_Flexcan4_Init().
 \COUNTERMEASURE \R [CM_CAN_HL01]

\ID SBSW_CAN_HL34
 \DESCRIPTION Write access to Can_30_Flexcan4MailboxState with mailbox handle as index. Index secured by ComStackLib [CM_CAN_HL29] and [CM_CAN_HL30] and checked against size of Can_30_Flexcan4MailboxState.
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
 \DESCRIPTION Write access to Can_30_Flexcan4LogStatus with controller as index. Controller is secured by qualified use-case CSL03 of ComStackLib.
 \COUNTERMEASURE \N [CM_CAN_HL02]

\ID SBSW_CAN_HL41
 \DESCRIPTION Write access to Can_30_Flexcan4ActiveSendState with activeSendObject as index. activeSendObject is temporary stored in a list of [1,3] elements where each element is initialized to a valid value and always written with size check before.
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
 \DESCRIPTION Write access to ControllerModePtr pointer within function Can_30_Flexcan4_GetControllerMode()
 \COUNTERMEASURE \N caller of Can_30_Flexcan4_GetControllerMode() has to ensure that ControllerModePtr pointer is valid. (global description available)

\ID SBSW_CAN_HL52
 \DESCRIPTION Write access to ErrorStatePtr pointer within function Can_30_Flexcan4_GetControllerErrorState()
 \COUNTERMEASURE \N caller of Can_30_Flexcan4_GetControllerErrorState() has to ensure that ErrorStatePtr pointer is valid. (global description available)

\ID SBSW_CAN_HL53
 \DESCRIPTION Write access to TxErrorCounterPtr pointer within function Can_30_Flexcan4_GetControllerTxErrorCounter()
 \COUNTERMEASURE \N caller of Can_30_Flexcan4_GetControllerTxErrorCounter() has to ensure that TxErrorCounterPtr pointer is valid. (global description available)

\ID SBSW_CAN_HL54
 \DESCRIPTION Write access to RxErrorCounterPtr pointer within function Can_30_Flexcan4_GetControllerRxErrorCounter()
 \COUNTERMEASURE \N caller of Can_30_Flexcan4_GetControllerRxErrorCounter() has to ensure that RxErrorCounterPtr pointer is valid. (global description available)

\ID SBSW_CAN_HL55
 \DESCRIPTION A ComStackLib generated variable is accessed.
 \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

\ID SBSW_CAN_HL56
 \DESCRIPTION Write access to local canPdu.sdu with canPdu.length as array size.
 \COUNTERMEASURE \R [CM_CAN_HL33]



\ID SBSW_CAN_LL01
 \DESCRIPTION Access to a hardware register (Global section) of the FlexCAN module using a pointer access. Pointer is derived using channel as index. Channel is checked by caller. (see also generator CM_CAN_LL08 qualified use case CSL02)
 \COUNTERMEASURE \R [CM_CAN_LL03]
                 \R [CM_CAN_LL05]
                 \S [CM_CAN_LL06]
                 \R [CM_CAN_LL07]

\ID SBSW_CAN_LL02
 \DESCRIPTION Access to a hardware register (mailbox section) of the FlexCAN module using a structure access. In addition it is referenced by the global FlexCAN module pointer that refernces the channel.
 \COUNTERMEASURE \R [CM_CAN_LL03]
                 \R [CM_CAN_LL04]
                 \R [CM_CAN_LL05]
                 \S [CM_CAN_LL06]
                 \R [CM_CAN_LL07]
                 \S [CM_CAN_LL09]

\ID SBSW_CAN_LL03
 \DESCRIPTION Call with pointer to CAN cell register (either global section or mailbox section) for write and read access.
 \COUNTERMEASURE \R [CM_CAN_LL03]
                 \R [CM_CAN_LL04]
                 \R [CM_CAN_LL05]
                 \S [CM_CAN_LL06]
                 \R [CM_CAN_LL07]
                 \S [CM_CAN_LL09]

\ID SBSW_CAN_LL04
 \DESCRIPTION Write access with pointer to initPara, txConfPara, rxBasicPara, rxFullPara, txCancellationPara, taskPara structure (local defined).
 \COUNTERMEASURE \N The structure is static and always valid (pointer to a local stack variable).

\ID SBSW_CAN_LL05
 \DESCRIPTION Write access to canTxBuf with dedicated integers as index.
 \COUNTERMEASURE \N Type of static Can_30_Flexcan4TxBuf structure.

 \ID SBSW_CAN_LL06
 \DESCRIPTION Write access to generated Can_30_Flexcan4RxMsgBuffer with channel as index.
 \COUNTERMEASURE \R [CM_CAN_LL01]
                 \R [CM_CAN_LL03]

\ID SBSW_CAN_LL07
 \DESCRIPTION Write access to localInterruptOldFlagPtr.
 \COUNTERMEASURE \N Caller ensures validity of pointer parameter. See SBSW_CAN_HL16.

\ID SBSW_CAN_LL08
 \DESCRIPTION Call GetCounterValue with timer index as parameter.
 \COUNTERMEASURE \R [CM_CAN_LL02]

\ID SBSW_CAN_LL09
 \DESCRIPTION Write access to section of the FlexCAN using a pointer derived from the FlexCAN module's base address (RXFIFO, Can_30_Flexcan4RxMask, Can_30_Flexcan4ParityCheck, DebugRegister, Can_30_Flexcan4FDRegister, IndivRxMask).
 \COUNTERMEASURE \R [CM_CAN_LL03]
                 \R [CM_CAN_LL04]
                 \R [CM_CAN_LL05]
                 \S [CM_CAN_LL06]

\ID SBSW_CAN_LL10
 \DESCRIPTION Write access to canFDTxBuf with index.
 \COUNTERMEASURE \N pduInfo_var_dli never exceeds maximum value of 64 bytes (this is secured by core: Can_30_Flexcan4_DliToBytesMap, Can_30_Flexcan4_BytesToDliMap). Thus it is ensured that sduWordLen and dataWordLen never exceeds size of canFDTxBuf.

\ID SBSW_CAN_LL11
 \DESCRIPTION Write access to canFDTxBuf with unsecured maximum value from caller.
 \COUNTERMEASURE \R [CM_CAN_LL11]

\ID SBSW_CAN_LL12
 \DESCRIPTION Write access to canFDTxBuf with index .
 \COUNTERMEASURE \R [CM_CAN_LL10]

\ID SBSW_CAN_LL13
 \DESCRIPTION Write access to canTxBuf with static index.
 \COUNTERMEASURE \N the memory access is warranty by the static index used (0,1).

\ID SBSW_CAN_LL14
 \DESCRIPTION Write access to canLL_canStartReinitState, canLL_PassiveReportFlag, canLL_Estat with channel as index.
 \COUNTERMEASURE \R [CM_CAN_LL03]
                 \R [CM_CAN_LL07]

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

\CM CM_CAN_LL01 Can_30_Flexcan4RxMsgBuffer size is checked during PowerOnInit against static defined size; canConfigInitFlag remains CAN_30_FLEXCAN4_STATUS_UNINIT in case of failure.
\CM CM_CAN_LL02 Loop timeout is checked during PowerOnInit against maximum value CAN_30_FLEXCAN4_MAX_LOOP); canConfigInitFlag remains CAN_30_FLEXCAN4_STATUS_UNINIT in case of failure.
\CM CM_CAN_LL03 Is covered by CM_CAN_HL01: A runtime check ensures controller index is always smaller than number of elements in arrays with controller amount as dimension (Qualified use-case CSL01 of ComStackLib size of ControllerData).
\CM CM_CAN_LL04 HwObjHandle for mailbox and individual receive masks index is secured by a runtime check and in addition it is secured by a DET.
\CM CM_CAN_LL05 TCASE-429: Code inspection ensures that the defined access structures match the memory structure of the FlexCAN module.
\CM CM_CAN_LL06 User must verify that the generated base address(es) for the FlexCAN controller matches to the used derivative (see SMI-815491).
\CM CM_CAN_LL07 channel number is checked in interrupt (runtime check).
\CM CM_CAN_LL08 Qualified use-case CSL02 of ComStackLib (size of Can_30_Flexcan4_GetBaseAddressOfControllerConfig == size of Can_30_Flexcan4ControllerData).
\CM CM_CAN_LL09 User must verify that the generated mailbox addresses in case of FD mode 2 matches the address range of the used derivative (see SMI-815491).
\CM CM_CAN_LL10 CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH() is checked during PowerOnInit against static defined size; canConfigInitFlag remains CAN_30_FLEXCAN4_STATUS_UNINIT in case of failure.
\CM CM_CAN_LL11 sduWordLen is restricted to maximum buffer size.

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

\ID COV_CAN_30_FLEXCAN4_SAFE_BSW_EXCLUSION
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


\ID COV_CAN_MULTIPLE_BASICCAN_TX
  \ACCEPT TF
  \ACCEPT TF tf tf
  \ACCEPT TF tx tf tx tx tx tx
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_SLEEP_SUPPORT
  \ACCEPT XF
  \REASON This feature is not supported - always deactivated.

\ID COV_CAN_WAKEUP_SUPPORT
  \ACCEPT XF
  \REASON This feature is not supported - always deactivated.

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
  \REASON This feature is not supported - always deactivated.

\ID COV_CAN_INDIVIDUAL_POLLING
  \ACCEPT TF
  \REASON The feature is platform and Project specific.

\ID COV_CAN_TX_POLLING_OR_CANCEL_IN_HW_TASK
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tf xf
  \REASON The feature is platform specific - C_30_FLEXCAN4_HL_ENABLE_CANCEL_IN_HW_TASK is always true or false.

\ID COV_CAN_GENERIC_CONFIRMATION_API2
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_FD_SUPPORT
  \ACCEPT TF
  \REASON The feature is platform and Project specific - fully supported.

\ID COV_CAN_MULTIPLE_BASICCAN
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX tf tx
  \ACCEPT TF tf tx
  \ACCEPT TF tx tf
  \ACCEPT TF tx tf tf tf tf tf
  \ACCEPT TF tx tf tx tx tx tx
  \REASON The feature is platform and Project specific - this feature is not supported.

\ID COV_CAN_TX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_RX_FULLCAN_OBJECTS
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HW_TRANSMIT_CANCELLATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_CANCEL_IN_HW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HARDWARE_CANCELLATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_EXTENDED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MIXED_ID
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_SECURE_TEMP_BUFFER
  \ACCEPT XF
  \REASON This feature is not supported - always deactivated.

\ID COV_CAN_OVERRUN_NOTIFICATION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_MULTIPLEXED_TRANSMISSION
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_TX_HW_FIFO
  \ACCEPT XF
  \REASON This feature is not supported - always deactivated.

\ID COV_CAN_RX_QUEUE
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HW_LAYOUT_TXBASIC_FIRST
  \ACCEPT TX
  \REASON For FlexCAN4 the mailbox ordering is TX Basic first - always activated.

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
  \ACCEPT XF xf xf xf xf
  \ACCEPT XF xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf xf xf
  \REASON The condition checks for hardware failures that cannot be stimulated. The code is verified by code inspection.

\ID COV_CAN_LL_HARDWARE_BEHAVIOUR
  \ACCEPT TX
  \ACCEPT XF
  \REASON The condition checks for hardware depended return value. The return value is always true because hardware never fail here. The code is verified by code inspection.

\ID COV_CAN_GENDATA_FAILURE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT TF tf xf
  \ACCEPT TX tx tx
  \ACCEPT TF xf tf
  \ACCEPT TF tf xf tf
  \ACCEPT TF xf tf xf tf
  \ACCEPT TX tx xf tx tx
  \ACCEPT TX tx tf tx tx
  \ACCEPT TX tx tx xx xx
  \ACCEPT XF xf xf xf xf
  \REASON The condition checks for generated data failures that cannot be simulated. The code is verified by code inspection.

\ID COV_CAN_GENDATA_NOT_IN_ALL_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT X
  \REASON Generated data does not contain all data sets but checked in code. Not encapsulated with preprocessor switch to simplify code.  

\ID COV_CAN_OS_USAGE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX xf tx
  \ACCEPT TX tx xf
  \ACCEPT TF tx tf
  \ACCEPT XF xf tx
  \ACCEPT TF xf xf
  \ACCEPT TF xf tf
  \ACCEPT XF tx xf
  \REASON Os may be used in component test or not. The code is verified by code inspection.

\ID COV_CAN_INTERRUPT_SUPPORT
  \ACCEPT TF tf tx
  \ACCEPT TX tf tx
  \REASON Busoff Interrupt is in some configurations always true

\ID COV_CAN_LOCK_ISR_BY_APPL
  \ACCEPT TX
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
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT XF xf xf
  \ACCEPT XF xf xx
  \ACCEPT TF tf tx
  \ACCEPT XF xf xf xx
  \REASON Hardware loop time out is not covered in all configurations.

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
  \ACCEPT XF xf xf xf xf
  \ACCEPT XF xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf xf
  \ACCEPT XF xf xf xf xf xf xf xf xf xf xf xf
  \REASON some configurations do not stimulate RamCheck by test suite, to check the standard CAN driver behaviour.

\ID COV_CAN_30_FLEXCAN4_SAFE_BSW
  \ACCEPT TF
  \REASON The feature is platform specific - fully supported.

\ID COV_CAN_HWOBJINDIVPOLLING
  \ACCEPT TF tf xf
  \ACCEPT TX
  \ACCEPT XF
  \REASON usage of individual hardware object polling is platform specific always true or false.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES
  \ACCEPT TX
  \ACCEPT XF
  \REASON Feature only available with FlexCAN4 derivatives having different number of hardware Mailboxes on the channels.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB
  \ACCEPT TX
  \ACCEPT TX tx tx
  \ACCEPT TX xf tx
  \ACCEPT XF
  \ACCEPT XF xf xf
  \REASON Depending on the FLexCAN4 derivatives different maximum number of Hardware Mailboxes are supported.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN
  \ACCEPT TX
  \REASON Always true for ARM32 FlexCAN4 derivatives.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_BIG_ENDIAN
  \ACCEPT TX
  \REASON Always true for MPC FlexCAN4 derivatives.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_SINGLE
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on the FlexCAN4 derivative a single interrupt source is available.

\ID COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on the FlexCAN4 derivative a multiple interrupt sources are available.

\ID COV_CAN_EQUAL_CHANNEL_LAYOUT
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF
  \REASON The test is executed on defined channels. The channel layout is equal for each channel.

\ID COV_CAN_LL_OSCAT_CONFIG_CHECK
  \REASON The define 'osdIsr' is only available with OSEK OS. It is only used to verify the configuration and does not influence the runtime code.

\ID COV_CAN_LL_ISR_PROTOTYPE
  \REASON Is not fully covered with tf tf but with all other combinations. Tool does not recognize it.

\ID COV_CAN_MAILBOX_ORDER
  \ACCEPT TF tx tf
  \ACCEPT TF tx tf
  \ACCEPT TF tf tx
  \ACCEPT TX tx tx
  \ACCEPT XF tf xf
  \ACCEPT XF tx xf
  \ACCEPT XF xf xx
  \ACCEPT TX
  \REASON Depends on order of generated RX, TX Basic CAN or Full CAN objects.

\ID COV_CAN_HARDWARE_PROCESSING_TIME
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XF xf xx
  \REASON Hardware processing time cannot be influenced. Is either true or false.

\ID COV_CAN_TESTSTIMULATION
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT TX tf tx
  \ACCEPT TF tf tx tf
  \REASON Not in all configurations fully covered.

\ID COV_CAN_INIT_STIMULATION
  \ACCEPT TX
  \ACCEPT TF xf tf
  \ACCEPT TX xf tx
  \ACCEPT TX tf tx
  \ACCEPT TF xf tf tf
  \ACCEPT TF xf tx tf
  \ACCEPT TF tf tx tf
  \REASON Test Automation does not cover disable mode during power on (manually tested). Busoff state only covered here if busoff recovery is chosen to be "not compliant (not waiting 128x11 recessive bits)".

\ID COV_CAN_CLK_SRC_GENERATION
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on hardware Usage of Oscillator Clock or System Clock is used.

\ID COV_CAN_STOP_BOFF_STIMULATION
  \ACCEPT TF
  \ACCEPT TX
  \ACCEPT XF
  \REASON Busoff condition during stop transition is not covered by all configurations.

\ID COV_CAN_REINITSTART_STIMULATION
  \ACCEPT TX
  \REASON If Reinit Start Feature is active condition is always true.

\ID COV_CAN_START_STIMULATION
  \ACCEPT TF
  \ACCEPT XF
  \REASON Depending on derivative or configuration Start Request State is hit or Started State is reached very fast.

\ID COV_CAN_BUSOFFEND_STIMULATION
  \ACCEPT TF
  \ACCEPT TX
  \ACCEPT XF
  \REASON Not all possible states covered (unlikely states). Manually tested.

\ID COV_CAN_CONTROLLER_INIT_STATE
  \ACCEPT XF
  \REASON SAFE_BSW check expected to be false.

\ID COV_CAN_GENDATA_TX_NOT_IN_ALL_CONFIG
  \ACCEPT TF tx tf tx tf
  \ACCEPT TF tf tx tx tf
  \ACCEPT TF tf tf tx tf
  \ACCEPT TF tx tf tf
  \ACCEPT TF tf tx tf
  \ACCEPT TF tf tf tx
  \ACCEPT TF tf tx tx
  \ACCEPT TF tx tf tx
  \ACCEPT TF tx tf
  \ACCEPT TF tf tx
  \REASON Generated data does not contain all data sets but checked in code. Not encapsulated with preprocessor switch to simplify code.

\ID COV_CAN_FULL_OVERRUN_HANDLING
  \ACCEPT XF
  \ACCEPT TF
  \REASON FullCAN overrun is only detected if BasicCAN filter is configured in such a way that FullCAN messages cannot be received in a BasicCAN mailbox.

\ID COV_CAN_BUSOFF_HANDLING
  \ACCEPT TX
  \ACCEPT TF
  \REASON Is always true in CanHL_30_Flexcan4_ErrorHandling() when it is only called due to busoff.

\ID COV_CAN_RXFIFO_HANDLING
  \ACCEPT XF xf xx
  \ACCEPT TF tf tx
  \ACCEPT TF tf tf
  \REASON Depends on whether messages in RX-FIFO are available and on the number of queued messages.

\ID COV_CAN_FULLCAN_LL_RX_POLLING
  \ACCEPT TX
  \ACCEPT TF
  \REASON Is always true when during a polling period a FullCAN is received.

\ID COV_CAN_FULLCAN_LL_TX_POLLING
  \ACCEPT TX
  \ACCEPT TF
  \REASON Is always true when during a polling period a message is transmitted.

\ID COV_CAN_MUMOFHANDLES_CORRECTON
  \ACCEPT TX
  \ACCEPT XF
  \REASON Correction needed if nofOfHandles is greater 0 (always true or false depending on generated data)

\ID COV_CAN_FD_BAUDRATE_AVAILABILITY
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF
  \REASON Generated data does not contain all data sets but checked in code. Not encapsulated with preprocessor switch to simplify code.

\ID COV_CAN_SAFE_BSW_EXCLUSION_FLEXCAN4
  \ACCEPT XF
  \ACCEPT XF xf xf
  \ACCEPT XF xf xf xf
  \REASON SafeBsw restricted feature will not be enforced to be deactivated or activated.

\ID COV_CAN_MAILBOX_INT_INDIVPOLLING
  \ACCEPT TF
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT XX
  \ACCEPT X
  \REASON In case of Individual Polling activated, mailbox interrupts are enabled. But if there are only polling objects interrupt is never called.

\ID COV_CAN_MISRA
  \ACCEPT XF
  \ACCEPT TX
  \REASON Only used for MISRA analysis not active in production code.

\ID COV_CAN_RAMCHECK_EXTENDED_CLKSRC_COMPATABILITY
  \ACCEPT XF tx xf
  \ACCEPT XF xf tx
  \ACCEPT TF xf xf
  \REASON Set to a default value if not generated. C_30_FLEXCAN4_ENABLE_CLKSRC_SELECTION shall be defined if clock source can be selected, C_30_FLEXCAN4_DISABLE_CLKSRC_SELECTION otherwise.
  
\ID COV_CAN_RAMCHECK_EXTENDED_CLKSRC_AVAILABILITY
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF tf tx
  \ACCEPT XF tf xf
  \REASON Extended RAM check can only be excecuted on devices that support clock source selection.
  
\ID COV_CAN_BUSOFF_INTERRUPT_ONLY
  \ACCEPT TX
  \ACCEPT TF TX
  \REASON Result is always true if Busoff Interrupt and RX/TX polling is configured.

\ID COV_CAN_ENHANCED_RXFIFO_SUPPORT
  \ACCEPT TF
  \ACCEPT TX
  \REASON Result is always true because Multiple BasicCAN feature is not supported. In case of RAM check failure it varies between true and false.

\ID COV_CAN_ENHANCED_FILTERTABLE_GENERATION
  \ACCEPT TF
  \ACCEPT XF
  \REASON If Standard or Extended Filter are not available the result is false.

\ID COV_CAN_ERROR_INTERRUPT_AVAILABLE
  \ACCEPT TF tx tf
  \ACCEPT XF xf tf
  \REASON Depending on the derivatives error interrupt is available or not available.

\ID COV_CAN_PHYSTOLOG_MAPPING
  \ACCEPT XF
  \REASON For Error interrupt not fully covered. The code is verified by code inspection.

\ID COV_CAN_ACTIVE_TRANSITION_STIMULATION
  \ACCEPT TF tx tf
  \REASON RX and TX error counters must be less than 128. One of them does never exceed 128 due error stimulation.

END_COVERAGE_JUSTIFICATION */ 


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/************   Organi, Version 3.9.2 Vector-Informatik GmbH  ************/
