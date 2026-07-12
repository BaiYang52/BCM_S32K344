/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_Net.h
 *         \unit  Net
 *        \brief  Contains public inline function definitions of Network unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_NET_H)
# define DCM_NET_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_NetTypes.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON) || \
     (DCM_SVC_10_RESET_AFTER_RESPONSE == STD_ON) || \
     (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
#  define DCM_NET_RX_BLOCKING_ENABLED                                STD_ON
# else
#  define DCM_NET_RX_BLOCKING_ENABLED                                STD_OFF
# endif

# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) || \
     (DCM_DIAG_VIRTUAL_REQUEST_ENABLED == STD_ON)
#  define DCM_NET_INTERNAL_REQ_ENABLED                               STD_ON
# else
#  define DCM_NET_INTERNAL_REQ_ENABLED                               STD_OFF
# endif

/*! Calculate the maximum number of events related to USDT transport objects, needed by a network related task */
# if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
#  define DCM_NET_MAX_NUM_EVENT_TRANSP_OBJECTS                       32u                            /*!< Take the worst case, limited to 32 for a bit mask usage (i.e. task events) */
# else
#  define DCM_NET_MAX_NUM_EVENT_TRANSP_OBJECTS                       DCM_NET_MAX_NUM_TRANSP_OBJECTS /*!< Allocate as many as needed by the concrete configuration */
# endif

/*! ComM channel message communication ability states */
# define DCM_NET_COMM_STATE_FLAG_RX_DIS                              ((Dcm_NetComMStateType)0x00u ) /*!< Message reception is disabled */
# define DCM_NET_COMM_STATE_FLAG_TX_DIS                              ((Dcm_NetComMStateType)0x00u ) /*!< Message transmission is disabled */
# define DCM_NET_COMM_STATE_FLAG_TX_EN                               ((Dcm_NetComMStateType)0x01u ) /*!< Message transmission is enabled */
# define DCM_NET_COMM_STATE_FLAG_RX_EN                               ((Dcm_NetComMStateType)0x02u ) /*!< Message reception is enabled */

/*! Calculates a combined reception and transmission message communication state */
# define DCM_NET_COMM_STATE_COMPOSER(rx,tx)                          ((Dcm_NetComMStateType)((rx)|(tx)))                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Presets of all possible message communication states in both directions (Rx and Tx) */
# define DCM_NET_COMM_STATE_RX_DIS_TX_DIS                            (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_DIS,DCM_NET_COMM_STATE_FLAG_TX_DIS)) /*!< Message reception and transmission are disabled (NoCommunicationMode) */
# define DCM_NET_COMM_STATE_RX_DIS_TX_EN                             (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_DIS,DCM_NET_COMM_STATE_FLAG_TX_EN))  /*!< Message reception is disabled, but transmission is enabled */
# define DCM_NET_COMM_STATE_RX_EN_TX_DIS                             (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_EN, DCM_NET_COMM_STATE_FLAG_TX_DIS)) /*!< Message reception is enabled, but transmission is disabled (SilentCommunicationMode) */
# define DCM_NET_COMM_STATE_RX_EN_TX_EN                              (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_EN, DCM_NET_COMM_STATE_FLAG_TX_EN))  /*!< Message reception and transmission are enabled (FullCommunicationMode) */

# define DCM_NET_INVALID_CONNHDL                                     ((Dcm_CfgNetNetIdRefMemType)Dcm_NetGetNumConnections())
# define DCM_NET_INVALID_TOBJID                                      ((Dcm_CfgNetTObjHandleMemType)Dcm_PbCfgNetNumTransportObjects)
# define DCM_NET_INVALID_PROTID                                      ((Dcm_NetProtRefMemType)Dcm_PbCfgNetNumProtocols)
# define DCM_NET_INVALID_PTXOBJ_HANDLE                               ((Dcm_CfgNetPTxObjHandleMemType)Dcm_NetGetNumPerTxObjects())

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Network variant/non-variant configuration abstraction */
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
#  define Dcm_NetCfgGetNumRxPduIds(comVariant)                       ((comVariant)->NumRxPduIds)                                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumTxPduIds(comVariant)                       ((comVariant)->NumTxPduIds)                                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetMinCanTpRxPduId(comVariant)                   ((comVariant)->CanTpRxPduIdMin)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetMaxCanTpRxPduId(comVariant)                   ((comVariant)->CanTpRxPduIdMax)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumCanRxPduIds(comVariant)                    ((comVariant)->NumCanTpRxPduIds)                                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumPerTxObjects(comVariant)                   ((comVariant)->NumPerTxObjects)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumConnections(comVariant)                    ((comVariant)->NumConnections)                                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumProtocols(comVariant)                      ((comVariant)->NumProtocols)                                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumComMChannels(comVariant)                   ((comVariant)->NumComMChannels)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumBuffers(comVariant)                        ((comVariant)->NumBuffers)                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumTranspObjects(comVariant)                  ((comVariant)->NumTranspObjects)                                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNumAllComMChannels(comVariant)                ((comVariant)->NumAllComMChannels)                                              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetCfgGetRxPduInfo(comVariant)                         ((comVariant)->RxPduIdTable)                                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetConnectionInfo(comVariant)                    ((comVariant)->ConnectionTable)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetProtocolInfo(comVariant)                      ((comVariant)->ProtocolTable)                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNetConnComMChannelMap(comVariant)             ((comVariant)->NetConnComMChannelMap)                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNetworkHdlLookUp(comVariant)                  ((comVariant)->NetworkHandleLookUpTable)                                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetTxPdu2ConnMap(comVariant)                     ((comVariant)->TxPduIdMap)                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetPerTxPdu2RsrsMap(comVariant)                  ((comVariant)->PerTxPduIdMap)                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetPerTxPduIdInfo(comVariant)                    ((comVariant)->PerTxPduIdTable)                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetComCtrlChannelListAll(comVariant)             ((comVariant)->ComCtrlChannelListAll)                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetNetAllComMChannelMap(comVariant)              ((comVariant)->NetAllComMChannelMap)                                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetCfgGetCanTp2DcmRxPduIdMap(comVariant)               ((comVariant)->CanTp2DcmRxPduIdMap)                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_NetCfgGetNumRxPduIds(comVariant)                       DCM_NET_NUM_RX_PDUIDS
#  define Dcm_NetCfgGetNumTxPduIds(comVariant)                       DCM_NET_NUM_TX_PDUIDS
#  define Dcm_NetCfgGetMinCanTpRxPduId(comVariant)                   DCM_NET_CANTP_RX_PDUID_MIN
#  define Dcm_NetCfgGetMaxCanTpRxPduId(comVariant)                   DCM_NET_CANTP_RX_PDUID_MAX
#  define Dcm_NetCfgGetNumCanRxPduIds(comVariant)                    DCM_NET_NUM_CANTP_RX_PDUIDS
#  define Dcm_NetCfgGetNumPerTxObjects(comVariant)                   DCM_NET_MAX_NUM_PERIODIC_TX_OBJ
#  define Dcm_NetCfgGetNumConnections(comVariant)                    DCM_NET_MAX_NUM_CONNECTIONS
#  define Dcm_NetCfgGetNumProtocols(comVariant)                      DCM_NET_NUM_PROTOCOLS
#  define Dcm_NetCfgGetNumComMChannels(comVariant)                   DCM_NET_MAX_NUM_COMM_CHANNELS
#  define Dcm_NetCfgGetNumBuffers(comVariant)                        DCM_NET_NUM_BUFFERS
#  define Dcm_NetCfgGetNumTranspObjects(comVariant)                  DCM_NET_MAX_NUM_TRANSP_OBJECTS
#  define Dcm_NetCfgGetNumAllComMChannels(comVariant)                DCM_NET_MAX_NUM_ALL_COMM_CHANNELS

#  define Dcm_NetCfgGetRxPduInfo(comVariant)                         Dcm_CfgNetRxPduInfo
#  define Dcm_NetCfgGetConnectionInfo(comVariant)                    Dcm_CfgNetConnectionInfo
#  define Dcm_NetCfgGetProtocolInfo(comVariant)                      Dcm_CfgNetProtocolInfo
#  define Dcm_NetCfgGetNetConnComMChannelMap(comVariant)             Dcm_CfgNetConnComMChannelMap
#  define Dcm_NetCfgGetNetworkHdlLookUp(comVariant)                  Dcm_CfgNetNetworkHandleLookUpTable
#  define Dcm_NetCfgGetTxPdu2ConnMap(comVariant)                     Dcm_CfgNetTxPduInfo
#  define Dcm_NetCfgGetPerTxPdu2RsrsMap(comVariant)                  Dcm_CfgNetPeriodicTxPduInfo
#  define Dcm_NetCfgGetPerTxPduIdInfo(comVariant)                    Dcm_CfgNetPeriodicTxPduIds
#  define Dcm_NetCfgGetComCtrlChannelListAll(comVariant)             Dcm_CfgNetComCtrlChannelListAll
#  define Dcm_NetCfgGetNetAllComMChannelMap(comVariant)              Dcm_CfgNetAllComMChannelMap
#  define Dcm_NetCfgGetCanTp2DcmRxPduIdMap(comVariant)               Dcm_CfgNetCanTp2DcmRxPduIdMap
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
#  define Dcm_NetPeriodicMsgGetTxPduIdByConn(connHdl, txObjIdx)      (Dcm_PbCfgNetPerTxPduIdInfo[Dcm_PbCfgNetConnectionInfo[(connHdl)].PeriodicTxRef + (txObjIdx)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgGetNumMsgsFor(connHdl)                   (Dcm_NetGetNumPeriodicTxPduIds(connHdl))                                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetPeriodicMsgGetTxPduIdCurrConn(txObjIdx)             (Dcm_NetPeriodicMsgGetTxPduIdByConn(Dcm_NetPeriodicMsgGetConnection(),(txObjIdx))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetPeriodicMsgSupportedFor(connHdl)                    (Dcm_NetPeriodicMsgGetNumMsgsFor(connHdl) != 0u)                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetPeriodicMsgNumMsgsCurrConnection()                  (Dcm_NetPeriodicMsgGetNumMsgsFor(Dcm_NetPeriodicMsgGetConnection()))            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgGetHandleFromTxPduId(txPduId)            ((Dcm_CfgNetPTxObjHandleOptType)(Dcm_PbCfgNetPerTxPdu2RsrsMap[txPduId]))        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetPeriodicMsgGetNextMsgHdl()                          (Dcm_SingletonContext.Network.PeriodicTxContext.NextTxObjectHdl)                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgResetNextMsgHdl()                        (Dcm_SingletonContext.Network.PeriodicTxContext.NextTxObjectHdl = 0u)           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgIncNextMsgHdl()                          (++Dcm_SingletonContext.Network.PeriodicTxContext.NextTxObjectHdl)              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_NetPeriodicMsgGetTxObjectSentCntr()                    (Dcm_SingletonContext.Network.PeriodicTxContext.TxObjectSentCntr)               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgResetTxObjectSentCntr()                  (Dcm_SingletonContext.Network.PeriodicTxContext.TxObjectSentCntr = 0u)          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgIncTxObjectSentCntr()                    (++Dcm_SingletonContext.Network.PeriodicTxContext.TxObjectSentCntr)             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
#   define Dcm_NetPeriodicMsgGetDelayCntr()                          (Dcm_SingletonContext.Network.PeriodicTxContext.DelayCntr)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_NetPeriodicMsgIncDelayCntr()                          (++Dcm_SingletonContext.Network.PeriodicTxContext.DelayCntr)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_NetPeriodicMsgDecDelayCntr()                          (--Dcm_SingletonContext.Network.PeriodicTxContext.DelayCntr)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_NetPeriodicMsgInitDelayCntr()                         (Dcm_SingletonContext.Network.PeriodicTxContext.DelayCntr = DCM_NET_DELAY_BULK_TRANSMISSION) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#   define Dcm_NetPeriodicMsgResetNextDelayTimer()                   (Dcm_SingletonContext.Network.PeriodicTxContext.NextDelayTimer = 0u)            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_NetPeriodicMsgGetNextDelayTimer()                     (Dcm_SingletonContext.Network.PeriodicTxContext.NextDelayTimer)                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#   define Dcm_NetPeriodicMsgGetFastestRate()                        (Dcm_SingletonContext.Network.PeriodicTxContext.FastestRate)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_NetPeriodicMsgInitFastestRate()                       (Dcm_SingletonContext.Network.PeriodicTxContext.FastestRate = DCM_SVC2A_RATE_TYPE_SLOW) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#   define Dcm_NetGetPeriodicDelayTime(Rate)                         (Dcm_CfgNetPeriodicDelayTime[Rate])                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#   define Dcm_NetPeriodicMsgGetDelayCntr()                          0u
#   define Dcm_NetPeriodicMsgIncDelayCntr()                          /* not used */
#   define Dcm_NetPeriodicMsgDecDelayCntr()                          /* not used */
#   define Dcm_NetPeriodicMsgInitDelayCntr()                         /* not used */

#   define Dcm_NetPeriodicMsgGetFastestRate()                        DCM_SVC2A_RATE_TYPE_SLOW
#   define Dcm_NetPeriodicMsgInitFastestRate()                       /* not used */

#   define Dcm_NetGetPeriodicDelayTime(Rate)                         0u
#  endif

#  define Dcm_NetPTxObjTimerGet(pPeriodicTxObj)                      ((pPeriodicTxObj)->Timer)                                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPTxObjTimerExpired(pPeriodicTxObj)                  (Dcm_NetPTxObjTimerGet(pPeriodicTxObj) == 0u)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */

/*! Returns the diagnostic client connection ID from a DcmRxPduID */
# define Dcm_NetCfgGetConnHdlOfRxPduId(comVariant, rxPduId)          (Dcm_NetCfgGetRxPduInfo(comVariant)[(rxPduId)].ConnRef)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns the diagnostic client's network address from its connection ID */
# define Dcm_NetCfgGetTesterAddressOfConnHdl(comVariant, connHdl)    (Dcm_NetCfgGetConnectionInfo(comVariant)[(connHdl)].ClientSrcAddr)              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Provides the ECUs network address of a specific client connection */
# define Dcm_NetCfgGetEcuAddressOfConnHdl(comVariant, connHdl)       (Dcm_NetCfgGetConnectionInfo(comVariant)[(connHdl)].EcuAddress)                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
/*! Indicates whether a connection is generic */
#  define Dcm_NetCfgIsGenericConnection(comVariant, connHdl)         (Dcm_NetCfgGetConnectionInfo(comVariant)[(connHdl)].MetaDataLength == DCM_NET_METADATA_LENGTH) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

/*! Returns the diagnostic client's network address from one of its DcmRxPduIDs */
# define Dcm_NetCfgGetTesterAddressOfRxPduId(comVariant, rxPduId)    (Dcm_NetCfgGetTesterAddressOfConnHdl((comVariant), Dcm_NetCfgGetConnHdlOfRxPduId((comVariant), (rxPduId)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns the ECUs network address of a specific client connection from one of its DcmRxPduIDs */
# define Dcm_NetCfgGetEcuAddressOfRxPduId(comVariant, rxPduId)       (Dcm_NetCfgGetEcuAddressOfConnHdl((comVariant), Dcm_NetCfgGetConnHdlOfRxPduId((comVariant), (rxPduId)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef P2CONST(Dcm_CfgNetBufferInfoType, TYPEDEF, DCM_CONST_PBCFG) Dcm_PbCfgNetBufferInfoPtrType;

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/*! Periodic message transmitter control states */
struct DCM_NETPERIODICTXCONTEXTTYPE_TAG
{
  DCM_PB_RAM_DATA_DECL(Dcm_NetPeriodicTxObjectType, TxObject, DCM_NET_MAX_NUM_PERIODIC_TX_OBJ) /*!< Periodic message control objects */
#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
    volatile Dcm_TmrTimerCntrMemType DelayTimer[DCM_NET_DELAY_BULK_TRANSMISSION]; /*!< Array of delay timers that assure gap times between periodic DIDs */
#  endif
  uint16                           ClientSrcAddr;    /*!< Client source address. Required only when generic connection is used */
#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  Dcm_CfgNetPTxObjHandleMemType    DelayCntr;        /*!< Number of remaining messages that can be dispatched before a delay is required */
  uint8                            NextDelayTimer;   /*!< Index of next delay timer to use */
  uint8                            FastestRate;      /*!< The fastest rate in which periodic DIDs are currently scheduled */
#  endif
  Dcm_NetConnRefMemType            ConnHdlInUse;      /*!< Current owner (diagnostic client) of the periodic transmitter */
  Dcm_CfgNetPTxObjHandleMemType    NextTxObjectHdl;  /*!< Pointer to the next periodic message to be sent. Scope is only the current periodic transmitter owners list of periodic messages */
  Dcm_CfgNetPTxObjHandleMemType    TxObjectSentCntr; /*!< Keeps track of the already sent periodic messages. Scope is only the current periodic transmitter owners list of periodic messages */
};
typedef struct DCM_NETPERIODICTXCONTEXTTYPE_TAG Dcm_NetPeriodicTxContextType;

typedef P2VAR(Dcm_NetPeriodicTxObjectType, TYPEDEF, DCM_RAM_PBCFG) Dcm_NetPeriodicTxObjectPtrType;
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetTxBuffer()
 *********************************************************************************************************************/
/*! \brief          Return the tx buffer of the periodic message with the given index.
 *  \details        -
 *  \param[in]      perTxObjHandle    Handle of periodic transport object to be transmitted
 *  \return         Tx buffer of periodic transport object
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            The periodic message shall first be allocated via Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
FUNC(Dcm_MsgType, DCM_CODE) Dcm_NetPeriodicMsgGetTxBuffer(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetPeriodicTxObject()
 *********************************************************************************************************************/
/*! \brief          Returns a periodic transport object element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a periodic transport object
 *  \return         Periodic transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetPeriodicTxObjectPtrType, DCM_CODE) Dcm_NetGetPeriodicTxObject(
  Dcm_CfgNetPTxObjHandleOptType index
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
/*! \brief          Periodic message allocation.
 *  \details        Allocates a resource for a periodic message.
 *  \param[in]      Did     DID of the message that is allocated
 *  \return         DCM_NET_INVALID_PTXOBJ_HANDLE      No free resource found
 *  \return         < DCM_NET_INVALID_PTXOBJ_HANDLE    A concrete valid handle is returned
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPeriodicMsgAllocate(
  uint16 Did
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgRelease()
 *********************************************************************************************************************/
/*! \brief          Releases a concrete periodic message resource.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgRelease(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgResetConnection()
 *********************************************************************************************************************/
/*! \brief          Resets the periodic message connection.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgResetConnection(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgStopDid()
 *********************************************************************************************************************/
/*! \brief          Stops transmission of DID and also removes queued transport objects.
 *  \details        -
 *  \param[in]      Did      DID that is to be stopped
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgStopDid(
  uint16 Did
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmit()
 *********************************************************************************************************************/
/*! \brief          Transmits a periodic message.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      length            The periodic response message length
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmit(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  PduLengthType length
  );
# endif

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetConnectionInfo()
 *********************************************************************************************************************/
/*! \brief          Gets the connection table entry.
 *  \details        -
 *  \return         Network connection table entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetConnectionInfoType, DCM_CODE) Dcm_PbCfgNetGetConnectionInfo(
  uint8_least index
  );

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetConnComMChannelMap()
 *********************************************************************************************************************/
/*! \brief          Gets the network handles table entry.
 *  \details        -
 *  \return         Network handles table entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_PbCfgNetGetConnComMChannelMap(
  uint8_least index
  );

# if (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_PbCfgNetGetComCtrlChannelListAll()
*********************************************************************************************************************/
/*! \brief         Gets the list of all ComM channels.
*  \details        -
*  \return         List of all ComM channels
*  \context        TASK
*  \reentrant      TRUE
*  \synchronous    TRUE
*  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_PbCfgNetGetComCtrlChannelListAll(
  uint8_least index
  );
# endif

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetNumAllComMChannels()
 *********************************************************************************************************************/
/*! \brief          Gets the number of channels from main connections and ComControl channels.
 *  \details        -
 *  \return         Number of channels
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(NetworkHandleType, DCM_CODE) Dcm_PbCfgNetGetNumAllComMChannels(void);

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PbRamNetGetComCtrlChannels()
 *********************************************************************************************************************/
/*! \brief          Gets the communication state [Rx/Tx][On/Off] of each ComM channel known to DCM.
 *  \details        -
 *  \return         Communication state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CommunicationModeType, DCM_CODE) Dcm_PbRamNetGetComCtrlChannels(
  uint8_least index
  );
# endif

# if (DCM_NET_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetSetComControlChannelState()
 *********************************************************************************************************************/
/*! \brief          Sets the state of a communication control channel of the given index.
 *  \details        -
 *  \param[in]      index    Index to the communication control channel
 *  \param[in]      value    New communication mode
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetSetComControlChannelState(
  uint8_least index,
  Dcm_CommunicationModeType value
  );
# endif

/**********************************************************************************************************************
*  Dcm_NetGetTransportObject()
*********************************************************************************************************************/
/*! \brief          Returns a transport object element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a transport object
 *  \return         Transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_NetGetTransportObject(
  Dcm_CfgNetTObjHandleOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetGetKeepAlive()
 *********************************************************************************************************************/
/*! \brief          Returns the keep alive guard.
 *  \details        -
 *  \return         Keep alive guard
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (boolean, DCM_CODE) Dcm_NetGetKeepAlive(void);

# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetActiveComVariant()
 *********************************************************************************************************************/
/*! \brief          Returns currently set variant.
 *  \details        Returns currently active communication variant.
 *  \return         Currently active communication variant
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (Dcm_CfgVarMgrComVariantType, DCM_CODE) Dcm_NetGetActiveComVariant(void);
# endif

# if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetSetRxAllowed()
 *********************************************************************************************************************/
/*! \brief          Sets the RxAllowed value.
 *  \details        -
 *  \param[in]      rxAllowed    Value to be set.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (void, DCM_CODE) Dcm_NetSetRxAllowed(
  boolean rxAllowed
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetInvalidConnHdl()
 *********************************************************************************************************************/
/*! \brief          Gets the value for an invalid connection handle.
 *  \details        -
 *  \return         Value of the invalid connection handle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetGetInvalidConnHdl(
  void
  );

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetAuthInfoRef()
 *********************************************************************************************************************/
/*! \brief          Gets the reference of the authentication info.
 *  \details        -
 *  \param[in]      connHdl    Connection handle.
 *  \return         Reference to the authentication info
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetGetAuthInfoRef(
  Dcm_NetConnRefMemType connHdl
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetSessionConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the session connection.
 *  \details        -
 *  \return         session connection
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetSessionConnection(
  void
  );

# if ((DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) && (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)) || \
     (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByConnectionId()
 *********************************************************************************************************************/
/*! \brief          Tries to find the corresponding connection of a connection handle.
 *  \details        -
 *  \param[in]      connectionId                 Connection identifier
 *  \return         < DCM_NET_INVALID_CONNHDL    Valid connection id found.
 *  \return         DCM_NET_INVALID_CONNHDL      No connection found for the source address.
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByConnectionId(
  uint16 connectionId
  );
# endif

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PbCfgNetAllComMChannelMapGetEntry()
 *********************************************************************************************************************/
/*! \brief          Sets the RxAllowed value.
 *  \details        -
 *  \param[in]      index    Index to a network handle
 *  \return         Network handle entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (NetworkHandleType, DCM_CODE) Dcm_PbCfgNetAllComMChannelMapGetEntry(
  Dcm_CfgNetNetIdRefOptType index
  );
# endif

# if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON) || \
     (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetHasRespPendOnBoot()
 *********************************************************************************************************************/
/*! \brief          Returns the HasRespPendOnBoot value.
 *  \details        -
 *  \param[in]      pContext   Pointer to the context
 *  \return         HasRespPendOnBoot value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (boolean, DCM_CODE) Dcm_NetGetHasRespPendOnBoot(
  Dcm_ContextPtrType pContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetProtObjOfTranspObj()
 *********************************************************************************************************************/
/*! \brief          Returns the client protocol value.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         Client protocol value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetProtocolInfoConstPtrType, DCM_CODE) Dcm_NetGetProtObjOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlOfRxPduId()
 *********************************************************************************************************************/
/*! \brief          Returns the diagnostic client connection ID of the rxPduID.
 *  \details        -
 *  \param[in]      rxPduId   The RxPDUID of the request
 *  \return         Client connection ID value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlOfRxPduId(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetGetNumConnections()
 *********************************************************************************************************************/
/*! \brief          Returns the maximum number of diagnostic client connections.
 *  \details        -
 *  \return         Maximum number of diagnostic client connections
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetNumConnections(
  void
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetNumPeriodicTxPduIds()
 *********************************************************************************************************************/
/*! \brief          Returns the number of periodic messages of the diagnostic client.
 *  \details        -
 *  \return         Number of periodic messages of the diagnostic client
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetGetNumPeriodicTxPduIds(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetNumPerTxObjects()
 *********************************************************************************************************************/
/*! \brief          Returns the maximum number of periodic transmission objects.
 *  \details        -
 *  \return         Maximum number of periodic transmission objects
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetGetNumPerTxObjects(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetConnection()
 *********************************************************************************************************************/
/*! \brief         Returns the current owner (diagnostic client) of the periodic transmitter.
 *  \details        -
 *  \return         Current owner (diagnostic client) of the periodic transmitter
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPeriodicMsgGetConnection(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetConnection()
 *********************************************************************************************************************/
/*! \brief          Set the current owner (diagnostic client) of the periodic transmitter.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetConnection(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Returns the diagnostic client's unique source address (identifier).
 *  \details        -
 *  \return         Client's unique source address (identifier)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetPeriodicMsgGetClientSrcAddr(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Set the diagnostic client's unique source address (identifier).
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetClientSrcAddr(
  uint16 clientSrcAddr
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetBufferInfo()
 *********************************************************************************************************************/
/*! \brief          Returns a buffer information element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a buffer information element
 *  \return         Buffer info object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_PbCfgNetBufferInfoPtrType, DCM_CODE) Dcm_NetGetBufferInfo(
  Dcm_CfgNetBufferRefOptType index
  );

# if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetRxIndInternal()
 *********************************************************************************************************************/
/*! \brief          Provides a means for virtual/internal diagnostic request.
 *  \details        This API is generally used for RoE (SID 0x86) purposes.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      sourceAddress      Client or tester address
 *  \param[in]      data               The data of the request
 *  \param[in]      length             The length of the request
 *  \return         DCM_E_OK           Reception was successful
 *  \return         DCM_E_NOT_OK       Reception was not successful
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_NetRxIndInternal(
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,
  PduLengthType length
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetComState()
 *********************************************************************************************************************/
/*! \brief          Returns the communication state for the given RxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The RxPDUID of the request
 *  \return         The communication state
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComState(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageLeave()
 *********************************************************************************************************************/
/*! \brief          Decrements the buffer usage counter.
 *  \details        -
 *  \param[in]      bufferHdl    buffer handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetBufferUsageLeave(
  Dcm_CfgNetBufferRefMemType bufferHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageEnter()
 *********************************************************************************************************************/
/*! \brief          Increments the buffer usage counter.
 *  \details        -
 *  \param[in]      bufferHdl    buffer handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetBufferUsageEnter(
  Dcm_CfgNetBufferRefMemType bufferHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetGetProtObjOfConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol info of the connection handle.
 *  \details        -
 *  \param[in]      connHdl    connection handle
 *  \return         Client protocol value
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetProtocolInfoConstPtrType, DCM_CODE) Dcm_NetGetProtObjOfConnection(
  Dcm_NetConnRefMemType connHdl
  );

# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetFastestRate()
 *********************************************************************************************************************/
/*! \brief          Sets a new fastest rate.
 *  \details        -
 *  \param[in]      Rate    The periodic rate
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetFastestRate(
  Dcm_Svc2ASchedRateOptType Rate
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_NET_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Net.h
 *********************************************************************************************************************/
