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
/**        \file  Dcm.c
 *         \unit  NoUnit
 *        \brief  Implementation of DCM core
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
#define DCM_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_UNSUPPORTED XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm.h"
#include "Rte_Dcm.h"
#include "Dcm_Tsk.h"
#include "Dcm_Tmr.h"
#include "Dcm_AuthMgr.h"
#if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
# include "NvM.h"
#endif /* (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON) */
#include "PduR_Dcm.h"
#include "ComM_Dcm.h"
#if ((DCM_DIAG_NOTIF_BSWM_APPL_UPDATED_ENABLED == STD_ON) || \
    (DCM_SVC_28_SUPPORT_ENABLED == STD_ON))
# include "BswM_Dcm.h"
#endif /* ((DCM_DIAG_NOTIF_BSWM_APPL_UPDATED_ENABLED == STD_ON) || \
 (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)) */
#include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if ((DCM_SW_MAJOR_VERSION != 18u) || \
    (DCM_SW_MINOR_VERSION !=  5u) || \
    (DCM_SW_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm.h are inconsistent"
#endif
#if ((DCM_CFG_MAJOR_VERSION != 18u) || \
    (DCM_CFG_MINOR_VERSION !=  5u) || \
    (DCM_CFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_Cfg.h are inconsistent"
#endif
#if ((DCM_LCFG_MAJOR_VERSION != 18u) || \
    (DCM_LCFG_MINOR_VERSION !=  5u) || \
    (DCM_LCFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_Lcfg.h are inconsistent"
#endif
#if ((DCM_PBCFG_MAJOR_VERSION != 18u) || \
    (DCM_PBCFG_MINOR_VERSION !=  5u) || \
    (DCM_PBCFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_PBcfg.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
# define DCM_NET_CONN_LOCK_ENABLED                                   STD_ON
#else
# define DCM_NET_CONN_LOCK_ENABLED                                   STD_OFF
#endif
/*! Specifies whether at least one diagnostic service needs global data storage.
    Extend this list if needed when adding a new diagnostic service with post-request memory */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# define DCM_DIAG_SVC_CONTEXT_ENABLED                                STD_ON
#else
# define DCM_DIAG_SVC_CONTEXT_ENABLED                                STD_OFF
#endif

/*! Specifies whether the sequence check for services with sub-functions is active */
#if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON) || \
    (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
# define DCM_DIAG_SF_SEQUENCE_CHECK_SUPPORT_ENABLED                  STD_ON
#else
# define DCM_DIAG_SF_SEQUENCE_CHECK_SUPPORT_ENABLED                  STD_OFF
#endif

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) || \
    (DCM_DIAG_SVC_CONTEXT_ENABLED == STD_ON) || \
    (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON) || \
    ((DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)) || \
    (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define DCM_DIAG_CONTEXT_ENABLED                                    STD_ON
#else
# define DCM_DIAG_CONTEXT_ENABLED                                    STD_OFF
#endif
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
#  define DCM_SVC_2C_RACE_CONDITION_READ_ENABLED                     STD_ON
# else
#  define DCM_SVC_2C_RACE_CONDITION_READ_ENABLED                     STD_OFF
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
#  error "Service 0x2C is not allowed when any paged DID is configured!"
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */

/*! ComM notification allowance states */
#define DCM_NET_COMM_ACTIVE                                          ((Dcm_NetActiveDiagnosticType)TRUE)  /*!< Do notify ComM for ActiveDiagnostics (default state) */
#define DCM_NET_COMM_NOT_ACTIVE                                      ((Dcm_NetActiveDiagnosticType)FALSE) /*!< Do NOT notify ComM for ActiveDiagnostics (sleep prevention by DCM is inhibited by application) */

/*! Network variant/non-variant configuration abstraction */
#define Dcm_PbCfgNetNumRxPduIds                                      (Dcm_NetCfgGetNumRxPduIds(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumTxPduIds                                      (Dcm_NetCfgGetNumTxPduIds(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetCanTpRxPduIdMin                                  (Dcm_NetCfgGetMinCanTpRxPduId(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetCanTpRxPduIdMax                                  (Dcm_NetCfgGetMaxCanTpRxPduId(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumCanRxPduIds                                   (Dcm_NetCfgGetNumCanRxPduIds(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumPerTxObjects                                  (Dcm_NetCfgGetNumPerTxObjects(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumConnections                                   (Dcm_NetCfgGetNumConnections(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumProtocols                                     (Dcm_NetCfgGetNumProtocols(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumComMChannels                                  (Dcm_NetCfgGetNumComMChannels(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumBuffers                                       (Dcm_NetCfgGetNumBuffers(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumTransportObjects                              (Dcm_NetCfgGetNumTranspObjects(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNumAllComMChannels                               (Dcm_NetCfgGetNumAllComMChannels(Dcm_SingletonContext.Network.ComVariant))

#define Dcm_PbCfgNetRxPduInfo                                        (Dcm_NetCfgGetRxPduInfo(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetConnectionInfo                                   (Dcm_NetCfgGetConnectionInfo(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetProtocolInfo                                     (Dcm_NetCfgGetProtocolInfo(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetConnComMChannelMap                               (Dcm_NetCfgGetNetConnComMChannelMap(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetNetworkHdlLookUp                                 (Dcm_NetCfgGetNetworkHdlLookUp(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetTxPdu2ConnMap                                    (Dcm_NetCfgGetTxPdu2ConnMap(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetPerTxPdu2RsrsMap                                 (Dcm_NetCfgGetPerTxPdu2RsrsMap(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetPerTxPduIdInfo                                   (Dcm_NetCfgGetPerTxPduIdInfo(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetComCtrlChannelListAll                            (Dcm_NetCfgGetComCtrlChannelListAll(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetAllComMChannelMap                                (Dcm_NetCfgGetNetAllComMChannelMap(Dcm_SingletonContext.Network.ComVariant))
#define Dcm_PbCfgNetCanTp2DcmRxPduIdMap                              (Dcm_NetCfgGetCanTp2DcmRxPduIdMap(Dcm_SingletonContext.Network.ComVariant))

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define Dcm_PbRamNetBufferContext                                   (Dcm_SingletonContext.Network.RamVariant.BufferContexts)
# define Dcm_PbRamNetTransportObject                                 (Dcm_SingletonContext.Network.RamVariant.TranspObjects)
# define Dcm_PbRamNetComMContext                                     (Dcm_SingletonContext.Network.RamVariant.ComMContexts)
# define Dcm_PbRamNetPeriodicTxObject                                (Dcm_SingletonContext.Network.RamVariant.PeriodicTxObjects)
# define Dcm_PbRamNetConnHdl2TObjMap                                 (Dcm_SingletonContext.Network.RamVariant.ConnHdl2TObjIdMap)
# define Dcm_PbRamNetComCtrlChannels                                 (Dcm_SingletonContext.Network.RamVariant.ComCtrlChannels)
#else
# define Dcm_PbRamNetBufferContext                                   (Dcm_SingletonContext.Network.BufferContext)
# define Dcm_PbRamNetTransportObject                                 (Dcm_SingletonContext.Network.TransportObject)
# define Dcm_PbRamNetComMContext                                     (Dcm_SingletonContext.Network.ComMContext)
# define Dcm_PbRamNetPeriodicTxObject                                (Dcm_SingletonContext.Network.PeriodicTxContext.TxObject)
# define Dcm_PbRamNetConnHdl2TObjMap                                 (Dcm_SingletonContext.Network.ConnHdl2TObjIdMap)
# define Dcm_PbRamNetComCtrlChannels                                 (Dcm_SingletonContext.Network.ComCtrlChannels)
#endif

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define Dcm_PbCfgNetBufferInfo                                      (Dcm_SingletonContext.Network.RomVariant.BufferInfo)
#else
# define Dcm_PbCfgNetBufferInfo                                      Dcm_CfgNetBufferInfo
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/*! Converts a Dcm related CanTp PduID to its corresponding DcmRxPduId */
# define Dcm_NetGetDcmRxPduIdOfCanTpPduId(canTpPduId)                (Dcm_PbCfgNetCanTp2DcmRxPduIdMap[(canTpPduId)-Dcm_PbCfgNetCanTpRxPduIdMin])     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define Dcm_NetGetDcmRxPduIdOfCanTpPduId(canTpPduId)                /* not used */
#endif

/*! Retrieves diagnostic protocol specific P2 time justification */
#define Dcm_NetGetP2AdjTime(protocolIdx)                             (Dcm_NetGetProtObjOfProtId(protocolIdx)->SrvAdjTime.P2)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Retrieves diagnostic protocol specific P2* time justification */
#define Dcm_NetGetP2StarAdjTime(protocolIdx)                         (Dcm_NetGetProtObjOfProtId(protocolIdx)->SrvAdjTime.P2Star)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! USDT transport object states */
#define DCM_NET_TOBJ_STATE_FREE                                      ((Dcm_NetTransportObjectStateType)0x00u)  /*!< Free to be used */
#define DCM_NET_TOBJ_STATE_ONRX                                      ((Dcm_NetTransportObjectStateType)0x01u)  /*!< USDT reception in progress */
#define DCM_NET_TOBJ_STATE_RX_END                                    ((Dcm_NetTransportObjectStateType)0x02u)  /*!< USDT reception already finished */
#define DCM_NET_TOBJ_STATE_PREPTX                                    ((Dcm_NetTransportObjectStateType)0x04u)  /*!< Ready for USDT transmission */
#define DCM_NET_TOBJ_STATE_ONTX                                      ((Dcm_NetTransportObjectStateType)0x08u)  /*!< USDT transmission in progress */
#define DCM_NET_TOBJ_STATE_READY                                     ((Dcm_NetTransportObjectStateType)0x10u)  /*!< Transmission (Rx/Tx) finished but transport object still in use */
#define DCM_NET_TOBJ_STATE_RESERVED                                  ((Dcm_NetTransportObjectStateType)0x20u)  /*!< Transport object is allocated, but not yet in use (transmission) */

/*! USDT transport object flags */
#define DCM_NET_TOBJ_FLAG_NONE                                       ((Dcm_NetTransportObjectFlagType)0x0000u)  /*!< No flags set */
#define DCM_NET_TOBJ_FLAG_INTERNAL                                   ((Dcm_NetTransportObjectFlagType)0x0001u)  /*!< The transport object is used for internal (virtual) request (e.g. RoE) */
#define DCM_NET_TOBJ_FLAG_COPYHEAD                                   ((Dcm_NetTransportObjectFlagType)0x0002u)  /*!< The diagnostic request head shall be copied into a temporary buffer */
#define DCM_NET_TOBJ_FLAG_BUSY                                       ((Dcm_NetTransportObjectFlagType)0x0004u)  /*!< Current diagnostic request reception is a pseudo-parallel one */
#define DCM_NET_TOBJ_FLAG_CANCELED                                   ((Dcm_NetTransportObjectFlagType)0x0008u)  /*!< Current message transmission (Rx/Tx) was canceled */
#define DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ                               ((Dcm_NetTransportObjectFlagType)0x0010u)  /*!< RCR-RP state: just requested to be sent */
#define DCM_NET_TOBJ_FLAG_RCRRP_ON_TX                                ((Dcm_NetTransportObjectFlagType)0x0020u)  /*!< RCR-RP state: currently on transmission */
#define DCM_NET_TOBJ_FLAG_RCRRP_SENT                                 ((Dcm_NetTransportObjectFlagType)0x0040u)  /*!< RCR-RP state: at least once has been sent */
#define DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL                            ((Dcm_NetTransportObjectFlagType)0x0080u)  /*!< RCR-RP state: current one was triggered by the application */
#define DCM_NET_TOBJ_FLAG_OBSOLETE                                   ((Dcm_NetTransportObjectFlagType)0x0100u)  /*!< Transport object is obsolete and shall be canceled at later time. The difference to FLAG_CANCELED is that the ongoing transmission is not affected */
#define DCM_NET_TOBJ_FLAG_IGNORE                                     ((Dcm_NetTransportObjectFlagType)0x0200u)  /*!< Transport object shall not restart S3 and Keep-Alive Timers */

/*! USDT transport object response types */
#define DCM_NET_TOBJ_RESTYPE_NONE                                    ((Dcm_NetResponseType)0u)  /*!< No response is ongoing */
#define DCM_NET_TOBJ_RESTYPE_LINEAR                                  ((Dcm_NetResponseType)1u)  /*!< Final response (linear buffer) */
#define DCM_NET_TOBJ_RESTYPE_PAGED                                   ((Dcm_NetResponseType)2u)  /*!< Final response (paged-buffer) */
#define DCM_NET_TOBJ_RESTYPE_RCRRP                                   ((Dcm_NetResponseType)3u)  /*!< RCR-RP response */
#define DCM_NET_TOBJ_RESTYPE_UNSOLICITED                             ((Dcm_NetResponseType)4u)  /*!< Unsolicited (response without request) (e.g. FBL final response after power on/reset) */
#define DCM_NET_TOBJ_RESTYPE_SIMPLE                                  ((Dcm_NetResponseType)5u)  /*!< Simply respond (without processing) e.g. final negative response NRC 0x21 for pseudo parallel client handling or protocol switch rejection */

/*! Diagnostic client USDT connection properties */
#define DCM_NET_CONN_PROP_NONE                                       ((Dcm_CfgNetConnPropertiesType)0x00u)  /*!< Nothing special */
#define DCM_NET_CONN_PROP_TX_ON_FUNC_RX                              ((Dcm_CfgNetConnPropertiesType)0x01u)  /*!< No response to be sent on functional requests over this connection */
#define DCM_NET_CONN_PROP_NO_MAIN_TX                                 ((Dcm_CfgNetConnPropertiesType)0x02u)  /*!< The USDT connection does not contain a PduRTxPduId */

/*! State definitions for periodic transport object */
#define DCM_NET_PERIODIC_TX_STATE_FREE                               ((Dcm_NetPerTxObjStateMemType)0)
#define DCM_NET_PERIODIC_TX_STATE_RESERVED                           ((Dcm_NetPerTxObjStateMemType)1)
#define DCM_NET_PERIODIC_TX_STATE_QUEUED                             ((Dcm_NetPerTxObjStateMemType)2)
#define DCM_NET_PERIODIC_TX_STATE_ONTX                               ((Dcm_NetPerTxObjStateMemType)3)

/*! AR 4.1.2+ PduR API argument values adaption */
#define DCM_NET_ARENV_NTFRSLT_OK                                     E_OK
#define DCM_NET_ARENV_NTFRSLT_NOT_OK                                 E_NOT_OK
/*! Network to diagnostic variant bridge abstraction */
#if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
# define Dcm_PbCfgDiagSvcId2ProtMap                                  (Dcm_SingletonContext.Diag.Svc2ProtocolMap)
#else
# define Dcm_PbCfgDiagSvcId2ProtMap                                  Dcm_CfgDiagSvcId2ProtMap
#endif

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
# define Dcm_PbCfgDiagVariantFilter                                  (Dcm_SingletonContext.Diag.RomVariant.VariantFilter)
#else
# define Dcm_PbCfgDiagVariantFilter                                  Dcm_CfgVarMgrVariantTable
#endif
/*! Service 0x10 processing steps */
#define DCM_SVC10_PROGRESS_CHECK_ACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
#define DCM_SVC10_PROGRESS_TRIGGER_RESET                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
#define DCM_SVC10_PROGRESS_WAIT_RESET_ACK                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
#define DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
#define DCM_SVC10_PROGRESS_SET_PRGCOND                               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# define DCM_SVC2C_INVALID_DYNDID_HDL                                (Dcm_UtiMaxValueOfUintType(Dcm_CfgDidMgrDynDidHandleMemType))

# define DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED                        0u

# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON) && \
     (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_COND_ENABLED == STD_ON)
#  define DCM_SVC_2C_NUM_PROCESS_CONTEXTS                            2u
#  define DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY                     1u
# else
#  define DCM_SVC_2C_NUM_PROCESS_CONTEXTS                            1u
#  define DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY                     0u
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# define DCM_SVC2C_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP                          ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
# define DCM_SVC2C_PROGRESS_03_DIDLOOKUP                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
# define DCM_SVC86_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! USDT buffer management */
#define Dcm_NetBufferIsFree(bufferHdl)                               (Dcm_NetGetBufferContext((bufferHdl))->IsInUseCnt == 0u)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetBufferLock(bufferHdl)                                 (Dcm_NetBufferUsageEnter((bufferHdl)))                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetBufferRelease(bufferHdl)                              (Dcm_NetBufferUsageLeave((bufferHdl)))                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#if(DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
# define Dcm_NetIsGenericConnection(connHdl)                         (Dcm_NetCfgIsGenericConnection(Dcm_SingletonContext.Network.ComVariant, (connHdl))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif
#define Dcm_NetGetNodeAddress(connHdl)                               (Dcm_NetCfgGetEcuAddressOfConnHdl(Dcm_SingletonContext.Network.ComVariant, (connHdl))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetTesterAddress(connHdl)                             (Dcm_NetCfgGetTesterAddressOfConnHdl(Dcm_SingletonContext.Network.ComVariant, (connHdl))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetConnHdlOfTxPduId(txPduId)                          (Dcm_PbCfgNetTxPdu2ConnMap[(txPduId)])                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetProtIdOfActiveProtocol()                           (Dcm_SingletonContext.Network.ActiveProtocol)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfConnection(connHdl)                        (Dcm_PbCfgNetConnectionInfo[(connHdl)].ProtRef)                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfSessionConnection()                        (Dcm_NetGetProtIdOfConnection(Dcm_NetGetSessionConnection()))                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfTranspObj(pTranspObj)                      (Dcm_NetGetProtIdOfConnection((pTranspObj)->ConnHdl))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfMsgContext(pMsgContext)                    (Dcm_NetGetProtIdOfConnection(Dcm_NetGetConnHdlOfRxPduId((pMsgContext)->rxPduId))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetAuthInfoRefByMsgContext(pMsgContext)               (Dcm_PbCfgNetConnectionInfo[Dcm_NetGetConnHdlOfRxPduId((pMsgContext)->rxPduId)].AuthInfoRef) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetProtObjOfProtId(protId)                            (&Dcm_PbCfgNetProtocolInfo[(protId)])                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetThreadIdOfActiveProtocol()                         (Dcm_NetGetProtObjOfProtId(Dcm_NetGetProtIdOfActiveProtocol())->ThreadId)       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetThreadIdOfTranspObj(pTranspObj)                    (Dcm_NetGetProtObjOfTranspObj((pTranspObj))->ThreadId)                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
# define Dcm_NetGetThreadIdOfSessionConnection()                     (Dcm_NetGetProtObjOfConnection(Dcm_NetGetSessionConnection())->ThreadId)        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
/*! During prioritization in default session, the session connection is not initialized, so map to DCM_THREAD_ID_BASIC here directly */
# define Dcm_NetGetThreadIdOfSessionConnection()                     (DCM_THREAD_ID_BASIC)                                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif

#define Dcm_NetSetSessionConnection(connHdl)                         (Dcm_SingletonContext.Network.SessionConnection = (connHdl))                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetSessionClientSrcAddr()                             (Dcm_SingletonContext.Network.SessionClientSrcAddr)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetSetSessionClientSrcAddr(clientSrcAddress)             (Dcm_SingletonContext.Network.SessionClientSrcAddr = (clientSrcAddress))        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetComStateByChannelId(channelId)                     (Dcm_PbRamNetComMContext[(channelId)].ComState)                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComStateByConnection(connHdl)                      (Dcm_NetGetComStateByChannelId(Dcm_PbCfgNetConnectionInfo[(connHdl)].NetworkIdRef)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComStateByRxPduId(rxPduId)                         (Dcm_NetGetComStateByConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId)))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComStateByTranspObject(pTranspObj)                 (Dcm_NetGetComStateByConnection((pTranspObj)->ConnHdl))                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define Dcm_NetGetProperties(pTranspObj)                             (Dcm_PbCfgNetConnectionInfo[(pTranspObj)->ConnHdl].Properties)                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetIsPropertieSet(pTranspObj, prop)                      (Dcm_UtiBitOpTest(Dcm_CfgNetConnPropertiesType, Dcm_NetGetProperties(pTranspObj), (prop))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Abstract interface of getter of the transport object carrying the diagnostic request to be processed at next Dcm_DiagTaskWorker() activation */
/*! Get/Set the queued transport object */
#define Dcm_DiagGetQueuedTranspObj(pContext)                         (Dcm_NetGetTransportObject(pContext->Diag.QueuedSet.TObjHdl))                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Abstract interface of the RCR-RP limiter counter management */
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/*! Reset the RCR-RP limiter */
# define Dcm_DiagResetRcrRpLimiter(pContext)                         (pContext->Diag.NumRcrRpTimeouts = DCM_DIAG_RCRRP_LIMIT_COUNT)                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Get current RCR-RP limiter value */
# define Dcm_DiagGetRcrRpLimiter(pContext)                           (pContext->Diag.NumRcrRpTimeouts)                                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Decrement by one RCR-RP limiter value */
# define Dcm_DiagDecrementRcrRpLimiter(pContext)                     (--(pContext->Diag.NumRcrRpTimeouts))                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define Dcm_DiagResetRcrRpLimiter(pContext)                         /* not used */
# define Dcm_DiagGetRcrRpLimiter(pContext)                           /* not used */
# define Dcm_DiagDecrementRcrRpLimiter(pContext)                     /* not used */
#endif
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# define Dcm_Svc2CIsDidSrcItem(srcItemIdx)                           (Dcm_UtiGenericBitSetTestBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc2CSetDidSrcItem(srcItemIdx)                          (Dcm_UtiGenericBitSetSetBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc2CSetMemSrcItem(srcItemIdx)                          (Dcm_UtiGenericBitSetClrBit(Dcm_Svc2CNvMData.Sequencer,(srcItemIdx)))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define Dcm_Svc2CGetSrcItemIndex(dynDidHandle, offset)              ((Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle) + (offset))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# define Dcm_Svc2CIsDynDidDefined(dynDidHandle)                      (Dcm_Svc2CGetItem(dynDidHandle)->Count != 0u)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc2CGetNetBufferSize(pContext)                         ((Dcm_NetGetBufferInfo(Dcm_DiagGetTranspObj(pContext)->BufferHdl)->Size))       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef boolean Dcm_NetActiveDiagnosticType;

typedef P2VAR(Dcm_NetComMContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_NetComMContextPtrType;
typedef P2VAR(Dcm_NetBufferContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_NetBufferContextPtrType;

/*! Control states of the network sub-component */
struct DCM_NETCONTEXTTYPE_TAG
{
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_CfgVarMgrComVariantType   ComVariant;                                                           /*!< Points to the COM-criteria */
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
  Dcm_CfgNetRamVariantInfoType  RamVariant;                                                           /*!< Relocatable RAM for the network sub-component */
  Dcm_CfgNetRomVariantInfoType  RomVariant;                                                           /*!< Relocatable ROM for the network sub-component */
#endif
  DCM_PB_RAM_DATA_DECL(Dcm_NetBufferContextType, BufferContext, DCM_NET_NUM_BUFFERS)                  /*!< Control states of the USDT message buffers (main and additional) */
  DCM_PB_RAM_DATA_DECL(Dcm_NetTransportObjectType, TransportObject, DCM_NET_MAX_NUM_TRANSP_OBJECTS)   /*!< Transport objects for simultaneous servicing of multiple clients */
  DCM_PB_RAM_DATA_DECL(Dcm_NetComMContextType, ComMContext, DCM_NET_MAX_NUM_COMM_CHANNELS)            /*!< Control states of ComM status notifications per DCM related com-channel (i.e. channel on which DCM communicates with a client */
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  Dcm_NetPeriodicTxContextType  PeriodicTxContext;                                                    /*!< Composition relation to the periodic message transmitter */
#endif
  DCM_PB_RAM_DATA_DECL(Dcm_CfgNetTObjHandleMemType, ConnHdl2TObjIdMap, DCM_NET_MAX_NUM_CONNECTIONS)   /*!< A map of type <key = connectionHdl, value = TransportObject>. Used for fast transport object allocation */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
  DCM_PB_RAM_DATA_DECL(Dcm_CommunicationModeType, ComCtrlChannels, DCM_NET_MAX_NUM_ALL_COMM_CHANNELS) /*!< Communication state [Rx/Tx][On/Off] of each ComM channel known to DCM */
#endif
  Dcm_NetProtRefMemType         ActiveProtocol;                                                       /*!< The protocol handle last activated */
  Dcm_NetConnRefMemType         NumActiveConnections;                                                 /*!< Current number of simultaneously serviced clients/connections */
  Dcm_NetConnRefMemType         SessionConnection;                                                    /*!< Owner (client) of the non-default session last started */
  uint16                        SessionClientSrcAddr;                                                 /*!< Source address of client which started the non-default session */
  Dcm_NetActiveDiagnosticType   ActiveDiagnostic;                                                     /*!< Guard for prevention of DCMs automatic ComM diag-activate signal notification (this state is managed by the application) */
#if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
  boolean                       RxAllowed;                                                            /*!< Guard for global blocking of any request reception */
#endif
  boolean                       KeepAlive;                                                            /*!< Guard for prevention of diag-inactive notifications from DCM to ComM (e.g. in the default-session) */
};
typedef struct DCM_NETCONTEXTTYPE_TAG Dcm_NetContextType;
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/*! Context for the processing of a specific DynDID */
struct DCM_SVC2CDYNDIDPROCESSCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType         SrcDidContext;    /*!< Info context of the currently requested source DID */
  Dcm_DidMgrDidOpTypeContextType       DidOpTypeContext; /*!< Operation type context */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType ItemInProgress;   /*!< Index of the currently processed source item */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType ItemToStop;       /*!< Index of the item after the last source item */
};
typedef struct DCM_SVC2CDYNDIDPROCESSCONTEXTTYPE_TAG Dcm_Svc2CDynDidProcessContextType;

typedef P2VAR(Dcm_Svc2CDynDidProcessContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CDynDidProcessContextPtrType;

/*! Context for the handling of the read access to a DynDID */
struct DCM_SVC2CDYNDIDACCESSCONTEXT_TAG
{
  Dcm_Svc2CDynDidProcessContextType ProcessContext[DCM_SVC_2C_NUM_PROCESS_CONTEXTS]; /*!< Process contexts for DynDIDs */
  Dcm_CfgDidMgrDynDidHandleMemType  DynDidHandleInUse;                               /*!< Handle of currently processed DynDID */
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
  Dcm_MsgItemType                   Buffer[DCM_SVC_2C_READ_BUFFER_SIZE];             /*!< DynDID read buffer */
# endif
};
typedef struct DCM_SVC2CDYNDIDACCESSCONTEXT_TAG Dcm_Svc2CDynDidReadAccessContext;
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/*! Context for the handling of DynDIDs */
struct DCM_SVC2CCONTEXTTYPE_TAG
{
  Dcm_Svc2CDynDidReadAccessContext   DynDidAccessContext; /*!< Access context of the DynDID */
  Dcm_MsgLenType                     ReadIndex;           /*!< Number of bytes that are already read */
};
typedef struct DCM_SVC2CCONTEXTTYPE_TAG Dcm_Svc2CContextType;

typedef P2CONST(Dcm_CfgSvc2CSubFuncInfoType, TYPEDEF, DCM_CONST) Dcm_CfgSvc2CSubFuncInfoPtrType;
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/*! Control data of the unsolicited response at ECU start-up task */
struct DCM_STARTUPRESPONSECONTEXTTYPE_TAG
{
  Dcm_ProgConditionsType          ProgConditions;                             /*!< Dedicated programming condition storage in case a parallel request needs the repeater proxy pool */
  Dcm_MsgItemType                 ResBuffer[DCM_DIAG_START_UP_FBL_RES_SIZE];  /*!< Dedicated response buffer for the final response */
  boolean                         IsResponseRequired;                         /*!< Response-required information extracted from the pre-boot time context */
  Dcm_NetTransmissionResultType   TxStatus;                                   /*!< Transports the response transmission result to the Dcm_DiagTaskFblRes() */
  sint8                           SessStateRef;                               /*!< Diagnostic session index (values <0 mean no session transition). Use sint8 since no more than 127 sessions are possible */
  Dcm_NetConnRefMemType           ConnHdl;                                    /*!< Connection handle on which the unsolicited response was sent for post-processing purposes */
};
typedef struct DCM_STARTUPRESPONSECONTEXTTYPE_TAG Dcm_DiagStartUpResContextType;
#endif

#if (DCM_DIAG_SVC_CONTEXT_ENABLED == STD_ON)
/*! Storage of all diagnostic services contexts that shall be kept in memory not only during a diagnostic request but during the whole ECU operation cycle */
struct DCM_DIAGSERVICESCONTEXTTYPE_TAG
{
  Dcm_Svc2CContextType  Svc2C;  /*!< Storage place for all "DynamicallyDefineIdentifier" data */
};
typedef struct DCM_DIAGSERVICESCONTEXTTYPE_TAG Dcm_DiagServicesContextType;
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/*! Control data of the specific cause code */
struct DCM_CAUSECODECONTEXTTYPE_TAG
{
  Dcm_SpecificCauseCodeType CauseCode;      /*!< The specific cause code */
  boolean                   SendCauseCode;  /*!< Flag to send the CauseCode */
};
typedef struct DCM_CAUSECODECONTEXTTYPE_TAG Dcm_DiagCauseCodeContextType;
#endif

#if (DCM_DIAG_CONTEXT_ENABLED == STD_ON)
/*! Control intanced data of the sub-component diagnostic kernel */
struct DCM_DIAGCONTEXTTYPE_TAG
{
# if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  P2CONST(Dcm_CfgDiagSvc2ProtMapMemType, AUTOMATIC, DCM_CONST_PBCFG) Svc2ProtocolMap; /*!< Points to the "diagnostic service to protocol map" for a particual DCM variant (criterion): Dcm_CfgDiagSvcId2ProtMap[] */
# endif
# if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  Dcm_CfgDiagRomVariantInfoType  RomVariant;        /*!< Relocatable ROM for the diagnostic sub-component */
# endif
# if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  Dcm_DiagStartUpResContextType  StartUpFbl;        /*!< The global data of the unsolicited response task (Dcm_DiagTaskFblRes()) */
# endif
# if (DCM_DIAG_SVC_CONTEXT_ENABLED == STD_ON)
  Dcm_DiagServicesContextType    Services;          /*!< Diagnostic service processors "long term" memory */
# endif
# if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
  Dcm_DiagCauseCodeContextType   CauseCodeContext;  /*!< Context of the specific cause code */
# endif
};
typedef struct DCM_DIAGCONTEXTTYPE_TAG Dcm_DiagContextType;
#endif

/*! Data container for data shared among all DCM threads */
struct DCM_SINGLETONCONTEXTTYPE_TAG
{
#if (DCM_DIAG_CONTEXT_ENABLED == STD_ON)
  Dcm_DiagContextType        Diag;        /*!< Diagnostic kernel sub-component per-instance memory */
#endif
  Dcm_NetContextType         Network;     /*!< Network sub-component shared memory */
#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
  Dcm_RecoveryContextType    Recovery;    /*!< Recovery context containing the operation state and the recovery information */
#endif
};
typedef struct DCM_SINGLETONCONTEXTTYPE_TAG Dcm_SingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled()
 *********************************************************************************************************************/
/*! \brief          Returns DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED as boolean.
 *  \details        -
 *  \return         TRUE         DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED is STD_ON
 *  \return         FALSE        DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED is STD_OFF
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetIsRxPduIdValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received rxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The rxPduId of the request
 *  \return         TRUE       The rxPduId is valid
 *  \return         FALSE      The rxPduId is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsRxPduIdValid(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetIsTxPduIdValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received txPduId.
 *  \details        -
 *  \param[in]      txPduId    The txPduId of the request
 *  \return         TRUE       The txPduId is valid
 *  \return         FALSE      The txPduId is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsTxPduIdValid(
  PduIdType txPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetGetComMContext()
 *********************************************************************************************************************/
/*! \brief          Returns a ComM context element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a ComM context
 *  \return         ComM context object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_NetComMContextPtrType, DCM_CODE) Dcm_NetGetComMContext(
  Dcm_CfgNetNetIdRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetGetBufferContext()
 *********************************************************************************************************************/
/*! \brief          Returns a buffer context element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a buffer context
 *  \return         Buffer context object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_NetBufferContextPtrType, DCM_CODE) Dcm_NetGetBufferContext(
  Dcm_CfgNetBufferRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetPutToConn2TObjMap()
 *********************************************************************************************************************/
/*! \brief          Sets a transport object handle of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a transport object handle
 *  \param[in]      value    New handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPutToConn2TObjMap(
  Dcm_NetConnRefOptType index,
  Dcm_CfgNetTObjHandleMemType value
  );

/**********************************************************************************************************************
 *  Dcm_NetWriteAddBufferU8At()
 *********************************************************************************************************************/
/*! \brief          Writes into the additional buffer of a transport object.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \param[in]      index         Index to the additional buffer
 *  \param[in]      value         New value of the transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetWriteAddBufferU8At(
  Dcm_NetTransportObjectPtrType pTranspObj,
  uint8_least index,
  Dcm_MsgItemType value
  );

/**********************************************************************************************************************
 *  Dcm_NetCommonInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the common network related states.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetCommonInit(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetComMInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the ComM related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComMInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the buffer related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetBufferInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetConnectionInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the connection related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetConnectionInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetTranspObjInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the transport object related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTranspObjInit(
  void
  );

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the periodic message related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgInit(
  void
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the network sub-module.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetInit(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetRegisterActiveConnection()
 *********************************************************************************************************************/
/*! \brief          Registers an active connection.
 *  \details        Function will be called from Dcm_NetStartOfReception.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetOnRequestDetection()
 *********************************************************************************************************************/
/*! \brief          Cancels any pending job if detected another ECU to be requested from the same client.
 *  \details        Function will be called from Dcm_NetStartOfReception.
 *  \param[in]      connHdl        The connection ID of the foreign ECU request
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetOnRequestDetection(
  Dcm_NetConnRefMemType connHdl
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationFindWinner()
 *********************************************************************************************************************/
/*! \brief          Determination of request with highest priority.
 *  \details        -
 *  \param[in]      ev                      The active event(s) of the task Dcm_NetTaskRx
 *  \param[out]     pBusyEvents             Bitfield for busy requests
 *  \return         ID of the winner
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetTObjHandleOptType, DCM_CODE) Dcm_NetRxPrioritisationFindWinner(
  Dcm_TskTaskEvOptType ev,
  Dcm_TskTaskEvPtrType pBusyEvents
  );

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessWinner()
 *********************************************************************************************************************/
/*! \brief          Processing of the request with the highest priority.
 *  \details        -
 *  \param[in]      pContext      Pointer to the context
 *  \param[in]      pTranspObj    The transport object
 *  \param[in,out]  pBusyEvents   Bitfield for busy requests
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessWinner(
  Dcm_ContextPtrType pContext,
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_TskTaskEvPtrType pBusyEvents
  );

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessLoosers()
 *********************************************************************************************************************/
/*! \brief          Processing of requests that were preempted.
 *  \details        -
 *  \param[in]      BusyEvents             Bitfield for busy requests
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessLoosers(
  Dcm_TskTaskEvOptType BusyEvents
  );

/**********************************************************************************************************************
 *  Dcm_NetExecStartProtocolOp()
 *********************************************************************************************************************/
/*! \brief          Executes a protocol start up.
 *  \details        -
 *  \param[in]      newProtocol    The new protocol ID to be evaluated for starting
 *  \return         TRUE           Proceed with protocol starting
 *  \return         FALSE          Skip any further service processing after return
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetExecStartProtocolOp(
  Dcm_ProtocolType newProtocol
  );

/**********************************************************************************************************************
 *  Dcm_NetComposeAddNegResponse()
 *********************************************************************************************************************/
/*! \brief          Composes a negative response.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \param[in]      resType       The response type
 *  \param[in]      nrc           The negative response code
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComposeAddNegResponse(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetResponseType resType,
  Dcm_NegativeResponseCodeType nrc
  );

/**********************************************************************************************************************
 *  Dcm_NetProcessStartProtocol()
 *********************************************************************************************************************/
/*! \brief          Starts a new protocol.
 *  \details        Delegates a new request with a new protocol and terminates the old one.
 *  \param[in]      pTranspObj            The transport object
 *  \param[in]      stopActiveProtocol    Whether the active protocol shall be stopped or not
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetProcessStartProtocol(
  Dcm_NetTransportObjectPtrType pTranspObj,
  boolean stopActiveProtocol
  );

/**********************************************************************************************************************
 *  Dcm_NetTransmitUsdtResponse()
 *********************************************************************************************************************/
/*! \brief          Start transmission on this connection.
 *  \details        Start an USDT transmission over a specific connection.
 *  \param[in]      pTranspObj            The transport object to be used during the transmission.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            All the items of the structure must be validly initialized:
 *                  hBuffer               Shall be the TxBuffer handle
 *                  pduInfo.SduDataPtr    Shall point to the TxBuffer
 *                  pduInfo.SduLength     Shall contain the length of the data to be sent
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTransmitUsdtResponse(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

#if (DCM_NET_CONN_LOCK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetLockConnection()
 *********************************************************************************************************************/
/*! \brief          Allocates a transport object for a concrete client connection.
 *  \details        -
 *  \param[in]      connHdl            The connection to be locked
 *  \param[out]     pTranspObj         The allocated transport object
 *  \return         BUFREQ_OK          The operation was successful
 *  \return         BUFREQ_E_NOT_OK    The given index was invalid
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetLockConnection(
  Dcm_NetConnRefMemType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationNotOk()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception with no success.
 *  \details        This function is called by Dcm_NetTpRxIndication.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationNotOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationOk()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception with success.
 *  \details        This function is called by Dcm_NetTpRxIndication.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerGet()
 *********************************************************************************************************************/
/*! \brief          Get remaining delay time.
 *  \details        Returns the remaining delay time of the given timer
 *  \param[in]      timer       ID of the timer
 *  \return         Remaining delay time
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerCntrOptType, DCM_CODE) Dcm_NetDelayTimerGet(
  uint8_least timer
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerSet()
 *********************************************************************************************************************/
/*! \brief          Set delay timeout.
 *  \details        Sets the timout for the given delay timer
 *  \param[in]      timer       ID of the timer
 *  \param[in]      timeout     Timeout value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetDelayTimerSet(
  uint8_least timer,
  Dcm_TmrTimerCntrOptType timeout
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerExpired()
 *********************************************************************************************************************/
/*! \brief          Check if the delay timer timed out.
 *  \details        Returns whether the given delay timer timed out
 *  \param[in]      timer       ID of the timer
 *  \return         TRUE        Timer has expired
 *  \return         FALSE       Timer is running
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetDelayTimerExpired(
  uint8_least timer
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerSet()
 *********************************************************************************************************************/
/*! \brief          Set the timeout for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      timeout           Timout of the timer
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerSet(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStart()
 *********************************************************************************************************************/
/*! \brief          Start the timer for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      timeout           Timout of the timer
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStart(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStop()
 *********************************************************************************************************************/
/*! \brief          Stop the timer for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStop(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgIncNextDelayTimer()
 *********************************************************************************************************************/
/*! \brief          Set delay timer index to next timer.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgIncNextDelayTimer(
  void
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxDelayTimerStart()
 *********************************************************************************************************************/
/*! \brief          Starts a delay timer for periodic message transmission.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxDelayTimerStart(
  void
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutTxObjectHandler()
 *********************************************************************************************************************/
/*! \brief          Executes jobs on timeout of a transport object timer.
 *  \details        -
 *  \return         Flag whether timer needs to be reloaded
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutTxObjectHandler(
  void
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutDelayTimerHandler()
 *********************************************************************************************************************/
/*! \brief          Executes jobs on timeout of a delay timer.
 *  \details        -
 *  \return         Flag whether timer needs to be reloaded
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutDelayTimerHandler(
  void
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmitTxObj()
 *********************************************************************************************************************/
/*! \brief          Transmits a periodic message.
 *  \details        -
 *  \param[in]      perTxObjHandle    Handle of periodic transport object to be transmitted
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            The periodic message shall first be allocated via Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmitTxObj(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetIsPduInfoValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received PduInfo.
 *  \details        -
 *  \param[in]      info       Message context (data and length of the portion)
 *  \return         TRUE       The info element is valid
 *  \return         FALSE      The info element is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsPduInfoValid(
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyRxData()
 *********************************************************************************************************************/
/*! \brief          Copies request data from the lower layer.
 *  \details        This function is called by Dcm_CopyRxData.
 *  \param[in]      pTranspObj         The transport object
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyRxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  );

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception.
 *  \details        This function is called by Dcm_TpRxIndication.
 *  \param[in]      rxPduId    The RxPDUID of the request.
 *  \param[in]      result     Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyLinearTxData()
 *********************************************************************************************************************/
/*! \brief          Copies transmit data in linear form to the lower layer.
 *  \details        This function is called by Dcm_NetCopyTxData for the linear response data case.
 *  \param[in]      pTranspObj          The transport object
 *  \param[in]      info                Pointer to a PduInfoType, which indicates the number of bytes to be copied
 *                                      (SduLength) and the location where the data have to be copied to (SduDataPtr).
 *                                      An SduLength of 0 is possible in order to poll the available transmit data
 *                                      count. In this case no data are to be copied and SduDataPtr might be invalid.
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call
 *  \return         BUFREQ_OK           A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK     Some error occurred (i.e. internal error detected)
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyLinearTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies transmit data to the lower layer.
 *  \details        This function is called by Dcm_CopyTxData.
 *  \param[in]      pTranspObj          The transport object
 *  \param[in]      info                Pointer to a PduInfoType, which indicates the number of bytes to be copied
 *                                      (SduLength) and the location where the data have to be copied to (SduDataPtr).
 *                                      An SduLength of 0 is possible in order to poll the available transmit data
 *                                      count. In this case no data are to be copied and SduDataPtr might be invalid.
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call.
 *  \return         BUFREQ_OK           Response data is available - all requested data are copied
 *  \return         BUFREQ_E_BUSY       The required amount of data is not available - try later again
 *  \return         BUFREQ_E_NOT_OK     Some error occurred (e.g. no more data, internal error detected etc.)
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferInitByThread()
 *********************************************************************************************************************/
/*! \brief          Reset paged-buffer any time.
 *  \details        Resetting the paged-buffer.
 *  \param[in]      ThreadId    Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferInitByThread(
  Dcm_ThreadIdOptType threadId
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferInit()
 *********************************************************************************************************************/
/*! \brief          Initialize paged-buffer any time.
 *  \details        Initialization of the paged-buffer.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferInit(
  void
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferDefragmentPage()
 *********************************************************************************************************************/
/*! \brief          Defragments the current paged-buffer page.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferDefragmentPage(
  Dcm_ContextPtrType pContext
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferUpdatePage()
 *********************************************************************************************************************/
/*! \brief          Dispatches the UpdatePage call.
 *  \details        Dispatches the UpdatePage call regarding the paged-buffer.
 *  \param[in]      pContext  Pointer to the context
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_OK              All data has been copied - evaluate the out parameters
 *  \return         DCM_E_PENDING         No result yet, retry later
 *  \return         DCM_E_NOT_OK          Something went wrong, stop updating(only in DET situation)
 *  \return         DCM_E_BUFFERTOOLOW    Some data copied but no place, retry later on TP buffer under-run
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferUpdatePage(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferCopyData()
 *********************************************************************************************************************/
/*! \brief          Copy from paged-buffer to the TP layer.
 *  \details        -
 *  \param[in]      pTranspObj          The transport object
 *  \param[in]      info                Message context (data and length of the portion)
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call
 *  \return         BUFREQ_OK           A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK     No free buffer is available - ignore request
 *  \return         BUFREQ_E_BUSY       No free buffer at this time - try later again
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_PagedBufferCopyData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagSetNewReqBaseToCurProgress()
 *********************************************************************************************************************/
/*! \brief          Sets the request context base to the current progress.
 *  \details        Updates all request related members to an initial value, starting with buffer that skips the already
 *                  consumed request bytes.
 *  \param[in,out]  pMsgContext   Current message context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewReqBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetQueuedTranspObj()
 *********************************************************************************************************************/
/*! \brief          Setter of the transport object carrying the diagnostic request to be processed at next.
 *                  Dcm_DiagTaskWorker() activation
 *  \details        -
 *  \param[in]      pContext      Pointer to the context with all thread specific elements
 *  \param[in]      pTranspObj    Currently active DCM external/internal connection
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetQueuedTranspObj(
  Dcm_ContextPtrType pContext,
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetNewResBaseToCurProgress()
 *********************************************************************************************************************/
/*! \brief          Sets the response context base to the current progress.
 *  \details        Updates all response related members to an initial value, starting with buffer that skips the already
 *                  provided response bytes.
 *  \param[in,out]  pMsgContext   Current message context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewResBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessorFunc()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to a post-processor function of the given index.
 *  \details        -
 *  \param[in]      index       Unique handle to a post-processor function
 *  \return         Post-handler
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_DiagSvcConfirmationFuncType, DCM_CODE) Dcm_DiagGetPostProcessorFunc(
  uint8_least index
  );

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetUpdateFunc()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to an update function of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to an update function
 *  \return         Paged-buffer update-handler
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_DiagSvcUpdateFuncType, DCM_CODE) Dcm_DiagGetUpdateFunc(
  uint8_least index
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagInit()
 *********************************************************************************************************************/
/*! \brief          Initializes DCM diagnostic sub-module.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts must be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInit(
  Dcm_ConfigPtrType configPtr
  );

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagPutStartUpFblResBuffer()
 *********************************************************************************************************************/
/*! \brief          Writes into the global start up FBL response buffer.
 *  \details        -
 *  \param[in]      index         Index to the start up FBL response buffer
 *  \param[in]      value         New value of the start up FBL response buffer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagPutStartUpFblResBuffer(
  uint8_least index,
  Dcm_MsgItemType value
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagAcceptNewRequest()
 *********************************************************************************************************************/
/*! \brief          Checks whether a new request will be accepted.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM USDT connection
 *  \return         TRUE          New request is accepted
 *  \return         FALSE         New request is not accepted
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DiagAcceptNewRequest(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagOnTxFinished()
 *********************************************************************************************************************/
/*! \brief          Notifies the diagnostic sub-module that an ongoing transmission just finished.
 *  \details        Called by the TP once an ongoing transmission is finished.
 *  \param[in]      pTranspObj    Currently active DCM USDT connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagOnTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

/**********************************************************************************************************************
 *  Dcm_DiagExecConfirmationFunc()
 *********************************************************************************************************************/
/*! \brief          Executes a confirmation callback and evaluates its return values.
 *  \details        -
 *  \param[in]     pContext       Pointer to the context
 *  \param[in]      notifList     The diagnostic request confirmation list to be executed
 *  \param[in]      confStatus    The post-processing status
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            The last element of the passed notifList must be marked by NULL_PTR.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagExecConfirmationFunc(
  Dcm_ContextPtrType pContext,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  Dcm_ConfirmationStatusType confStatus
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessResult()
 *********************************************************************************************************************/
/*! \brief          Calculates the Dcm_ConfirmationType value for the post processors.
 *  \details        -
 *  \param[in]      pContext    Pointer to the context
 *  \return         The confirmation status
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_ConfirmationStatusType, DCM_CODE) Dcm_DiagGetPostProcessResult(
  Dcm_ContextPtrType pContext
  );

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoverySignature()
 *********************************************************************************************************************/
/*! \brief          Composes the recovery information control check-sum signature.
 *  \details        -
 *  \return         The control checksum
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_DiagGetRecoverySignature(
  void
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProvideRecoveryStates()
 *********************************************************************************************************************/
/*! \brief          Takes the restored DCM state over.
 *  \details        Takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProcessRecoveryInfo()
 *********************************************************************************************************************/
/*! \brief          Takes the restored DCM state over.
 *  \details        Takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      opStatus        Current operation status
 *  \context        TASK
 *  \return         DCM_E_OK        Processing succeeded
 *  \return         DCM_E_PENDING   Disable DTC API call is pending
 *  \return         DCM_E_NOT_OK    Disable DTC API call failed
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSafeProcessRecoveryInfo()
 *********************************************************************************************************************/
/*! \brief          Checks and takes the restored DCM state over.
 *  \details        Checks and takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      opStatus        Current operation status
 *  \context        TASK
 *  \return         DCM_E_OK        Processing succeeded
 *  \return         DCM_E_PENDING   Disable DTC API call is pending
 *  \return         DCM_E_NOT_OK    Processing failed
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSafeProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  );
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSetSpecificCauseCode()
 *********************************************************************************************************************/
/*! \brief          Registers a specific cause code.
 *  \details        Registers a specific cause code to the request in progress if no other specific cause code already
 *                  set.
 *  \param[in]      specificCauseCode    The specific cause code to be registered
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  );
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsProcessingContextSwitchAllowed()
 *********************************************************************************************************************/
/*! \brief          Checks whether a switch of the processing context is allowed.
 *  \details        A switch is only allowed, if no kill job event is set to any thread.
 *  \param[in]      pContext    Pointer to the context
 *  \return         DCM_E_OK         No kill job event in progress, switch allowed
 *  \return         DCM_E_NOT_OK     A kill job event was received for the own thread, switch not allowed
 *  \return         DCM_E_PENDING    A kill job event was received for another thread, try next cycle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsProcessingContextSwitchAllowed(
  Dcm_ContextPtrType pContext
  );
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsSessionChangeAllowed()
 *********************************************************************************************************************/
/*! \brief          Checks whether any other thread is currently busy with a parallel request.
 *  \details        -
 *  \param[in]      pContext    Pointer to the context
 *  \return         DCM_E_OK        ECU is idle, session change allowed
 *  \return         DCM_E_NOT_OK    Another thread is busy, session change is not allowed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsSessionChangeAllowed(
  Dcm_ContextPtrType pContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcCancelOpenJobs()
 *********************************************************************************************************************/
/*! \brief          Cancel open diagnostic jobs.
 *  \details        -
 *  \param[in]      pContext    Pointer to the context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcCancelOpenJobs(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcReviveKilledTasks()
 *********************************************************************************************************************/
/*! \brief          Revive tasks that were killed during cancellation.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcReviveKilledTasks(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcessing()
 *********************************************************************************************************************/
/*! \brief          Performs diagnostic job cancellation.
 *  \details        If a diagnostic job is ongoing the appropriate action will be taken to close it deterministically.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcessing(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoPostProcessing()
 *********************************************************************************************************************/
/*! \brief          Performs post processing of currently finished request.
 *  \details        -
 *  \param[in]      pContext        Pointer to the context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoPostProcessing(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetDefSessionExtern()
 *********************************************************************************************************************/
/*! \brief          Performs session transition to the default session on external request.
 *  \details        Synchronizes an external session change request with the internal DCM state. If a diagnostic job is
 *                  still in processing, the session change request will be postponed till the job is finished. This is
 *                  required in order to guarantee clean sequence flow on session change and active diagnostic jobs.
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetDefSessionExtern(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagSwitchProcessingContext()
 *********************************************************************************************************************/
/*! \brief          Performs a context switch of the current working serivice processing set.
 *  \details        -
 *  \param[in]      pContext    Pointer to the context
 *  \context        TASK
 *  \return         DCM_E_OK                Initialization succeeded, proceed with next step
 *  \return         DCM_E_NOT_OK            Initialization failed, repeat again next time
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSwitchProcessingContext(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagInitMsgContextBufferInfo()
 *********************************************************************************************************************/
/*! \brief          Initializes the given message context.
 *  \details        Initializes the given message context so that it points always to a valid buffer.
 *  \param[in,out]  pMsgContext   Current message context
 *  \param[in]      index         Unique handle to a buffer information element
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitMsgContextBufferInfo(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetBufferRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_DiagInitiateServiceProcessing()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic kernel for new request processing.
 *  \details        -
 *  \param[in]      pContext        Pointer to the context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *  \note           The parameter pMsgContext is passed to this function for initialization.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitiateServiceProcessing(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagHandleSuppressBit()
 *********************************************************************************************************************/
/*! \brief          Handles the SPRMIB if set and supported by the requested service.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pTranspObj     The transport object
 *  \param[in,out]  pMsgContext    Current request context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleSuppressBit(
  Dcm_ContextPtrType pContext,
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagHandleDispatching()
 *********************************************************************************************************************/
/*! \brief          Handles the service dispatching.
 *  \details        -
 *  \param[in]      sid            The SID of the request
 *  \param[in,out]  pMsgContext    Current request context
 *  \param[in]      pContext       Pointer to the context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleDispatching(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagValidateAndDispatchService()
 *********************************************************************************************************************/
/*! \brief          Validates received SID and dispatches further processing.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current request context
 *  \param[out]     ErrorCode               The error code to be returned
 *  \return         DCM_E_OK                Validation succeeded. Initiate recognized service processing
 *  \return         DCM_E_NOT_OK            Validation failed. Finish service processing with the ErrorCode result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Dcm_DiagInitiateServiceProcessing() shall be called first.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagValidateAndDispatchService(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessNewRequest()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service processing once a new request is received.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessNewRequest(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessRcrRpTxEnd()
 *********************************************************************************************************************/
/*! \brief          Prepares for application notification after enforced RCR-RP transmission.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessRcrRpTxEnd(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerCancelOperation()
 *********************************************************************************************************************/
/*! \brief          Cancels any ongoing diagnostic operation.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerCancelOperation(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Performs security level bypass.
 *  \details        Enables or disables a security bypass mode. If a diagnostic job is still in processing,
 *                  the security bypass mode will be postponed until the job is finished. This is required
 *                  in order to prevent security state inconsistencies.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetSecurityBypass(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoRepeat()
 *********************************************************************************************************************/
/*! \brief          Triggers a scheduled diagnostic job activity.
 *  \details        Each registered diagnostic job activity will be called out from this point.
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoRepeat(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoGetProgCond()
 *********************************************************************************************************************/
/*! \brief          Gets the programming conditions from FBL/Application.
 *  \details        Checks and takes the external stored and recovered DCM state over.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoGetProgCond(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoWaitTxComm()
 *********************************************************************************************************************/
/*! \brief          Waits until the ComM has processed the DCM communication request.
 *  \details        Waits for either P2Star timeout of ComM acknowledge of channel wake up.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoWaitTxComm(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoResetFlags()
 *********************************************************************************************************************/
/*! \brief          Resets all FBL flags.
 *  \details        Resets all FBL related flags so at next power-on/reset no final response to be sent.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoResetFlags(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoProcessTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Performs a session transition after a final positive response.
 *  \details        If the final response was for SID 0x10, the corresponding session transition is performed here.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoProcessTxConfirmation(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagTxPostponeFinalPagedResponse()
 *********************************************************************************************************************/
/*! \brief          Postpones the diagnostic service final response transmission if paged-buffer used.
 *  \details        The postponing is required only if the SPRMIB was set. Since the SPMRIB=TRUE means
 *                  "execute job, but do not send a response", in the case of paged-buffer the response is sent during
 *                  the "job execution". So if the P2 time is exceeded while performing the job, the final response may
 *                  not be able to provide all of the response data (the previous data pages are already lost)
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxPostponeFinalPagedResponse(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagTxStartFinalResponse()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service final response transmission.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartFinalResponse(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagTxStartRcrRpResponse()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service RCR-RP response transmission.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartRcrRpResponse(
  Dcm_ContextPtrType pContext
  );

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoComMChannelState()
*********************************************************************************************************************/
/*! \brief          Returns a pointer to a ComM channel state of the recovery info.
 *  \details        -
 *  \param[in]      RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      index           Index to a ComM channel state
 *  \return         A valid pointer to a ComM channel state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoComMChannelStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoComMChannelState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo,
  Dcm_CfgNetNetIdRefOptType index
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON && DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoCommControlState()
*********************************************************************************************************************/
/*! \brief          Returns a pointer to the communication state of a ComM channel of the recovery info.
 *  \details        -
 *  \param[in]      RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      index           Index to the communication state of a ComM channel
 *  \return         A valid pointer to the communication state of a ComM channel
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoCommControlStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoCommControlState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo,
  Dcm_CfgNetNetIdRefOptType index
  );
#endif
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_TriggerReset()
 *********************************************************************************************************************/
/*! \brief          Triggers an Ecu Reset.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_TriggerReset(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif


#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForResetAck()
 *********************************************************************************************************************/
/*! \brief          Wait for Ecu Reset acknowledgment.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForResetAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForRcrRpAck()
 *********************************************************************************************************************/
/*! \brief          Wait for RCR-RP acknowledgment.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_SetProgConditions()
 *********************************************************************************************************************/
/*! \brief          Sets programming conditions.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Trigger to stop repeater (programming conditions set)
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_Svc10Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x10 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send final positive response
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc10CheckAccess()
 *********************************************************************************************************************/
/*! \brief          Checks whether transistion to requested session can be performed.
 *  \details        If e.g. any OBD request is currently in progress the session change request has to be delayed.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send final positive response
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc10UndoPendingSession()
 *********************************************************************************************************************/
/*! \brief          Undo the pending session expectation by synchronizing it with the active session.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc10UndoPendingSession(
  void
  );
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrInit()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        Service 0x2C DID definition manager initializer.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadSrcItems()
 *********************************************************************************************************************/
/*! \brief          Performs reading on a single DynDID source item (DID or memory area).
 *  \details        -
 *  \param[in]      opStatus         Current DynDID read operation status.
 *  \param[in,out]  pDataContext     Pointer to the data context
 *  \param[in,out]  pProcessContext  The process context
 *  \return         DCM_E_OK         Read of all source items finished with success.
 *  \return         DCM_E_PENDING    One of the source items needs more time to return final result, call again.
 *  \return         DCM_E_NOT_OK     One of the source items returned an error.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadSrcItems(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CGetSrcItem()
 *********************************************************************************************************************/
/*! \brief          Return the requested source item
 *  \details        -
 *  \param[in]      srcItemIndex    Index of the source item
 *  \return         The requested source item or any valid source item if the provided index is invalid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidSrcItemPtrType, DCM_CODE) Dcm_Svc2CGetSrcItem(
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType srcItemIndex
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CGetItem()
 *********************************************************************************************************************/
/*! \brief          Return the requested item
 *  \details        -
 *  \param[in]      itemIndex    Index of the item
 *  \return         The requested item or any valid item if the provided index is invalid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidItemPtrType, DCM_CODE) Dcm_Svc2CGetItem(
  Dcm_CfgDidMgrDynDidHandleOptType itemIndex
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CGetDDDidProcessContext()
 *********************************************************************************************************************/
/*! \brief          Return the requested process context
 *  \details        -
 *  \param[in]      processContextIndex    Index of the process context
 *  \return         The process context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidProcessContextPtrType, DCM_CODE) Dcm_Svc2CGetDDDidProcessContext(
  uint8 processContextIndex
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01DefMgrRead()
 *********************************************************************************************************************/
/*! \brief          Reads a single DID source item.
 *  \details        -
 *  \param[in]      opStatus        Current DID sourceItem operation status.
 *  \param[in,out]  pDataContext    Pointer to the data context
 *  \param[in,out]  pProcessContext The process context
 *  \return         DCM_E_OK        Reading finished with success.
 *  \return         DCM_E_PENDING   Reading needs some additional time. Call again.
 *  \return         DCM_E_NOT_OK    Reading finished with failed.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  );
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02DefMgrRead()
 *********************************************************************************************************************/
/*! \brief          Reads a single Memory source item.
 *  \details        -
 *  \param[in]      opStatus        Current Memory sourceItem operation status.
 *  \param[in,out]  pDataContext    Pointer to the data context
 *  \return         DCM_E_OK            Reading finished with success.
 *  \return         DCM_E_FORCE_RCRRP   Send RCR-RP response. Once sent, call again.
 *  \return         DCM_E_PENDING       Reading needs some additional time. Call again.
 *  \return         DCM_E_NOT_OK        Reading finished with failed.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadCheckAccessAndInit()
 *********************************************************************************************************************/
/*! \brief          Check read acces for requested DynDID.
 *  \details        -
 *  \param[in]      dynDidHandle    Current dynDid handle to be read.
 *  \param[in,out]  pOpStatus       Current and modified operation status.
 *  \return         DCM_E_OK            Check succeeded, proceed with read.
 *  \return         DCM_E_PENDING       The DynDID is still in usage, try later
 *  \return         DCM_E_NOT_OK        Check failed, leave the DynDID reading.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadCheckAccessAndInit(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  P2VAR(Dcm_OpStatusType, AUTOMATIC, AUTOMATIC) pOpStatus
  );
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CfgSvc2CSubFuncInfoGetEntry()
 *********************************************************************************************************************/
/*! \brief          Get subfunction descriptor.
 *  \details        -
 *  \param[in]      SubSvcRef       Index to a subfunction descriptor
 *  \param[out]     pSubFuncInfo    The subfunction descriptor
 *  \param[out]     ErrorCode       Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK        The operation was successful
 *  \return         DCM_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_CfgSvc2CSubFuncInfoGetEntry(
  Dcm_DiagSubServiceRefOptType SubSvcRef,
  CONSTP2VAR(Dcm_CfgSvc2CSubFuncInfoPtrType, AUTOMATIC, AUTOMATIC) pSubFuncInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CSubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C SubFunction Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CSubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidLookUp()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Performs a lookup for a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidCheckCondition()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Checks the condition of a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidGetLength()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Calculates and stores the length of a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02SrcItemsGetLength()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x02.
 *  \details        Calculates the total length of all source items
 *  \param[in]      alfid                   The ALFID of the request
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Calculation succeeded
 *  \return         DCM_E_NOT_OK            Error during calculation
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02SrcItemsGetLength(
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2C03DidLookUp()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C 0x03 dedicated DID look up client.
 *  \details        -
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[out]     pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C03DidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CSetMaxAllowedDynDIDLength()
 *********************************************************************************************************************/
/*! \brief          Set the maximum allowed length for the currently processed DynDID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pRepContext    Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CSetMaxAllowedDynDIDLength(
  Dcm_ContextPtrType pContext,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CInitDidInfoFromDidInfoIdx()
 *********************************************************************************************************************/
/*! \brief          Returns information of a specific DID.
 *  \details        Retrieve the DID information using DidInfoIdx.
 *  \param[in]      didInfoIdx        The current operation status
 *  \param[in,out]  pSrcDidContext    The source DID context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CInitDidInfoFromDidInfoIdx(
  Dcm_CfgDidMgrDidInfoRefType didInfoIdx,
  Dcm_DidMgrDidInfoContextPtrType pSrcDidContext
  );
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CfgSvc86SubFuncInfoGetEntry()
 *********************************************************************************************************************/
/*! \brief          Get subfunction descriptor.
 *  \details        -
 *  \param[in]      SubSvcRef       Index to a subfunction descriptor
 *  \param[out]     pSubFuncInfo    The subfunction descriptor
 *  \param[out]     ErrorCode       Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK        The operation was successful
 *  \return         DCM_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_CfgSvc86SubFuncInfoGetEntry(
  Dcm_DiagSubServiceRefOptType SubSvcRef,
  CONSTP2VAR(Dcm_CfgSvc86SubFuncInfoPtrType, AUTOMATIC, AUTOMATIC) pSubFuncInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_Svc86Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 SubFunction Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetUnRegisterComMActivity()
 *********************************************************************************************************************/
/*! \brief          Tries to unregister a ComM channel activity.
 *  \details        -
 *  \param[in]      connHdl    The connection which has finished its job
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            Must be called only for external diagnostic requests
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUnRegisterComMActivity(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetRegisterComMActivity()
 *********************************************************************************************************************/
/*! \brief          Tries to register a ComM channel activity.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object which starts diagnostic job
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetRegisterComMActivity(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetUsdtTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Transport object transmission finalization.
 *  \details        Finalization of a transmitted USDT message.
 *  \param[in]      pTranspObj    The transport object used during transmission
 *  \param[in]      result        The transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUsdtTxConfirmation(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType result
  );

/**********************************************************************************************************************
 *  Dcm_NetAllocateOrGetTranspObject()
 *********************************************************************************************************************/
/*! \brief          Transport object allocation.
 *  \details        Allocates a transport object for a concrete client connection.
 *  \param[in]      connHdl            The connection identifier
 *  \param[out]     pTranspObj         The allocated transport object
 *  \return         BUFREQ_OK          The operation was successful
 *  \return         BUFREQ_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Must be called within a critical section to guarantee unique result
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetAllocateOrGetTranspObject(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetGetTranspObjOfConnection()
 *********************************************************************************************************************/
/*! \brief          Returns the transport object of the given connection.
 *  \details        -
 *  \param[in]      connHdl            The connection identifier
 *  \param[out]     pTranspObj         The transport object assigned to the given connection
 *  \return         BUFREQ_OK          The operation was successful
 *  \return         BUFREQ_E_NOT_OK    The given index was invalid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            Since only getter, can be called outside of a critical section, but for consistent results do call within critical section
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetGetTranspObjOfConnection(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetTranspObjRelease()
 *********************************************************************************************************************/
/*! \brief          Release transport object resource.
 *  \details        Releases a given transport object.
 *  \param[in]      pTranspObj    The transport object to be released
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetTranspObjRelease(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByTesterAddress()
 *********************************************************************************************************************/
/*! \brief          Tries to find the corresponding connection of a diagnostic client.
 *  \details        -
 *  \param[in]      testerAddress    The diagnostic client address to look for
 *  \return         < DCM_NET_INVALID_CONNHDL    Valid connection id found.
 *  \return         DCM_NET_INVALID_CONNHDL      No connection found for the source address.
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByTesterAddress(
  uint16 testerAddress
  );

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterActiveConnection()
 *********************************************************************************************************************/
/*! \brief          Unregister a connection.
 *  \details        Unregister an active connection referenced by the given transport object.
 *  \param[in]      pTranspObj    The transport object to be released
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUnRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetStartOfReception()
 *********************************************************************************************************************/
/*! \brief          Start of new request reception.
 *  \details        This function is called by Dcm_StartOfReception.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[in]      tpSduLength        The total length of the TP message
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \param[in]      addTObjFlags       Additional transport object flags from caller
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \return         BUFREQ_E_OVFL      The request message is too long for the configured buffer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetStartOfReception(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr,
  Dcm_NetTransportObjectFlagType addTObjFlags
  );

/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveRequest()
 *********************************************************************************************************************/
/*! \brief          Check whether request is functional TesterPresent with SPRMIB set.
 *  \details        -
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[in]      neededLength       The length to be a TesterPresent  message
 *  \return         TRUE               Request is a functional TesterPresent with SPRMIB set
 *  \return         FALSE              Request is not a functional TesterPresent with SPRMIB set
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveRequest(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  uint8 neededLength
  );

/**********************************************************************************************************************
 *  Dcm_NetSetComModeEntered()
 *********************************************************************************************************************/
/*! \brief          Internal utility for updating the new communication state.
 *  \details        -
 *  \param[in]      networkId      The communication channel on which this state is now active
 *  \param[in]      newComState    The new state of the communication channel (e.g. DCM_NET_COMM_STATE_RX_DIS_TX_DIS)
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetComModeEntered(
  NetworkHandleType networkId,
  Dcm_NetComMStateType newComState
  );
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferExecute()
 *********************************************************************************************************************/
/*! \brief          Starts polling a paged buffer update function.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pDataContext          The pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_OK              All data has been copied - evaluate the out parameters
 *  \return         DCM_E_PENDING         No result yet, retry later
 *  \return         DCM_E_PENDING_LIMITER Runtime limit reached, retry later
 *  \return         DCM_E_NOT_OK          Something went wrong, stop updating (only in DET situation)
 *  \return         DCM_E_BUFFERTOOLOW    Some data copied but no place, retry later on TP buffer under-run
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferExecute(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferCancel()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CancelPage call.
 *  \details        Dispatches the CancelPage call regarding the paged-buffer.
 *  \param[in]      pContext        Pointer to the context
 *  \param[out]     ErrorCode       The NRC
 *  \return         DCM_E_OK        Operation finished with success
 *  \return         DCM_E_NOT_OK    Operation failed, ErrorCode contains the NRC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferCancel(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#endif
/**********************************************************************************************************************
 *  Dcm_DiagRcrRpTxFinished()
 *********************************************************************************************************************/
/*! \brief          Performs post processing of currently finished RCR-RP transmission.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRcrRpTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetP2Timings()
 *********************************************************************************************************************/
/*! \brief          Sets the adjusted P2 timings.
 *  \details        -
 *  \param[in]      sesStateIdx    The index of the current session.
 *  \param[in]      protocolIdx    The index of the current protocol.
 *  \param[in]      threadId       Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagSetP2Timings(
  Dcm_StateIndexMemType sesStateIdx,
  Dcm_NetProtRefMemType protocolIdx,
  Dcm_ThreadIdMemType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagRxIndication()
 *********************************************************************************************************************/
/*! \brief          Called once an incoming reception is finished.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM external/internal connection
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRxIndication(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_SetNegResponse()
 *********************************************************************************************************************/
/*! \brief          Registers a NRC to the request in progress if no other NRC already set.
 *  \details        -
 *  \param[in]      nrc         The error code to be registered
 *  \param[in]      threadId    Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_SetNegResponse(
  Dcm_NegativeResponseCodeType nrc,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_ProcessingDone()
 *********************************************************************************************************************/
/*! \brief          Finalizes the service job processing.
 *  \details        -
 *  \param[in]      threadId    Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_ProcessingDone(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagLookUpFilter()
 *********************************************************************************************************************/
/*! \brief          Performs a lookup result filtering.
 *  \details        -
 *  \param[in,out]  pMsgContext          The current request context
 *  \param[in]      lookUpTable          Pointer to the table to be scanned (first element is table-size!)
 *  \param[in]      lookUpFilterTable    List of filters
 *  \param[in]      value                Value to be found
 *  \param[in]      chkLevel             Hierarchy level on which the lookup is performed
 *  \param[out]     errorCode            The error code to be returned
 *  \return         -1                   Diagnostic entity not active
 *  \return         >=0                  Success, index of the matched position
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(sint16_least, DCM_CODE) Dcm_DiagLookUpFilter(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType errorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithSubFuncStateCheck()
 *********************************************************************************************************************/
/*! \brief          Performs a state rule check for a service with sub-function.
 *  \details        -
 *  \param[in,out]  pMsgContext               Current request context
 *  \param[in]      stateRef                  Reference to the state precondition table
 *  \param[in]      subFuncSequenceChecker    Function for accessing a service specific sequence check
 *  \param[in]      subSvcRef                 The sub-function zero-based index
 *  \param[out]     ErrorCode                 The NRC
 *  \return         DCM_E_OK                  State rule check was sucessful
 *  \return         DCM_E_NOT_OK              State rule check was not sucessful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSvcWithSubFuncStateCheck(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagExecIndicationFunc()
 *********************************************************************************************************************/
/*! \brief          Executes an indication callback and evaluates its return values.
 *  \details        -
 *  \param[in]      pContext            Pointer to the context
 *  \param[in]      sid                 The requested SID
 *  \param[in]      notificationType    The notification kind (OEM, SYS)
 *  \param[in]      notifList           The request indication list to be executed
 *  \param[out]     ErrorCode           The NRC
 *  \return         DCM_E_OK            Proceed with service validation after return
 *  \return         DCM_E_NOT_OK        At least one Xxx_Indication() has rejected further processing
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            The last element of the passed notifList must be marked by NULL_PTR.
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagExecIndicationFunc(
  Dcm_ContextPtrType pContext,
  Dcm_MsgItemType sid,
  Dcm_DiagApplNotificationType notificationType,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagTxFinalization()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagTxFinalization(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnFblResTxFinished()
 *********************************************************************************************************************/
/*! \brief          Called by the TP once an ongoing transmission is finished.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnFblResTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagCancelProcessing()
 *********************************************************************************************************************/
/*! \brief          Cancel any ongoing job.
 *  \details        This function cancels any ongoing service processing.
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagCancelProcessing(
  uint8 threadId
  );
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrClear()
 *********************************************************************************************************************/
/*! \brief          Clear all DDID definitions.
 *  \details        Clears all previously defined DDIDs.
 *  \param[in]      dynDidHandle    DynDID reference to be cleared
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrClear(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle
  );
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C sub-function length getter.
 *  \details        Always returns length of 0 byte to show that the requests have dynamic length and the concrete
 *                  length will be checked later.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc2CReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CParseAndCheckDynDid()
 *********************************************************************************************************************/
/*! \brief          Parses and validates the DynDID to be defined.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[out]     pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK       Send response
 *  \return         DCM_E_NOT_OK   Send neagative response
 *  \return         DCM_E_PENDING  Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CParseAndCheckDynDid(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CCheckAndSetNumOfItems()
 *********************************************************************************************************************/
/*! \brief          Check DynDID has enough space to keep all requested definitions.
 *  \details        If the check succeeds also stores the number of requested items for further processing.
 *  \param[in]      numRequestedItems  Number of requested Items
 *  \param[out]     ErrorCode          The NRC
 *  \param[in,out]  pRepContext        Current repeater proxy context
 *  \return         DCM_E_OK        Check succeeded
 *  \return         DCM_E_NOT_OK    Check failed (NRC is stored in ErrorCode)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CCheckAndSetNumOfItems(
  Dcm_MsgLenType numRequestedItems,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc86ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Sub-components per-thread memory */
DCM_LOCAL VAR(Dcm_ContextType, DCM_VAR_NOINIT) Dcm_ThreadContext[DCM_NUM_THREADS];                                                                   /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
/*! Sub-components shared memory */
DCM_LOCAL VAR(Dcm_SingletonContextType, DCM_VAR_NOINIT) Dcm_SingletonContext;
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/*! Service 0x2C Data that can be read from and written to NvM (if NvM is configured) */
VAR(Dcm_Svc2CDynDidNvMDataType, DCM_VAR_NOINIT) Dcm_Svc2CNvMData;
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Diagnostic service verification level to NRC map */
DCM_LOCAL CONST(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DiagObjNotFoundNrcs[DCM_DIAG_NUM_CHK_LVLS]=                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_E_SERVICENOTSUPPORTED     /*!< on SID level */
 ,DCM_E_REQUESTOUTOFRANGE       /*!< on Parameter (DID, RID, etc.) level */
 ,DCM_E_SUBFUNCTIONNOTSUPPORTED /*!< on SubFunction level */
};
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferInitByThread()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferInitByThread(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(threadId);

  pContext->PagedBuffer.State = DCM_PAGEDBUFFER_STATE_INACTIVE;                                                                                      /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferInit(
  void
  )
{
  Dcm_ThreadIdOptType lThreadIter;
  for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
  {
    Dcm_PagedBufferInitByThread(lThreadIter);
  }
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferDefragmentPage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferDefragmentPage(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;
  Dcm_CfgNetBufferSizeOptType   lPageSize;

  lBufferInfo = Dcm_NetGetBufferInfo(pContext->PagedBuffer.BufferHdl);
  lPageSize = (Dcm_CfgNetBufferSizeOptType)(pContext->PagedBuffer.WriteIndex)
            - (Dcm_CfgNetBufferSizeOptType)(pContext->PagedBuffer.ReadIndex);

  Dcm_UtiMemCopySafe(&(lBufferInfo->BufferPtr[pContext->PagedBuffer.ReadIndex])
                    ,lBufferInfo->BufferPtr
                    ,0u
                    ,lBufferInfo->Size
                    ,(Dcm_MsgLenType)lPageSize);                                                                                                     /* SBSW_DCM_POINTER_WRITE_BUFFERINFO */

  /* Enter critical section (Reason: Protect against hi-priority task /ISR CopyTxData) */
  Dcm_UtiEnterCriticalSection();
  /* Update Read- and Write-Index */
  pContext->PagedBuffer.ReadIndex  = 0u;                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->PagedBuffer.WriteIndex = (Dcm_CfgNetBufferSizeMemType)lPageSize;                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferUpdatePage()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferUpdatePage(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagDataContextType       lDataContext;
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;
  uint8                         lUpdaterDataProvisionState;

  lBufferInfo = Dcm_NetGetBufferInfo(pContext->PagedBuffer.BufferHdl);

  Dcm_DebugAssert((pContext->PagedBuffer.State != DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                           /* COV_DCM_RTM_DEV_DEBUG XF */

  /* If defragmentation needed: */
  if ((pContext->PagedBuffer.UpdaterDataProvisionState == DCM_PAGEDBUFFER_WAIT_FOR_DATA)
    &&(pContext->PagedBuffer.ReadIndex != 0u) )
  {
    Dcm_PagedBufferDefragmentPage(pContext);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* Derive new data context for the remaining data to be read */
  Dcm_UtiInitDataContext(&lDataContext, lBufferInfo->BufferPtr, lBufferInfo->Size);                                                                  /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */
  Dcm_UtiCommitData(&lDataContext, pContext->PagedBuffer.WriteIndex);                                                                                /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

  /* Stronger limitation of the available buffer size - cut to the response length */
  if(lDataContext.AvailLen > pContext->PagedBuffer.RemainingLen)
  {
    Dcm_UtiDataContextSetAvailLen(&lDataContext, pContext->PagedBuffer.RemainingLen);                                                                /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
  }

  /* Delegate to the page writer */
  lStdResult = Dcm_PagedBufferExecute(pContext, &lDataContext, ErrorCode);                                                                           /* SBSW_DCM_COMB_PTR_FORWARD */

  switch(lStdResult)
  {
    case DCM_E_OK:
    case DCM_E_NOT_OK:
      pContext->PagedBuffer.UpdateFinished = TRUE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_ALL_DATA_PROVIDED;
      break;
    case DCM_E_DATA_READY_PADDING:
      pContext->PagedBuffer.OpStatus = DCM_INITIAL;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Delegate to the padding writer */
      pContext->PagedBuffer.UpdateFuncRef = DCM_PAGEDBUFFER_PADDING_FUNC_REF;                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Application is done, now call the padding-bytes provider */
      pContext->PagedBuffer.UpdateFinished = TRUE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    case DCM_E_PENDING:
      pContext->PagedBuffer.OpStatus = DCM_PENDING;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Data could also be written although DCM_E_PENDING was returned */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    case DCM_E_PENDING_LIMITER:
      /* Keep the last opStatus on "pending" forced by Dcm_PagedBufferExecute code */
      /* Data could also be written although DCM_E_PENDING was returned */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      lStdResult = DCM_E_PENDING;
      break;
    case DCM_E_BUFFERTOOLOW:
      pContext->PagedBuffer.OpStatus = DCM_PENDING;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    default:/* Unexpected return value */                                                                                                            /* COV_DCM_RTM_UNREACHABLE X */
      lStdResult = DCM_E_NOT_OK; /* Assure only valid values from here on */
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      pContext->PagedBuffer.UpdateFinished = TRUE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_ALL_DATA_PROVIDED;
      break;
  }

  if(lStdResult == DCM_E_NOT_OK)
  {
    if(pContext->PagedBuffer.State == DCM_PAGEDBUFFER_STATE_ACTIVATED)
    {
      /* Reset paged-buffer usage and delegate to the linear buffer processor */
      pContext->PagedBuffer.State = DCM_PAGEDBUFFER_STATE_CANCELED;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Wait for under-run transmission interruption (simulate successful update with zero data size) */
      lStdResult = DCM_E_OK;
    }
    pContext->PagedBuffer.UpdaterDataProvisionState = lUpdaterDataProvisionState;                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Enter critical section (Reason: Protect against hi-priority task /ISR CopyTxData) */
    Dcm_UtiEnterCriticalSection();
    /* Update statistic */
    pContext->PagedBuffer.RemainingLen -= (Dcm_MsgLenType)(lDataContext.Usage - pContext->PagedBuffer.WriteIndex);                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    pContext->PagedBuffer.WriteIndex = (Dcm_CfgNetBufferSizeMemType)lDataContext.Usage;                                                              /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Unlock CopyTx Data if more data is expected to be provided by a paged-buffer updater */
    pContext->PagedBuffer.UpdaterDataProvisionState = lUpdaterDataProvisionState;                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    /* If transmission not yet started and some data is already available for transmission: */
    if ((pContext->PagedBuffer.State == DCM_PAGEDBUFFER_STATE_ACTIVATED)
      && (lStdResult != DCM_E_PENDING))
    {
      /* Activate transmission */
      pContext->PagedBuffer.State = DCM_PAGEDBUFFER_STATE_ONTX;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      pContext->Diag.State |= DCM_DIAG_STATE_ON_TX; /* update diagnostic state (PROCESS && ON_TX) */                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_PAGED, pContext->ThreadId);
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferCopyData()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_PagedBufferCopyData(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr                                                                                    /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  BufReq_ReturnType  lResult = BUFREQ_OK;
  Dcm_ContextPtrType pContext;

  DCM_IGNORE_UNREF_PARAM(pTranspObj);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pContext = Dcm_GetThreadContext(Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                                                        /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_DebugAssert((pContext->PagedBuffer.State != DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_COPYTXDATA
                  ,DCM_E_ILLEGAL_STATE);                                                                                                             /* COV_DCM_RTM_DEV_DEBUG XF */

  *availableDataPtr = (PduLengthType)(pContext->PagedBuffer.WriteIndex)
                                      - (PduLengthType)(pContext->PagedBuffer.ReadIndex); /* Calculate and return page size */                       /* SBSW_DCM_PARAM_PTR_WRITE */

  if( (*availableDataPtr == 0u) /* no data at all */
    ||(*availableDataPtr < info->SduLength) ) /* real under-run */
  {
    if(info->SduLength != 0u)
    {
      lResult = BUFREQ_E_BUSY;
    }/* else - just buffer size check */

    if ((pContext->PagedBuffer.UpdaterDataProvisionState == DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED)
      &&(pContext->PagedBuffer.RemainingLen != 0u) )
    {
      pContext->PagedBuffer.UpdaterDataProvisionState = DCM_PAGEDBUFFER_WAIT_FOR_DATA;                                                               /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
      Dcm_TskSetEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));/* re-activate page updater */ /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    }/* else - still waiting for the data or all data provided */
  }
  else /* (lPageSize != 0u) && (lPageSize >= info->SduLength) => <check buffer availability> */
  {
    if ((pContext->PagedBuffer.UpdaterDataProvisionState == DCM_PAGEDBUFFER_WAIT_FOR_DATA) /* still waiting for data OR data is currently being written */
      &&(pContext->PagedBuffer.ReadIndex != 0u) /* with needed defragmentation */
      &&(info->SduLength != 0u) )/* no buffer availability check request */
    {
      lResult = BUFREQ_E_BUSY;/* avoid coping data during defragmentation of the paged-buffer (i.e. in case of adaptive CopyTxData call with reducing SduLength each time, or when SduLength == 0, and immediately called again with SduLength != 0 */
    }
    else
    {
      Dcm_PbCfgNetBufferInfoPtrType lBufferInfo = Dcm_NetGetBufferInfo(pContext->PagedBuffer.BufferHdl);

      *availableDataPtr -= info->SduLength; /* Update remained data part */                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Take the linear case always */
      Dcm_UtiMemCopySafe(&(lBufferInfo->BufferPtr[pContext->PagedBuffer.ReadIndex])
                        ,info->SduDataPtr
                        ,0u
                        ,info->SduLength
                        ,info->SduLength);                                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      pContext->PagedBuffer.ReadIndex += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                             /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
    }
  }

  return lResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagAcceptNewRequest()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DiagAcceptNewRequest(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  boolean lResult = TRUE;
  /* If SID in range: [0x40, 0x7F]U[0xC0, 0xFF] */
#if (DCM_DIAG_IGNORE_RESPONSE_SID_ENABLED == STD_ON)
  if((pTranspObj->RxData[0] & 0x40u) != 0u)
  {
    pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_IGNORE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = FALSE;
  }
  else
#endif
  /* If valid functional "0x3E 0x80" */
  if( (pTranspObj->RxData[0] == 0x3Eu)
    &&(pTranspObj->RxData[1] == 0x80u)
    &&(pTranspObj->RxLength  == 2u)
    &&(Dcm_PbCfgNetRxPduInfo[pTranspObj->RxPduId].IsFuncReq == TRUE) )                                                                               /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    /* In case of generic connections, a connection can be used by a pool of testers. Tester address has to be checked as well. */
    if( (Dcm_StateIsDefaultSessionPending() == FALSE) /* A non-default session is/will be active */
      &&( (Dcm_NetGetSessionConnection() != pTranspObj->ConnHdl)
        ||(Dcm_NetGetSessionClientSrcAddr() != pTranspObj->ClientSrcAddr)) ) /* Request from different tester */
    {
      pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_IGNORE;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }
#endif
    lResult = FALSE;
  }
  else
  {
    /* nothing to do */
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagOnTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagOnTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  switch(pTranspObj->ResType)
  {
  case DCM_NET_TOBJ_RESTYPE_LINEAR:
  case DCM_NET_TOBJ_RESTYPE_PAGED:
    /* Do finalize main response */
    Dcm_DiagTxFinalization(pTranspObj, txStatus);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_NET_TOBJ_RESTYPE_RCRRP:
    Dcm_DiagRcrRpTxFinished(pTranspObj, txStatus);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  case DCM_NET_TOBJ_RESTYPE_UNSOLICITED:
    Dcm_DiagOnFblResTxFinished(pTranspObj, txStatus);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    /* Unknown response type */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }
}
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CGetSrcItem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidSrcItemPtrType, DCM_CODE) Dcm_Svc2CGetSrcItem(
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType srcItemIndex
  )
{
  Dcm_Svc2CDynDidSrcItemPtrType pSourceItem;

  if (Dcm_DebugDetectRuntimeError(srcItemIndex >= DCM_NUM_DYNDID_ITEMS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pSourceItem = &Dcm_Svc2CNvMData.SrcItems[0];
  }
  else
  {
    pSourceItem = &Dcm_Svc2CNvMData.SrcItems[srcItemIndex];
  }

  return pSourceItem;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CGetItem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidItemPtrType, DCM_CODE) Dcm_Svc2CGetItem(
  Dcm_CfgDidMgrDynDidHandleOptType itemIndex
  )
{
  Dcm_Svc2CDynDidItemPtrType pItem;

  if (Dcm_DebugDetectRuntimeError(itemIndex >= DCM_NUM_DYNDIDS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pItem = &Dcm_Svc2CNvMData.Items[0];
  }
  else
  {
    pItem = &Dcm_Svc2CNvMData.Items[itemIndex];
  }

  return pItem;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CGetDDDidProcessContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidProcessContextPtrType, DCM_CODE) Dcm_Svc2CGetDDDidProcessContext(
  uint8 processContextIndex
  )
{
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;

  if (Dcm_DebugDetectRuntimeError(processContextIndex >= DCM_SVC_2C_NUM_PROCESS_CONTEXTS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pProcessContext = &Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.ProcessContext[0];
  }
  else
  {
    pProcessContext = &Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.ProcessContext[processContextIndex];
  }

  return pProcessContext;
}
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled(
  void
  )
{
  return (DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED == STD_ON);                                                                            /* PRQA S 2995, 2996 */ /* MD_Dcm_ConstExpr */
}
/**********************************************************************************************************************
 *  Dcm_NetIsRxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsRxPduIdValid(
  PduIdType rxPduId
  )
{
  boolean lResult = TRUE;

  if( (rxPduId >= Dcm_PbCfgNetNumRxPduIds)
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    ||(Dcm_NetGetConnHdlOfRxPduId(rxPduId) >= Dcm_PbCfgNetNumConnections)
#endif
    )
  {
    lResult = FALSE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetIsTxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsTxPduIdValid(
  PduIdType txPduId
  )
{
  boolean lResult = TRUE;

  if( (txPduId >= Dcm_PbCfgNetNumTxPduIds)
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    ||(Dcm_NetGetConnHdlOfTxPduId(txPduId) >= Dcm_PbCfgNetNumConnections)
#endif
    )
  {
    lResult = FALSE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetComMContext()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_NetComMContextPtrType, DCM_CODE) Dcm_NetGetComMContext(
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_NetComMContextPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumComMChannels))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetComMContext[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetComMContext[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetBufferContext()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_NetBufferContextPtrType, DCM_CODE) Dcm_NetGetBufferContext(
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_NetBufferContextPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumBuffers))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetBufferContext[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetBufferContext[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetPutToConn2TObjMap()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPutToConn2TObjMap(
  Dcm_NetConnRefOptType index,
  Dcm_CfgNetTObjHandleMemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumConnections))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_PbRamNetConnHdl2TObjMap[index] = value;                                                                                                      /* SBSW_DCM_POINTER_WRITE_TOBJHANDLE */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetWriteAddBufferU8At()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetWriteAddBufferU8At(
  Dcm_NetTransportObjectPtrType pTranspObj,
  uint8_least index,
  Dcm_MsgItemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= DCM_NET_TOBJ_ADDBUFFER_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pTranspObj->AddBuffer[index] = value;                                                                                                            /* SBSW_DCM_POINTER_WRITE_TOBJ_ADDBUFFER */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetCommonInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetCommonInit(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.ComVariant = configPtr->ComVariant;
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.RamVariant = configPtr->RamVariant->NetworkRam;
  Dcm_SingletonContext.Network.RomVariant = configPtr->RomVariant->NetworkRom;
#endif

  Dcm_SingletonContext.Network.NumActiveConnections = 0u;
  Dcm_SingletonContext.Network.ActiveProtocol = DCM_NET_INVALID_PROTID;
  Dcm_SingletonContext.Network.ActiveDiagnostic = DCM_NET_COMM_ACTIVE;
#if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.RxAllowed = TRUE;
#endif
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.KeepAlive = FALSE;
#endif
}

/**********************************************************************************************************************
 *  Dcm_NetComMInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComMInit(
  void
  )
{
  Dcm_CfgNetNetIdRefOptType lNetHandleIter;

  for(lNetHandleIter = 0; lNetHandleIter < Dcm_PbCfgNetNumComMChannels; ++lNetHandleIter)
  {
    Dcm_NetComMContextPtrType pComMContext;

    pComMContext = Dcm_NetGetComMContext(lNetHandleIter);

    pComMContext->ComState           = DCM_NET_COMM_STATE_RX_DIS_TX_DIS;                                                                             /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    pComMContext->RegisteredNetworks = 0u;                                                                                                           /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetBufferInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetBufferInit(
  void
  )
{
  Dcm_CfgNetBufferRefOptType lBufferIter;

  for(lBufferIter = 0; lBufferIter < Dcm_PbCfgNetNumBuffers; ++lBufferIter)
  {
    Dcm_NetBufferContextPtrType pBufferContext;

    pBufferContext = Dcm_NetGetBufferContext(lBufferIter);

    pBufferContext->IsInUseCnt = 0u;                                                                                                                 /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetConnectionInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetConnectionInit(
  void
  )
{
  Dcm_NetConnRefOptType lConnIter;

  for(lConnIter = 0; lConnIter < Dcm_PbCfgNetNumConnections; ++lConnIter)
  {
    Dcm_NetPutToConn2TObjMap(lConnIter, DCM_NET_INVALID_TOBJID);
  }
}

/**********************************************************************************************************************
 *  Dcm_NetTranspObjInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTranspObjInit(
  void
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjIter;

  for(lTObjIter = 0; lTObjIter < Dcm_PbCfgNetNumTransportObjects; ++lTObjIter)
  {
    Dcm_NetTransportObjectPtrType pTranspObj;

    pTranspObj = Dcm_NetGetTransportObject(lTObjIter);

    pTranspObj->ResType  = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->State    = DCM_NET_TOBJ_STATE_FREE;                                                                                                  /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->Flags    = DCM_NET_TOBJ_FLAG_NONE;                                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    Dcm_NetWriteAddBufferU8At(pTranspObj, 0, 0x7Fu); /* always used for negative responses */                                                        /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->Handle   = (Dcm_CfgNetTObjHandleMemType)lTObjIter;                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
  }
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgInit(
  void
  )
{
  /* Initialize periodic transmission manager state machine */
  Dcm_NetPeriodicMsgResetConnection();
  Dcm_NetPeriodicMsgInitDelayCntr();
  Dcm_NetPeriodicMsgInitFastestRate();
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  {
    /* Initialization of delay timers */
    uint8_least lDelayTmrIter;
    for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
    {
      Dcm_NetDelayTimerSet(lDelayTmrIter, 0);
    }

    Dcm_NetPeriodicMsgResetNextDelayTimer();
  }
# endif
  {
    Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
    for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_PbCfgNetNumPerTxObjects; ++lPerTxObjIter)
    {
      Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

      pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(lPerTxObjIter);

      /* Initialize the transport object */
      pPeriodicTxObj->Timer = 0u;                                                                                                                    /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_FREE;                                                                                        /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      pPeriodicTxObj->TxPduInfo.SduDataPtr = pPeriodicTxObj->TxBuffer;                                                                               /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    }
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetInit(
  Dcm_ConfigPtrType configPtr
  )
{
  /* First initialize common variables */
  Dcm_NetCommonInit(configPtr);                                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Now initialize all remaining network related sub-elements. The order of execution shall not be relevant */
  Dcm_NetComMInit();                                                                                                                                 /* PRQA S 2987 */ /* MD_Dcm_2987 */
  Dcm_NetBufferInit();
  Dcm_NetConnectionInit();                                                                                                                           /* PRQA S 2987 */ /* MD_Dcm_2987 */
  Dcm_NetTranspObjInit();
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  Dcm_NetPeriodicMsgInit();
#endif
}

/**********************************************************************************************************************
 *  Dcm_NetComposeAddNegResponse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComposeAddNegResponse(                                                                                  /* COV_DCM_RTM_NO_RCR_RP_TX */
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetResponseType resType,
  Dcm_NegativeResponseCodeType nrc
  )
{
  Dcm_NetWriteAddBufferU8At(pTranspObj, 2, nrc); /* NRC = nrc, 0x7F and SID are already copied! */                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  pTranspObj->BuffInfo.SduDataPtr = pTranspObj->AddBuffer; /* use the temp buffer to free the main buffer now */                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->BuffInfo.SduLength  = 3u;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->ResType             = resType;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State               = DCM_NET_TOBJ_STATE_PREPTX;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
}

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetOnRequestDetection()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetOnRequestDetection(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;
  BufReq_ReturnType             lBufReqResult;

  /* Retrieve the transport object from the given connection */
  lBufReqResult = Dcm_NetGetTranspObjOfConnection(connHdl, &pTranspObj);                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* Check whether a transport object is in use */
  if(lBufReqResult == BUFREQ_OK)
  {
    /* Enter critical section (Reason: the transport object may not be modified from outside (e.g. Dcm_TpTxConfirmation releases it) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /*  Retrieve again the transport object from connection to guarantee it has still the same state */
    lBufReqResult = Dcm_NetGetTranspObjOfConnection(connHdl, &pTranspObj);                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

    /* Check the connection still uses the same transport object (it is always 1:1 association between connection and a transport object) */
    if(lBufReqResult == BUFREQ_OK)                                                                                                                   /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      /* Check whether the connection is free */
      if(pTranspObj->State != DCM_NET_TOBJ_STATE_FREE)
      {
        /* Check if suppression of responses on functional addressed requests is enabled */
        if( (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_TX_ON_FUNC_RX))
          &&(Dcm_PbCfgNetRxPduInfo[pTranspObj->RxPduId].IsFuncReq == TRUE) )
        {
          /*
           * Interruption of functional addressed request is not necessary because response will already be
           * suppressed
           */
        }
        /* Otherwise, interrupt ongoing service processing */
        else
        {
          /* If a new request is being received: */
          if(pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX)
          {
            /* Isolate this connection */
            pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED;                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */

            /* Terminate the ongoing reception immediately */
            (void)PduR_DcmCancelReceive(pTranspObj->RxPduId);
          }
          /* Otherwise, let the task cancel any pending jobs */
          else
          {
            /* Mark transport object as obsolete. This is used to distinguish between two allocations of the same transport object before the Dcm_NetTaskTaMonitor is called */
            pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_OBSOLETE;                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
            Dcm_TskSetEvent(DCM_TSK_ID_NET_TA_MONITOR, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle));
          }
        }
      }/* else - nothing to interrupt */
    }/* else - request no more in progress -> ignore */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }/* else - no request in progress or different tester -> ignore */
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  Dcm_NetExecStartProtocolOp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetExecStartProtocolOp(
  Dcm_ProtocolType newProtocol
  )
{
  boolean      lResult = TRUE;
  uint16_least lIter;

  for (lIter = 0u; Dcm_CfgNetProtocolNotificationInfo[lIter].StartFunc != NULL_PTR; ++lIter)                                                         /* PRQA S 2842 */ /* MD_Dcm_DerefInvalidPointer */
  {
    if(Dcm_CfgNetProtocolNotificationInfo[lIter].StartFunc(newProtocol) != DCM_E_OK)                                                                 /* SBSW_DCM_CALL_FUNCPTR_TERMINATED_LIST */
    {
      lResult = FALSE;
      break;
    }
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetProcessStartProtocol()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetProcessStartProtocol(
  Dcm_NetTransportObjectPtrType pTranspObj,
  boolean stopActiveProtocol
  )
{
  boolean startProtocolAllowed;

  startProtocolAllowed = Dcm_NetExecStartProtocolOp(Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfTranspObj(pTranspObj)].Id);
  if(startProtocolAllowed == FALSE)
  {
    /* Neg response 0x22 */
    Dcm_NetComposeAddNegResponse(pTranspObj
                                ,DCM_NET_TOBJ_RESTYPE_SIMPLE
                                ,DCM_E_CONDITIONSNOTCORRECT);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
  }
  else
  {
    if(stopActiveProtocol == TRUE)                                                                                                                   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    {
      uint8 lThreadId = Dcm_NetGetThreadIdOfSessionConnection();

      Dcm_DiagCancelProcessing(lThreadId);

      /* According to AR DCM SWS the session switch is executed at start protocol, not at stop! */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, lThreadId);
    } /* else - no protocol enabled -> no need to reset anything in case that parallelization is not enabled */

    Dcm_SingletonContext.Network.ActiveProtocol = Dcm_NetGetProtIdOfTranspObj(pTranspObj); /* Set new active protocol */

    /* Update P2 timings at once */
    Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfTranspObj(pTranspObj), Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_DiagRxIndication(pTranspObj);                                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationFindWinner()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetTObjHandleOptType, DCM_CODE) Dcm_NetRxPrioritisationFindWinner(
  Dcm_TskTaskEvOptType ev,
  Dcm_TskTaskEvPtrType pBusyEvents
  )
{
  Dcm_TskTaskEvOptType lEv = ev;
  uint16 lMaxPrio = 0x100u;/* lower than the lowest configurable priority (256 > 255) */

  Dcm_CfgNetTObjHandleOptType lTObjCntr = 0u;
  Dcm_CfgNetTObjHandleOptType lTranspObjWinnerId = DCM_NET_INVALID_TOBJID;

  *pBusyEvents = 0;                                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    /* Scan all arrived transport objects */
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u)) /* start scan from the LSB since TObjects are allocated by reversed iteration and thus -> first allocated (last checked here) will be the winner */
    {
      uint8 tmpProtPrio = Dcm_NetGetProtObjOfTranspObj(Dcm_NetGetTransportObject(lTObjCntr))->Priority;                                              /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

      /* Higher priority */
      if(lMaxPrio <= tmpProtPrio)
      {
        /* Lower priority than current request/pending response */
        *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, lTObjCntr);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      else
      {
        if(lTranspObjWinnerId < DCM_NET_INVALID_TOBJID)
        {
          /* Mark busy for the last winner */
          *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, lTranspObjWinnerId);                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }

        /* Update maximum priority */
        lMaxPrio = tmpProtPrio;

        /* potential winner found */
        lTranspObjWinnerId = lTObjCntr;
      }
    }
    ++lTObjCntr;
  }
  return lTranspObjWinnerId;
}

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessWinner()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessWinner(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_TskTaskEvPtrType pBusyEvents
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_NetGetProtIdOfActiveProtocol() != Dcm_NetGetProtIdOfTranspObj(pTranspObj))/* the winner is a different protocol as the old one -> interrupt the old protocol */
  {
    if( (Dcm_NetGetProtIdOfActiveProtocol() >= DCM_NET_INVALID_PROTID) /* very first protocol activation - start the protocol/send NRC 0x21 responses */
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)
      ||(Dcm_StateIsDefaultSessionPending() == TRUE) /* if default session ... */                                                                    /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#endif
      )
    {
      Dcm_NetProcessStartProtocol(pTranspObj, FALSE);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)
      if( (Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfTranspObj(pTranspObj)].Priority
           < Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfSessionConnection()].Priority) /* new one is higher priority than session owner */
#else
      if( (Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfTranspObj(pTranspObj)].Priority
           < Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfActiveProtocol()].Priority) /* new one is higher priority */
        ||( (((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE | DCM_DIAG_QUEUE_FLAG_IS_WAITING)
              & pContext->Diag.QueuedSet.Flags) == 0u ) /* if idle or post-processing (considered for back-to-back requests) ... */
          &&(Dcm_StateIsDefaultSessionPending() == TRUE) ) /* ... still default session - continue with new request */                               /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#endif
        )
      {
        Dcm_NetProcessStartProtocol(pTranspObj, TRUE);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        /* Mark busy for the winner */
        *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle);                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }
  else /* same protocol - proceed with the new request/NRC 0x21 responses */
  {
    Dcm_DiagRxIndication(pTranspObj);                                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessLoosers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessLoosers(
  Dcm_TskTaskEvOptType BusyEvents
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjCntr = 0u;
  Dcm_TskTaskEvOptType        lBusyEvents = BusyEvents;

  DCM_TSK_EVENT_SCAN_LOOP(lBusyEvents)
  {
    /* Check who did just loose */
    if (Dcm_TskIsLocalEventSet(lBusyEvents, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj = Dcm_NetGetTransportObject(lTObjCntr);

      Dcm_NetBufferRelease(pTranspObj->BufferHdl);
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
      pTranspObj->Flags |= (Dcm_NetTransportObjectFlagType)(DCM_NET_TOBJ_FLAG_BUSY | DCM_NET_TOBJ_FLAG_IGNORE);                                      /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
#else
# if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
      if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u) /* internal requests do not manage the ComM state */
# endif
      {
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
      }
      Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
#endif
    }
    ++lTObjCntr;
  }
}

/**********************************************************************************************************************
 *  Dcm_NetRegisterActiveConnection()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)/* internal requests do not manage the S3 timer */
#endif
  {
    Dcm_DebugAssert((Dcm_SingletonContext.Network.NumActiveConnections != Dcm_UtiMaxValueOfUintType(uint8)), DCM_SID_INTERNAL, DCM_E_PARAM);         /* COV_DCM_RTM_DEV_DEBUG XF */

    /* Enter critical section (Reason: protect the statistic counter against parallel request notifications or transmission confirmations, executed from within interrupts ) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
     BEGIN CRITICAL SECTION
     *=================================*/
    /* Reset the idle connection deauthentication timer for this connection */
#if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
    /* Check if authentication restriction is supported for this connection */
    if(Dcm_NetGetAuthInfoRef(pTranspObj->ConnHdl) != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      Dcm_AuthMgrTimerReset(Dcm_NetGetAuthInfoRef(pTranspObj->ConnHdl));
    }
#endif

    if(Dcm_SingletonContext.Network.NumActiveConnections == 0u)
    {
      Dcm_TmrStopTimer(DCM_TMR_ID_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrStopTimer(DCM_TMR_ID_KEEP_ALIVE);
#endif
    }

    /* Register a new active connection */
    ++Dcm_SingletonContext.Network.NumActiveConnections;
    /*=================================*
     END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}

/**********************************************************************************************************************
 *  Dcm_NetTransmitUsdtResponse()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTransmitUsdtResponse(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  boolean doCallTxConfirmation = TRUE;

  /* The Tx path must be still "ready" here. if not, check the caller - it shall take care about this state! */
  Dcm_DebugAssert((pTranspObj->State == DCM_NET_TOBJ_STATE_PREPTX), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                          /* COV_DCM_RTM_DEV_DEBUG XF */

  /* Prepare the given transport object for USDT response transmission (set all relevant attributes to their initial values) */
  pTranspObj->BuffPos = 0u;                                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State = DCM_NET_TOBJ_STATE_ONTX;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

  /* If the USDT response shall be suppressed for any non-diagnostic but transport protocol reasons (e.g. no response on functional requests or no TxPduId configured), do: */
  if( (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_TX_ON_FUNC_RX))
    &&(Dcm_PbCfgNetRxPduInfo[pTranspObj->RxPduId].IsFuncReq == TRUE)
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
    &&(pTranspObj->ResType != DCM_NET_TOBJ_RESTYPE_UNSOLICITED)
#endif
    )
  {
    lStdResult = DCM_E_OK; /* No response on functional requests required -> leave with success */
  }
  else if (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_NO_MAIN_TX))
  {
    lStdResult = DCM_E_OK; /* No associated TxPduId -> leave with success */
  }
  else
  {
    /* Otherwise, if the communication mode on the ComM channel associated with the transport object still allows any response transmissions: */
    if((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByTranspObject(pTranspObj)) != 0u)
    {
      PduInfoType    lTpTransmitData;
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      uint8          lMetaData[DCM_NET_METADATA_LENGTH];
#endif

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      /* In case of generic connection, the transmit data references the meta data, otherwise, it references the payload. */
      if(Dcm_NetIsGenericConnection(pTranspObj->ConnHdl))
      {
        lMetaData[DCM_NET_METADATA_SA_OFFSET] = Dcm_NetGetNodeAddress(pTranspObj->ConnHdl);                                                          /* SBSW_DCM_LOCAL_ARRAY_WRITE */
        lMetaData[DCM_NET_METADATA_TA_OFFSET] = (uint8)pTranspObj->ClientSrcAddr; /* safe cast, since ClientSrcAddr is source address of MetaData because ConnHdl is generic */ /* SBSW_DCM_LOCAL_ARRAY_WRITE */
        lTpTransmitData.SduDataPtr = &lMetaData[0];
      }
      else
#endif
      {
        lTpTransmitData.SduDataPtr = pTranspObj->BuffInfo.SduDataPtr;
      }

      /* The length of the Sdu does not consider the meta data. */
      lTpTransmitData.SduLength = pTranspObj->BuffInfo.SduLength;

      lStdResult = PduR_DcmTransmit(Dcm_PbCfgNetConnectionInfo[pTranspObj->ConnHdl].TxPduIdMain
                                   ,&lTpTransmitData);                                                                                               /* SBSW_DCM_POINTER_FORWARD_STACK */

      /* If this transmission request was accepted, wait for the Tx-Confirmation from the XxxTp */
      if(lStdResult == E_OK)
      {
        doCallTxConfirmation = FALSE;
      }
    }
  }
  /* Otherwise, take the shortcut and loop-back through the USDT message confirmation with failed result (this response had to be sent, but couldn't and no retry is allowed (SWS)) */

  /* Make a loop-back and finalize transmission */
  if(doCallTxConfirmation == TRUE)
  {
    Dcm_NetUsdtTxConfirmation(pTranspObj, lStdResult);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

#if (DCM_NET_CONN_LOCK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetLockConnection()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetLockConnection(
  Dcm_NetConnRefMemType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult;

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Try allocating a new or getting an already allocated transport object */
  lResult = Dcm_NetAllocateOrGetTranspObject(connHdl, pTranspObj);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lResult == BUFREQ_OK)
  {
    if((*pTranspObj)->State == DCM_NET_TOBJ_STATE_FREE)
    {
      /* Setup the transport object */
      (*pTranspObj)->ConnHdl = connHdl;                                                                                                              /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
      (*pTranspObj)->BufferHdl = Dcm_NetGetProtObjOfConnection(connHdl)->RxTxBufferRef;                                                              /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

      /* Is the buffer still free ? */
      if(Dcm_NetBufferIsFree((*pTranspObj)->BufferHdl))
      {
        Dcm_NetBufferLock((*pTranspObj)->BufferHdl); /* the buffer is in use now */
        (*pTranspObj)->State = DCM_NET_TOBJ_STATE_RESERVED;                                                                                          /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lResult = BUFREQ_OK;
      }
      else
      {
        Dcm_NetTranspObjRelease(*pTranspObj);                                                                                                        /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lResult = BUFREQ_E_NOT_OK;
      }
    }
    else
    {
      lResult = BUFREQ_E_NOT_OK;
    }
  }/* else - nothing found to be free */
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationNotOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationNotOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) == 0u) /* consider only real buffer usage */                                                       /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
#endif
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
  }
  Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationOk()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u) /* CancelReceive was called before -> ignore request although reception couldn't be canceled */
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
#endif
  {
    boolean keepTranspObj;
    keepTranspObj = Dcm_DiagAcceptNewRequest(pTranspObj);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    pTranspObj->State = DCM_NET_TOBJ_STATE_RX_END;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

    if(keepTranspObj == TRUE)
    {
      Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) != 0u)
      {
        Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
      }
      else
#endif
      {
        /* Store SID for RCR-RP/busy on protocol prioritisation responses */
        Dcm_NetWriteAddBufferU8At(pTranspObj, 1u, pTranspObj->RxData[0]);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        /* Signal processing start */
        Dcm_TskSetEventByThread(DCM_TSK_ID_NET_RX, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle), Dcm_NetGetThreadIdOfTranspObj(pTranspObj)); /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }
    else
    {
      if (Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled())
      {
        if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_IGNORE) == 0u)
        {
          /* Register at ComM */
          Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
          Dcm_SingletonContext.Network.KeepAlive = TRUE;
          /* Unregister at ComM */
          Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
        }
      }
      Dcm_NetTpRxIndicationNotOk(pTranspObj);                                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerCntrOptType, DCM_CODE) Dcm_NetDelayTimerGet(
  uint8_least timer
  )
{
  return Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[timer];
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetDelayTimerSet(
  uint8_least timer,
  Dcm_TmrTimerCntrOptType timeout
  )
{
  if (Dcm_DebugDetectRuntimeError(timer >= DCM_NET_DELAY_BULK_TRANSMISSION))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[timer] = (Dcm_TmrTimerCntrMemType)timeout;                                             /* SBSW_DCM_POINTER_WRITE_DELAYTIMER */
  }
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerExpired()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetDelayTimerExpired(
  uint8_least timer
  )
{
  return (boolean)(Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[timer] == 0u);                                                          /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerSet(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  )
{
  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  pPeriodicTxObj->Timer = timeout;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_SplitTaskLeaveCS();
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStart(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  )
{
  Dcm_NetPTxObjTimerSet(pPeriodicTxObj, timeout);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (!Dcm_TmrIsTimerRunning(DCM_TMR_ID_PERIODIC_TX))
  {
    Dcm_TmrStartTimer(DCM_TMR_ID_PERIODIC_TX, 1u);
  }
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStop()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStop(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  )
{
  Dcm_NetPTxObjTimerSet(pPeriodicTxObj, 0u);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgIncNextDelayTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgIncNextDelayTimer(
  void
  )
{
  ++Dcm_SingletonContext.Network.PeriodicTxContext.NextDelayTimer;
  if (Dcm_NetPeriodicMsgGetNextDelayTimer() >= DCM_NET_DELAY_BULK_TRANSMISSION)
  {
    Dcm_NetPeriodicMsgResetNextDelayTimer();
  }
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxDelayTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxDelayTimerStart(
  void
  )
{
  Dcm_NetDelayTimerSet(Dcm_NetPeriodicMsgGetNextDelayTimer()
                      ,Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate()));
  Dcm_NetPeriodicMsgIncNextDelayTimer();

  /* Make sure system timer for periodic tx is running */
  Dcm_TmrStartTimer(DCM_TMR_ID_PERIODIC_TX, 1u);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutTxObjectHandler()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutTxObjectHandler(
  void
  )
{
  Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
  boolean                       lReloadTimer = FALSE;

  for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_PbCfgNetNumPerTxObjects; ++lPerTxObjIter)
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(lPerTxObjIter);
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    Dcm_UtiEnterCriticalSection(); /* protect against high level interrupts from the network interface */
    if(!Dcm_NetPTxObjTimerExpired(pPeriodicTxObj))
    {
      Dcm_NetPTxObjTimerSet(pPeriodicTxObj, (Dcm_TmrTimerCntrMemType)(Dcm_NetPTxObjTimerGet(pPeriodicTxObj) - 1u));                                  /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      if(Dcm_NetPTxObjTimerExpired(pPeriodicTxObj))
      {
        (void)PduR_DcmCancelTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(lPerTxObjIter)); /* try to cancel still ongoing Tx */                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
        Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      }
      else
      {
        lReloadTimer = TRUE; /* at least one active timer - reload timer */
      }
    }
    Dcm_UtiLeaveCriticalSection();
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
  }

  return lReloadTimer;
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutDelayTimerHandler()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutDelayTimerHandler(
  void
  )
{
  uint8_least lDelayTmrIter;
  boolean     lReloadTimer = FALSE;
  boolean     lSetTxEvent = FALSE;
  for(lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
  {
    if(Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
    {
      Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter)-1u));
      if(Dcm_NetDelayTimerExpired(lDelayTmrIter) == TRUE)
      {
        Dcm_NetPeriodicMsgIncDelayCntr();
        lSetTxEvent = TRUE;
      }
      else
      {
        lReloadTimer = TRUE; /* at least one active timer - reload timer */
      }
    }
  }

  if (lSetTxEvent == TRUE)
  {
    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_UUDT);
  }

  return lReloadTimer;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmitTxObj()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmitTxObj(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  )
{
  Std_ReturnType lStdResult;
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

  pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(perTxObjHandle);

  if ((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByConnection(Dcm_NetPeriodicMsgGetConnection())) != 0u)
  {
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
    if (Dcm_NetPeriodicMsgGetDelayCntr() == 0u)
    {
      /* Retry sending */
      lStdResult = DCM_E_OK;
    }
    else
# endif
    {
      P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pPduInfo;

      Dcm_NetPeriodicMsgDecDelayCntr();

      pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_ONTX;                                                                                        /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      /* Note: In case the following two lines are not executed within 100ms, the timeout may occur before the message
       * was sent (only if split tasks used). This may lead to inconsistent periodic data, since the transmitted
       * message is free to be reused again while it is on transmission. The design avoiding this very rare scenario
       * will increase the software complexity unnecessarily. Entering such conditions means that the ECU is not
       * capable of holding the periodic rates too.
       */
      Dcm_NetPTxObjTimerStart(pPeriodicTxObj, Dcm_PbCfgNetConnectionInfo[Dcm_NetPeriodicMsgGetConnection()].PeriodicTxConfTimeout);                  /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */

      pPduInfo = &(pPeriodicTxObj->TxPduInfo);
      lStdResult = PduR_DcmTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(perTxObjHandle), pPduInfo);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
  else
  {
    lStdResult = DCM_E_NOT_OK;
  }

  if(lStdResult != DCM_E_OK)
  {
    /* Do not retry transmission -> make a loop-back and finalize transmission */
    Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                       /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetIsPduInfoValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsPduInfoValid(
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
  )
{
  boolean lResult;

  if(info == NULL_PTR)
  {
    lResult = FALSE;
  }
  else if( (info->SduLength  != 0u)
         &&(info->SduDataPtr == NULL_PTR) )
  {
    lResult = FALSE;
  }
  else
  {
    lResult = TRUE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetCopyRxData()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyRxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* If the XxxTp (e.g. CanTp or FrTp) requests Dcm to copy any data: */
  if(info->SduLength != 0u) /* any payload now? */
  {
    /* ----- Development Error Checks -------------------------------------- */
    if (Dcm_DebugDetectError(pTranspObj->BuffPos >= pTranspObj->RxLength))
    {
      lErrorId = DCM_E_PARAM;
    }
    else if(Dcm_DebugDetectError(((PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos)) < info->SduLength))
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      /* ----- Implementation ---------------------------------------------- */
#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
      /* If connection has been yet canceled (if RxPath supports cancellation), do return failed copy data result */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
      {
        lResult = BUFREQ_E_NOT_OK;
      }
      else
#endif
      {

        /* If the transmission is for a main-client (full request reception), do copy the data portion provided by the XxxTp */
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
        if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) != 0u)
        {
          /* For the very first call of this function for this request reception, do copy the request head (SID and SubFunction) for later usage (i.e. for 0x3E 0x80 detection) */
          if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_COPYHEAD) != 0u)
          {
            Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_COPYHEAD);  /* Toggle flag for next calls to not extract any SID/SUBSID data */ /* SBSW_DCM_PARAM_PTR_WRITE */
            Dcm_NetWriteAddBufferU8At(pTranspObj, 1, info->SduDataPtr[0]); /* copy the SID */                                                        /* PRQA S 2822 */ /* MD_Dcm_2822 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
            Dcm_NetWriteAddBufferU8At(pTranspObj, 2, info->SduDataPtr[1]); /* copy the SUBSID */                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
          }
        }
        else
#endif
        {
          Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

          lBufferInfo = Dcm_NetGetBufferInfo(pTranspObj->BufferHdl);

          Dcm_UtiMemCopySafe(info->SduDataPtr
                            ,lBufferInfo->BufferPtr
                            ,pTranspObj->BuffPos
                            ,lBufferInfo->Size
                            ,info->SduLength);                                                                                                       /* SBSW_DCM_POINTER_WRITE_BUFFERINFO */
        }
        /* Update the transport object's buffer-copy-progress */
        pTranspObj->BuffPos += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

        /* Report the remaining available DCM buffer size to the XxxTp */
        *bufferSizePtr = (PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos);                                               /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = BUFREQ_OK;
      }
    }
    /* Otherwise if connection has been canceled, return negative result to signal the XxxTp to abort the transmission (if RxPath supports cancellation) */
  }
  else
  {
    /* Otherwise, the XxxTp requests Dcm only to report the available buffer size  */
    *bufferSizePtr = (PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos); /* Notify lower layer about available buffer space */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = BUFREQ_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYRXDATA, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndication()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;
  BufReq_ReturnType             lBufReqResult;
  lBufReqResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                                 /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* If no transport object has been associated with this connection yet, do ignore this indication */
  if(lBufReqResult == BUFREQ_OK)
  {
    /* Otherwise, if the transport object is determined to belong to the expected request, that means: */
    if( (pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX) /* expected state for Rx indication? */
      &&(rxPduId == pTranspObj->RxPduId) ) /* Consider also any nested request on different TP connection but from the same tester! */
    {
      if(result != DCM_NET_ARENV_NTFRSLT_OK)
      {
        Dcm_NetTpRxIndicationNotOk(pTranspObj);                                                                                                      /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
      else
      {
        Dcm_NetTpRxIndicationOk(pTranspObj);                                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }/* Otherwise, just ignore this call since this API call is most probably due to a (concurrent request (i.e. 0x3E 0x80) from the same tester) OR
      * (indication of unexpected connection) */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetCopyLinearTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyLinearTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError( (info->SduLength != 0u)
                         &&(pTranspObj->BuffPos >= pTranspObj->BuffInfo.SduLength) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError((PduLengthType)(pTranspObj->BuffInfo.SduLength - pTranspObj->BuffPos) < info->SduLength))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Copy requested amount of data to the XxxTp */
    Dcm_UtiMemCopySafe(&(pTranspObj->BuffInfo.SduDataPtr[pTranspObj->BuffPos])
                      ,info->SduDataPtr
                      ,0u
                      ,info->SduLength
                      ,info->SduLength);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    /* Update Tx-buffer read progress */
    pTranspObj->BuffPos += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Report remaining amount of data in the Tx-buffer */
    *availableDataPtr = (PduLengthType)(pTranspObj->BuffInfo.SduLength - pTranspObj->BuffPos);                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = BUFREQ_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYTXDATA, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult;

  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
  {
    lResult = BUFREQ_E_NOT_OK;
  }
  else
  {
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
    if(pTranspObj->ResType == DCM_NET_TOBJ_RESTYPE_PAGED)
    {
      lResult = Dcm_PagedBufferCopyData(pTranspObj, info, availableDataPtr);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
#endif
    {
      lResult = Dcm_NetCopyLinearTxData(pTranspObj, info, availableDataPtr);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagSetNewReqBaseToCurProgress()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewReqBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  )
{
  if (Dcm_DebugDetectRuntimeError(pMsgContext->reqIndex >= pMsgContext->reqBufSize))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pMsgContext->reqBufSize -= pMsgContext->reqIndex;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_UtiNextItemByPtr(pMsgContext->reqData, pMsgContext->reqIndex);                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->reqIndex = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagSetQueuedTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetQueuedTranspObj(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  pContext->Diag.QueuedSet.TObjHdl = pTranspObj->Handle;                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
  Dcm_UtiBitOpSet(Dcm_TskTaskEvMemType                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
                 ,pContext->Diag.QueuedSet.QueuedTObjects
                 ,Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle));                                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif
}

/**********************************************************************************************************************
 *  Dcm_DiagSetNewResBaseToCurProgress()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewResBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  )
{
  if (Dcm_DebugDetectRuntimeError(pMsgContext->resIndex >= pMsgContext->resBufSize))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pMsgContext->resBufSize -= pMsgContext->resIndex;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resMaxDataLen = pMsgContext->resBufSize;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resProtHeadLen += pMsgContext->resDataLen;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resDataLen = 0;                                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_UtiNextItemByPtr(pMsgContext->resData, pMsgContext->resIndex);                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resIndex = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessorFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_DiagSvcConfirmationFuncType, DCM_CODE) Dcm_DiagGetPostProcessorFunc(
  uint8_least index
  )
{
  Dcm_DiagSvcConfirmationFuncType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_CFGDIAGSVCPOSTPROCESSORS_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = Dcm_CfgDiagSvcPostProcessors[0];
  }
  else
  {
    lResult = Dcm_CfgDiagSvcPostProcessors[index];
  }

  return lResult;
}

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetUpdateFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_DiagSvcUpdateFuncType, DCM_CODE) Dcm_DiagGetUpdateFunc(
  uint8_least index
  )
{
  Dcm_DiagSvcUpdateFuncType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_CFGDIAGSVCUPDATERS_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = Dcm_CfgDiagSvcUpdaters[0];
  }
  else
  {
    lResult = Dcm_CfgDiagSvcUpdaters[index];
  }

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInit(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  Dcm_SingletonContext.Diag.Svc2ProtocolMap = configPtr->BridgeVariant.Svc2ProtocolMap;
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  Dcm_SingletonContext.Diag.RomVariant = configPtr->RomVariant->DiagRom;
#endif


  {
    Dcm_ThreadIdOptType lThreadIter;
    for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      Dcm_ContextPtrType pContext = Dcm_GetThreadContext(lThreadIter);

      pContext->ThreadId = (Dcm_ThreadIdMemType)lThreadIter;                                                                                         /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */

      /*
      * Initialize pMsgContext so that it points always to a valid buffer.
      * From now on the buffer information can only be changed via Dcm_DiagInitiateServiceProcessing().
      * So in case that Dcm_DiagInitiateServiceProcessing() was called, but for a different (wrong) thread,
      * DCM still never writes to an invalid memory locations due to this initialization.
      */
      Dcm_DiagInitMsgContextBufferInfo(&(pContext->Diag.MsgContext), 0u);                                                                            /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */
    }
  }

  /* Initialize all processors */
  {
    uint8_least lIter;

    for(lIter = 0u; Dcm_CfgDiagSvcInitializers[lIter] != NULL_PTR; ++lIter)                                                                          /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    {
      Dcm_CfgDiagSvcInitializers[lIter]();                                                                                                           /* SBSW_DCM_CALL_FUNCPTR_TERMINATED_LIST */
    }
  }

  {
    Dcm_ThreadIdOptType lThreadIter;

    /* Initialize all P2 Timers */
    for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      Dcm_ContextPtrType pContext = Dcm_GetThreadContext(lThreadIter);

      pContext->Diag.QueuedSet.Flags = DCM_DIAG_QUEUE_FLAG_NONE;                                                                                     /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
      pContext->Diag.QueuedSet.QueuedTObjects = 0;                                                                                                   /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
#endif
      pContext->Diag.State = DCM_DIAG_STATE_IDLE;                                                                                                    /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

      Dcm_DiagSetP2Timings(0, 0, pContext->ThreadId);
    }
  }

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.RxAllowed = FALSE;
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_RECOVERY, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE);
#endif

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  /* Delegate to the first task activation */
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_SEND);
#endif
}

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagPutStartUpFblResBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagPutStartUpFblResBuffer(
  uint8_least index,
  Dcm_MsgItemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= DCM_DIAG_START_UP_FBL_RES_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[index] = value;                                                                                   /* SBSW_DCM_POINTER_WRITE_START_UP_FBL_RES_BUFFER */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagExecConfirmationFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagExecConfirmationFunc(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgDiagNotificationInfoPtrType notifList,                                                                                                      /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType confStatus
  )
{
  uint16_least lIter;

  for(lIter = 0u; notifList[lIter].ConfFunc != NULL_PTR; ++lIter)                                                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    (void)notifList[lIter].ConfFunc(pContext->Diag.PostProcessorContext.Sid
                                   ,pContext->Diag.PostProcessorContext.ReqType
                                   ,pContext->Diag.PostProcessorContext.SrcAddr
                                   ,confStatus);                                                                                                     /* SBSW_DCM_CALL_FUNCPTR_TERMINATED_LIST */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_ConfirmationStatusType, DCM_CODE) Dcm_DiagGetPostProcessResult(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ConfirmationStatusType confStatus  = DCM_DIAG_RES_ANY_OK;

  if(pContext->Diag.TxStatus != DCM_E_OK)                                                                                                            /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    /* Set Tx error flag */
    confStatus |= DCM_DIAG_RES_ANY_NOT_OK;
  }

  if(pContext->Diag.ErrorRegister != DCM_E_POSITIVERESPONSE)
  {
    /* Set neg response flag */
    confStatus |= DCM_DIAG_RES_NEG_ANY;
  }
  return confStatus;
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoverySignature()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_DiagGetRecoverySignature(
  void
  )
{
  uint32 lResult;

  lResult = Dcm_PbCfgNetNumComMChannels;

# if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
  lResult |= Dcm_UtiMake32Bit(0u, 0u, Dcm_PbCfgNetNumAllComMChannels, 0u);                                                                           /* PRQA S 2985, 2986 */ /* MD_Dcm_Redundant_2985, MD_Dcm_Redundant_2986 */
# endif

  lResult |= Dcm_UtiMake32Bit(Dcm_UtiGetHiNibble(Dcm_PbCfgNetNumProtocols) /* Since at most 32 protocols are allowed the high nibble never exceeds 1 bit */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                             ,Dcm_UtiGetLoNibble(Dcm_PbCfgNetNumProtocols)
                             ,0u
                             ,0u);

  lResult |= Dcm_UtiGetBitFromIndex(uint32, 21u);

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 22u);
# endif

# if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 23u);
# endif

# if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 24u);
# endif

# if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 25u);
# endif

  lResult |= Dcm_UtiMake32Bit(0x08, 0x00, 0u, 0u);

  lResult |= Dcm_UtiMake32Bit(0xA0, 0u, 0u, 0u);

  return lResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProvideRecoveryStates()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  )
{
  /* Retrieve ComM channel states */
  Dcm_CfgNetNetIdRefOptType lNetHandleIter;
  Dcm_RecoveryInfoComMChannelStatePtrType pChannelState;
  for(lNetHandleIter = 0; lNetHandleIter < Dcm_PbCfgNetNumComMChannels; ++lNetHandleIter)
  {
    pChannelState = Dcm_DiagGetRecoveryInfoComMChannelState(RecoveryInfo, lNetHandleIter);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    *pChannelState = (boolean)(Dcm_PbRamNetComMContext[lNetHandleIter].RegisteredNetworks != 0u);                                                    /* SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCHANNELSTATE */ /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  }

  /* Retrieve session state */
  RecoveryInfo->SessionLevel = Dcm_UtiGetBitPosition(Dcm_StateGetSession());                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve session connection and tester */
  RecoveryInfo->SessionConnection = Dcm_NetGetSessionConnection();                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  RecoveryInfo->SessionClientSrcAddr = Dcm_NetGetSessionClientSrcAddr();                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve active protocol */
  RecoveryInfo->ActiveProtocol = Dcm_NetGetProtIdOfActiveProtocol();                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve security state */
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
#  if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_StateIsSecurityBypassPending())
  {
    /* Store locked security */
    RecoveryInfo->SecurityLevel = 0u;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
#  endif
  {
    RecoveryInfo->SecurityLevel = Dcm_UtiGetBitPosition(Dcm_StateGetSecurity());                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
# endif

  /* Retrieve DTC settings state */
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
#  if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
  RecoveryInfo->ControlDTCSettingDTCGroup = Dcm_ModeGetControlDtcSettingGroup();                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
#  endif
  RecoveryInfo->ControlDTCSettingDisabled = (boolean)(Dcm_ModeGetControlDtcSettingMode() == RTE_MODE_DcmControlDtcSetting_DISABLEDTCSETTING);        /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
# endif

  /* Retrieve communication control state */
# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
  {
    uint8_least lChannelIter;
    Dcm_RecoveryInfoCommControlStatePtrType pControlState;
    for(lChannelIter = 0; lChannelIter < Dcm_PbCfgNetNumAllComMChannels; ++lChannelIter)
    {
      pControlState = Dcm_DiagGetRecoveryInfoCommControlState(RecoveryInfo, lChannelIter);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      *pControlState = Dcm_ModeGetCommControlState(lChannelIter);                                                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCONTROLSTATE */
    }
  }
# endif

  RecoveryInfo->Signature = Dcm_DiagGetRecoverySignature();                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProcessRecoveryInfo()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if (opStatus == DCM_INITIAL)
  {
    /* If a non-default session was active: */
    if(RecoveryInfo->SessionLevel != 0u)
    {
      /* Activate ComM channels */
      Dcm_CfgNetNetIdRefOptType lNetHandleIter;
      for(lNetHandleIter = 0; lNetHandleIter < Dcm_PbCfgNetNumComMChannels; ++lNetHandleIter)
      {
        if(RecoveryInfo->ComMChannelState[lNetHandleIter] == TRUE)
        {
          ComM_DCM_ActiveDiagnostic(Dcm_PbCfgNetAllComMChannelMap[Dcm_PbCfgNetConnComMChannelMap[lNetHandleIter]]);
        }
      }

      /* Recover session state */
      Dcm_StateSetSession(RecoveryInfo->SessionLevel);

      /* Recover active protocol */
      Dcm_SingletonContext.Network.ActiveProtocol = RecoveryInfo->ActiveProtocol;

      /* Update P2 timings at once */
      Dcm_DiagSetP2Timings(RecoveryInfo->SessionLevel, Dcm_NetGetProtIdOfActiveProtocol(), Dcm_NetGetThreadIdOfActiveProtocol());

      /* Recover session connection */
      Dcm_NetSetSessionConnection(RecoveryInfo->SessionConnection);
      Dcm_NetSetSessionClientSrcAddr(RecoveryInfo->SessionClientSrcAddr);

      Dcm_TmrStartTimer(DCM_TMR_ID_S3, DCM_DIAG_TIME_S3);
    }

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
    /* If any security level was unlocked: */
    if(RecoveryInfo->SecurityLevel != 0u)
    {
      /* Recover security state */
      Dcm_StateSetSecurity(RecoveryInfo->SecurityLevel);
    }
# endif
  }

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  /* If DTC settings were disabled: */
  if(RecoveryInfo->ControlDTCSettingDisabled == TRUE)
  {
    /* Recover DTC updating state */
    lStdResult = Dcm_Svc85DisableDtcSetting(
#  if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
                                            RecoveryInfo->ControlDTCSettingDTCGroup
#  else
                                            DEM_DTC_GROUP_ALL_DTCS
#  endif
                                           ,Dcm_NetGetProtObjOfProtId(Dcm_NetGetProtIdOfActiveProtocol())->DemClientId                               /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */
                                           );

  }
# endif

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
#  if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  /* If no DTC settings update was necessary or it succeeded */
  if (lStdResult == DCM_E_OK)
#  endif
  /* Recover communication control states */
  {
    uint8_least lChannelIter;
    for(lChannelIter = 0; lChannelIter < Dcm_PbCfgNetNumAllComMChannels; ++lChannelIter)
    {
      if(RecoveryInfo->CommControlState[lChannelIter] != DCM_ENABLE_RX_TX_NORM_NM)
      {
        Dcm_ModeSwitchCommControl(lChannelIter, RecoveryInfo->CommControlState[lChannelIter]);
      }
    }
  }
# endif
  return lStdResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSafeProcessRecoveryInfo()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSafeProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  )
{
  uint8          lErrorId   = DCM_E_NO_ERROR;
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(RecoveryInfo->Signature != Dcm_DiagGetRecoverySignature()))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(RecoveryInfo->SessionLevel >= DCM_STATE_NUM_SESSION))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(RecoveryInfo->ActiveProtocol >= DCM_NET_INVALID_PROTID) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(RecoveryInfo->SessionConnection >= DCM_NET_INVALID_CONNHDL) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(Dcm_NetGetProtIdOfConnection(RecoveryInfo->SessionConnection) != RecoveryInfo->ActiveProtocol) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(Dcm_DebugDetectError(RecoveryInfo->SecurityLevel >= DCM_STATE_NUM_SECURITY))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = Dcm_DiagProcessRecoveryInfo(RecoveryInfo, opStatus);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETRECOVERYSTATES, lErrorId);

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSetSpecificCauseCode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  )
{
  if(Dcm_SingletonContext.Diag.CauseCodeContext.SendCauseCode == FALSE)
  {
    Dcm_SingletonContext.Diag.CauseCodeContext.CauseCode = specificCauseCode;
    Dcm_SingletonContext.Diag.CauseCodeContext.SendCauseCode = TRUE;
  }
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsProcessingContextSwitchAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsProcessingContextSwitchAllowed(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* No (nested) kill job received? */
  if ((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pContext->ThreadId)) != 0u)
  {
    lStdResult = DCM_E_NOT_OK; /* skip any further processing in this Dcm_DiagTaskWorker() activation (i.e. return immediately) */
  }
# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  else /* No (nested) kill job received for any other thread? */
  {
    Dcm_ThreadIdOptType lThreadIter;

    for (lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      Dcm_ContextConstPtrType lContext = Dcm_GetThreadContext(lThreadIter);

      if ((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, lContext->ThreadId)) != 0u)
      {
        lStdResult = DCM_E_PENDING;
        break;
      }
    }
  }
# endif

  return lStdResult;
}
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsSessionChangeAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsSessionChangeAllowed(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* If a non-default session will be active: */
  if(Dcm_StateIsDefaultSessionPending() == FALSE)
  {
    Dcm_ThreadIdOptType lThreadIter;

    for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      /* Skip own thread */
      if(lThreadIter != pContext->ThreadId)
      {
        Dcm_ContextConstPtrType otherContext = Dcm_GetThreadContext(lThreadIter);

        if( (otherContext->Diag.State != DCM_DIAG_STATE_IDLE)
          ||(otherContext->Diag.QueuedSet.Flags != DCM_DIAG_QUEUE_FLAG_NONE) )
        {
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }
  }

  return lStdResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcCancelOpenJobs()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcCancelOpenJobs(
  Dcm_ContextPtrType pContext
  )
{
  /* #10 If any diagnostic job still open (PagedBuffer transfers are not part of them), call the job handle one last time with DCM_CANCEL signal */
  if(pContext->Diag.State == DCM_DIAG_STATE_PROCESS)
  {
    Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */
    pContext->Repeater.OpStatus = DCM_CANCEL;/* Override any other opStatus values up to now */                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    (void)Dcm_RepeaterExecute(pContext, &lNrc);/* signal cancellation */                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_SetNegResponse(lNrc, pContext->ThreadId); /* register negative result */
  }

  /* #20 If any diagnostic job still open (including PagedBuffer transfers) or already on transmission: */
  if(((DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX) & pContext->Diag.State) != 0u) /* any open diagnostic job? (optimize interrupt lock times) */
  {
    /* Immediately cancel any ongoing transmission (RCR-RP, final response (RoE or external)) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    if(((DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX) & pContext->Diag.State) != 0u) /* STILL any open diagnostic job -> diagnostic transport object is still in use -> try cancel it */ /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      Dcm_NetTransportObjectPtrType pTranspObj;

      pTranspObj = Dcm_DiagGetTranspObj(pContext);

      /* #30 If any USDT response transmission is still ongoing (incl. RCR-RPs): */
      if(pTranspObj->State == DCM_NET_TOBJ_STATE_ONTX)
      {
        pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED;/* isolate this connection */                                                                /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        /* Short cut to speed up the service processing finalization - RCR-RP responses will just end with the cancellation below */
        Dcm_DiagTxFinalization(pTranspObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

        /* Try cancel any ongoing response. The return value does not matter:
          * OK -> there will be a confirmation for this cancellation
          * N_OK -> request rejected, but there will be a confirmation due to the ongoing transmission */
        (void)PduR_DcmCancelTransmit(Dcm_PbCfgNetConnectionInfo[pTranspObj->ConnHdl].TxPduIdMain);
        /* Any response (final, RCR-RP, etc) will just end with releasing the connection assigned to the tObject! No confirmation post-processing will be performed! */
      }
      /* Otherwise, if there is no USDT response initiated yet, just notify the diagnostic kernel for the transmission finalization to accomplish the diagnostic job cycle */
      else if (pTranspObj->State == DCM_NET_TOBJ_STATE_READY)                                                                                        /* COV_DCM_RTM_UNREACHABLE TX */
      {
        /* no transmission yet started -> just loop back the diagnostic confirmation immediately. Note: no other Tx-confirmation type is expected here */
        Dcm_DiagTxFinalization(pTranspObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }
      else
      {
        /* Otherwise, unexpected transport object state reached (inconsistent in the context of diagnostic kernel state, showing a job is still in progress) */
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      }
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
  /* Otherwise, diagnostic kernel is in "post processing" or "idle" state and they are to be handled in a normal way
         Note: This case can be reached since the prioritization just fires a cancellation event, disregarding any internal states of the diagnostic kernel */
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcReviveKilledTasks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcReviveKilledTasks(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pEventContext);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* All cancellation activities are done now - revive all killed tasks to allow normal processing of the newly received diagnostic job (the one caused the interruption) */
  /* Look ahead: Any nested kill job received? If so, no task revival! */
  if((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pContext->ThreadId)) == 0u)                                     /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
  {
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
    Dcm_TskTaskEvOptType queuedTObjBitSet = 0; /* nothing to clean (for now) */
#endif
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
     /* STILL no nested kill job received? */
    if((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pContext->ThreadId)) == 0u)                                   /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      Dcm_TskReviveAllTasks(pContext->ThreadId);/* from now on enable tasks for working */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
      queuedTObjBitSet = pContext->Diag.QueuedSet.QueuedTObjects;

      /* Clear all except the last queued one  */
      Dcm_UtiBitOpClr(Dcm_TskTaskEvOptType, queuedTObjBitSet, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, Dcm_DiagGetQueuedTranspObj(pContext)->Handle));

      /* Remove all processed transport objects from the queue (the last queued one will be removed from the queue once transfered to the active tObject or canceled by higher prio request here again) */
      Dcm_UtiBitOpClr(Dcm_TskTaskEvMemType, pContext->Diag.QueuedSet.QueuedTObjects, queuedTObjBitSet);                                              /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
    if(queuedTObjBitSet > 0u)
    {
      /* Some connections to clean up (in background)? */
      Dcm_TskSetEvent(DCM_TSK_ID_DIAG_GARB_COL, (Dcm_TskTaskEvMemType)queuedTObjBitSet);
    }
#endif
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcessing(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_DiagWorkerDoCancelProcCancelOpenJobs(pContext); /* Cancel open diagnostic jobs */                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Sync with TxEnd events set above OR Tx confirmation. No ISR lock since:
   * - No Tx confirmation can occur from now on
   * - If above Tx confirmation executed -> no other changes of the state are possible!
   * - If there was already a Tx Confirmation (above code not executed) -> the task will just be re-triggered
   */
  /* If the Dcm_DiagTaskWorker() has (by finished transmission or through the above cancellation activity) already received a post-processing event: */
  if((DCM_TSK_EV_DIAG_WORK_TX_END_FINAL & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pContext->ThreadId)) != 0u)
  {
    Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL, pContext->ThreadId);/* clear any potentially globally set Tx-end-response (from the above Tx cancellation code) */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL);/* set/transfer the Tx end event */                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* Otherwise, either no Tx-end event OR nothing to do OR it is already set in the local "ev" -> will be post-processed in next task stages */

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON) && (DCM_MULTI_THREAD_ENABLED == STD_ON)
  /* Prevent that a pending repeater post event is executed the next main function cycle for the currently killed job */
  Dcm_TskClrLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
#endif

  Dcm_DiagWorkerDoCancelProcReviveKilledTasks(pContext, pEventContext); /* Revive killed tasks */                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoPostProcessing()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoPostProcessing(
  Dcm_ContextPtrType pContext
  )
{
  Dcm_ConfirmationStatusType   confStatus;
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  /* Reset any pending PB processing */
  if(Dcm_PagedBufferCancel(pContext, &lNrc) == DCM_E_NOT_OK)                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    Dcm_SetNegResponse(lNrc, pContext->ThreadId);/* Set NRC now to affect the post-processor confirmation status */
  }
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  Dcm_RsrcMgrReleaseServiceLock(pContext->ThreadId);
#endif

  confStatus = Dcm_DiagGetPostProcessResult(pContext);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* First process any diagnostic service post-handler (e.g. to perform any state transitions at this stage): */
  if((DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR & pContext->Diag.ApplNotification) != 0u)
  {
    Dcm_DiagSvcConfirmationFuncType lPostProcessorFunc;
    lPostProcessorFunc = Dcm_DiagGetPostProcessorFunc(Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].ConfFuncRef);

    lPostProcessorFunc(pContext, confStatus);/* execute any internal service processor post-handlers */                                              /* SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS */
  }

  /* Process the application notification confirmation */
  if((DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION & pContext->Diag.ApplNotification) != 0u)
  {
    Dcm_Confirmation(0, pContext->Diag.MsgContext.rxPduId, confStatus);/* notify the application about transmission end (of any service) */
  }

  /* At next process any system supplier and OEM notification (e.g. to perform any additional state transitions at this stage): */
  if((DCM_DIAG_APPL_NOTIFICATION_SYS & pContext->Diag.ApplNotification) != 0u)
  {
    Dcm_DiagExecConfirmationFunc(pContext, Dcm_CfgDiagSysNotificationInfo, confStatus);                                                              /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }

  if((DCM_DIAG_APPL_NOTIFICATION_OEM & pContext->Diag.ApplNotification) != 0u)
  {
    Dcm_DiagExecConfirmationFunc(pContext, Dcm_CfgDiagOemNotificationInfo, confStatus);                                                              /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if(pContext->Diag.IsInternRequest == FALSE)/* manage ComM states only on processed requests from external clients */
#endif
  {
    /* Process here to take a potentially new session change into account */
    Dcm_NetUnRegisterComMActivity(Dcm_NetGetConnHdlOfRxPduId(pContext->Diag.MsgContext.rxPduId));                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  }
  pContext->Diag.State = DCM_DIAG_STATE_IDLE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetDefSessionExtern()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetDefSessionExtern(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* If diagnostic kernel is still busy with a diagnostic job, retry next task activation if there is no other such request yet */
  if(Dcm_DiagIsKernelBusy())
  {
    if(!Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF))                                                                 /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY XX */
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF); /* retry later again */                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    /* Redirect the external request for resetting the session to the internal request handler (same activity) */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF);                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    /* If non-default session is active the current protocol id is unequal to DCM_NET_INVALID_PROTID */
    if(!Dcm_StateIsDefaultSessionActive())
    {
      Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfActiveProtocol(), pContext->ThreadId);
    }/* else - P2 timings of default session already set */

#if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
    Dcm_SingletonContext.Network.RxAllowed = TRUE;
#endif
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagSwitchProcessingContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSwitchProcessingContext(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
#if (DCM_SPLIT_TASKS_ENABLED == STD_OFF)
  lStdResult = DCM_E_OK;
#else
  lStdResult = Dcm_DiagIsProcessingContextSwitchAllowed(pContext);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
#endif
  {
    Dcm_NetTransportObjectConstPtrType pQueuedTObj;

    pQueuedTObj = Dcm_DiagGetQueuedTranspObj(pContext);

    Dcm_UtiBitOpClr(Dcm_DiagProcessorFlagType, pContext->Diag.QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_WAITING);                                      /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_UtiBitOpSet(Dcm_DiagProcessorFlagType, pContext->Diag.QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_ACTIVE);                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_DiagSetTranspObj(pContext, pQueuedTObj); /* switch work contexts! RCR-RP will be sent also from this one (QueuedCtxt) */                     /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_GARB_COL_ENABLED ==STD_ON)
    Dcm_UtiBitOpClr(Dcm_TskTaskEvMemType, pContext->Diag.QueuedSet.QueuedTObjects, (Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pQueuedTObj->Handle))); /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_SplitTaskLeaveCS();

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagInitMsgContextBufferInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitMsgContextBufferInfo(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

  lBufferInfo = Dcm_NetGetBufferInfo(index);

  pMsgContext->reqData = &(lBufferInfo->BufferPtr[0]);                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resData = &(lBufferInfo->BufferPtr[0]);                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqDataLen = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resDataLen = 0u;                                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqBufSize = lBufferInfo->Size;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resBufSize = lBufferInfo->Size;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resMaxDataLen = lBufferInfo->Size;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resProtHeadLen = 0;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqIndex = 0u;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resIndex = 0u;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagInitiateServiceProcessing()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitiateServiceProcessing(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectConstPtrType pTranspObj;
  Dcm_MsgContextPtrType              pMsgContext;

  /* Now take the active transport object */
  pTranspObj = Dcm_DiagGetTranspObj(pContext);
  pMsgContext = &(pContext->Diag.MsgContext);

  /* Compose message context (skip SID byte) */
  Dcm_DiagInitMsgContextBufferInfo(pMsgContext, pTranspObj->BufferHdl);                                                                              /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */

  pMsgContext->reqDataLen = pTranspObj->RxLength;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->rxPduId = pTranspObj->RxPduId;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->msgAddInfo.reqType  = (uint8)((Dcm_PbCfgNetRxPduInfo[pTranspObj->RxPduId].IsFuncReq == TRUE)?1u:0u);                                  /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->msgAddInfo.suppressPosResponse = 0;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->threadId = pContext->ThreadId;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Update diagnostic kernel context */
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  pContext->Diag.IsInternRequest    = (boolean)((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u);                                             /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */ /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
  pContext->Diag.ErrorRegister      = DCM_E_POSITIVERESPONSE;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->Diag.ApplNotification   = DCM_DIAG_APPL_NOTIFICATION_NONE;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->Diag.DoSuppressResponse = FALSE;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->Diag.State              = DCM_DIAG_STATE_PROCESS;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
  /* Initialize cause code context */
  Dcm_SingletonContext.Diag.CauseCodeContext.CauseCode = 0x00;
  Dcm_SingletonContext.Diag.CauseCodeContext.SendCauseCode = FALSE;
#endif

  /* Initialize IdsM security event */
  Dcm_DebugResetSecurityEventStatus(pContext->ThreadId);

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  Dcm_PagedBufferInitByThread(pContext->ThreadId);
#endif
}

/**********************************************************************************************************************
 *  Dcm_DiagHandleSuppressBit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleSuppressBit(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check if the new service supports sub-function parameter */
  if((Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].Props & DCM_DIAG_SVC_CFG_PROP_HAS_SUBFUNC)  != 0u)
  {
    uint8 reqSf = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                                /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
    /* If so, deal with the SPRMIB: extract and store */
    if ((reqSf & 0x80u) != 0u)
    {
      Dcm_UtiSetReqDataAsU8(pMsgContext, (uint8)(reqSf & 0x7Fu));/* mask out the SPRMIB */                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_SplitTaskEnterCS();/* protect against hi-priority Dcm_DiagTaskRx/Tx */
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
      /* On split task it could happen that RCR-RP is already on transmission */
      if((pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_ON_TX | DCM_NET_TOBJ_FLAG_RCRRP_SENT)) == 0u)/* RCR-RP is NOT on transmission or was sent */
#endif
      {
        pMsgContext->msgAddInfo.suppressPosResponse = 1;                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_SplitTaskLeaveCS();
    }
    /* copy the sub-function byte once split Rx/Tx buffers are supported.
    pMsgContext->resData[0] = pMsgContext->reqData[0];
    */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagHandleDispatching()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleDispatching(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(sid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pContext->Diag.ProcessServiceExtern = FALSE;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_SERVICE_DISPATCHER_ENABLED == STD_ON)
  /*  Check for necessity of service dispatching (service supports the dispatching parameter) */
  if((Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].Props & DCM_DIAG_SVC_CFG_PROP_CALL_SVC_DISPATCHER) != 0u)
  {
    boolean lResult;

    /* Execute the call-out to the application and verify the result */
    lResult = Dcm_HandleServiceExtern(sid
                                     ,pMsgContext->reqData
                                     ,(uint16)pMsgContext->reqDataLen
                                     ,(uint8) pMsgContext->msgAddInfo.reqType
                                     ,Dcm_DiagGetTranspObj(pContext)->ClientSrcAddr);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_DebugDetectError((lResult != TRUE) && (lResult != FALSE)))
    {
      Dcm_DebugReportError(DCM_SID_SVCDISPATCHER, DCM_E_INTERFACE_RETURN_VALUE);                                                                     /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
    {
      pContext->Diag.ProcessServiceExtern = lResult;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  /* Set the flags for post processing on internal service handling */
  if(pContext->Diag.ProcessServiceExtern == FALSE)
#endif
  {
    if(Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].ConfFuncRef != 0u)
    {
      pContext->Diag.ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR; /* mark application post processor */                             /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    if(Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].FastConfFuncRef != 0u)
    {
      pContext->Diag.ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR; /* mark application fast post processor */                    /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  /* From now on, mark application calls for post-processing later */
  pContext->Diag.ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION; /* mark application notification at last here */                       /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagValidateAndDispatchService()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagValidateAndDispatchService(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  Dcm_MsgItemType lSid;

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lSid);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiProvideResDataAsU8(pMsgContext, (uint8)(lSid + 0x40u));                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Skip SID data, since already processed */
  Dcm_DiagSetNewReqBaseToCurProgress(pMsgContext);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_DiagSetNewResBaseToCurProgress(pMsgContext);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Execute manufacturer specific environment checks */
  if(Dcm_DiagExecIndicationFunc(pContext, lSid, DCM_DIAG_APPL_NOTIFICATION_OEM, Dcm_CfgDiagOemNotificationInfo, ErrorCode) == DCM_E_OK)              /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    sint16_least sidIter = Dcm_DiagLookUpFilter(pMsgContext
                                               ,Dcm_CfgDiagSvcIdLookUpTable
                                               ,Dcm_CfgDiagSvcIdExecPrecondTable
                                               ,lSid
                                               ,DCM_DIAG_CHK_LVL_SERVICE_ID
                                               ,ErrorCode);                                                                                          /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_COMB_PTR_FORWARD */
    /* Service found? */
    if(sidIter >= 0)
    {
      pContext->Diag.SidIndex = (uint8)sidIter;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

      if((Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].Props & DCM_DIAG_SVC_CFG_PROP_CALL_POST_HDLR_ALWAYS)  != 0u)
      {
        /* mark application notification once the SID is identified! */
        pContext->Diag.ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR;                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      }

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
      /* Check authentication access rights */
      lStdResult = Dcm_AuthMgrCheckDispatcherLevel(lSid
                                                  ,Dcm_CfgDiagSvcIdExecPrecondTable[pContext->Diag.SidIndex]
                                                  ,pMsgContext
                                                  ,Dcm_NetGetAuthInfoRefByMsgContext(pMsgContext)
                                                  ,ErrorCode);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

      if(lStdResult == DCM_E_OK)
#endif
      {
        /* Check SID pre-conditions */
        lStdResult = Dcm_StateCheck(Dcm_CfgDiagSvcIdExecPrecondTable[pContext->Diag.SidIndex]
                                   ,DCM_DIAG_CHK_LVL_SERVICE_ID
                                   ,ErrorCode);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

        if(lStdResult == DCM_E_OK)
        {
          /* Execute supplier specific environment checks */
          lStdResult = Dcm_DiagExecIndicationFunc(pContext, lSid, DCM_DIAG_APPL_NOTIFICATION_SYS, Dcm_CfgDiagSysNotificationInfo, ErrorCode);        /* SBSW_DCM_COMB_PTR_FORWARD */

          if(lStdResult == DCM_E_OK)
          {
            /* Check for available minimum length */
            if(pMsgContext->reqDataLen >= Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].MinLength)
            {
              /* Handle SPRMIB */
              Dcm_DiagHandleSuppressBit(pContext, Dcm_DiagGetTranspObj(pContext), pMsgContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

              /* Handle service dispatching and set the flags for post-processing */
              Dcm_DiagHandleDispatching(lSid, pMsgContext, pContext);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */
            }
            else
            {
              /* Minimum length is not reached  */
              *ErrorCode = (Dcm_NegativeResponseCodeType)( (lSid < 0x10u)
                                                          ? DCM_E_REQUESTOUTOFRANGE
                                                          : DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);                                            /* SBSW_DCM_PARAM_PTR_WRITE */
              lStdResult = DCM_E_NOT_OK;
            }
          } /* else - NRC already set - finalize processing */
        }/* else - NRC already set - finalize processing */
      }/* else - NRC already set - finalize processing */
    }/* else - already written "Unsupported service requested" */
  }/* else - NRC already set - finalize processing */

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessNewRequest()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessNewRequest(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType               lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */

  lStdResult = Dcm_DiagSwitchProcessingContext(pContext);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    Dcm_DiagInitiateServiceProcessing(pContext);                                                                                                     /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */

    /* Validate received diagnostic service on SID level */
    lStdResult = Dcm_DiagValidateAndDispatchService(&(pContext->Diag.MsgContext)
                                                   ,pContext
                                                   ,&lNrc);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)
    {
      /* Execute Main-Handler (register it and it will be called immediately) */
      Dcm_RepeaterSetCallee(pContext, pContext->Diag.SidIndex);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      /* Let the service processor take over immediately */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* DCM_E_NOT_OK */
      Dcm_SetNegResponse(lNrc, pContext->ThreadId);
      Dcm_ProcessingDone(pContext->ThreadId);
    }
  }
  else if (lStdResult == DCM_E_PENDING)
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_NEW_REQ); /* Retry next time */                                                 /* PRQA S 2880 */ /* MD_MSR_Unreachable */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else /* DCM_E_NOT_OK */
  {
    /* skip any further processing in this Dcm_DiagTaskWorker() activation (i.e. return immediately) */
    pEventContext->Ev = 0;                                                                                                                           /* PRQA S 2880 */ /* MD_MSR_Unreachable */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessRcrRpTxEnd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessRcrRpTxEnd(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetTranspObj(pContext);

  if((DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL & pTranspObj->Flags) != 0u)/* this is a RCR-RP response - check which kind */
  {
    Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL);                                           /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pContext->Repeater.OpStatus = (Dcm_OpStatusType)((pContext->Diag.TxStatus != DCM_E_OK)
                                        ? DCM_FORCE_RCRRP_NOT_OK
                                        : DCM_FORCE_RCRRP_OK);                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT); /* notify immediately */                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerCancelOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerCancelOperation(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NegativeResponseCodeType lNrc;

  DCM_IGNORE_UNREF_PARAM(pEventContext);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_PagedBufferCancel(pContext, &lNrc) == DCM_E_NOT_OK)                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    /* Stop polling and wait for confirmation */
    Dcm_SetNegResponse(lNrc, pContext->ThreadId);
    Dcm_ProcessingDone(pContext->ThreadId);
  }
}
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetSecurityBypass()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetSecurityBypass(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Enforce bypass mode if no service is in processing! */
  if(Dcm_DiagIsKernelBusy())
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS); /* retry later again */                                      /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    if(Dcm_StateIsSecurityBypassPending())
    {
      /* Unlock security (excluding locked level = 0x00) */
      Dcm_StateSetSecurityBypass();
    }
    else
    {
      /* Lock security */
      Dcm_StateSetSecurity(0);
    }
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoRepeat()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoRepeat(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_CONDITIONSNOTCORRECT;
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetTranspObj(pContext);

#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_CANCEL_OP))
  {
    pContext->Repeater.OpStatus = DCM_CANCEL; /* override any other opStatus values up to now */                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  lStdResult = Dcm_RepeaterCheckAndGetAccess(pContext, &lNrc);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
#endif
  {
    lStdResult = Dcm_RepeaterExecute(pContext, &lNrc);                                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  switch (lStdResult)
  {
  case DCM_E_NOT_OK:
    Dcm_SetNegResponse(lNrc, pContext->ThreadId);
    /* fall through */
  case DCM_E_OK:                                                                                                                                     /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
  case DCM_E_PROCESSINGDONE:
    Dcm_ProcessingDone(pContext->ThreadId);
    break;
  case DCM_E_PENDING:
    pContext->Repeater.OpStatus = DCM_PENDING;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    /* fall through - keep the last opStatus on "pending" forced by Dcm_RepeaterExecute code */
  case DCM_E_PENDING_LIMITER:                                                                                                                        /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
    /* Retry next cycle with the same parameter */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case DCM_E_FORCE_RCRRP:
    Dcm_UtiEnterCriticalSection();/* protect against Dcm_TpTxConfirmation or hi-priority Dcm_DiagTaskRx */
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL;                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    if ((pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_ON_TX | DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ)) == 0u)/* no RCR-RP already requested for transmission/ongoing? */
    {
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_TMR_P2_TO, pContext->ThreadId);/* trigger a RCR-RP event timeout */
    }/* else - just wait for confirmation signal */

    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();
    break;
  default: /* DCM_E_STOP_REPEATER */
    Dcm_RepeaterSetCallee(pContext, DCM_DIAG_SERVICEINFO_REPEATEREND_IDX); /* any other return value leads to a dead end */                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoGetProgCond()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoGetProgCond(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Read the programming conditions of the pre-boot cycle */
  /* #10 If the ECU boot type is marked as a transition from the bootloader: */
  if(Dcm_GetProgConditions(&Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions) == DCM_WARM_START)                                                  /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  {
    sint16_least sesSfIter = -1;

# if (DCM_DIAG_NOTIF_BSWM_APPL_UPDATED_ENABLED == STD_ON)
    /* #20 If the application has been updated, notify the BswM (if enabled BswM notification) */
    if(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ApplUpdated == TRUE)
    {
      BswM_Dcm_ApplicationUpdated();
    }
# endif

    /* Check the diagnostic service identifier to be processed here: */
# if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
    /* #30 If the SID is for "EcuReset": */
    if(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.Sid == 0x11u)
    {
      sint16_least subFncIter = -1;

      subFncIter = Dcm_DiagVariantLookUpFilter(Dcm_CfgSvc11SubFuncLookUpTable
                                              ,Dcm_CfgSvc11SubFuncExecPrecondTable
                                              ,Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.SubFuncId);                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

      /* Check if reset was internally triggered */
      if( (subFncIter >= 0)
        &&(Dcm_CfgSvc11SubFuncInfo[subFncIter].ResponseType == DCM_SVC11_RESPONSE_AFTER_RESET) )
      {
        Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_ECU_RESET);
      }
    }
# endif

    /* #40 If the SID is for "DiagnosticSessionControl": */
    if(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.Sid == 0x10u)
    {
      sesSfIter = Dcm_DiagVariantLookUpFilter(Dcm_CfgSvc10SubFuncLookUpTable
                                             ,Dcm_CfgSvc10SubFuncExecPrecondTable
                                             ,Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.SubFuncId);                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    }
    /* Otherwise, send a response with any other SID and sub-function directly */

    if( (Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ResponseRequired == TRUE) /* a response is to be sent */
      ||(sesSfIter > 0) ) /* OR a non-default session is to be activated (need ComM active) */
    {
      Dcm_NetConnRefMemType lConnHdl;

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      lConnHdl = Dcm_NetGetConnHdlByConnectionId(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ConnectionId);
# else
      lConnHdl = Dcm_NetGetConnHdlByTesterAddress(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.TesterSourceAddr);
# endif

      if( (lConnHdl < DCM_NET_INVALID_CONNHDL)
# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
        &&( ( (Dcm_NetIsGenericConnection(lConnHdl))
            &&(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.TesterSourceAddr <= DCM_NET_MAX_VAL_GENERIC_SRC_ADDRESS) )
          ||( (!Dcm_NetIsGenericConnection(lConnHdl))
            &&(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.TesterSourceAddr == Dcm_NetGetTesterAddress(lConnHdl)) ) )
# endif
         )
      {
        Dcm_NetTransportObjectPtrType pTranspObj; /* use a local copy to avoid collision with a RxIndication! */
        BufReq_ReturnType             lResult;

        lResult = Dcm_NetLockConnection(lConnHdl, &pTranspObj);                                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

        if(lResult == BUFREQ_OK) /* try to lock the connection to this tester */
        {
          Dcm_SingletonContext.Diag.StartUpFbl.ConnHdl = pTranspObj->ConnHdl;
          /* Source address of the request shall be the target address of the response. */
          pTranspObj->ClientSrcAddr = Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.TesterSourceAddr;                                          /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
          Dcm_DiagSetTranspObj(pContext, pTranspObj);/* store working context */                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          Dcm_TmrStartTimer(DCM_TMR_ID_FBLRES, pContext->Diag.P2Timings.P2Star); /* wait at most P2 star to finalize the FBL positive response */
          /* Start communication */
          Dcm_NetRegisterActiveConnection(pTranspObj);                                                                                               /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
          Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
          Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM);/* Try immediately */                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        /* else - there is a problem locking the channel (there is already a tester request) */
      }
      else
      {
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_PARAM);
      }
    }
    /* In any case store any analysis results for later use (i.e. during the clear response required flag) */
    Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef = (sint8)sesSfIter;
    Dcm_SingletonContext.Diag.StartUpFbl.IsResponseRequired = Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ResponseRequired;
    /* Signal the current task to start immediately the parallel job for clearing the response required flag, to avoid re-triggering of this sequence at next power on cycle */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL */ /* MD_MSR_STMIF */
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoWaitTxComm()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoWaitTxComm(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetTranspObj(pContext);

  /* If the corresponding ComM channel state is in "FullComMode": */
  if((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByTranspObject(pTranspObj)) != 0u)
  {
    Dcm_TmrStopTimer(DCM_TMR_ID_FBLRES);

    if(Dcm_SingletonContext.Diag.StartUpFbl.IsResponseRequired == FALSE) /* is final response required ? */
    {
      Dcm_DiagOnFblResTxFinished(pTranspObj, DCM_E_OK);/* Loop-back to the confirmation (successful "virtual" transmission) to clean up transportObject and ComM */ /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    }
    else
    {
      pTranspObj->State   = DCM_NET_TOBJ_STATE_PREPTX;                                                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_UNSOLICITED;                                                                                        /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduDataPtr = &Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[0];                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduLength  = 2u; /* only service and a sub-function to be sent */                                                         /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      /* Prepare final response to be sent */
      Dcm_DiagPutStartUpFblResBuffer(0, (uint8)(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.Sid + 0x40u));
      Dcm_DiagPutStartUpFblResBuffer(1, Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.SubFuncId);

      if(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.Sid == 0x10u)
      {
        if(Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef >= 0)
        {
          Dcm_DiagPutStartUpFblResBuffer(2, Dcm_UtiGetHiByte(Dcm_CfgSvc10SubFuncInfo[Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef].ResponseTime.P2));
          Dcm_DiagPutStartUpFblResBuffer(3, Dcm_UtiGetLoByte(Dcm_CfgSvc10SubFuncInfo[Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef].ResponseTime.P2));
          Dcm_DiagPutStartUpFblResBuffer(4, Dcm_UtiGetHiByte(Dcm_CfgSvc10SubFuncInfo[Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef].ResponseTime.P2Star));
          Dcm_DiagPutStartUpFblResBuffer(5, Dcm_UtiGetLoByte(Dcm_CfgSvc10SubFuncInfo[Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef].ResponseTime.P2Star));
          pTranspObj->BuffInfo.SduLength = 6u;                                                                                                       /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
        else
        {
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_PARAM);
        }
      }/* else - for SID 0x11 or other it is not critical to send any response - it will be up to the application to use the API! */

      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
    }
  }
  else
  { /* Otherwise, check if still shall wait for ComM (deadline not yet expired): */
    if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_CANCEL))/* stop polling since time is up */
    {
      /* Loop-back to the confirmation (failed transmission) to clean up transportObject and ComM */
      Dcm_DiagOnFblResTxFinished(pTranspObj, DCM_E_NOT_OK);                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    }
    else
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM); /* Retry next time */                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoResetFlags()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoResetFlags(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Reset all programming condition fields to their inactive states */
  Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ReprogrammingRequest = FALSE;
  Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ApplUpdated      = FALSE;
  Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.ResponseRequired = FALSE;

  /* Write the updated pre-conditions */
  lStdResult = Dcm_SetProgConditions(&Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions);                                                          /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  switch(lStdResult)
  {
  case DCM_E_OK:
    break;
  case DCM_E_PENDING:
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS);/* Retry next time */                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:                                                                                                                                           /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoProcessTxConfirmation()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoProcessTxConfirmation(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pEventContext);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the final response was for a non-default session and was successfully sent: */
  if( (Dcm_SingletonContext.Diag.StartUpFbl.TxStatus == DCM_E_OK) /* only if successfully sent response ... */
    &&(Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef > 0) )/* ... of diagnostic session control for a non-default session ... */
  {
    /* Enter critical section (Reason: Protect against Dcm_StartOfReception) */
    Dcm_UtiEnterCriticalSection();

    /* Register the diagnostic client for this session */
    Dcm_NetSetSessionConnection(Dcm_SingletonContext.Diag.StartUpFbl.ConnHdl); /* set connection for the session */
    Dcm_NetSetSessionClientSrcAddr(Dcm_SingletonContext.Diag.StartUpFbl.ProgConditions.TesterSourceAddr); /* set tester for the session */

    /* Register new active protocol */
    Dcm_SingletonContext.Network.ActiveProtocol = Dcm_NetGetProtIdOfConnection(Dcm_SingletonContext.Diag.StartUpFbl.ConnHdl);

    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    /* Perform the corresponding session transition */
    Dcm_StateSetSession((Dcm_StateIndexMemType)Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef);

    /* Update P2 timings at once */
    Dcm_DiagSetP2Timings((Dcm_StateIndexMemType)Dcm_SingletonContext.Diag.StartUpFbl.SessStateRef
                        ,Dcm_NetGetProtIdOfActiveProtocol()
                        ,pContext->ThreadId);
  }
  /* Unregister diagnostic client in order to consider any non-default session activated above */
  Dcm_NetUnRegisterComMActivity(Dcm_SingletonContext.Diag.StartUpFbl.ConnHdl);
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagTxPostponeFinalPagedResponse()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxPostponeFinalPagedResponse(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Wait for RCR-RP if SPRMIB was set */
  if (pContext->Diag.MsgContext.msgAddInfo.suppressPosResponse != 0u)
  {
    /* wait for P2 timeout and RCR-RP response transmission */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_TX_SEND_PAGED); /* retry later */                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_TskClrLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED); /* avoid transmit attempt now */                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagTxStartFinalResponse()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartFinalResponse(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetTranspObj(pContext);

  /* kill any pending RCR-RP orders */
  Dcm_TskClrLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_RCRRP);                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  /* stop in any case since from now on only the service will be finalized (either with or without response) */
  Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, pContext->ThreadId);

  if ( (pContext->Diag.DoSuppressResponse == TRUE)                 /* No response to send */
      &&(0u == (pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_SENT | DCM_NET_TOBJ_FLAG_RCRRP_ON_TX /* Suppress only if RcrRp was not sent or not waiting for RCR-RP confirmation. If just attempted -> skip RCR-RP transmission */
                              /* | DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ - since DCM_TSK_EV_DIAG_TX_SEND_RCRRP cleared -> RCR-RP will not be sent. Instead finish the service processing at P2 time */))) )
  {
    Dcm_DiagTxFinalization(pTranspObj, DCM_E_OK);/* always OK */                                                                                     /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
  }
  else
  {
    /* prepare final response transmission */
    if(pTranspObj->State == DCM_NET_TOBJ_STATE_READY)
    {
      Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

      lBufferInfo = Dcm_NetGetBufferInfo(pTranspObj->BufferHdl);

      /* clear flag */
      Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, (DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ | DCM_NET_TOBJ_FLAG_RCRRP_ON_TX));          /* PRQA S 4399 */ /* MD_Dcm_BitNegation_4399 */ /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->State = DCM_NET_TOBJ_STATE_PREPTX;                                                                                                 /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduDataPtr = lBufferInfo->BufferPtr;/* reassign the worker buffer */                                                      /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

      if(pContext->Diag.ErrorRegister == DCM_E_POSITIVERESPONSE)
      {
        /* Positive response */
        pTranspObj->BuffInfo.SduLength = (PduLengthType)(pContext->Diag.MsgContext.resDataLen
                                                       + pContext->Diag.MsgContext.resProtHeadLen); /* don't forget the SID or/and SF bytes */       /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
        if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED))
        {
          pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_PAGED;                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
        else
#endif
        {
          pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_LINEAR;                                                                                         /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
      }
      else
      {
        /* Neg response */
        Dcm_NetComposeAddNegResponse(pTranspObj
                                    ,DCM_NET_TOBJ_RESTYPE_LINEAR
                                    ,pContext->Diag.ErrorRegister);                                                                                  /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
        if(Dcm_SingletonContext.Diag.CauseCodeContext.SendCauseCode == TRUE)
        {
          /* Provide specific cause code to negative response */
          Dcm_NetWriteAddBufferU8At(pTranspObj, 3, Dcm_SingletonContext.Diag.CauseCodeContext.CauseCode);                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
          /* Add the additional byte of the cause code to the SduLength */
          pTranspObj->BuffInfo.SduLength = 4u;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
          Dcm_SingletonContext.Diag.CauseCodeContext.SendCauseCode = FALSE;
        }
#endif
      }

      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
    }
    else
    {
      /* retry later */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, (pEventContext->Ev & (Dcm_TskTaskEvOptType)(DCM_TSK_EV_DIAG_TX_SEND_PAGED | DCM_TSK_EV_DIAG_TX_SEND_LINEAR))); /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagTxStartRcrRpResponse()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartRcrRpResponse(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pQueuedTObj;

  pQueuedTObj = Dcm_DiagGetQueuedTranspObj(pContext);

  /*
   * Don't start the P2* timer within the RCR-RP confirmation but here because of prioritization and synchronization
   * issues which could occur otherwise.
   * Start now to cover also the application triggered RCR-RPs.
   */
  Dcm_TmrStartTimerByThread(DCM_TMR_ID_P2, pContext->Diag.P2Timings.P2Star, pContext->ThreadId);

  pContext->Diag.MsgContext.msgAddInfo.suppressPosResponse = 0; /* reset the suppress bit and send final response */                                 /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pQueuedTObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ);                                               /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
  Dcm_UtiBitOpSet(Dcm_NetTransportObjectFlagType, pQueuedTObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_ON_TX); /* mark transmission flag */                   /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pQueuedTObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
  {
    /* suppress any RCR-RP responses on RoE - just do loop back ! */
    Dcm_DiagRcrRpTxFinished(pQueuedTObj, DCM_E_OK); /* always OK */                                                                                  /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
  }
  else
#endif
  {
    Dcm_NetComposeAddNegResponse(pQueuedTObj
                                ,DCM_NET_TOBJ_RESTYPE_RCRRP
                                ,DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING);                                                                     /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT); /* delegate job */
  }
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoComMChannelState()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoComMChannelStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoComMChannelState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo ,                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_RecoveryInfoComMChannelStatePtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_NET_MAX_NUM_COMM_CHANNELS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &(RecoveryInfo->ComMChannelState[0]);
  }
  else
  {
    lResult = &(RecoveryInfo->ComMChannelState[index]);
  }
  return lResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON && DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoCommControlState()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoCommControlStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoCommControlState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo ,                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_RecoveryInfoCommControlStatePtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_NET_MAX_NUM_ALL_COMM_CHANNELS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &(RecoveryInfo->CommControlState[0]);
  }
  else
  {
    lResult = &(RecoveryInfo->CommControlState[index]);
  }
  return lResult;
}
#endif
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_TriggerReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_TriggerReset(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Prepare programming conditions */
  pRepContext->ProgConditions.ProtocolId = Dcm_NetGetProtObjOfTranspObj(pTranspObj)->Id;                                                             /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */ /* SBSW_DCM_PARAM_PTR_WRITE */
# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  pRepContext->ProgConditions.ConnectionId = Dcm_PbCfgNetConnectionInfo[pTranspObj->ConnHdl].ConnectionId;                                           /* SBSW_DCM_PARAM_PTR_WRITE */
# endif
  pRepContext->ProgConditions.TesterSourceAddr = pTranspObj->ClientSrcAddr;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.Sid = 0x10u;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.SubFuncId = Dcm_CfgSvc10SubFuncLookUpTable[pRepContext->SesStateIdx+1u];                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.ReprogrammingRequest = TRUE;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  /* This flag shall be initialized from the FBL, not the DCM!
  pRepContext->ProgConditions.ApplUpdated = TRUE; */
  /* To be set once the Dcm_setProgCond API is called in order to consider the current RCR-RP status!
    * pRepContext->ProgConditions.ResponseRequired = ...; */
  pRepContext->ResetMode = (Rte_ModeType_DcmEcuReset)( (Dcm_CfgSvc10SubFuncInfo[pRepContext->SesStateIdx].BootType == DCM_SVC10_BOOT_TYPE_OEM)
                                                      ? RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER
                                                      : RTE_MODE_DcmEcuReset_JUMPTOSYSSUPPLIERBOOTLOADER);                                           /* SBSW_DCM_PARAM_PTR_WRITE */

  if(Dcm_ModeSwitchEcuReset(pRepContext->ResetMode) == DCM_E_OK)                                                                                     /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    /* Determine entry point - with or without RCR-RP */
    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_WAIT_RESET_ACK);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* start immediately */
  }
  else
  {
    /* for some reason, no mode switch is possible - exit with NRC */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForResetAck()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForResetAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check mode switch state */
  lStdResult = Dcm_ModeSwitchAckEcuReset();
  switch(lStdResult)
  {
  case DCM_E_PENDING:                                                                                                                                /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    break;
  case DCM_E_OK:
    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(Dcm_NetGetProtObjOfTranspObj(Dcm_DiagGetTranspObj(pContext))->HasRespPendOnBoot == TRUE)                                                      /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* COV_DCM_RTM_DEPENDS_ON_UNSUPPORTED TX */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      lStdResult = DCM_E_FORCE_RCRRP;
    }
    else
    {
      lStdResult = DCM_E_LOOP;
    }
    break;/* go on with next step - fall through */
  default:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForRcrRpAck()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(opStatus == DCM_FORCE_RCRRP_NOT_OK)
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
# if (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_ON)
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    Dcm_SplitTaskEnterCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    if(pMsgContext->msgAddInfo.suppressPosResponse != 0u)
    {
      pRepContext->ProgConditions.ResponseRequired = FALSE;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Stop to avoid inconsistency between P2 timer and the SetProgCond asynchronous API's "ResponseRequired" value */
      Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, pContext->ThreadId);
    }
    else
    {
      pRepContext->ProgConditions.ResponseRequired = TRUE;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    Dcm_SplitTaskLeaveCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
# else
    pRepContext->ProgConditions.ResponseRequired = FALSE;/* no response required since DCM sends the final response */                               /* SBSW_DCM_PARAM_PTR_WRITE */
# endif

    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_SET_PRGCOND);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_SetProgConditions()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_SetProgConditions(&pRepContext->ProgConditions);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  switch(lStdResult)
  {
  case DCM_E_OK:
# if (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_ON)
    lStdResult = Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    if(lStdResult == DCM_E_OK)/* fire and forget */                                                                                                  /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
    {
      /* just wait for reset */
      lStdResult = DCM_E_STOP_REPEATER;
    }
    else
    {
      /* This code shall not be reached, since the programming conditions are already set! */
      *ErrorCode = DCM_E_PANIC_NRC; /* Use panic NRC since the first mode switch has succeeded, and this one shall too */                            /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      lStdResult = DCM_E_NOT_OK;
    }
# endif
    break; /* send final response and if applicable - do reset */
  case DCM_E_PENDING:
    /* already set */
    break;
  case DCM_E_NOT_OK:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

/**********************************************************************************************************************
 *  Dcm_Svc10Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10Handler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_DiagSubServiceRefOptType  subSvcRef;
  Std_ReturnType                lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                    ,Dcm_CfgSvc10SubFuncLookUpTable
                                                    ,Dcm_CfgSvc10SubFuncExecPrecondTable
                                                    ,Dcm_DiagSvcWithOnlySubFuncReqLengthGetter
                                                    ,Dcm_DiagNoSequenceChecker
                                                    ,&subSvcRef
                                                    ,ErrorCode);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
  if(lStdResult == DCM_E_OK)
  {
    /* Compose positive response */
    Dcm_UtiProvideResDataAsU16(pMsgContext, Dcm_CfgSvc10SubFuncInfo[subSvcRef].ResponseTime.P2);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU16(pMsgContext, Dcm_CfgSvc10SubFuncInfo[subSvcRef].ResponseTime.P2Star);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    pRepContext->SesStateIdx = (uint8)subSvcRef;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_CHECK_ACCESS);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* start immediately */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc10CheckAccess()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10CheckAccess(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext);

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  if( ( ((DCM_STATE_VALUE_SESSION_INIT & Dcm_StateGetPendingSession()) == 0u) /* a non-default session is/will be active */
      &&( (Dcm_NetGetSessionConnection() != pTranspObj->ConnHdl)
        ||(Dcm_NetGetSessionClientSrcAddr() != pTranspObj->ClientSrcAddr) ) /* not the owner of the (non-default) session */
      &&(Dcm_NetGetProtObjOfConnection(pTranspObj->ConnHdl)->Priority
        >= Dcm_NetGetProtObjOfConnection(Dcm_NetGetSessionConnection())->Priority) /* priority not enough */
      )
    )
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#endif
  {
    if(opStatus == DCM_INITIAL)
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      Dcm_StateSetPendingSession(Dcm_UtiGetBitFromIndex(Dcm_CfgStateGroupMemType, pRepContext->SesStateIdx));

      /*
       * Set always a potential tester owner: In case of failed positive response confirmation or negative response,
       * the session will remain in its old state (default) -> the set value does not matter!
       * The situation that the session was a non-default one and another client has requested SID 0x10 is not possible:
       * - Each protocol switch invokes session transition to the default one!
       * - As long as a non-default session is active, no other client with the same priority can access the ECU!
       */
      Dcm_NetSetSessionConnection(pTranspObj->ConnHdl);
      Dcm_NetSetSessionClientSrcAddr(pTranspObj->ClientSrcAddr);
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
    lStdResult = Dcm_DiagIsSessionChangeAllowed(pContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_NOT_OK)
    {
      lStdResult = DCM_E_PENDING;
    }
    else
#endif
    {
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
      pRepContext->ResetMode = RTE_MODE_DcmEcuReset_NONE;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      if(Dcm_CfgSvc10SubFuncInfo[pRepContext->SesStateIdx].BootType != DCM_SVC10_BOOT_TYPE_NONE)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_TRIGGER_RESET);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      }
      else
#endif
      {
        /* Just send the final response */
        lStdResult = DCM_E_OK;
      }
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_Svc10UndoPendingSession()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc10UndoPendingSession(
  void
  )
{
    /* Undo pending session expectation */
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    Dcm_UtiEnterCriticalSection();/* avoid interrupt from the Dcm_NetTaskRx || Dcm_NetStartOfReception() */
#else
    Dcm_SplitTaskEnterCS();/* avoid interrupt from the Dcm_NetTaskRx */
#endif
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    Dcm_StateSyncPendingSession();/* synchronize with the currently active session */
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    Dcm_UtiLeaveCriticalSection();
#else
    Dcm_SplitTaskLeaveCS();
#endif
}
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrInit()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrInit(
  void
  )
{
  Dcm_CfgDidMgrDynDidHandleOptType dynDidIter;

  /* If DynDIDs were restored from non-volatile memory: */
# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
  if((Dcm_Svc2CNvMData.MagicNumber == DCM_CFG_FINAL_MAGIC_NUMBER)
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
   &&(Dcm_Svc2CNvMData.CfgVariantId == Dcm_VarMgrGetActiveCfgVariantId())                                                                            /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  endif
    )
  {
    /* Check if preconditions are still fulfilled */
#  if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) && \
      (DCM_DIAG_STATE_RECOVERY_ENABLED            == STD_OFF)
    /*
     * In case that state recovery feature is enabled, the DynDID preconditions shall be checked after
     * state recovery.
     * In case of jump from FBL (warm start) just clear all DynDIDs which are not supported
     * in default session (KISS principle).
     * After TX confirmation of the unsolicited response, Dcm_Svc2COnStateChanged() will be
     * called again by Dcm_StateSetSession().
     * Please note, that it was always not allowed to use state recovery and warm start at the same time.
     */
    Dcm_Svc2COnStateChanged();
#  endif
  }
  else
# endif
  {
    for(dynDidIter = 0; dynDidIter < DCM_NUM_DYNDIDS; ++dynDidIter)
    {
      Dcm_Svc2CDynDidItemPtrType pItem;

      pItem = Dcm_Svc2CGetItem(dynDidIter);
      pItem->Length = 0u;                                                                                                                            /* SBSW_DCM_POINTER_WRITE_2CITEM */
      pItem->Count  = 0u;                                                                                                                            /* SBSW_DCM_POINTER_WRITE_2CITEM */
    }

# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc2CNvMData.MagicNumber = DCM_CFG_FINAL_MAGIC_NUMBER;
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
    Dcm_Svc2CNvMData.CfgVariantId = Dcm_VarMgrGetActiveCfgVariantId();
#  endif
# endif
  }

  Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.DynDidHandleInUse = DCM_SVC2C_INVALID_DYNDID_HDL;
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01DefMgrRead()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  )
{
  Dcm_OpStatusType lOpStatus = opStatus;
  Std_ReturnType lStdResult = DCM_E_OK;

  Dcm_Svc2CDynDidSrcItemConstPtrType pDidInfo = Dcm_Svc2CGetSrcItem(pProcessContext->ItemInProgress);

  if(lOpStatus == DCM_INITIAL)
  {
    Dcm_Svc2CInitDidInfoFromDidInfoIdx(pDidInfo->DidDescriptor.DidInfoIdx, &(pProcessContext->SrcDidContext));                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2C) == FALSE)
  {
    /* Check for access rights (avoids race conditions) */
    lStdResult = Dcm_RsrcMgrGetDidLock(lOpStatus, pProcessContext->SrcDidContext.Did, DCM_RSRCMGR_DIDLOCK_OWNER_SID2C);

    if(lStdResult == DCM_E_OK)
    {
      lOpStatus = DCM_INITIAL;
      Dcm_SingletonContext.Diag.Services.Svc2C.ReadIndex = 0;
    }
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_NegativeResponseCodeType lNrc;
    Dcm_DiagDataContextType lDataContext;

    Dcm_DidMgrInitOpClassInfo(&(pProcessContext->SrcDidContext)
                             ,DCM_DIDMGR_OPTYPE_READ);                                                                                               /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */

    /* Init data context for asynchronous DIDs only initialy (not in pending status) */
    Dcm_UtiInitDataContext(&lDataContext                                                                                                             /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
                           ,Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.Buffer
                           ,DCM_SVC_2C_READ_BUFFER_SIZE);

    Dcm_UtiCommitData(&lDataContext, Dcm_SingletonContext.Diag.Services.Svc2C.ReadIndex);                                                            /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    lStdResult = Dcm_DidMgrReadDid(lOpStatus
                                  ,&lDataContext                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
                                  ,&(pProcessContext->SrcDidContext)                                                                                 /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                  ,&(pProcessContext->DidOpTypeContext)                                                                              /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                  ,&lNrc);                                                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

    Dcm_SingletonContext.Diag.Services.Svc2C.ReadIndex = lDataContext.Usage;

    /* Finished processing a DIDSrcItem */
    if(lStdResult != DCM_E_PENDING)
    {
      if(lStdResult == DCM_E_OK)
      {
        Dcm_UtiProvideDataAsUN(pDataContext, &lDataContext.Buffer[pDidInfo->DidDescriptor.Offset], pDidInfo->DidDescriptor.Size);                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2C);
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02DefMgrRead()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_OpStatusType lOpStatus = opStatus;

  lStdResult = DCM_E_OK;

  if(Dcm_RsrcMgrHasMemLock(DCM_RSRCMGR_MEM_ACCESS_REQ_INT) == TRUE)
  {
    /* Proceed with reading */
  }
  else
  {
    /* Still in process getting access! */
    lStdResult = Dcm_RsrcMgrGetMemLock(lOpStatus, DCM_RSRCMGR_MEM_ACCESS_REQ_INT);

    if (lStdResult == DCM_E_OK)
    {
      lOpStatus = DCM_INITIAL; /* prepare for first reading */
    }
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_NegativeResponseCodeType lNrc;
    Dcm_CfgDidMgrDynDidSrcItemIdxMemType lItemInProgress;

    lItemInProgress = Dcm_Svc2CGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED)->ItemInProgress;
    lStdResult = Dcm_MemMgrReadMemory(lOpStatus
                                    ,&(Dcm_Svc2CGetSrcItem(lItemInProgress)->MemDescriptor.MemBlock)
                                    ,pDataContext
                                    ,&lNrc);                                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadSrcItems()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadSrcItems(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;
  Dcm_OpStatusType lOpStatus = opStatus;

  for(; pProcessContext->ItemInProgress < pProcessContext->ItemToStop; pProcessContext->ItemInProgress++)                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    if(Dcm_Svc2CIsDidSrcItem(pProcessContext->ItemInProgress))
    {
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
      lStdReturn = Dcm_Svc2C01DefMgrRead(lOpStatus, pDataContext, pProcessContext);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
    }
    else
    {
# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
      lStdReturn = Dcm_Svc2C02DefMgrRead(lOpStatus, pDataContext);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
    }

    if(lStdReturn == DCM_E_OK)
    {
      /* prepare for next item */
      lOpStatus = DCM_INITIAL;
    }
    else
    {
      break;
    }
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadCheckAccessAndInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadCheckAccessAndInit(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  P2VAR(Dcm_OpStatusType, AUTOMATIC, AUTOMATIC) pOpStatus                                                                                            /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(dynDidHandle);                                                                                                              /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /*-----------------------------------------------*
   * DynDID usage check and initialization
   *-----------------------------------------------*/
  if(Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.DynDidHandleInUse == DCM_SVC2C_INVALID_DYNDID_HDL)
  {
    if(*pOpStatus == DCM_CANCEL)
    {
      lStdResult = DCM_E_NOT_OK; /* we have waited for too long to get access to the DynDID reader, but it was canceled -> Stop here since no reading has been started yet */
    }
    else
    {
      /*
       * Reset opStatus in case some DCM_E_PENDING had to be returned due to concurrent access (i.e. 0x22 and 0x2A for
       * different DDDIDs)
       */
      *pOpStatus = DCM_INITIAL;                                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
# if (DCM_SVC_2C_RACE_CONDITION_READ_ENABLED == STD_ON)
  else
  {
    if(dynDidHandle != Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.DynDidHandleInUse)
    {
      lStdResult = DCM_E_PENDING;
    } /* else - same handle -> process it */
  }
# endif
  return lStdResult;
}
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CfgSvc2CSubFuncInfoGetEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_CfgSvc2CSubFuncInfoGetEntry(
  Dcm_DiagSubServiceRefOptType SubSvcRef,
  CONSTP2VAR(Dcm_CfgSvc2CSubFuncInfoPtrType, AUTOMATIC, AUTOMATIC) pSubFuncInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  if (Dcm_DebugDetectRuntimeError(SubSvcRef >= DCM_CFGSVC2CSUBFUNCINFO_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);

    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pSubFuncInfo = &Dcm_CfgSvc2CSubFuncInfo[SubSvcRef];                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CHandler()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DiagSubServiceRefOptType lSubSvcRef;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc2CSubFuncLookUpTable
                                                   ,Dcm_CfgSvc2CSubFuncExecPrecondTable
                                                   ,Dcm_Svc2CReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Perform SID specific checks */
    if (Dcm_CfgSvc2CSubFuncInfo[(pRepContext->SubSvcRef)].MinReqLength <= pMsgContext->reqDataLen) /* min length = (DDID, (MEMdef|DIDdef))? */
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_SUBFUNCTION); /* delegate job */                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* speed up processing */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CSubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CSubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc2CSubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgSvc2CSubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC2CSUBFUNC */
  }

  return lStdResult;
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidLookUp()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
     /* Extract source DID */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->SrcItemDidInfoContext.Did));                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(opStatus
                                   ,&(pRepContext->SrcItemDidInfoContext)
                                   ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                   ,DCM_DIDMGR_OP_READ);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  switch(lStdReturn)
  {
  case DCM_E_OK:
    if(Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OP_DEFINE))                                /* SBSW_DCM_PARAM_PTR_WRITE */
    {
      /* else - no DynDID may be referenced! */
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
    else
    {
      lStdReturn = Dcm_StateCheckDID(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                    ,pRepContext->SrcItemDidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext))->ExecCondRef
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
      if(lStdReturn == DCM_E_OK)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS); /* next job */                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP;
      } /* else DCM_E_NOT_OK (ErrorCode already set) */
    }
    break;
  case DCM_E_PENDING:
    /* lStdReturn is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
    break;
  }

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidCheckCondition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OPTYPE_READCHKCOND))                   /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

    /* check condition on this DID */
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->SrcItemDidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,&(pRepContext->SrcItemDidInfoContext)
                                        ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

  } /* else - not supported operation - go on with getLength */

  if(lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH); /* next job */                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* speed up processing */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidGetLength()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_Svc2CDynDidSrcItemPtrType pSrcItem;
  Std_ReturnType lStdReturn = DCM_E_OK;

  pSrcItem = Dcm_Svc2CGetSrcItem(pRepContext->SrcItemIndex);

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OPTYPE_READLENGTH))                    /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->SrcItemDidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Calculate DID data length if applicable */
    lStdReturn = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->SrcItemDidInfoContext)
                                     ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(lStdReturn == DCM_E_OK)
  {
    uint8 srcDidPos;
    uint8 srcDidSize;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &srcDidPos);                                                                                              /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &srcDidSize);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

    if ((srcDidPos == 0u)
      ||(srcDidSize == 0u)
      ||(((Dcm_DidMgrDidLengthType)srcDidPos + (Dcm_DidMgrDidLengthType)srcDidSize - 1u) > pRepContext->SrcItemDidInfoContext.DidLength) )
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* source DID referenced data out of boundary or memory size of 0 */                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    } /* check for possible read buffer or response overflow */
    else if ((!Dcm_UtiIsAdditionSafe(Dcm_DidMgrDidLengthType, pRepContext->DynDidLength, srcDidSize))                                                /* PRQA S 2995 */ /* MD_Dcm_ConstExpr */
      || (Dcm_UtiPromotedSum(Dcm_DidMgrDidLengthType, pRepContext->DynDidLength, srcDidSize) > pRepContext->MaxAllowedLength))
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* too much data referenced */                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    }
    else /* Add this DID to the definition */
    {
      /* update total length of the DynDID for commitment */
      pRepContext->DynDidLength += srcDidSize;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

      /* store referenced data */
      pSrcItem->DidDescriptor.DidInfoIdx = pRepContext->SrcItemDidInfoContext.Descriptor.DidInfoIdx;                                                 /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      pSrcItem->DidDescriptor.Offset = (uint8)(srcDidPos - 1u);                                                                                      /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      pSrcItem->DidDescriptor.Size = srcDidSize;                                                                                                     /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      Dcm_Svc2CSetDidSrcItem(pRepContext->SrcItemIndex); /* set the type of the source item to DID */                                                /* SBSW_DCM_POINTER_WRITE_BITSET */

      ++(pRepContext->SrcItemIndex); /* next item to be configured */                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      ++(pRepContext->DynDidItemCount); /* update number of items for commitment */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

      --(pRepContext->ReqNumOfItems); /* commit processed item */                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      if (pRepContext->ReqNumOfItems > 0u) /* any item left? */
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP); /* next job */                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP; /* speed up processing */
      }
      else
      {
        lStdReturn = DCM_E_OK; /* all source items finished service processing immediately */
      }
    }
  }
  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02SrcItemsGetLength()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02SrcItemsGetLength(
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn = DCM_E_NOT_OK;
  Dcm_MemMgrMemBlockType lMemBlock;

  while(pRepContext->ReqNumOfItems > 0u)
  {
    Dcm_MemMgrParseMemStream(alfid, pMsgContext, &lMemBlock);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    lStdReturn = Dcm_MemMgrCheckMemBlock(&lMemBlock, DCM_MEMMGR_OP_READ, ErrorCode);                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdReturn == DCM_E_NOT_OK)
    {
      /* stop immediately and report the error */
      pRepContext->ReqNumOfItems = 0u; /* break */                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    } /* else - check for definition length overflow */
    else if( (!Dcm_UtiIsAdditionSafe(uint32, pRepContext->DynDidLength, lMemBlock.Length))                                                           /* PRQA S 2995 */ /* MD_Dcm_ConstExpr */
           ||(Dcm_UtiPromotedSum(uint32, pRepContext->DynDidLength, lMemBlock.Length) > pRepContext->MaxAllowedLength) )
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* too much data referenced */                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
      /* finish service processing immediately */
      pRepContext->ReqNumOfItems = 0u; /* break */                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* else - add this address to the DynDID definition */
      Dcm_Svc2CDynDidSrcItemPtrType pSrcItem = Dcm_Svc2CGetSrcItem(pRepContext->SrcItemIndex);

      /* update total length of the DynDID for commitment */
      pRepContext->DynDidLength += (Dcm_DidMgrDidLengthType)(lMemBlock.Length);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */


      pSrcItem->MemDescriptor.MemBlock = lMemBlock;                                                                                                  /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */

      Dcm_Svc2CSetMemSrcItem(pRepContext->SrcItemIndex); /* set the type of the source item to MEMORY */                                             /* SBSW_DCM_POINTER_WRITE_BITSET */
      ++(pRepContext->DynDidItemCount); /* update number of items for commitment */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      ++(pRepContext->SrcItemIndex);    /* next item to be configured */                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      --(pRepContext->ReqNumOfItems);   /* commit processed item */                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2C03DidLookUp()
 *********************************************************************************************************************/
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C03DidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->DynDidInfoContext.Did));                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU16(pMsgContext, pRepContext->DynDidInfoContext.Did); /* positive response contains the DynDID (2Byte) */                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(opStatus
                                   ,&pRepContext->DynDidInfoContext
                                   ,&pRepContext->DynDidOpTypeContext
                                   ,DCM_DIDMGR_OP_DEFINE);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch(lStdReturn)
  {
  case DCM_E_OK:
    /* clear concrete DynDID */
    Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DynDidInfoContext))->OpRef));
    break;
  case DCM_E_PENDING:
    /* lStdReturn is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }

  return lStdReturn;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CSetMaxAllowedDynDIDLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CSetMaxAllowedDynDIDLength(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  /* initialize common DynDID request context */
# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
  if (Dcm_Svc2AIsPeriodicDid(pRepContext->DynDidInfoContext.Did))
  {
    Dcm_NetConnRefMemType lConnHdl = Dcm_Svc2AGetConnectionOfDID(pRepContext->DynDidInfoContext.Did);
    if (lConnHdl < DCM_NET_INVALID_CONNHDL)
    {
      pRepContext->MaxAllowedLength = (Dcm_DidMgrDidLengthType)Dcm_PbCfgNetConnectionInfo[lConnHdl].UudtFrameSize;                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      pRepContext->MaxAllowedLength = DCM_SVC2A_MAX_RESPONSE_LEN;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
# endif
  {
    /*
     * Perform length check based on the current protocol only. Sid 0x22 will do additional checks at read time in
     * case a client from another protocol is trying to access the DynDID!
     */
    Dcm_CfgNetBufferSizeMemType lBufferSize = Dcm_Svc2CGetNetBufferSize(pContext);
    if (lBufferSize > Dcm_UtiMaxValueOfUintType(Dcm_DidMgrDidLengthType))
    {
      pRepContext->MaxAllowedLength = Dcm_UtiMaxValueOfUintType(Dcm_DidMgrDidLengthType) - 3u;         /* for response: 0x62 DID_H DID_L <DATA> */   /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
    {
      pRepContext->MaxAllowedLength = (Dcm_DidMgrDidLengthType)lBufferSize - 3u;                       /* for response: 0x62 DID_H DID_L <DATA> */   /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CInitDidInfoFromDidInfoIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CInitDidInfoFromDidInfoIdx(
  Dcm_CfgDidMgrDidInfoRefType didInfoIdx,
  Dcm_DidMgrDidInfoContextPtrType pSrcDidContext
  )
{
  pSrcDidContext->Did = Dcm_CfgWrapDidMgrDidLookUpTable(didInfoIdx + 1u);                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  (void)Dcm_DidMgrConcreteDidLookUp(pSrcDidContext, DCM_DIDMGR_OP_READ);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CfgSvc86SubFuncInfoGetEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_CfgSvc86SubFuncInfoGetEntry(
  Dcm_DiagSubServiceRefOptType SubSvcRef,
  CONSTP2VAR(Dcm_CfgSvc86SubFuncInfoPtrType, AUTOMATIC, AUTOMATIC) pSubFuncInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  if (Dcm_DebugDetectRuntimeError(SubSvcRef >= DCM_CFGSVC86SUBFUNCINFO_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);

    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pSubFuncInfo = &Dcm_CfgSvc86SubFuncInfo[SubSvcRef];                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc86Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;
  uint8                         lReqSf;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lReqSf = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                                       /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc86SubFuncLookUpTable
                                                   ,Dcm_CfgSvc86SubFuncExecPrecondTable
                                                   ,Dcm_Svc86ReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef  = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->StoreState = (Dcm_UtiBitOpTest(uint8, lReqSf, 0x40) ? TRUE : FALSE);                                                                /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC86_PROGRESS_SUBFUNCTION); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc86SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc86SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgSvc86SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC86SUBFUNC */
  }

  return lStdResult;
}
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetRegisterComMActivity()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetRegisterComMActivity(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)/* internal requests do not manage the ComM activity */
#endif
  {
    Dcm_CfgNetNetIdRefMemType lNetHandle;
    Dcm_NetComMContextPtrType pComMContext;

    lNetHandle   = Dcm_PbCfgNetConnectionInfo[pTranspObj->ConnHdl].NetworkIdRef;
    pComMContext = Dcm_NetGetComMContext(lNetHandle);

    Dcm_DebugAssert((pComMContext->RegisteredNetworks != Dcm_UtiMaxValueOfUintType(Dcm_NetConnRefMemType)), DCM_SID_INTERNAL, DCM_E_PARAM);          /* COV_DCM_RTM_DEV_DEBUG XF */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    if(pComMContext->RegisteredNetworks == 0u)
    {
      /* activate only if default session is active, otherwise already active */
      if(   Dcm_StateIsDefaultSessionActive()
        && (Dcm_SingletonContext.Network.ActiveDiagnostic == DCM_NET_COMM_ACTIVE)
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
        && (Dcm_SingletonContext.Network.KeepAlive == FALSE)
#endif
        )
      {
        /* Register one connection */
        ComM_DCM_ActiveDiagnostic(Dcm_PbCfgNetAllComMChannelMap[Dcm_PbCfgNetConnComMChannelMap[lNetHandle]]);
      }
    }

    /* Register one connection */
    ++pComMContext->RegisteredNetworks;                                                                                                              /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterComMActivity()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUnRegisterComMActivity(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_CfgNetNetIdRefMemType lNetHandle;
  Dcm_NetComMContextPtrType pComMContext;

  lNetHandle   = Dcm_PbCfgNetConnectionInfo[connHdl].NetworkIdRef;
  pComMContext = Dcm_NetGetComMContext(lNetHandle);

  Dcm_DebugAssert((pComMContext->RegisteredNetworks != 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                                                          /* COV_DCM_RTM_DEV_DEBUG XF */

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Unregister one connection */
  --pComMContext->RegisteredNetworks;                                                                                                                /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */

  if(pComMContext->RegisteredNetworks == 0u)
  {
    /* return only if default session is active */
    if (Dcm_StateIsDefaultSessionActive()
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
       &&(Dcm_SingletonContext.Network.KeepAlive == FALSE)
#endif
       )
    {
      ComM_DCM_InactiveDiagnostic(Dcm_PbCfgNetAllComMChannelMap[Dcm_PbCfgNetConnComMChannelMap[lNetHandle]]);
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterActiveConnection()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUnRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
  {
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
    Dcm_ExtSvc86TxEnd();
# endif
  }
  else
#endif
  {
    Dcm_DebugAssert((Dcm_SingletonContext.Network.NumActiveConnections != 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                                       /* COV_DCM_RTM_DEV_DEBUG XF */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
     BEGIN CRITICAL SECTION
     *=================================*/
    /* Start the idle connection timer for the authenticated session connection */
#if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
    /* Check if authentication restriction is supported for this connection */
    if (Dcm_NetGetAuthInfoRef(pTranspObj->ConnHdl) != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      Dcm_AuthMgrTimerStart(Dcm_NetGetAuthInfoRef(pTranspObj->ConnHdl), DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT);
    }
#endif

    /* If this was not a functional request from different tester with 0x3E 0x80 or a denied parallel tester request */
    if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_IGNORE) == 0u)
    {
      /* The timers shall be restarted later with the original timer values (when the last connection goes inactive) */
      Dcm_TmrSetTimer(DCM_TMR_ID_S3, DCM_DIAG_TIME_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrSetTimer(DCM_TMR_ID_KEEP_ALIVE, DCM_NET_KEEP_ALIVE_TIME);
#endif
    }

    /* Un-register an active connection */
    --Dcm_SingletonContext.Network.NumActiveConnections;

    if(Dcm_SingletonContext.Network.NumActiveConnections == 0u)
    {
      /* Start or resume the S3 timer and keep-alive timer with the last set values */
      Dcm_TmrResumeTimer(DCM_TMR_ID_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrResumeTimer(DCM_TMR_ID_KEEP_ALIVE);
#endif
    }

    /*=================================*
     END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
  Dcm_NetTranspObjRelease(pTranspObj);                                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_NetAllocateOrGetTranspObject()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetAllocateOrGetTranspObject(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK; /* No free transport object found */

  if(Dcm_DebugDetectRuntimeError(connHdl >= Dcm_PbCfgNetNumConnections))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    if(Dcm_PbRamNetConnHdl2TObjMap[connHdl] >= DCM_NET_INVALID_TOBJID)/* check if already assigned */
    {
      Dcm_CfgNetTObjHandleOptType lTObjIter;

      for(lTObjIter = 0; lTObjIter < Dcm_PbCfgNetNumTransportObjects; ++lTObjIter)
      {
        if(Dcm_PbRamNetTransportObject[lTObjIter].State == DCM_NET_TOBJ_STATE_FREE)
        {
          Dcm_NetPutToConn2TObjMap(connHdl, (Dcm_CfgNetTObjHandleMemType)lTObjIter); /* Assign ownership information */
          *pTranspObj = Dcm_NetGetTransportObject(lTObjIter);                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
          lResult = BUFREQ_OK;
          break;
        }/* Transport Object is in use - check next one */
      }
    }
    else
    {
      *pTranspObj = Dcm_NetGetTransportObject(Dcm_PbRamNetConnHdl2TObjMap[connHdl]);                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetTranspObjOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetGetTranspObjOfConnection(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  if(Dcm_DebugDetectRuntimeError(connHdl >= Dcm_PbCfgNetNumConnections))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    if(Dcm_PbRamNetConnHdl2TObjMap[connHdl] < DCM_NET_INVALID_TOBJID)
    {
      *pTranspObj = Dcm_NetGetTransportObject(Dcm_PbRamNetConnHdl2TObjMap[connHdl]);                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetTranspObjRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetTranspObjRelease(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
  /* If the transport object is still in use: */
  if(pTranspObj->State != DCM_NET_TOBJ_STATE_FREE) /* assure the object has validly initialized fields */
  {
    /* Its states are valid and can be used if needed */
    /* Reset only the relevant states of the transport object */
    Dcm_NetPutToConn2TObjMap(pTranspObj->ConnHdl, DCM_NET_INVALID_TOBJID); /* Clear the ownership information */
    pTranspObj->ResType  = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pTranspObj->Flags    = DCM_NET_TOBJ_FLAG_NONE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    /* Reset the guard state (object in use) at last to avoid critical section usage */
    pTranspObj->State    = DCM_NET_TOBJ_STATE_FREE;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  }
  /* Otherwise nothing to do */
}

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByTesterAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByTesterAddress(
  uint16 testerAddress
  )
{
  Dcm_NetConnRefOptType lConnIter;
  Dcm_NetConnRefMemType lResult = DCM_NET_INVALID_CONNHDL;

  for(lConnIter = 0; lConnIter < Dcm_PbCfgNetNumConnections; ++lConnIter)
  {
    if (testerAddress == Dcm_PbCfgNetConnectionInfo[lConnIter].ClientSrcAddr)
    {
      lResult = (Dcm_NetConnRefMemType)lConnIter;
      break;
    }
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetUsdtTxConfirmation()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUsdtTxConfirmation(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType result
  )
{
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    /* ComM is already released in the DiagPostProcessing task */
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    if(pTranspObj->ResType == DCM_NET_TOBJ_RESTYPE_SIMPLE)
    {
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
      if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)
#endif
      {
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
      }
      Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      Dcm_DiagOnTxFinished(pTranspObj, result);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveRequest(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  uint8 neededLength
  )
{
  boolean lIsTesterPresent = FALSE;
  if (info != NULL_PTR)                                                                                                                              /* PRQA S 2822 */ /* MD_Dcm_2822 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    if (info->SduLength == neededLength)
    {
      if ((Dcm_PbCfgNetRxPduInfo[rxPduId].IsFuncReq == TRUE)
         && (info->SduDataPtr[0] == 0x3Eu) && (info->SduDataPtr[1] == 0x80u))                                                                        /* PRQA S 2822 */ /* MD_Dcm_2822 */
      {
        lIsTesterPresent = TRUE;
      }
    }
  }
  return lIsTesterPresent;
}

/**********************************************************************************************************************
 *  Dcm_NetSetComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetComModeEntered(
  NetworkHandleType networkId,
  Dcm_NetComMStateType newComState
  )
{
  /* used to always verify for valid parameter */
  sint16_least lDcmComMHandle = Dcm_UtiLookUpUint8(Dcm_PbCfgNetNetworkHdlLookUp, networkId);                                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if(lDcmComMHandle >= 0) /* consider only DCM relevant channels */
  {
    Dcm_NetComMContextPtrType pComMContext;

    pComMContext = Dcm_NetGetComMContext((Dcm_CfgNetNetIdRefOptType)lDcmComMHandle);

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    pComMContext->ComState = newComState;                                                                                                            /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }/* else - not a DCM channel -> ignore */
}

/**********************************************************************************************************************
 *  Dcm_NetStartOfReception()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetStartOfReception(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr,
  Dcm_NetTransportObjectFlagType addTObjFlags
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlOfRxPduId(rxPduId);                                                                              /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  DCM_IGNORE_UNREF_PARAM(info);                                                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the communication state allows any diagnostic request reception (at least SilentCommunicationMode): */
  if( ((DCM_NET_COMM_STATE_FLAG_RX_EN & Dcm_NetGetComStateByConnection(lConnHdl)) != 0u)
#if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
    &&(Dcm_SingletonContext.Network.RxAllowed == TRUE)
#endif
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    &&( (!Dcm_NetIsGenericConnection(lConnHdl))
      ||(Dcm_PbCfgNetRxPduInfo[rxPduId].IsFuncReq == TRUE)
      ||(info->SduDataPtr[info->SduLength-1u] == Dcm_PbCfgNetConnectionInfo[lConnHdl].EcuAddress) )                                                  /* PRQA S 2822 */ /* MD_Dcm_2822 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
#endif
    )
  {
    Dcm_NetTransportObjectPtrType pTranspObj;

    /* Enter critical section (Reason: Needed only on multi-channel DCM configurations to guarantee the correct
     * transport object allocation and initialization) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /* Allocate new or get existing, already associated with this DcmRxPduId transport object */
    /* If allocation or getter succeeded: */
    if(Dcm_NetAllocateOrGetTranspObject(lConnHdl, &pTranspObj) == BUFREQ_OK)                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If the transport object is allocated as a new one: */
      if(pTranspObj->State == DCM_NET_TOBJ_STATE_FREE)
      {
        Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

        pTranspObj->BufferHdl = Dcm_NetGetProtObjOfConnection(lConnHdl)->RxTxBufferRef;                                                              /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lBufferInfo = Dcm_NetGetBufferInfo(pTranspObj->BufferHdl);

        /* If the request message length fits the Rx-buffer associated with the DcmRxPduId: */
        if(tpSduLength <= lBufferInfo->Size)
        {
          /* Prepare for positive result or NRC 0x21 response: */
          /* Prepare result to be reported for the available Rx-buffer size */
          *bufferSizePtr = lBufferInfo->Size;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

          /* Initialize the transport object */
          pTranspObj->RxPduId  = rxPduId;                                                                                                            /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->ConnHdl   = (Dcm_NetConnRefMemType)lConnHdl;                                                                                   /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

          /* Set up Rx message context */
          pTranspObj->RxLength = (Dcm_CfgNetBufferSizeMemType)tpSduLength;/* safe cast, since tpSduLength <= bufferSize = value fits Dcm_CfgNetBufferSizeMemType */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->BuffPos  = 0u; /* start writing from the buffer begin */                                                                       /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->State    = DCM_NET_TOBJ_STATE_ONRX;                                                                                            /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->Flags    = addTObjFlags;                                                                                                       /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
          /* #20 If the transport object uses generic connection, set meta data. */
          if (Dcm_NetIsGenericConnection(lConnHdl))
          {
            /* Source address of the request shall be the target address of the response. */
            pTranspObj->ClientSrcAddr = info->SduDataPtr[info->SduLength-(PduLengthType)2u];                                                         /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          }
          else
#endif
          {
            pTranspObj->ClientSrcAddr = Dcm_PbCfgNetConnectionInfo[lConnHdl].ClientSrcAddr;                                                          /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          }

          /* #30 If the new request can be received - that is: */
          if( Dcm_NetBufferIsFree(pTranspObj->BufferHdl) /* Is the buffer free? */
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
            &&( (Dcm_StateIsDefaultSessionPending() == TRUE)/* a default session is/will be active */                                                /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
              ||( (Dcm_NetGetSessionConnection() == lConnHdl)
                &&(Dcm_NetGetSessionClientSrcAddr() == pTranspObj->ClientSrcAddr) ) /* the owner of the (non-default) session */
              ||(Dcm_NetGetProtObjOfConnection(lConnHdl)->Priority
                < Dcm_NetGetProtObjOfConnection(Dcm_NetGetSessionConnection())->Priority) /* higher priority -> let it through */
              )
#endif
            )
          {
            /* Lock the buffer and allow further request reception */
            pTranspObj->RxData = lBufferInfo->BufferPtr;                                                                                             /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            Dcm_NetBufferLock(pTranspObj->BufferHdl); /* the buffer is in use now */
            lResult = BUFREQ_OK;
          }
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
          else
          {
            /* Otherwise, prepare for pseudo request reception (only SID and sub-function will be copied) (if multiple client environment enabled) */
            pTranspObj->RxData = &pTranspObj->AddBuffer[1];                                                                                          /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            /* no free "real buffer" object - ignore or simulate reception for NRC 0x21 */
            pTranspObj->Flags |= (Dcm_NetTransportObjectFlagType)(DCM_NET_TOBJ_FLAG_BUSY | DCM_NET_TOBJ_FLAG_COPYHEAD | DCM_NET_TOBJ_FLAG_IGNORE);   /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            lResult = BUFREQ_OK;
          }
#endif
        }
        else
        {
          /* Otherwise, reject the message reception with the result "message too long" */
          lResult = BUFREQ_E_OVFL;
        }

        /* #40 If the message reception is to be continued, register the client connection as active  */
        if(lResult == BUFREQ_OK)
        {
          Dcm_NetRegisterActiveConnection(pTranspObj);                                                                                               /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        }
        else
        {
          /* Otherwise, release the allocated transport object */
          Dcm_NetTranspObjRelease(pTranspObj);/* no further processing needed */                                                                     /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        }
      }/* Otherwise, the transport object is already in use (i.e. parallel request from the same tester):
        *   - reject the message reception if no functional TesterPresent (checked later on) */
    }/* Otherwise, no free transport object found (more clients than transport objects configured, legal use case
      * to save resources) - reject the message reception if no functional TesterPresent (checked later on) */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    if (lResult == BUFREQ_E_NOT_OK)
    {
      uint8 lLengthForTesterPresent = 2u; /* 0x3E 0x80 */

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      if (Dcm_NetIsGenericConnection(lConnHdl))
      {
        lLengthForTesterPresent += DCM_NET_METADATA_LENGTH;
      }
#endif

      if (Dcm_NetIsKeepAliveRequest(rxPduId, info, lLengthForTesterPresent))                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        /* functional Tester Present as a parallel request */
        *bufferSizePtr = 2u;                                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = BUFREQ_OK;
      }
    }
  } /* Otherwise, reject the message reception */
  return lResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferExecute()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferExecute(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;                                                                                                          /* PRQA S 2981 */ /* MD_Dcm_RetVal */

  Dcm_UtiResetRuntimeLimiter();

  DCM_UTI_LOOP_FOR_EVER                                                                                                                              /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    Dcm_DiagSvcUpdateFuncType pUpdateFunc;

    pUpdateFunc = Dcm_DiagGetUpdateFunc(pContext->PagedBuffer.UpdateFuncRef);

    /* Delegate to the page writer */
    lStdResult = pUpdateFunc(pContext
                            ,pContext->PagedBuffer.OpStatus
                            ,pDataContext
                            ,ErrorCode);                                                                                                             /* SBSW_DCM_CALL_FUNCPTR_SVCUPDATERS */

    if(lStdResult != DCM_E_LOOP)
    {
      break;
    }/* else - just loop again */

    pContext->PagedBuffer.OpStatus = DCM_INITIAL;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferCancel()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferCancel(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult = DCM_E_OK;

  /* If page buffer is currently activated: */
  if(pContext->PagedBuffer.State != DCM_PAGEDBUFFER_STATE_INACTIVE)
  {
    /* If not all data was written by the application: */
    if(pContext->PagedBuffer.UpdateFinished == FALSE)
    {
      /* Notify the application about the cancellation */
      Dcm_DiagSvcCancelFuncType lCancelFunc = Dcm_DiagGetCancelFunc(pContext->PagedBuffer.CancelFuncRef);

      lCancelFunc(pContext);                                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS */
    }

    /* If no transmission was started yet: */
    if(pContext->PagedBuffer.State == DCM_PAGEDBUFFER_STATE_ACTIVATED)                                                                               /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
    {
      /* Send the appropriate NRC by using the linear buffer if needed */
      *ErrorCode = DCM_DIAG_CANCEL_OP_NRC;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = DCM_E_NOT_OK;
    }

    /* Reset page buffer state */
    Dcm_PagedBufferInitByThread(pContext->ThreadId);
    /* Clear any pending events in the Dcm_PagedBufferTask() to avoid any further processing */
    Dcm_TskClrEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, pContext->ThreadId);
  }

  return lResult;
}
#endif /* (DCM_PAGED_BUFFER_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  Dcm_DiagRcrRpTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRcrRpTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* COV_DCM_RTM_NO_RCR_RP_TX */ /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                                     /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  pContext->Diag.TxStatus = txStatus;/* store current Tx state */                                                                                    /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_ON_TX);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiBitOpSet(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_SENT);/* mark RCR-RP at least once sent state */        /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State   = DCM_NET_TOBJ_STATE_READY;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_RCRRP, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));  /* Notify for sent RCR_RP */ /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_DiagSetP2Timings()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagSetP2Timings(
  Dcm_StateIndexMemType sesStateIdx,
  Dcm_NetProtRefMemType protocolIdx,
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(threadId);

  Dcm_UtiEnterCriticalSection();

  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/

  /* Calculate the current P2 timings considering the protocol related adjust timings */
  pContext->Diag.P2Timings.P2 = Dcm_DiagGetP2Time(sesStateIdx)
                              - Dcm_NetGetP2AdjTime(protocolIdx);                                                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  pContext->Diag.P2Timings.P2Star = Dcm_DiagGetP2StarTime(sesStateIdx)
                                  - Dcm_NetGetP2StarAdjTime(protocolIdx);                                                                            /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_DiagCancelProcessing()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagCancelProcessing(
  uint8 threadId
  )
{
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  if((DCM_DIAG_QUEUE_FLAG_IS_WAITING & Dcm_GetThreadContext(threadId)->Diag.QueuedSet.Flags) == 0u)
#endif
  {
    Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, threadId);
  }

  Dcm_TskKillAllTasks(threadId);
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_KILL_JOBS, threadId);
}

/**********************************************************************************************************************
 *  Dcm_DiagRxIndication()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRxIndication(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                                     /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

#if (DCM_DIAG_RST2DEF_ON_FUNC_OBD_REQ_ENABLED == STD_ON)
  /* #10 Handle OBD requests (SID [0x00-0x0F]) when only one diagnostic client is used */
  if ( (pTranspObj->RxData[0] < 0x10u)
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
     &&(Dcm_PbCfgNetNumProtocols == 1u)
# endif
     &&(Dcm_StateIsDefaultSessionPending() == FALSE)                                                                                                 /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
     &&(Dcm_PbCfgNetRxPduInfo[pTranspObj->RxPduId].IsFuncReq == TRUE)                                                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
     )
  {
    /* The number of threads is limited to one, so use basic thread here */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, DCM_THREAD_ID_BASIC);
    Dcm_DiagSetP2Timings(0, 0, DCM_THREAD_ID_BASIC);
  }
#endif

  /* #20 Update diagnostic kernel queue flags: */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  pContext->Diag.QueuedSet.Flags |= DCM_DIAG_QUEUE_FLAG_IS_WAITING;                                                                                  /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_UtiLeaveCriticalSection();

  /* #30 Prepare for new diagnostic request processing by setting all relevant for the real-time task properties: */
  Dcm_DiagSetQueuedTranspObj(pContext, pTranspObj);                                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_TmrStartTimerByThread(DCM_TMR_ID_P2, pContext->Diag.P2Timings.P2, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                  /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
  Dcm_SingletonContext.Network.KeepAlive = TRUE;
#endif

  /* #40 Signal the Dcm_DiagTaskWorker that the request is now ready for processing */
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_NEW_REQ, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_DiagTxFinalization()
 *********************************************************************************************************************/
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
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagTxFinalization(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_ContextPtrType pContext;

  pContext = Dcm_GetThreadContext(Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                                                                        /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  pContext->Diag.TxStatus = txStatus;                                                                                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->Diag.State    = DCM_DIAG_STATE_POST_PROCESS;                                                                                             /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  Dcm_UtiBitOpClr(Dcm_DiagProcessorFlagType, pContext->Diag.QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_ACTIVE);                                         /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_SplitTaskLeaveCS();

  if(((DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR)& pContext->Diag.ApplNotification) != 0u)                                                        /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    Dcm_DiagSvcConfirmationFuncType lFastConfFunc;
    Dcm_ConfirmationStatusType      lConfStatus;

    lFastConfFunc = Dcm_DiagGetPostProcessorFunc(Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].FastConfFuncRef);                                   /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    lConfStatus   = Dcm_DiagGetPostProcessResult(pContext);                                                                                          /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

    /* Execute any internal service processors fast-post-handlers */
    lFastConfFunc(pContext, lConfStatus);                                                                                                            /* SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  }

  /* Release the connection */
  if((DCM_NET_TOBJ_FLAG_CANCELED & pTranspObj->Flags) == 0u) /* let the connection expire with the Tx confirmation (after CancelTransmit) */
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* Start the S3 timer if needed here to have maximum real-time accuracy (from communication bus point of view) */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                       /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnFblResTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnFblResTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_NetBufferRelease(pTranspObj->BufferHdl);
  Dcm_NetUnRegisterActiveConnection(pTranspObj);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_SingletonContext.Diag.StartUpFbl.TxStatus = txStatus;
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_TXCONF);
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagExecIndicationFunc()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagExecIndicationFunc(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgItemType sid,
  Dcm_DiagApplNotificationType notificationType,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lResult = DCM_E_OK; /* positive case */
  uint16_least   lIter;

  pContext->Diag.ApplNotification |= notificationType; /* mark application has been notified */                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Prepare for post processing later */
  pContext->Diag.PostProcessorContext.Sid     = sid;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->Diag.PostProcessorContext.SrcAddr = Dcm_DiagGetTranspObj(pContext)->ClientSrcAddr;                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pContext->Diag.PostProcessorContext.ReqType = (uint8)pContext->Diag.MsgContext.msgAddInfo.reqType;                                                 /* SBSW_DCM_PARAM_PTR_WRITE */

  *ErrorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  for(lIter = 0u; notifList[lIter].IndFunc != NULL_PTR; ++lIter)                                                                                     /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    Std_ReturnType               lStdResult;
    Dcm_NegativeResponseCodeType lNrc = DCM_E_POSITIVERESPONSE; /* Initialize out parameter */

    lStdResult = notifList[lIter].IndFunc(sid
                                         ,pContext->Diag.MsgContext.reqData
                                         ,(uint16)pContext->Diag.MsgContext.reqDataLen
                                         ,(uint8)pContext->Diag.MsgContext.msgAddInfo.reqType
                                         ,pContext->Diag.PostProcessorContext.SrcAddr
                                         ,&lNrc);                                                                                                    /* SBSW_DCM_CALL_FUNCPTR_TERMINATED_LIST */

    if(lStdResult != DCM_E_OK)
    {
      lResult = DCM_E_NOT_OK; /* Mark error, but let other checks to be performed - check for higher priority NOT_ACCEPTED result */

      switch(lStdResult)
      {
      case DCM_E_NOT_OK:
        Dcm_UtiHandleApplNrc(lStdResult, &lNrc, DCM_E_CONDITIONSNOTCORRECT);                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */
        break;
      case DCM_E_REQUEST_NOT_ACCEPTED:
        lNrc = DCM_E_CONDITIONSNOTCORRECT; /* In DCM_E_REQUEST_NOT_ACCEPTED case no positive response shall be sent! */
        pContext->Diag.DoSuppressResponse = TRUE; /* Try to suppress, but if an RCR-RP has been already sent - send the registered NRC */            /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      default:
        /* Unexpected return value */
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        lNrc = DCM_E_PANIC_NRC;
        break;
      }

      /* Try to register NRC if no other has been set! */
      if (*ErrorCode == DCM_E_POSITIVERESPONSE)
      {
        *ErrorCode = lNrc;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }

  return lResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_ProcessingDone()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_ProcessingDone(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(threadId);

  /* If positive response to be sent: */
  if(pContext->Diag.ErrorRegister == DCM_E_POSITIVERESPONSE)
  {
    /* If SPRMIB == TRUE (suppression of positive response): */
    if(pContext->Diag.MsgContext.msgAddInfo.suppressPosResponse != 0u)
    {
      /* Mark no response to be sent */
      pContext->Diag.DoSuppressResponse = TRUE;                                                                                                      /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
    }/* Otherwise - proceed with positive response transmission */
  }
  else /* Otherwise (negative response): */
  {
    /* If functionally requested diagnostic job: */
    if(pContext->Diag.MsgContext.msgAddInfo.reqType != 0u)
    {
      switch(pContext->Diag.ErrorRegister)
      {
        case DCM_E_SERVICENOTSUPPORTED:
        case DCM_E_SUBFUNCTIONNOTSUPPORTED:
        case DCM_E_REQUESTOUTOFRANGE:
#if (DCM_DIAG_SUPPRESS_NRC_7F_7E_ENABLED == STD_ON)
        case DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION:
        case DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION:
#endif
          /* Suppress negative response for certain NRCs only */
          pContext->Diag.DoSuppressResponse = TRUE;                                                                                                  /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
          break;
        default:
          /* Otherwise - proceed with negative response transmission */
          break;
      }
    }/* Otherwise - proceed with negative response transmission */
    /* Update negative response code IdsM events */
    Dcm_DebugReportSecurityEventNRC(threadId, pContext->Diag.ErrorRegister);
  }
  /* Update diagnostic sub-component activity state */
  pContext->Diag.State = DCM_DIAG_STATE_ON_TX;                                                                                                       /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  /* Notify the transmission task for linear data response transmission/suppression */
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_LINEAR, threadId);
}

/**********************************************************************************************************************
 *  Dcm_SetNegResponse()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_SetNegResponse(
  Dcm_NegativeResponseCodeType nrc,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(threadId);

  if(pContext->Diag.ErrorRegister == DCM_E_POSITIVERESPONSE)
  {
    pContext->Diag.ErrorRegister = nrc;                                                                                                              /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  }
}
/**********************************************************************************************************************
 *  Dcm_DiagLookUpFilter()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(sint16_least, DCM_CODE) Dcm_DiagLookUpFilter(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  sint16_least lLookUpResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check if diagnostic object exists in current variant */
  lLookUpResult = Dcm_DiagVariantLookUpFilter(lookUpTable, lookUpFilterTable, value);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* If diagnostic object not supported in current variant at all: */
  if(lLookUpResult < 0)
  {
    /* Set appropriate NRC depending on the diagnostic object type */
    *errorCode = Dcm_DiagObjNotFoundNrcs[chkLevel];                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* Otherwise if requested SID is not accessible by the client sent it, exit service validation by sending NRC 0x11 */
  else if( (chkLevel == DCM_DIAG_CHK_LVL_SERVICE_ID)
         &&((Dcm_PbCfgDiagSvcId2ProtMap[lLookUpResult] & Dcm_UtiGetBitFromIndex(Dcm_CfgDiagSvc2ProtMapMemType, Dcm_NetGetProtIdOfMsgContext(pMsgContext))) == 0u) )
  {
    lLookUpResult = -1; /* override result */
    *errorCode = DCM_E_SERVICENOTSUPPORTED;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Otherwise return positive result  */
  }

  return lLookUpResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithSubFuncStateCheck()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSvcWithSubFuncStateCheck(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(subFuncSequenceChecker);                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(subSvcRef);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */


  if(!Dcm_StateIsSupportedInSession(Dcm_StateGetPreconditionStates(stateRef)))
  {
    /* If session is not correct, send NRC 0x7E */
    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckRole(stateRef, Dcm_NetGetAuthInfoRefByMsgContext(pMsgContext)) == DCM_E_NOT_OK)
  {
    /* If authentication role is not correct, send NRC 0x34 */
    lStdReturn = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
#endif
  /* If supported, verify the security access level pre-condition (UDS verification order) */
#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(!Dcm_StateIsSupportedInSecurityLevel(Dcm_StateGetPreconditionStates(stateRef)))
  {
    /* If not met, do return NRC 0x33 (UDS) */
    *ErrorCode = DCM_E_SECURITYACCESSDENIED;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
#endif
#if (DCM_DIAG_SF_SEQUENCE_CHECK_SUPPORT_ENABLED == STD_ON)
  if(subFuncSequenceChecker(pMsgContext, subSvcRef, ErrorCode) != DCM_E_OK)                                                                          /* SBSW_DCM_PARAM_FUNCPTR_CALL */
  {
    /* If SF sequence is not correct, ErrorCode already set */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
#endif
  {
#if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
    /* Otherwise, if supported, get the the mode-related pre-condition expression(s) result */
    lStdReturn = Dcm_ModeCheck(Dcm_CfgStatePreconditions[stateRef].ModeRuleRef, ErrorCode);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
#else
    /* Otherwise, everything fine - return positive response code */
    lStdReturn = DCM_E_OK;
#endif
  }

  return lStdReturn;
}
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrClear(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle
  )
{
  Dcm_Svc2CDynDidItemPtrType pItem;

#  if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
  {
    uint16 lDid = Dcm_DidMgrGetDynDidIdFromHandle(dynDidHandle);
    /* Try stopping any active periodic DID (can be still read at that time)! */
    Dcm_Svc2ASchdStopItemByDid(lDid);
  }
#  endif

  pItem = Dcm_Svc2CGetItem(dynDidHandle);
  pItem->Length = 0u;                                                                                                                                /* SBSW_DCM_POINTER_WRITE_2CITEM */
  pItem->Count  = 0u;                                                                                                                                /* SBSW_DCM_POINTER_WRITE_2CITEM */
}
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CCheckAndSetNumOfItems()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CCheckAndSetNumOfItems(
  Dcm_MsgLenType numRequestedItems,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if (((Dcm_MsgLenType)Dcm_DidMgrGetDynDidSize(pRepContext->DynDidHandle) - pRepContext->DynDidItemCount) < numRequestedItems)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  {
    pRepContext->ReqNumOfItems = (uint8)numRequestedItems;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_OK;
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CParseAndCheckDynDid()
 *********************************************************************************************************************/
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CParseAndCheckDynDid(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &pRepContext->DynDidInfoContext.Did);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(opStatus
                                   ,&pRepContext->DynDidInfoContext                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,&pRepContext->DynDidOpTypeContext                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,DCM_DIDMGR_OP_DEFINE);

  switch(lStdReturn)
  {
  case DCM_E_OK:
    lStdReturn = Dcm_StateCheckDID(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                                  /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                  ,pRepContext->DynDidInfoContext.Did
                                  ,DCM_DIDMGR_OP_READ
                                  ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DynDidInfoContext))->ExecCondRef
                                  ,ErrorCode);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    if (lStdReturn == DCM_E_OK)
    {
      Dcm_Svc2CDynDidItemConstPtrType pItem;

      Dcm_Svc2CSetMaxAllowedDynDIDLength(pContext, pRepContext);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
      pRepContext->DynDidHandle = (Dcm_CfgDidMgrDynDidHandleMemType)Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DynDidInfoContext))->OpRef;               /* SBSW_DCM_PARAM_PTR_WRITE */

      pItem = Dcm_Svc2CGetItem(pRepContext->DynDidHandle);
      pRepContext->DynDidLength = pItem->Length; /* set current length for updates during service processing */                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      pRepContext->SrcItemIndex = Dcm_Svc2CGetSrcItemIndex(pRepContext->DynDidHandle, pItem->Count);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      pRepContext->DynDidItemCount = pItem->Count;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

      Dcm_UtiProvideResData(pMsgContext, 2u); /* positive response contains the DynDID (2Byte) */                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      /* lStdReturn = DCM_E_OK - already set */
    } /* else DCM_E_NOT_OK (ErrorCode already set) */
    break;
  case DCM_E_PENDING:
    break;
  default: /* DCM_E_NOT_OK */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
    break;
  }
  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_Svc2CReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc2CReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  DCM_IGNORE_UNREF_PARAM(sfRef);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  /* Return sub-function specific length (always zero -> dynamic since it depends on the complete request content) */
  return 0u;
}
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc86ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return Dcm_CfgSvc86SubFuncInfo[sfRef].ReqLength;
}
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_APPL_DATA) versioninfo
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(versioninfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    versioninfo->vendorID = (uint16)DCM_VENDOR_ID;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->moduleID = (uint16)DCM_MODULE_ID;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_major_version = (uint8)DCM_SW_MAJOR_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_minor_version = (uint8)DCM_SW_MINOR_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_patch_version = (uint8)DCM_SW_PATCH_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETVERSIONINFO, lErrorId);
}
#endif
#if (DCM_NET_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetSetComControlChannelState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetComControlChannelState(
  uint8_least index,
  Dcm_CommunicationModeType value
  )
{
  if (Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumAllComMChannels))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_PbRamNetComCtrlChannels[index] = value;                                                                                                      /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_COMCONTROLCHANNEL */
  }
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_TriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerTransmit(
  PduIdType txPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) pTxPduInfo
  )
{
  Std_ReturnType lResult  = E_NOT_OK;                                                                                                                /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(pTxPduInfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(pTxPduInfo->SduDataPtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgGetConnection() != Dcm_NetGetConnHdlOfTxPduId(txPduId)))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_CfgNetPTxObjHandleMemType lPerTxObjHandle;

    lPerTxObjHandle = (Dcm_CfgNetPTxObjHandleMemType)Dcm_NetPeriodicMsgGetHandleFromTxPduId(txPduId);

    if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgNumMsgsCurrConnection() <= lPerTxObjHandle))
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
    else
    {
      P2CONST(PduInfoType, AUTOMATIC, DCM_VAR_NOINIT) txPduInfoPtr = &(Dcm_NetGetPeriodicTxObject(lPerTxObjHandle)->TxPduInfo);

      Dcm_UtiMemCopySafe(txPduInfoPtr->SduDataPtr
                        ,pTxPduInfo->SduDataPtr
                        ,0u
                        ,pTxPduInfo->SduLength
                        ,txPduInfoPtr->SduLength);                                                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */

      pTxPduInfo->SduLength = txPduInfoPtr->SduLength;                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TRIGGERTRANSMIT, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  Dcm_GetActiveProtocol()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(
  P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_APPL_DATA) ActiveProtocol                                                                                   /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  )
{
  uint8 lErrorId;

  DCM_IGNORE_UNREF_PARAM(ActiveProtocol);                                                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ---------------------------------------- */
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)
  lErrorId = DCM_E_INVALID_CONFIG;
#else
  lErrorId = DCM_E_NO_ERROR;

  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(ActiveProtocol == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if(Dcm_NetGetProtIdOfActiveProtocol() >= DCM_NET_INVALID_PROTID)
    {
      *ActiveProtocol = DCM_NO_ACTIVE_PROTOCOL;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      *ActiveProtocol = Dcm_PbCfgNetProtocolInfo[Dcm_NetGetProtIdOfActiveProtocol()].Id;                                                             /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
#endif

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETACTIVEPROTOCOL, lErrorId);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_GetTesterSourceAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetTesterSourceAddress(
  PduIdType DcmRxPduId,
  P2VAR(uint16, AUTOMATIC, DCM_APPL_DATA) TesterSourceAddress
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;                                                                                                            /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(DcmRxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(TesterSourceAddress == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  else if(Dcm_DebugDetectError(Dcm_NetIsGenericConnection(Dcm_NetGetConnHdlOfRxPduId(DcmRxPduId))))
  {
    lErrorId = DCM_E_PARAM;
  }
#endif
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = DCM_E_OK;
    *TesterSourceAddress = Dcm_PbCfgNetConnectionInfo[Dcm_NetGetConnHdlOfRxPduId(DcmRxPduId)].ClientSrcAddr;                                         /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETTESTERSOURCEADDRESS, lErrorId);

  return lResult;
}

#if (DCM_DIAG_VIRTUAL_REQUEST_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessVirtualRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProcessVirtualRequest(
  PduIdType RxPduId,
  Dcm_MsgType Data,
  PduLengthType Length
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(RxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Data == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  /* if (length > <BufferSize>)
  {
    This check is done through the Dcm_NetRxIndInternal API!
  }
  else */
  {
    /* ----- Implementation ------------------------------------------------ */
    uint16 lClientSrcAddr;

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    if(Dcm_NetIsGenericConnection(Dcm_NetGetConnHdlOfRxPduId(RxPduId)))
    {
      /* The client source address is stored behind payload data */
      lClientSrcAddr = Data[Length];
    }
    else
# endif
    {
      lClientSrcAddr = Dcm_NetGetTesterAddress(Dcm_NetGetConnHdlOfRxPduId(RxPduId));
    }

    lResult = Dcm_NetRxIndInternal(RxPduId, lClientSrcAddr, Data, Length);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_PROCESSVIRTUALREQUEST, lErrorId);

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_SetActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(
  boolean active
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (active != DCM_NET_COMM_ACTIVE) && (active != DCM_NET_COMM_NOT_ACTIVE) ))
  {
    lErrorId = DCM_E_PARAM;                                                                                                                          /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Enter critical section (Reason: API is reentrant) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /* Apply new ComM management state */
    Dcm_SingletonContext.Network.ActiveDiagnostic = active;
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETACTIVEDIAGNOSTIC, lErrorId);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_GetRequestKind()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetRequestKind(
  uint16 TesterSourceAddress,
  P2VAR(Dcm_RequestKindType, AUTOMATIC, DCM_APPL_DATA) RequestKind
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(RequestKind == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetConnRefMemType lConnHdl;

    lConnHdl = Dcm_NetGetConnHdlByTesterAddress(TesterSourceAddress);

    /* Check if the tester source address is a valid one */
    if (lConnHdl < DCM_NET_INVALID_CONNHDL)
    {
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      if (Dcm_DebugDetectError(Dcm_NetIsGenericConnection(lConnHdl)))
      {
        lErrorId = DCM_E_PARAM;
      }
      else
#endif
      {
        Dcm_NetTransportObjectPtrType pTranspObj;
        BufReq_ReturnType             lBufReqResult;

        /* Assure that no interrupt will change the state of the conn2tobj map and the tobj state/flags */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
          *=================================*/
        lBufReqResult = Dcm_NetGetTranspObjOfConnection(lConnHdl, &pTranspObj);                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

        if (lBufReqResult == BUFREQ_OK)
        {
          /* If for this tester any request is ongoing: */
          if (pTranspObj->State != DCM_NET_TOBJ_STATE_FREE)
          {
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
            /* Determine the kind of the request */
            if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
            {
              *RequestKind = DCM_REQ_KIND_ROE;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
            }
            else
#endif
            {
              *RequestKind = DCM_REQ_KIND_EXTERNAL;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
            }
          }
          else
          {
            /* Otherwise consider this tester idling */
            *RequestKind = DCM_REQ_KIND_NONE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
          }
        }
        else
        {
          *RequestKind = DCM_REQ_KIND_NONE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        /*=================================*
          END CRITICAL SECTION
          *=================================*/
        Dcm_UtiLeaveCriticalSection();

        lResult = DCM_E_OK;
      }
    } /* else - result already set to NOT_OK */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETREQUESTKIND, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterAllComMActivity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetUnRegisterAllComMActivity(
  void
  )
{
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  {
    Dcm_CfgNetNetIdRefOptType lNetHandleIter;
    for(lNetHandleIter = 0; lNetHandleIter < Dcm_PbCfgNetNumComMChannels; ++lNetHandleIter)
    {
      /* unregister only if no connection is ongoing */
      if(Dcm_PbRamNetComMContext[lNetHandleIter].RegisteredNetworks == 0u)
      {
        ComM_DCM_InactiveDiagnostic(Dcm_PbCfgNetAllComMChannelMap[Dcm_PbCfgNetConnComMChannelMap[lNetHandleIter]]);
      }
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_ComM_NoComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_DIS_TX_DIS);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERNOCOMMODE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ComM_SilentComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_EN_TX_DIS);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERSILENTCOMMODE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ComM_FullComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_EN_TX_EN);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERFULLCOMMODE, lErrorId);
}

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnRequestDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_OnRequestDetection(
  PduIdType canTpRxPduId,
  uint8 tpAddrExtension
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  /* Check initialization state */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Check whether the CanTpPduId is between the upper and lower bound of DCM CanTpPduIds */
    if(
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
        (Dcm_PbCfgNetNumCanRxPduIds > 0u) &&  /* Check for a non-empty map */
# endif
        ( (canTpRxPduId >= Dcm_PbCfgNetCanTpRxPduIdMin)
        &&(canTpRxPduId <= Dcm_PbCfgNetCanTpRxPduIdMax) )
      )
    {
      PduIdType lDcmRxPduId;

      /* Retrieve the DCM RxPduId from the CanTp PduId */
      lDcmRxPduId = Dcm_NetGetDcmRxPduIdOfCanTpPduId(canTpRxPduId);

      /* Check for DCM connection on this SDU */
      if(lDcmRxPduId < Dcm_PbCfgNetNumRxPduIds)
      {
        Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlOfRxPduId(lDcmRxPduId);

        /* Compare the N_TA with the own ECU address. React only on external ECU requests */
        if(Dcm_NetGetNodeAddress(lConnHdl) != tpAddrExtension)
        {
          Dcm_NetOnRequestDetection(lConnHdl);
        }/* else - a DcmPduId and the same N_TA -> request will be ignored on task level */
      }/* else - not a DcmPduId or a functional DcmPduId -> ignore */
    }/* else - not a DcmPduId -> ignore */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_ONREQUESTDETECTION, lErrorId);
}
#endif

/**********************************************************************************************************************
 *  Dcm_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(
  PduIdType rxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(bufferSizePtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  /* info is not NULL_PTR when the connection is generic. */
  else if(Dcm_DebugDetectError( (info == NULL_PTR) &&  Dcm_NetIsGenericConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId)) ))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError(((info != NULL_PTR) /* info pointer check done again to avoid a Misra justification */
                                && Dcm_NetIsGenericConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId))
                                && ((info->SduLength < DCM_NET_METADATA_LENGTH)
                                   || ((info->SduLength > DCM_NET_METADATA_LENGTH) && (tpSduLength < (info->SduLength-DCM_NET_METADATA_LENGTH)))
                                   ))))
  {
    lErrorId = DCM_E_PARAM;
  }
#endif
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = Dcm_NetStartOfReception(rxPduId, info, tpSduLength, bufferSizePtr, DCM_NET_TOBJ_FLAG_NONE);                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_STARTOFRECEPTION, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_CopyRxData()
 *********************************************************************************************************************/
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
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(
  PduIdType rxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType  lResult  = BUFREQ_E_NOT_OK;                                                                                                     /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8              lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsPduInfoValid(info) == FALSE))                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(bufferSizePtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;

    lResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                                     /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

    DCM_IGNORE_UNREF_PARAM(rxPduId);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

    /* Check always for valid call state - if free or ready -> ignore call
    * This check is necessary in case Dcm_Init is called some time during ongoing transmission and in case of a parallel functional TesterPresent
    */
    if ((lResult == BUFREQ_OK)
      && (pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX)
      && (rxPduId == pTranspObj->RxPduId))
    {
      lResult = Dcm_NetCopyRxData(pTranspObj, info, bufferSizePtr);                                                                                  /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
    }
    else if (Dcm_NetIsKeepAliveRequest(rxPduId, info, 2u))                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      *bufferSizePtr = 0u;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
    else if (info->SduLength == 0u)
    {
      *bufferSizePtr = 2u;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
    else
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYRXDATA, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_TpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTpRxIndication(rxPduId, result);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TPRXINDICATION, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(
  PduIdType txPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) retry ,                                                                                             /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsPduInfoValid(info) == FALSE))                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(availableDataPtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError( (retry != NULL_PTR) /* else - if(retry == NULL_PTR) -> not used => OK */
                              &&(retry->TpDataState != TP_DATACONF) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;

    lResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetGetConnHdlOfTxPduId(txPduId), &pTranspObj);                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */

    DCM_IGNORE_UNREF_PARAM(txPduId);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
    DCM_IGNORE_UNREF_PARAM(retry);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

    if(lResult == BUFREQ_OK)
    {
      if(Dcm_DebugDetectError(pTranspObj->State != DCM_NET_TOBJ_STATE_ONTX))                                                                         /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY XF */
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        lResult = Dcm_NetCopyTxData(pTranspObj, info, availableDataPtr);                                                                             /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }
    else
    {
      /* Check for valid call state */
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYTXDATA, lErrorId);

  return lResult;
}                                                                                                                                                    /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_TpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(
  PduIdType txPduId,
  Std_ReturnType result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  DCM_IGNORE_UNREF_PARAM(txPduId);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;
    BufReq_ReturnType             lResult;

    lResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetGetConnHdlOfTxPduId(txPduId), &pTranspObj);                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */

    if(lResult == BUFREQ_OK)
    {
      if(Dcm_DebugDetectError(pTranspObj->State != DCM_NET_TOBJ_STATE_ONTX))
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        Dcm_NetUsdtTxConfirmation(pTranspObj, result);                                                                                               /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }
    else
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TPTXCONFIRMATION, lErrorId);
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TxConfirmation(
  PduIdType txPduId
  )
{
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  uint8 lErrorId = DCM_E_NO_ERROR;

  DCM_IGNORE_UNREF_PARAM(txPduId);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgGetConnection() != Dcm_NetGetConnHdlOfTxPduId(txPduId)))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgGetHandleFromTxPduId(txPduId) >= Dcm_NetPeriodicMsgNumMsgsCurrConnection()))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetPeriodicMsgRelease(Dcm_NetGetPeriodicTxObject(Dcm_NetPeriodicMsgGetHandleFromTxPduId(txPduId)));                                          /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TXCONFIRMATION, lErrorId);
#else
  /* Function is not used since 0x2A or periodic messages are not available.
   * Note: PduR may still need this API to be able to link in case periodic messages are available, but SID 0x2A shall
   * not be supported.
   */
  DCM_IGNORE_UNREF_PARAM(txPduId);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_PagedBufferStart(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgLenType resDataLen
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(pMsgContext->threadId);

  /* Once multiple multi frame response started, can not be started again */
  Dcm_DebugAssert((pContext->PagedBuffer.State == DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                           /* COV_DCM_RTM_DEV_DEBUG XF */
  /* No multiple multi frame response if a NRC has been already registered */
  Dcm_DebugAssert((pContext->Diag.ErrorRegister == DCM_E_OK), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                /* COV_DCM_RTM_DEV_DEBUG XF */

  /*  Initialize paged-buffer response transmission */
  pContext->PagedBuffer.UpdateFuncRef = Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].UpdateFuncRef;                                               /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.CancelFuncRef = Dcm_CfgDiagServiceInfo[pContext->Diag.SidIndex].CancelFuncRef;                                               /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.BufferHdl = Dcm_DiagGetTranspObj(pContext)->BufferHdl;                                                                       /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.WriteIndex = (Dcm_CfgNetBufferSizeMemType)(pContext->Diag.MsgContext.resDataLen                                              /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
    + pContext->Diag.MsgContext.resProtHeadLen); /* Write behind SID and the prepared data */
  pContext->PagedBuffer.ReadIndex = 0u;                                                                                                              /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.RemainingLen = resDataLen; /* Keep track of response data to be copied */                                                    /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.State = DCM_PAGEDBUFFER_STATE_ACTIVATED;                                                                                     /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.UpdateFinished = FALSE;                                                                                                      /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.UpdaterDataProvisionState = DCM_PAGEDBUFFER_WAIT_FOR_DATA;                                                                   /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  pContext->PagedBuffer.OpStatus = DCM_INITIAL;                                                                                                      /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  /*  Calculate total response length, considering any already linearly written data */
  pContext->Diag.MsgContext.resDataLen += (Dcm_MsgLenType)resDataLen;                                                                                /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  /*  Initiate paged-data reader */
  Dcm_TskSetEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, pMsgContext->threadId);
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferTask()
 *********************************************************************************************************************/
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
 */
FUNC(void, DCM_CODE) Dcm_PagedBufferTask(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType               lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  /* If any diagnostic job is open: */
  if( ((pContext->Diag.State & (DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX)) != 0u)
    && (!Dcm_NetBufferIsFree(pContext->PagedBuffer.BufferHdl)))                                                                                      /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    /* Avoid buffer releasing on asynchronous TxConfirmation with failed status */
    Dcm_NetBufferUsageEnter(pContext->PagedBuffer.BufferHdl);
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();

    /* Update paged data */
    lStdResult = Dcm_PagedBufferUpdatePage(pContext, &lNrc);                                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    Dcm_NetBufferUsageLeave(pContext->PagedBuffer.BufferHdl);

    /* If the application needs more time: */
    if(lStdResult == DCM_E_PENDING)
    {
      /* If the paged-buffer was not the last user, try getting data again */
      if(!Dcm_NetBufferIsFree(pContext->PagedBuffer.BufferHdl))
      {
        Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_PGDBUF_UPDATE_PAGE);                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      }/* else - buffer is free - no more data is to be expected */
    }/* else - that is all */
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();

    if (lStdResult == DCM_E_NOT_OK)
    {
      Dcm_SetNegResponse(lNrc, pContext->ThreadId);
      Dcm_ProcessingDone(pContext->ThreadId);
    }
  }
  else
  {
    /*=================================*
    END CRITICAL SECTION
    *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif
/**********************************************************************************************************************
 *  Dcm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_InitMemory(
  void
  )
{
  Dcm_DebugDeInit();
}

/**********************************************************************************************************************
 *  Dcm_Init()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_Init(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#if (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  if(Dcm_VarMgrInit(configPtr) == TRUE)                                                                                                              /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* SBSW_DCM_PARAM_PTR_FORWARD */
#endif
  {
    /* Initialize run-time kernel sub-system */
    Dcm_TmrInit();
    Dcm_TskInit();

    /* Initialize communication sub-systems */
    Dcm_NetInit(configPtr);                                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
    Dcm_PagedBufferInit();
#endif

    /* Initialize diagnostic service helpers sub-systems */
#if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_DidMgrInit();                                                                                                                                /* PRQA S 2987 */ /* MD_Dcm_2987 */
#endif
#if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_RidMgrInit();
#endif
    Dcm_RsrcMgrInit();

    /* Initialize diagnostic state management sub-systems */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON) || \
    (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
    Dcm_ModeInit();
#endif
    Dcm_StateInit();
#if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_AuthMgrInit();                                                                                                                               /* PRQA S 2987 */ /* MD_Dcm_2987 */
#endif

    /* Initialize diagnostic service processing kernel sub-system */
    Dcm_DiagInit(configPtr);                                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Initialize debug sub-component as last to mark DCM as completely initialized */
    Dcm_DebugInit();/* from now on the DCM is ready to work */
#if (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_FblMgrReset();
#endif /* (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON) */
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunctionTimer(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_HIGH);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionWorker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunctionWorker(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_LOW);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  Dcm_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunction(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_NONE);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

/**********************************************************************************************************************
 *  Dcm_GetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_ContextPtrType, DCM_CODE) Dcm_GetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_ContextPtrType pContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pContext = &Dcm_ThreadContext[0];
  }
  else
  {
    pContext = &Dcm_ThreadContext[threadId];
  }

  return pContext;
}
/**********************************************************************************************************************
 *  Dcm_DiagIsKernelBusy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsKernelBusy(
  void
  )
{
  boolean             lResult = FALSE;
  Dcm_ThreadIdOptType lThreadIter;

  for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
  {
    Dcm_ContextConstPtrType pContext = Dcm_GetThreadContext(lThreadIter);

    if (pContext->Diag.State != DCM_DIAG_STATE_IDLE)
    {
      lResult = TRUE;
      break;
    }
  }
  return lResult;
}

#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsEnabledInActiveVariants(
  Dcm_CfgStateRefOptType cfgVariantRef
  )
{
  return Dcm_VarMgrIsEnabledInActiveVariants(Dcm_PbCfgDiagVariantFilter[cfgVariantRef]);                                                             /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagVariantLookUpFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(sint16_least, DCM_CODE) Dcm_DiagVariantLookUpFilter(
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value
  )
{
  sint16_least lLookUpResult = Dcm_UtiLookUpUint8(lookUpTable, value);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

  DCM_IGNORE_UNREF_PARAM(lookUpFilterTable);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  if (lLookUpResult >= 0)
  {
    if (Dcm_DiagIsEnabledInActiveVariants(lookUpFilterTable[lLookUpResult]) == FALSE)                                                                /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      lLookUpResult = -1; /* override result */
    }
  }
#endif

  return lLookUpResult;
}

/**********************************************************************************************************************
 *  Dcm_ExternalProcessingDone()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ExternalProcessingDone(
  Dcm_MsgContextPtrType pMsgContext                                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_ProcessingDone(pMsgContext->threadId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_EXT_PROCESSINGDONE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ExternalSetNegResponse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ExternalSetNegResponse(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodeType errorCode
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_SetNegResponse(errorCode, pMsgContext->threadId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_EXT_SETNEGRESPONSE, lErrorId);
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProvideRecoveryStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  )
{
  Std_ReturnType  lResult  = DCM_E_NOT_OK;
  uint8           lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(RecoveryInfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagProvideRecoveryStates(RecoveryInfo);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_PROVIDERECOVERYSTATES, lErrorId);

  return lResult;
}
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSpecificCauseCode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagSetSpecificCauseCode(specificCauseCode);
    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETSPECIFICCAUSECODE, lErrorId);

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagGetDemClientId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_DiagGetDemClientId(Dcm_ReadOnlyMsgContextPtrType pMsgContext)
{
  return (Dcm_NetGetProtObjOfProtId(Dcm_NetGetProtIdOfMsgContext(pMsgContext))->DemClientId);
}

/**********************************************************************************************************************
 *  Dcm_DiagGetCancelFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_DiagSvcCancelFuncType, DCM_CODE) Dcm_DiagGetCancelFunc(
  uint8_least index
  )
{
  Dcm_DiagSvcCancelFuncType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_CFGDIAGSVCCANCELLERS_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = Dcm_CfgDiagSvcCancellers[0];
  }
  else
  {
    lResult = Dcm_CfgDiagSvcCancellers[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithOnlySubFuncReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_DiagSvcWithOnlySubFuncReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  DCM_IGNORE_UNREF_PARAM(sfRef);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return 1u;
}

/**********************************************************************************************************************
 *  Dcm_DiagNoSequenceChecker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagNoSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(subSvcRef);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_DiagVerifyServiceWithSubFunction()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagVerifyServiceWithSubFunction(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  Dcm_DiagServiceLengthGetterFuncType svcLengthGetter,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptPtrType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  sint16_least   lLookUpResult;
  uint8          sfID = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                          /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  *pSubSvcRef = 0;                                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Min Length already checked in DiagDispatcher and expected to be 1 !!! */
  Dcm_DebugAssert((pMsgContext->reqDataLen > 0u), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                            /* COV_DCM_RTM_DEV_DEBUG XF */

  lLookUpResult = Dcm_DiagLookUpFilter(pMsgContext
                                      ,lookUpTable
                                      ,lookUpFilterTable
                                      ,sfID
                                      ,DCM_DIAG_CHK_LVL_SUBFUNC
                                      ,ErrorCode);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lLookUpResult >= 0)
  {
    /* Mark found position in any case if found */
    *pSubSvcRef = (Dcm_DiagSubServiceRefOptType)lLookUpResult;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Check security, sequence, and mode */
    lStdResult = Dcm_DiagSvcWithSubFuncStateCheck(pMsgContext
                                                 ,lookUpFilterTable[lLookUpResult]
                                                 ,subFuncSequenceChecker
                                                 ,*pSubSvcRef
                                                 ,ErrorCode);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */


    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
    {
      Dcm_DiagSubServiceLengthType expectedLength;
      expectedLength = svcLengthGetter(*pSubSvcRef);                                                                                                 /* SBSW_DCM_PARAM_FUNCPTR_CALL */

      if( (expectedLength != 0u) /* dynamic length */
        &&(expectedLength != pMsgContext->reqDataLen) ) /* SF + data */
      {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
    /* Extract SF in any case if supported */
    Dcm_UtiConsumeReqData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, sfID);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_DiagSetNewReqBaseToCurProgress(pMsgContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_DiagSetNewResBaseToCurProgress(pMsgContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* else - ErrorCode is already set */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_MSR_STPAR */

/**********************************************************************************************************************
 *  Dcm_DiagTaskRx()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_DiagTaskRx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pQueuedTObj;

  pQueuedTObj = Dcm_DiagGetQueuedTranspObj(pContext);

  /* If new request received: */
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RX_NEW_REQ))
  {
    /* Initialize related objects */
    pQueuedTObj->State = DCM_NET_TOBJ_STATE_READY;                                                                                                   /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
    Dcm_DiagResetRcrRpLimiter(pContext);                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    /* Synchronize with diagnostic job processing task */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_NEW_REQ, pContext->ThreadId);
  }

  /* If P2 timeout detected: */
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RX_TMR_P2_TO))
  {
    /* Pre-processing a P2 timeout in oder to cancel the working thread in time (on non-split task configuration) */
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
    /* If RCR-RP limit reached: */
    if(Dcm_DiagGetRcrRpLimiter(pContext) == 0u)
    {
      /* Reset any RCR-RP related states and notify worker task to initiate job cancellation */
      Dcm_DiagRcrRpTxFinished(pQueuedTObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

      /*
       * Signal cancellation to the worker. If worker gets interrupted by this high priority task,
       * while already called "processing done", the cancellation signal will be lost and
       * the final response will be sent at next worker task call.
       */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_CANCEL_OP, pContext->ThreadId);

      /*
       * Do not send RCR-RP and restart the P2 timer again since the limit has been reached and the worker will
       * finalize the job, once the cancel signal is received.
       */
    }
    else
#endif
    {
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
      /* Otherwise - update RCR-RP limiter */
      Dcm_DiagDecrementRcrRpLimiter(pContext);                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
      /* Notify diagnostic transmission task to initiate RCR-RP transmission */
      pQueuedTObj->Flags |= DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ;                                                                                          /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, pContext->ThreadId);
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagTaskWorker()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_DiagTaskWorker(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Important: this event is a synchronization event between high- and low-priority tasks.
   * Never move this event processing into another task than the one that initiates the new request processing (DCM_TSK_EV_DIAG_WORK_NEW_REQ).
   * Otherwise the clean-up jobs from DCM_TSK_EV_DIAG_WORK_KILL_JOBS event may be performed AFTER the new request processing has been started.
   * This will lead to out of sync of services post- and main-processing!
  */
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_KILL_JOBS))
  {
    Dcm_DiagWorkerDoCancelProcessing(pContext, pEventContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL))
  {
    Dcm_DiagWorkerDoPostProcessing(pContext);                                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF))
  {
    Dcm_DiagWorkerSetDefSessionExtern(pContext, pEventContext);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF))
  {
    Dcm_StateSetSession(0);
  }

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_IDLE_CONN))
  {
    Dcm_AuthMgrDeauthenticateIdleConn();
  }
#endif

#if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SETROLE))
  {
    Dcm_AuthMgrSetDeauthenticatedRole(pContext, pEventContext);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS))
  {
    Dcm_DiagWorkerSetSecurityBypass(pContext, pEventContext);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#endif

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_NEW_REQ))
  {
    Dcm_DiagWorkerProcessNewRequest(pContext, pEventContext);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_RCRRP))
  {
    Dcm_DiagWorkerProcessRcrRpTxEnd(pContext, pEventContext);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_CANCEL_OP))
  {
    Dcm_DiagWorkerCancelOperation(pContext, pEventContext);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#endif

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT))
  {
    Dcm_DiagWorkerDoRepeat(pContext, pEventContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_DiagTaskTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskTx(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED))
  {
    Dcm_DiagTxPostponeFinalPagedResponse(pContext, pEventContext);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#endif

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, (DCM_TSK_EV_DIAG_TX_SEND_PAGED | DCM_TSK_EV_DIAG_TX_SEND_LINEAR)))
  {
    Dcm_DiagTxStartFinalResponse(pContext, pEventContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_RCRRP))
  {
    Dcm_DiagTxStartRcrRpResponse(pContext);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

#if (DCM_TSK_NUM_DIAG_RECOVERY > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskRecovery()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_DiagTaskRecovery(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE))
  {
    Std_ReturnType lResult;

    /* Retrieve the recovery information from the application */
    lResult = Dcm_GetRecoveryStates(&Dcm_SingletonContext.Recovery.RecoveryInfo);                                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

    switch(lResult)
    {
    case DCM_E_OK:
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_PROCESS);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_SingletonContext.Recovery.OpStatus = DCM_INITIAL;
      break;
    case DCM_E_PENDING:
      /* Retry next time */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE);                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      Dcm_DebugAssert((lResult == DCM_E_NOT_OK), DCM_SID_GETRECOVERYSTATES, DCM_E_INTERFACE_RETURN_VALUE);                                           /* COV_DCM_RTM_DEV_DEBUG XF */
      /* Abort recovering */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_PROCESS))
  {
    Std_ReturnType   lResult;
    lResult = Dcm_DiagSafeProcessRecoveryInfo(&Dcm_SingletonContext.Recovery.RecoveryInfo
                                             ,Dcm_SingletonContext.Recovery.OpStatus);                                                               /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

    switch(lResult)
    {
    case DCM_E_OK:
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    case DCM_E_PENDING:
      /* Retry next time */
      Dcm_SingletonContext.Recovery.OpStatus = DCM_PENDING;
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_RECOVERY_PROCESS);                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default: /* DCM_E_NOT_OK */
      /* Abort recovering */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH))
  {
# if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) && \
     (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc2COnStateChanged();
# endif
    Dcm_SingletonContext.Network.RxAllowed = TRUE;
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_GARB_COL > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskGarbageCollector()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_DiagTaskGarbageCollector(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_TskTaskEvOptType        lEv = pEventContext->Ev;
  Dcm_CfgNetTObjHandleOptType lTObjCntr;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Cleanup or reject all transport objects that never succeeded and are now canceled */
  lTObjCntr = 0;
  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    /* Check who shall be removed */
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj;

      pTranspObj = Dcm_NetGetTransportObject(lTObjCntr);
      /* Always unregister ComM activity here, since CANCELED flag will not clean it up!
       * If no RCR-RP on transmission: everything has to be cleaned up here!
       */
      Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);

      /* If just requested, then only this TObject could be the trigger for P2 timer - avoid any RCR-RP transmission */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ) != 0u)
      {
        Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, Dcm_NetGetThreadIdOfTranspObj(pTranspObj));                       /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }

      Dcm_UtiEnterCriticalSection();
      /*=================================*
       BEGIN CRITICAL SECTION
       *=================================*/
      /* Check if any RCR-RP for this object is on Tx (DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ is excluded since the high-prio
       * Dcm_DiagTaskTx() shall already have switched to ON_TX!) */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_RCRRP_ON_TX) != 0u)
      {
        pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED; /* isolate this connection */                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

        /* Try cancel any ongoing response. The return value does not matter:
         * OK -> there will be a confirmation for this cancellation
         * N_OK -> request rejected, but there will be a confirmation due to the ongoing transmission
         */
        (void)PduR_DcmCancelTransmit(Dcm_PbCfgNetConnectionInfo[pTranspObj->ConnHdl].TxPduIdMain);
        /* Any RCR-RP will just end with releasing the connection assigned to the tObject!
         * No confirmation post-processing will be performed! */
      }
      else
      {
        Dcm_NetBufferRelease(pTranspObj->BufferHdl);
        Dcm_NetUnRegisterActiveConnection(pTranspObj); /* no further processing needed */                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }
      /*=================================*
       END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }
    ++lTObjCntr;
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskFblRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskFblRes(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Check for eventual FBL final response request */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_SEND))
  {
    Dcm_DiagFblResDoGetProgCond(pContext, pEventContext);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If final response required, wait for ComM before sending the response */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM))
  {
    Dcm_DiagFblResDoWaitTxComm(pContext, pEventContext);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* Once final FBL response request processed, reset all flags to prevent from a new response at next power-on/reset */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS))
  {
    Dcm_DiagFblResDoResetFlags(pContext, pEventContext);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If any FBL final response confirmation received, do the designated job (e.g. session transition) */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_TXCONF))
  {
    Dcm_DiagFblResDoProcessTxConfirmation(pContext, pEventContext);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}
#endif
#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetSingle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetSingle(
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (VsgId >= DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgId == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError( (State != DCM_VSG_ENABLED)
                               &&(State != DCM_VSG_DISABLED) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = DCM_E_OK;

    Dcm_VarMgrVsgSetSingle(VsgId, State);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGSETSINGLE, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActive(
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (VsgId >= DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgId == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError(State == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = DCM_E_OK;

    Dcm_VarMgrVsgIsActive(VsgId, State);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGISACTIVE, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetMultiple()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetMultiple(
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  Dcm_VsgStateType State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(VsgIdList == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError( (VsgListSize > DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgListSize == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_VsgIdentifierType lVsgIter;

    /* Iterate over all passed VSGs */
    for (lVsgIter = 0; lVsgIter < VsgListSize; ++lVsgIter)
    {
      lStdResult = Dcm_VsgSetSingle(VsgIdList[lVsgIter], State);

      if(lStdResult == DCM_E_NOT_OK)
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGSETMULTIPLE, lErrorId);

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActiveAnyOf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActiveAnyOf(
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(VsgIdList == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError( (VsgListSize > DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgListSize == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError(State == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_VsgIdentifierType lVsgIter;

    /* Iterate over all passed VSGs */
    for (lVsgIter = 0; lVsgIter < VsgListSize; ++lVsgIter)
    {
      lStdResult = Dcm_VsgIsActive(VsgIdList[lVsgIter], State);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

      if( (lStdResult == DCM_E_NOT_OK)
        ||(*State == DCM_VSG_ENABLED) )
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGISACTIVEANY, lErrorId);

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
# if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2COnStateChanged()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_Svc2COnStateChanged(
  void
  )
{
  /* clear all DynDID definitions not supported any more */
  Dcm_NegativeResponseCodeType     lNrc;
  Std_ReturnType                   lStdResult;
  Dcm_CfgDidMgrDynDidHandleOptType lDynDidIter;
#  if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
  boolean                          lAnyCleared = FALSE;
#  endif

  for (lDynDidIter = 0; lDynDidIter < DCM_NUM_DYNDIDS; ++lDynDidIter)
  {
    if (Dcm_Svc2CIsDynDidDefined(lDynDidIter))
    {
      lStdResult = Dcm_DidMgrStaticDidNoAuthStateCheck(Dcm_DidMgrGetDynDidReadExecPrecond(lDynDidIter), &lNrc);                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

#  if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
      if (lStdResult == DCM_E_OK) /* all DDDID conditions OK - check inside */
      {
        lStdResult = Dcm_Svc2CStateCheckSrcItems(DCM_NET_INVALID_CONNHDL, lDynDidIter, &lNrc);                                                       /* SBSW_DCM_POINTER_FORWARD_STACK */
      }
#  endif

      if (lStdResult == DCM_E_NOT_OK) /* any condition not fulfilled? */
      {
        Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)lDynDidIter);
#  if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
        if (lAnyCleared == FALSE)
        {
          lAnyCleared = TRUE;
          Dcm_UtiNvMSetRamBlockStatus((uint16)DCM_SVC_2C_NVRAM_BLOCKID);
        }
#  endif
      }
    } /* else - DDDID not defined yet */
  }
}
# endif
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_MsgType, DCM_CODE) Dcm_NetPeriodicMsgGetTxBuffer(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  )
{
  return Dcm_NetGetPeriodicTxObject(perTxObjHandle)->TxBuffer;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetPeriodicTxObject()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetPeriodicTxObjectPtrType, DCM_CODE) Dcm_NetGetPeriodicTxObject(
  Dcm_CfgNetPTxObjHandleOptType index
  )
{
  Dcm_NetPeriodicTxObjectPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumPerTxObjects))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetPeriodicTxObject[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetPeriodicTxObject[index];
  }

  return lResult;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPeriodicMsgAllocate(
  uint16 Did
  )
{
  Dcm_CfgNetPTxObjHandleMemType perTxObjHandle = DCM_NET_INVALID_PTXOBJ_HANDLE;

  /* If all queued messages were sent, reset counters (continue with highest prio UUDT) */
  if(Dcm_NetPeriodicMsgGetNextMsgHdl() == Dcm_NetPeriodicMsgGetTxObjectSentCntr())
  {
    /* Note: No critical section needed to avoid interrupt by Dcm_NetPeriodicMsgRelease because at this point, all
    *       messages are already sent and so no more notification interrupt can occur */
    Dcm_NetPeriodicMsgResetNextMsgHdl();
    Dcm_NetPeriodicMsgResetTxObjectSentCntr();
  }

  /* If there is a message ID left that has a lower priority: */
  if(Dcm_NetPeriodicMsgGetNextMsgHdl() < Dcm_NetPeriodicMsgNumMsgsCurrConnection())
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    perTxObjHandle = (Dcm_CfgNetPTxObjHandleMemType)Dcm_NetPeriodicMsgGetNextMsgHdl();
    pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(perTxObjHandle);
    pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_RESERVED; /* Mark tx object as reserved */                                                     /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    pPeriodicTxObj->Did = Did; /* Store the DID */                                                                                                   /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    Dcm_NetPeriodicMsgIncNextMsgHdl(); /* Increment message handle for next message */
  }

  return perTxObjHandle;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgRelease(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  )
{
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  if(pPeriodicTxObj->State == DCM_NET_PERIODIC_TX_STATE_ONTX)
  {
    Dcm_NetPTxDelayTimerStart(); /* start delay timer */
  }
# endif

  Dcm_NetPTxObjTimerStop(pPeriodicTxObj);                                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_FREE;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_NetPeriodicMsgIncTxObjectSentCntr(); /* Update sent message statistics */
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgResetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgResetConnection(
  void
  )
{
  Dcm_NetPeriodicMsgResetNextMsgHdl();
  Dcm_NetPeriodicMsgResetTxObjectSentCntr();
  Dcm_NetPeriodicMsgSetConnection(DCM_NET_INVALID_CONNHDL);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgStopDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgStopDid(
  uint16 Did
  )
{
  Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
  for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPeriodicMsgGetNextMsgHdl(); lPerTxObjIter++)
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    pPeriodicTxObj = Dcm_NetGetPeriodicTxObject(lPerTxObjIter);

    if((pPeriodicTxObj->State != DCM_NET_PERIODIC_TX_STATE_FREE)
      && (pPeriodicTxObj->Did == Did))
    {
      /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
      Dcm_UtiEnterCriticalSection(); /* Enter critical section (Reason: Protect against high level interrupts from the network interface) */
      switch (pPeriodicTxObj->State)
      {
        case DCM_NET_PERIODIC_TX_STATE_ONTX:
          (void)PduR_DcmCancelTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(lPerTxObjIter)); /* try to cancel still ongoing Tx */                    /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
          /* fall through */
        case DCM_NET_PERIODIC_TX_STATE_RESERVED:                                                                                                     /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
        case DCM_NET_PERIODIC_TX_STATE_QUEUED:
# endif
          Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                 /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
          break;
        default:
          /* nothing to do */
          break;
      }
      Dcm_UtiLeaveCriticalSection();
      /*=================================*
      END CRITICAL SECTION
      *=================================*/
    }
  }
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmit(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  PduLengthType length
  )
{
  PduLengthType lLength = length;

  Dcm_DebugAssert((Dcm_NetPeriodicMsgGetConnection() < DCM_NET_INVALID_CONNHDL), DCM_SID_INTERNAL, DCM_E_PARAM);                                     /* COV_DCM_RTM_DEV_DEBUG XF */

# if (DCM_NET_PERIODIC_MSG_PADDING_ENABLED == STD_ON)
  if(Dcm_DebugDetectRuntimeError(Dcm_PbCfgNetConnectionInfo[Dcm_NetPeriodicMsgGetConnection()].UudtFrameSize > DCM_NET_PERIODIC_BUFFER_SIZE))        /* COV_DCM_RTM_RUNTIME_CHECK XF */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    for (; lLength < Dcm_PbCfgNetConnectionInfo[Dcm_NetPeriodicMsgGetConnection()].UudtFrameSize; lLength++)
    {
      pPeriodicTxObj->TxBuffer[lLength] = DCM_NET_PERIODIC_MSG_PADDING_PATTERN;                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
# endif

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  /* In case of generic connection, append the meta data at the end of the paylaod. */
  if(Dcm_NetIsGenericConnection(Dcm_NetPeriodicMsgGetConnection()))
  {
    if(Dcm_DebugDetectRuntimeError(lLength > (PduLengthType)(DCM_NET_PERIODIC_BUFFER_SIZE - DCM_NET_METADATA_LENGTH)))                               /* COV_DCM_RTM_RUNTIME_CHECK XF */
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    }
    else
    {
      pPeriodicTxObj->TxBuffer[lLength] = Dcm_NetGetNodeAddress(Dcm_SingletonContext.Network.PeriodicTxContext.ConnHdlInUse);                        /* SBSW_DCM_PARAM_PTR_WRITE */
      lLength++;
      pPeriodicTxObj->TxBuffer[lLength] = (Dcm_MsgItemType)Dcm_NetPeriodicMsgGetClientSrcAddr();                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      lLength++;
    }
  }
# endif

  pPeriodicTxObj->TxPduInfo.SduLength = lLength;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_QUEUED;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_UUDT);
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetGetKeepAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetGetKeepAlive(
  void
  )
{
  return Dcm_SingletonContext.Network.KeepAlive;
}

#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetActiveComVariant()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgVarMgrComVariantType, DCM_CODE) Dcm_NetGetActiveComVariant(
  void
  )
{
  return Dcm_SingletonContext.Network.ComVariant;
}
#endif

#if (DCM_NET_RX_BLOCKING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetSetRxAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetRxAllowed(
  boolean rxAllowed
  )
{
  Dcm_SingletonContext.Network.RxAllowed = rxAllowed;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetGetInvalidConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetGetInvalidConnHdl(
  void
  )
{
  return (Dcm_CfgNetNetIdRefMemType)Dcm_PbCfgNetNumConnections;
}

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetAuthInfoRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetGetAuthInfoRef(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_PbCfgNetConnectionInfo[connHdl].AuthInfoRef;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetGetSessionConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetSessionConnection(
  void
  )
{
  return Dcm_SingletonContext.Network.SessionConnection;
}

#if ((DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) && (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)) || \
    (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByConnectionId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByConnectionId(
  uint16 connectionId
  )
{
  Dcm_NetConnRefOptType lConnIter;
  Dcm_NetConnRefMemType lResult = DCM_NET_INVALID_CONNHDL;

  for(lConnIter = 0; lConnIter < Dcm_PbCfgNetNumConnections; ++lConnIter)
  {
    if(connectionId == Dcm_PbCfgNetConnectionInfo[lConnIter].ConnectionId)
    {
      lResult = (Dcm_NetConnRefMemType)lConnIter;
      break;
    }
  }
  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetConnectionInfo()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetConnectionInfoType, DCM_CODE) Dcm_PbCfgNetGetConnectionInfo(
  uint8_least index
  )
{
  return Dcm_PbCfgNetConnectionInfo[index];
}

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetConnComMChannelMap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_PbCfgNetGetConnComMChannelMap(
  uint8_least index
  )
{
  return Dcm_PbCfgNetConnComMChannelMap[index];
}

#if (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetComCtrlChannelListAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_PbCfgNetGetComCtrlChannelListAll(
  uint8_least index
  )
{
  return Dcm_PbCfgNetComCtrlChannelListAll[index];
}
#endif

/**********************************************************************************************************************
 *  Dcm_PbCfgNetGetNumAllComMChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NetworkHandleType, DCM_CODE) Dcm_PbCfgNetGetNumAllComMChannels(
  void
  )
{
  return Dcm_PbCfgNetNumAllComMChannels;
}

#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PbRamNetGetComCtrlChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CommunicationModeType, DCM_CODE) Dcm_PbRamNetGetComCtrlChannels(
  uint8_least index
  )
{
  return Dcm_PbRamNetComCtrlChannels[index];
}
#endif

#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PbCfgNetAllComMChannelMapGetEntry()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(NetworkHandleType, DCM_CODE) Dcm_PbCfgNetAllComMChannelMapGetEntry(
  Dcm_CfgNetNetIdRefOptType index
  )
{
  return Dcm_PbCfgNetAllComMChannelMap[index];
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON) || \
    (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetHasRespPendOnBoot()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetGetHasRespPendOnBoot(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  return Dcm_NetGetProtObjOfTranspObj(Dcm_DiagGetTranspObj(pContext))->HasRespPendOnBoot;                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetGetProtObjOfTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetProtocolInfoConstPtrType, DCM_CODE) Dcm_NetGetProtObjOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetGetProtObjOfConnection(pTranspObj->ConnHdl);
}

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlOfRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlOfRxPduId(
  PduIdType rxPduId
  )
{
  return Dcm_NetCfgGetConnHdlOfRxPduId(Dcm_SingletonContext.Network.ComVariant, rxPduId);
}

/**********************************************************************************************************************
 *  Dcm_NetGetNumConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetNumConnections(
  void
  )
{
  return Dcm_NetCfgGetNumConnections(Dcm_SingletonContext.Network.ComVariant);
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetNumPeriodicTxPduIds()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetGetNumPeriodicTxPduIds(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_PbCfgNetConnectionInfo[connHdl].NumPeriodicTxPduIds;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetGetNumPerTxObjects()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetGetNumPerTxObjects(
  void
  )
{
  return Dcm_NetCfgGetNumPerTxObjects(Dcm_SingletonContext.Network.ComVariant);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPeriodicMsgGetConnection(
  void
  )
{
  return Dcm_SingletonContext.Network.PeriodicTxContext.ConnHdlInUse;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_SingletonContext.Network.PeriodicTxContext.ConnHdlInUse = connHdl;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetClientSrcAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetPeriodicMsgGetClientSrcAddr(
  void
  )
{
  return Dcm_SingletonContext.Network.PeriodicTxContext.ClientSrcAddr;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_NetPeriodicMsgSetClientSrcAddr()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetClientSrcAddr(
  uint16 clientSrcAddr
  )
{
  Dcm_SingletonContext.Network.PeriodicTxContext.ClientSrcAddr = clientSrcAddr;
}
#endif

/**********************************************************************************************************************
*  Dcm_NetGetBufferInfo()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_PbCfgNetBufferInfoPtrType, DCM_CODE) Dcm_NetGetBufferInfo(
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumBuffers))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbCfgNetBufferInfo[0];
  }
  else
  {
    lResult = &Dcm_PbCfgNetBufferInfo[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetTransportObject()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_NetGetTransportObject(
  Dcm_CfgNetTObjHandleOptType index
  )
{
  Dcm_NetTransportObjectPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= Dcm_PbCfgNetNumTransportObjects))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetTransportObject[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetTransportObject[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetComState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComState(
  PduIdType rxPduId
  )
{
  Dcm_NetComMStateType lResult  = DCM_NET_COMM_STATE_RX_DIS_TX_DIS;                                                                                  /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8                lErrorId = DCM_E_NO_ERROR;

  DCM_IGNORE_UNREF_PARAM(rxPduId);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(Dcm_NetIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = Dcm_NetGetComStateByRxPduId(rxPduId);                                                                                                  /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_INTERNAL, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageLeave()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetBufferUsageLeave(
  Dcm_CfgNetBufferRefMemType bufferHdl
  )
{
  Dcm_NetGetBufferContext(bufferHdl)->IsInUseCnt--;                                                                                                  /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageEnter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetBufferUsageEnter(
  Dcm_CfgNetBufferRefMemType bufferHdl
  )
{
  Dcm_NetGetBufferContext(bufferHdl)->IsInUseCnt++;                                                                                                  /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_NetGetProtObjOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetProtocolInfoConstPtrType, DCM_CODE) Dcm_NetGetProtObjOfConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetGetProtObjOfProtId(Dcm_NetGetProtIdOfConnection(connHdl));
}

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetRxIndInternal()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_NetRxIndInternal(
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,                                                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  PduLengthType length
  )
{
  Std_ReturnType                              notifResult = DCM_NET_ARENV_NTFRSLT_NOT_OK;
  BufReq_ReturnType                           bufReq;
  PduLengthType                               bufferSize;
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC)  pInfo;
# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  uint8                                     lMetaData[DCM_NET_METADATA_LENGTH];
  PduInfoType                               lInfo;
# endif

  DCM_IGNORE_UNREF_PARAM(sourceAddress);                                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  if(Dcm_NetIsGenericConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId)))
  {
    lMetaData[DCM_NET_METADATA_SA_OFFSET] = (uint8)sourceAddress;                                                                                    /* SBSW_DCM_LOCAL_ARRAY_WRITE */
    lMetaData[DCM_NET_METADATA_TA_OFFSET] = Dcm_NetGetNodeAddress(Dcm_NetGetConnHdlOfRxPduId(rxPduId));                                              /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_LOCAL_ARRAY_WRITE */
    lInfo.SduDataPtr = lMetaData;
    lInfo.SduLength = DCM_NET_METADATA_LENGTH;
    pInfo = &lInfo;
  }
  else
# endif
  {
    pInfo = NULL_PTR;
  }

# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_DebugAssert((Dcm_NetGetConnHdlOfRxPduId(rxPduId) < Dcm_PbCfgNetNumConnections), DCM_SID_INTERNAL, DCM_E_PARAM);                                /* COV_DCM_RTM_DEV_DEBUG XF */
# endif

  /* start virtual request; */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Initiate reception for the required RxPduID. Mark this reception as an internal one in order to get correct
   * ComM management.
   */
  bufReq = Dcm_NetStartOfReception(rxPduId, pInfo, length, &bufferSize, DCM_NET_TOBJ_FLAG_INTERNAL);                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();

  /* If reception accepted: */
  if(bufReq == BUFREQ_OK)
  {
    Dcm_NetTransportObjectPtrType pTranspObj;
    bufReq = Dcm_NetGetTranspObjOfConnection(Dcm_NetGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                                      /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

    if(bufReq == BUFREQ_OK)                                                                                                                          /* COV_DCM_RTM_UNREACHABLE XF */
    {
# if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
      /* If not a pseudo parallel request */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) == 0u)
# endif
      {
        PduInfoType pduInfo;

        pduInfo.SduDataPtr = data;
        pduInfo.SduLength = length;

        /* Copy received data */
        bufReq = Dcm_CopyRxData(rxPduId, &pduInfo, &bufferSize);                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
        /* If data copy succeeded: */
        if(bufReq == BUFREQ_OK)                                                                                                                      /* COV_DCM_RTM_UNREACHABLE TX */
        {
          /* Notify DCM about successful reception */
          notifResult = DCM_NET_ARENV_NTFRSLT_OK;
        }/* else - Dcm_CopyRxData failed -> exit with Dcm_TpRxIndication(failed) */
      }/* else - a pseudo parallel request will not be processed over an internal request */
    }
    else
    {
      /* Check for valid call state */
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    }
    /* Notify DCM about reception accomplishment only in case the reception was accepted */
    Dcm_TpRxIndication(rxPduId, notifResult);
  }/* else - no reception possible exit with fail */
  /* Return the operation result to the caller */
  return notifResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetTaskTaMonitor()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_NetTaskTaMonitor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjCntr;
  Dcm_TskTaskEvOptType        lEv = pEventContext->Ev;

  /* Check whether any request which has to be canceled is in progress */
  if(((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE | DCM_DIAG_QUEUE_FLAG_IS_WAITING) & pContext->Diag.QueuedSet.Flags) != 0u)
  {

    /* Enter critical section (Reason: the transport object may not be modified from outside) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/

    /* If a request is in progress */
    if((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE & pContext->Diag.QueuedSet.Flags) != 0u)
    {
      Dcm_TskTaskEvMemType tObjEv = Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, Dcm_DiagGetTranspObj(pContext)->Handle);

      /* If the ongoing request is from the same tester as the new request with the foreign N_TA */
      if (Dcm_TskIsLocalEventSet(lEv, tObjEv))
      {
        /* Cancel the request processing */
        Dcm_DiagCancelProcessing(DCM_THREAD_ID_BASIC);
      }/* else - post processing or idle are to be handled in a normal way */
    }

    /* If a waiting new request is in the queue */
    if((DCM_DIAG_QUEUE_FLAG_IS_WAITING & pContext->Diag.QueuedSet.Flags) != 0u)
    {
       Dcm_TskTaskEvMemType tObjEv = Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, Dcm_DiagGetQueuedTranspObj(pContext)->Handle);

       /* If the queued request is from the same tester as the new request with the foreign N_TA */
       if (Dcm_TskIsLocalEventSet(lEv, tObjEv))
       {
         /* Cancel the queued request in the next task cycle */
         pEventContext->PostEv = tObjEv;/* retry later */                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
       }
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }

  /* Scan all affected transport objects */
  lTObjCntr = 0u;
  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj = Dcm_NetGetTransportObject(lTObjCntr);

      /* If a received but not yet processed request was interrupted by another request with a foreign N_TA */
      if( ( pTranspObj->State == DCM_NET_TOBJ_STATE_RX_END)
        &&((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_OBSOLETE) != 0u ) ) /* This is still the original transport object marked to be canceled in the Dcm_OnRequestIndication */
      {
        /* Prevent the processing of the received but not yet processed request */
        Dcm_TskClrEventByThread(DCM_TSK_ID_NET_RX, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle), DCM_THREAD_ID_BASIC);

        /* Deallocate the transport object and free the connection */
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl); /* Deallocate transport object */
        Dcm_NetBufferRelease(pTranspObj->BufferHdl);
        Dcm_NetUnRegisterActiveConnection(pTranspObj); /* No further processing needed */                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      } /* Otherwise, connection is not obsolete or will be canceled during service processing (already in state
         * DCM_NET_TOBJ_STATE_READY) */
    }
    ++lTObjCntr;
  }
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif


/**********************************************************************************************************************
 *  Dcm_NetTaskRx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetTaskRx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_TskTaskEvMemType          lBusyEvents;
  Dcm_CfgNetTObjHandleOptType   lTranspObjWinnerId;
  Dcm_NetTransportObjectPtrType pTranspObj;

  lTranspObjWinnerId = Dcm_NetRxPrioritisationFindWinner(pEventContext->Ev, &lBusyEvents);                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

  pTranspObj = Dcm_NetGetTransportObject(lTranspObjWinnerId);

  Dcm_NetRxPrioritisationProcessWinner(pContext, pTranspObj, &lBusyEvents);                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_NetRxPrioritisationProcessLoosers(lBusyEvents);
}

/**********************************************************************************************************************
 *  Dcm_NetTaskTx()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_NetTaskTx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_NET_TX_SEND_USDT))
  {
    {
      Dcm_CfgNetTObjHandleOptType lTObjIter;

      /* find candidate for NRC 0x21 or final response */
      for (lTObjIter = 0; lTObjIter < Dcm_PbCfgNetNumTransportObjects; ++lTObjIter)
      {
        Dcm_NetTransportObjectPtrType pTranspObj;
        pTranspObj = Dcm_NetGetTransportObject(lTObjIter);

#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
        if ((pTranspObj->State == DCM_NET_TOBJ_STATE_RX_END) /* Completely received ... */
          && ((DCM_NET_TOBJ_FLAG_BUSY & pTranspObj->Flags) != 0u))/* ... a parallel request */
        {
          /* Send NRC 0x21 response */
          Dcm_NetComposeAddNegResponse(pTranspObj, DCM_NET_TOBJ_RESTYPE_SIMPLE, DCM_E_BUSYREPEATREQUEST);                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
#endif

        if (pTranspObj->State == DCM_NET_TOBJ_STATE_PREPTX)
        {
          Dcm_NetTransmitUsdtResponse(pTranspObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
      }
    }
  }

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_NET_TX_SEND_UUDT))
  {
    Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;

    for (lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPeriodicMsgGetNextMsgHdl(); lPerTxObjIter++)
    {
      if (Dcm_NetGetPeriodicTxObject(lPerTxObjIter)->State == DCM_NET_PERIODIC_TX_STATE_QUEUED)
      {
        Dcm_NetPeriodicMsgTransmitTxObj(lPerTxObjIter);
      }
    }
  }
#endif
}
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutPeriodicTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutPeriodicTx(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType lReloadTick;
  boolean                 lDoReload;

  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lDoReload = Dcm_OnTimeoutTxObjectHandler();
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  if (!lDoReload)
  {
    lDoReload = Dcm_OnTimeoutDelayTimerHandler();
  }
# endif

  lReloadTick = (Dcm_TmrTimerCntrMemType)((lDoReload == TRUE)?1u:0u);

  return lReloadTick;
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetFastestRate()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetFastestRate(
  Dcm_Svc2ASchedRateOptType Rate
  )
{
  /* If the new rate differs from the previous rate */
  if (Dcm_NetPeriodicMsgGetFastestRate() != Rate)
  {
    Dcm_TmrTimerCntrMemType lRateDiff;
    uint8_least lDelayTmrIter;

    /* If changing to a faster rate */
    if (Rate > Dcm_NetPeriodicMsgGetFastestRate())
    {
      lRateDiff = (Dcm_TmrTimerCntrMemType) ( Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate())
                                            - Dcm_NetGetPeriodicDelayTime(Rate) );

      for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
      {
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_SplitTaskEnterCS(); /* Protect against interrupt from timer task */
        /* If delay timer is set */
        if (Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
        {
          if (Dcm_NetDelayTimerGet(lDelayTmrIter) <= lRateDiff)
          {
            Dcm_NetDelayTimerSet(lDelayTmrIter, 1); /* immediate timeout */
          }
          else
          {
            /* decrease timer value by rate difference */
            Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter) - lRateDiff));
          }
        }
        Dcm_SplitTaskLeaveCS();
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
      }
    }
    else /* Otherwise (changing to a slower rate) */
    {
      lRateDiff = (Dcm_TmrTimerCntrMemType) ( Dcm_NetGetPeriodicDelayTime(Rate)
                                            - Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate()) );

      for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
      {
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_SplitTaskEnterCS(); /* Protect against interrupt from timer task */
        /* If delay timer is set */
        if (Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
        {
          /* increase timer value by rate difference */
          Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter) + lRateDiff));
        }
        Dcm_SplitTaskLeaveCS();
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
      }
    }

    Dcm_SingletonContext.Network.PeriodicTxContext.FastestRate = (Dcm_Svc2ASchedRateMemType)Rate;
  }
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif
/**********************************************************************************************************************
 *  Dcm_Service10Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service10Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc10RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc10;

  switch(Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc10Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_CHECK_ACCESS:
    lStdResult = Dcm_Svc10CheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
  case DCM_SVC10_PROGRESS_TRIGGER_RESET:
    lStdResult = Dcm_Svc10_TriggerReset(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_WAIT_RESET_ACK:
    lStdResult = Dcm_Svc10_WaitForResetAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK:
    lStdResult = Dcm_Svc10_WaitForRcrRpAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_SET_PRGCOND:
    lStdResult = Dcm_Svc10_SetProgConditions(opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service10FastPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service10FastPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    Dcm_DiagSetP2Timings(pContext->Repeater.Context.Svc10.SesStateIdx
                        ,Dcm_NetGetProtIdOfActiveProtocol()
                        ,pContext->ThreadId);

#if (DCM_SVC_10_RESET_AFTER_RESPONSE == STD_ON)
    if(pContext->Repeater.Context.Svc10.ResetMode != RTE_MODE_DcmEcuReset_NONE)
    {
      Dcm_SingletonContext.Network.RxAllowed = FALSE; /* lock any further communication while the ECU is getting reset */
    }
#endif
  }
}

/**********************************************************************************************************************
 *  Dcm_Service10PostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service10PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    /* execute transition */
    Dcm_StateSetSession(pContext->Repeater.Context.Svc10.SesStateIdx);
#if (DCM_SVC_10_RESET_AFTER_RESPONSE == STD_ON)
    if(pContext->Repeater.Context.Svc10.ResetMode != RTE_MODE_DcmEcuReset_NONE)
    {
      /* fire and forget - if failed - no chance to send any NRC from here */
      (void)Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    }
#endif
  }
  else
  {
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
    Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext);
#endif

    /* on the way of changing a session */
    if ( (Dcm_StateGetSession() != Dcm_StateGetPendingSession())
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
       &&(Dcm_NetGetSessionConnection() == pTranspObj->ConnHdl) /* and the owner of the pending session ... */
#endif
       )
    {
      Dcm_Svc10UndoPendingSession();
    }/* else - nothing to undo */
  }
}
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrRead()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrRead(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdReturn;
  Dcm_OpStatusType lOpStatus = opStatus;

  /* The DDDID shall be a defined one! */
  Dcm_DebugAssert((Dcm_Svc2CIsDynDidDefined(dynDidHandle)), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                  /* COV_DCM_RTM_DEV_DEBUG XF */

  lStdReturn = Dcm_Svc2CDefMgrReadCheckAccessAndInit(dynDidHandle, &lOpStatus);                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

  if(lStdReturn == DCM_E_OK)
  {
    Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;

    pProcessContext = Dcm_Svc2CGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED);

    if(lOpStatus == DCM_INITIAL)
    {
      Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.DynDidHandleInUse = dynDidHandle;
      pProcessContext->ItemInProgress = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);                                                            /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
      pProcessContext->ItemToStop = (Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(pProcessContext->ItemInProgress + Dcm_Svc2CGetItem(dynDidHandle)->Count); /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
    }

    /* Process source items */
    lStdReturn = Dcm_Svc2CDefMgrReadSrcItems(lOpStatus, pDataContext, pProcessContext);                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
  }

  if(lStdReturn != DCM_E_PENDING)
  {
    /* Release resource */
    Dcm_SingletonContext.Diag.Services.Svc2C.DynDidAccessContext.DynDidHandleInUse = DCM_SVC2C_INVALID_DYNDID_HDL;
  }
  return lStdReturn;
}

# if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_COND_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrConditionCheckRead()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrConditionCheckRead(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK; /* if the DynDID does not contain any DID sources -> always OK */
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;
  Dcm_OpStatusType lOpStatus = opStatus;

  /* The DDDID shall be a defined one! */
  Dcm_DebugAssert((Dcm_Svc2CIsDynDidDefined(dynDidHandle)), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                  /* COV_DCM_RTM_DEV_DEBUG XF */

  pProcessContext = Dcm_Svc2CGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY);

  if(lOpStatus == DCM_INITIAL)
  {
    pProcessContext->ItemInProgress = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);                                                              /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
    pProcessContext->ItemToStop = (Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(pProcessContext->ItemInProgress
                                                                         + Dcm_Svc2CGetItem(dynDidHandle)->Count);                                   /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
  }

  for(; pProcessContext->ItemInProgress < pProcessContext->ItemToStop; ++pProcessContext->ItemInProgress)                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    if(Dcm_Svc2CIsDidSrcItem(pProcessContext->ItemInProgress))
    {
      Dcm_Svc2CInitDidInfoFromDidInfoIdx(Dcm_Svc2CGetSrcItem(pProcessContext->ItemInProgress)->DidDescriptor.DidInfoIdx
                                         ,&(pProcessContext->SrcDidContext));                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

      if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pProcessContext->SrcDidContext))                                                    /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                    ,DCM_DIDMGR_OPTYPE_READCHKCOND))
      {
        Dcm_DidMgrInitOpClassInfo(&(pProcessContext->SrcDidContext)                                                                                  /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                 ,DCM_DIDMGR_OPTYPE_READCHKCOND);

        lStdResult = Dcm_DidMgrReadCheckCond(lOpStatus
                                            ,&(pProcessContext->SrcDidContext)
                                            ,&(pProcessContext->DidOpTypeContext)
                                            ,errorCode);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
        if(lStdResult == DCM_E_OK)
        {
          /* prepare for next item */
          lOpStatus = DCM_INITIAL;
        }
        else
        {
          break; /* just delegate the return value */                                                                                                /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
        }
      } /* else - no check condition supported by this signal */
    } /* else - memory ranges cannot be checked in advance! */
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadDataLength(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_DidMgrDidLengthPtrType dataLength
  )
{
  *dataLength = Dcm_Svc2CGetItem(dynDidHandle)->Length;                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  return DCM_E_OK;
}

# if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CStateCheckSrcItems()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CStateCheckSrcItems(
  Dcm_NetConnRefMemType connHdl,
  Dcm_CfgDidMgrDynDidHandleOptType dynDidHandle,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType currItem;
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType endItem;

  DCM_IGNORE_UNREF_PARAM(connHdl);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  currItem = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);
  endItem = (Dcm_CfgDidMgrDynDidSrcItemIdxOptType)(currItem + Dcm_Svc2CGetItem(dynDidHandle)->Count);

  Dcm_DebugAssert((currItem < endItem), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                                      /* COV_DCM_RTM_DEV_DEBUG XF */

  for(; currItem < endItem; currItem++)
  {
    if(Dcm_Svc2CIsDidSrcItem(currItem))
    {
#  if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
      Dcm_DidMgrDidInfoContextType lDidInfoContext;

      Dcm_Svc2CInitDidInfoFromDidInfoIdx(Dcm_Svc2CGetSrcItem(currItem)->DidDescriptor.DidInfoIdx, &lDidInfoContext);                                 /* SBSW_DCM_POINTER_FORWARD_STACK */

      lStdResult = Dcm_StateCheckDID(connHdl
                                    ,lDidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&lDidInfoContext)->ExecCondRef                                                        /* PRQA S 2962 */ /* MD_Dcm_2962 */
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
#  endif
    }
    else
    {
#  if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
      lStdResult = Dcm_MemMgrCheckMemBlock(&(Dcm_Svc2CGetSrcItem(currItem)->MemDescriptor.MemBlock)
                                          ,DCM_MEMMGR_OP_READ
                                          ,ErrorCode);                                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
#  endif
    }
    if (lStdResult != DCM_E_OK)
    {
      break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CIsDidDefined()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_Svc2CIsDidDefined(
  Dcm_CfgDidMgrDidInfoConstPtrType pDidInfo
  )
{
  return ((!Dcm_DidMgrIsOpSupported(pDidInfo, DCM_DIDMGR_OP_DEFINE))
          || Dcm_Svc2CIsDynDidDefined((Dcm_CfgDidMgrDynDidHandleMemType)(pDidInfo->OpRef)));                                                         /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
}
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service2CInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service2CInit(
  void
  )
{
  Dcm_Svc2CDefMgrInit();
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service2C_01Processor()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_01Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2C;

  lStdReturn = Dcm_Svc2CParseAndCheckDynDid(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    /* there shall be an iteration (+[4Byte]) */
    if( (pMsgContext->reqDataLen > 0u)
      &&( (pMsgContext->reqDataLen % 4u) == 0u) )
    {
      Dcm_MsgLenType lNumReqItems = pMsgContext->reqDataLen / 4u; /* calculate number of source items to be processed */

      /* check DynDID capacity */
      lStdReturn = Dcm_Svc2CCheckAndSetNumOfItems(lNumReqItems, ErrorCode, pRepContext);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      if(lStdReturn == DCM_E_OK)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP);                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP; /* speed up processing */
      } /* else - Nrc and return value already set */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
  } /* else - just return the value: DCM_E_PENDING, DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service2C_02Processor()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_02Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;
  uint8          lAlfid;
  uint8          lBlockLength;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2C;

  lStdReturn = Dcm_Svc2CParseAndCheckDynDid(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    /* Check, whether the ALFID is available */
    if (pMsgContext->reqDataLen != 0u)
    {
      /* the ALFID availability is checked within next call */
      lStdReturn = Dcm_MemMgrValidateAndGetAlfid(pMsgContext, &lAlfid, &lBlockLength, ErrorCode);                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      if (lStdReturn == DCM_E_OK)
      {
        /* pMsgContext->reqDataLen cannot be zero! */
        if((pMsgContext->reqDataLen % lBlockLength) == 0u) /* the remained request length shall be a multiple of the memory address and size request block */
        {
          Dcm_MsgLenType lNumReqItems = pMsgContext->reqDataLen / lBlockLength; /* calculate number of source items to be processed */

          /* check DynDID capacity */
          lStdReturn = Dcm_Svc2CCheckAndSetNumOfItems(lNumReqItems, ErrorCode, pRepContext);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
          if (lStdReturn == DCM_E_OK)
          {
            lStdReturn = Dcm_Svc2C02SrcItemsGetLength(lAlfid, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
          } /* else - ErrorCode and return value already set */
        }
        else
        {
          /* else - ErrorCode set to 0x13 */
          *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdReturn = DCM_E_NOT_OK;
        }
      } /* else - DCM_E_NOT_OK (ErrorCode already set) */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
  } /* else - DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Service2C_03Processor()
 *********************************************************************************************************************/
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2C;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->DynDidItemCount = 0u; /* prepare for a successful clear */                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

  if(pMsgContext->reqDataLen == 0u)
  {
    /* clear all DynDID definitions */
    Dcm_CfgDidMgrDynDidHandleOptType dynDidIter;
    for(dynDidIter = 0; dynDidIter < DCM_NUM_DYNDIDS; ++dynDidIter)
    {
      Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)dynDidIter);
    }
    lStdReturn = DCM_E_OK;
  }
  else if (pMsgContext->reqDataLen == 2u)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_03_DIDLOOKUP); /* next job */                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP; /* speed up processing */
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  return lStdReturn;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service2CProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2CProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2C;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc2CHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc2CSubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP:
    lStdResult = Dcm_Svc2C01SrcDidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS:
    lStdResult = Dcm_Svc2C01SrcDidCheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH:
    lStdResult = Dcm_Svc2C01SrcDidGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
  case DCM_SVC2C_PROGRESS_03_DIDLOOKUP:
    lStdResult = Dcm_Svc2C03DidLookUp(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service2CPostProcessor()
 *********************************************************************************************************************/
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
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2CPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    Dcm_Svc2CRepeaterProxyContextConstPtrType pRepContext = &pContext->Repeater.Context.Svc2C;

    if(pRepContext->DynDidItemCount != 0u) /* this is a definition service , not the clear one */
    {
      Dcm_Svc2CDynDidItemPtrType pItem;

# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
      uint16 lDid = Dcm_DidMgrGetDynDidIdFromHandle(pRepContext->DynDidHandle);
      /* Kill any still not finished read periodic DID jobs to avoid inconsistent response data/length ! */
      Dcm_Svc2ASchdCancelReadByDid(lDid);
# endif
      /* Commit DynDID definition immediately */
      pItem = Dcm_Svc2CGetItem(pRepContext->DynDidHandle);
      pItem->Length = pRepContext->DynDidLength;                                                                                                     /* SBSW_DCM_POINTER_WRITE_2CITEM */
      pItem->Count  = pRepContext->DynDidItemCount;                                                                                                  /* SBSW_DCM_POINTER_WRITE_2CITEM */
    } /* else - the dynDid has been already cleared */

# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_UtiNvMSetRamBlockStatus((uint16)DCM_SVC_2C_NVRAM_BLOCKID);
# endif
  } /* else - the dynDid list properties will not be changed - all prepared data will be discarded */
}
#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service86Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service86Init(
  void
  )
{
  Dcm_ExtService86Init();/* delegate to the extension */
}
/**********************************************************************************************************************
 *  Dcm_Service86Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc86RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc86;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc86Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc86SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferDataPadding()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_PagedBufferDataPadding(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check for remaining parts to be padded */
  Dcm_UtiMemSetUintX(Dcm_UtiGetDataContextBuffer(pDataContext), Dcm_MsgItemType, 0x00u, pDataContext->AvailLen);                                     /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
  /* Signalize that all requested data are provided */
  Dcm_UtiCommitData(pDataContext, pDataContext->AvailLen);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* Enforce transmission of the current part */
  return DCM_E_BUFFERTOOLOW;
}
#endif
/**********************************************************************************************************************
 *  Dcm_OnTimeoutP2()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutP2(
  Dcm_ThreadIdMemType threadId                                                                                                                       /* COV_DCM_RTM_NO_RCR_RP_TX */
  )
{
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_TMR_P2_TO, threadId);
  return 0u;
}

/**********************************************************************************************************************
 *  Dcm_OnTimeoutS3()
 *********************************************************************************************************************/
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
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutS3(
  Dcm_ThreadIdMemType threadId
  )
{
  if(!Dcm_StateIsDefaultSessionActive())
  {
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, threadId);
    Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfActiveProtocol(), threadId);

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
    {
      Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(Dcm_NetGetSessionConnection());

      if (lAuthInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
      {
        if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED)
        {
          Dcm_AuthMgrTimerExpireMaskSetBit(lAuthInfoRef);
          Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_IDLE_CONN, threadId);
        }
      }
    }
#endif
  }
  return 0u;
}

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutFblRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutFblRes(
  Dcm_ThreadIdMemType threadId
  )
{
  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_CANCEL);
  return 0u;
}
#endif

#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutKeepAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutKeepAlive(
  Dcm_ThreadIdMemType threadId
  )
{
  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_StateIsDefaultSessionActive())                                                                                                              /* COV_DCM_RTM_DEPENDS_ON_CFG */
  {
    Dcm_NetUnRegisterAllComMActivity();
  } /* else - ComM activity will be unregistered by S3 timeout */

  Dcm_SingletonContext.Network.KeepAlive = FALSE;

  return 0u; /* No reload */
}
#endif
#define DCM_STOP_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  COMPONENT DETAILED DESIGN OF GENERATED FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Dcm_Cfg<Unit>Get<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_ModeRule<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ModeCondGet_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ModeOnComControlModeChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_DidMgr<XXX>ReadData()
 *********************************************************************************************************************/
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

/**********************************************************************************************************************
 *  Dcm_DidMgr<DynDID>Read<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<XXX>WriteData()
 *********************************************************************************************************************/
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

/**********************************************************************************************************************
 *  Dcm_DidMgr<OBD_AID>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<MID>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_ControlState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_DeadEnd_Write_IOControlRequest_ControlState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_IOOperationRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Read_IOControlRequest_UnderControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_UnderControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<DID>_IoControlOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<DID>_ReturnControlToECU()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<SupportedID>_ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<SupportedID>_ReadDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<NvMDID>Read()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<NvMDID>Write()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<Internal>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_RidMgr<XXX>()
 *********************************************************************************************************************/
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

/**********************************************************************************************************************
 *  Dcm_RidMgr_<XXX>_OBD_AID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_RidMgr_<XXX>_OBD_TID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_SvcWrapper_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_SubSvcWrapper_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoUpdater()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_Svc27_Dummy_GetSecurityAttemptCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_Svc27_Dummy_SetSecurityAttemptCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* Following markers are only used in generated or extension files, thus "unused marker" warnings can be ignored.*/
                                                                                                                                                     /* PRQA S 0310 */ /* MD_Dcm_DifferentPointerType_0310 */
                                                                                                                                                     /* PRQA S 2982 */ /* MD_Dcm_Redundant_2982 */
                                                                                                                                                     /* PRQA S 1330 */ /* MD_Dcm_RteSpecific_1330 */
                                                                                                                                                     /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
                                                                                                                                                     /* PRQA S 0612 */ /* MD_Dcm_0612 */

                                                                                                                                                     /* module specific MISRA deviations:

   MD_Dcm_DifferentPointerType_0310:
     Description: Rule 11.3
                  A cast shall not be performed between a pointer to object type and a pointer to a different object type.
     Reason:      To optimize access without the need of local variable, pointers to data types of the same size (uint8, sint8)
                  will be casted to the corresponding target pointer type.
     Risk:        Compile error/warnings.
     Prevention:  Compiler compatibility is tested during integration.

   MD_Dcm_0313:
     Description: Rule 11.1
                  Conversions shall not be performed between a pointer to a function and any other type.
     Reason:      No efficient dynamic-cast operation available in ANSI-C.
     Risk:        Data corruption due to miscast.
     Prevention:  Covered by descriptive pointer types and code review for correct usage.

   MD_Dcm_Rule19.2:
     Description: Rule 19.2
                  The union keyword should not be used.
     Reason:      Data without shared lifetime is stored in union for efficiency.
     Risk:        None due to no shared lifetime.
     Prevention:  None.

   MD_Dcm_Optimize_0771:
     Description: Rule 15.4
                  There should be no more than one break or goto statement used to terminate any iteration statement.
     Reason:      The loop needs multiple exit points since error conditions cannot be verified prior to the loop.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Rule1.2_1039:
    Description: Rule 1.2
                 Treating array of length one as potentially flexible member.
    Reason:      In a special configurations, a struct type has only one memeber of an array type with a size of one.
    Risk:        Some compilers would consider the array as a flexible memeber.
    Prevention:  Testing the code under several compilers.

   MD_Dcm_RteSpecific_1330:
     Description: Rule 8.3
                  All declarations of an object or function shall use the same names and type qualifiers.
     Reason:      The function declaration generated by the RTE are RTE-implementation specific and unknown at
                  the DCM development time.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_ObjectOnlyAccessedOnce:
    Description: Rule 8.9
                 An object should be defined at block scope if its identifier only appears in a single function.
    Reason:      Autosar rules or OEM requirements mandate this implementation.
    Risk:        None.
    Prevention : None.

   MD_Dcm_Optimize_2003:
     Description: Rule 16.3
                  An unconditional break statement shall terminate every switch-clause Enforcement.
     Reason:      To use optimal design small switch-case dispatchers use fall-through cases.
     Risk:        None, since documented as an explicit fall-through case.
     Prevention:  None.

   MD_Dcm_Redundant_2982:
    Description: Rule 2.2
                 This assignment is redundant. The value of this object is never used before being modified.
    Reason:      Initializing global variables.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2983:
    Description: Rule 2.2
                 This assignment is redundant. The value of this object is never subsequently used.
    Reason:      In the last iteration of an iterative structure, a statement might be executed but not used.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2985:
    Description: Rule 2.2
                 This operation is redundant. The value of the result is always that of the left-hand operand.
    Reason:      Apparent redundant-like operations are needed for better code readability.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2986:
    Description: Rule 2.2
                 This operation is redundant. The value of the result is always that of the right-hand operand.
    Reason:      Apparent redundant-like operations are needed for better code readability.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Optimize_2889:
     Description: Rule 15.5
                  A function should have a single point of exit at the end.
     Reason:      For code run-time/ROM usage reason this function terminates immediately.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_ConstExpr:
    Description: Rule 2.2
                 Controlling expressions shall not be invariant.
    Reason:      A configuration dependent situation could lead to a constant logical expression.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_CodingRule_3218:
     Description: Rule 8.9
                  An object should be defined at block scope if its identifier only appears in a single function.
     Reason:      Vector style guide prevents usage of static variables/constant objects in function scope.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Rule13.5:
     Description: Rule 13.5
                  The right hand operand of a logical && or || operator shall not contain persistent side effects.
     Reason:      The function called on the right hand side has no side effects.
     Risk:        The function may introduce side effects at a later time.
     Prevention:  None.

   MD_Dcm_UnknownSize_3684:
     Description: Rule 8.11
                  When an array with external linkage is declared, its size should be explicitly specified.
     Reason:      Array declared with unknown size to reduce number of variants and therefore complexity.
                  The user of that array does not need the size of the array.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_APIStd_3673:
     Description: Rule 8.13
                  A pointer should point to a const-qualified type whenever possible.
     Reason:      The API is defined by the AUTOSAR standardization.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Design_3673:
     Description: Rule 8.13
                  A pointer should point to a const-qualified type whenever possible.
     Reason:      The API has to be compatible to a common prototype, defined by module's design to serve generic purposes.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_BitNegation_4399:
     Description: Rule 10.8
                  An expression which is the result of a ~ or << operation has been cast to a wider type.
     Reason:      Using the ~ operator avoids errors in manually typed bit masks.
     Risk:        May generate compiler warnings although explicit casts are used.
     Prevention:  None.

   MD_Dcm_2987:
     Description: Rule 2.2
                  This function call produces no side effects and is redundant.
     Reason:      The functions produces side effects, but only in specific configurations.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_RetVal:
     Description: Rule 2.2
                  This initialization is redundant. The value of this object is never used before being modified.
     Reason:      Without this initialization some not so smart compilers will erroneously complain about use of uninitialized variable.
     Risk:        The redundant initialization code might be confusing.
     Prevention:  None.

  MD_Dcm_0612:
     Description: Rule 1.1
                  Size of object exceeds 32767 bytes - program does not conform strictly to ISO:C90.
     Reason:      A DCM buffer was configured with a size greater than 32767 bytes.
     Risk:        Decreased code portability.
     Prevention:  None.

  MD_Dcm_DerefInvalidPointer:
     Description: Rule 18.1
                  Dereference of an invalid pointer value.
     Reason:      This is a false positive and a known PRQA issue.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_ComputingInvalidPointer:
     Description: Rule 18.1
                  Computing an invalid pointer value.
     Reason:      This is a false positive and a known PRQA issue.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_8.5_Rte:
     Description: Rule 8.5
                  Multiple declarations of external object or function.
     Reason:      MICROSAR Rte generates prototypes for callbacks. Some callbacks are also contained in a replacement
                  header in case no Rte is used. This leads to duplicate definitions.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_OptimizedInlineCode_3387:
     Description: Rule 13.3
                  A full expression containing an increment (++) or decrement (--) operator should have no other
                  potential side effects other than that caused by the increment or decrement operator
     Reason:      This kind of operator is used only in case of function-like macros where also a return value is expected.
                  Needed for maximum inlining efficiency and less other MISRA deviations.
     Risk:        The risks are at minimum since the affected locations are simple expressions.
     Prevention:  These code sections are thoroughly tested by dedicated unit tests.

  MD_Dcm_Design_3679:
     Description: Rule 8.13
                  The object referenced is not modified through it, so the object could be declared with type const*.
     Reason:      The API has to be compatible to a common prototype, defined by module's design to serve generic purposes.
     Risk:        None.
     Prevention:  None.
     
  MD_Dcm_2962:
     Description: Rule 9.1
                  The value of an object with automatic storage duration shall not be read before it has been set.
     Reason:      The variable is initialized in a function call before the variable is used. It is ensured that the variable
                  is initialized in the function call, because a check whether the DID exists is already done earlier. 
     Risk:        None.
     Prevention:  None.

  MD_Dcm_2822:
     Description: Rule 21.1
                  Arithmetic operation on NULL pointer.
     Reason:      This is a false positive.
     Risk:        None.
     Prevention:  None.
     
*/
/**********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 *********************************************************************************************************************/
/* COV_JUSTIFICATION_BEGIN

\ID COV_DCM_MACRO_OVERRIDE
  \ACCEPT TX
  \ACCEPT XF
  \REASON The value of a macro may be overridden by user config for test purposes. Since this is not officially
          supported, it is not covered by the test suite.

\ID COV_DCM_UNSUPPORTED
  \REASON [COV_MSR_UNSUPPORTED]

\ID COV_DCM_SUPPORT_PARTIALLY
  \REASON [COV_MSR_UNSUPPORTED]

\ID COV_DCM_SUPPORT_ALWAYS
  \REASON This configuration switch is always on, but the code is enclosed in a pre-processor precondition for consistency reasons and safe future changes.

\ID COV_DCM_SUPPORT_RESTRICTED
  \REASON This configuration switch is always on/off, due to feature range limitation.

\ID COV_DCM_SUPPORT_XOR
  \REASON Disjunction of configuration switches that are mutually exclusive and one of them is always true.

\ID COV_DCM_SUPPORT_WITH_MSR_DEM
  \REASON This configuration switch is always on as long as MSR4 DEM is used (required for SafeBSW context).

\ID COV_DCM_REQUIRED_BY_SAFEBSW
  \REASON This configuration switch is always on as required by SafeBSW context.

\ID COV_DCM_DEPENDS_ON_UNSUPPORTED
  \REASON This configuration switch is always on/off due to a justified expression it is involved in.

\ID COV_DCM_DEV_DEBUG
  \REASON The justified object is uncovered, since it is only used during component development to ease the detection of errors!

\ID COV_DCM_UNSUPPORTED_ERROR_DETECTION
  \REASON The justified object is uncovered, since it is only used to ease the detection of configuration errors!

\ID COV_DCM_NOUNIT
  \REASON The justified object is always undefined, since it is only used during component unit testing.

\ID COV_DCM_RTM_NO_RCR_RP_TX
  \ACCEPT XX
  \REASON The justified object is unreachable, since the configuration does not support any feature that could use it.
          A complete encapsulation of all use-cases where no RCR-RP will be sent would be too complex and risky.

\ID COV_DCM_RTM_DEV_DEBUG
  \REASON [COV_DCM_DEV_DEBUG]

\ID COV_DCM_RTM_UNREACHABLE
  \REASON The justified object is uncovered, since it is not intended to be reached! It just exists because of other reasons like MISRA (default case in a switch)
          or because of added debug assert monitoring for development purposes (see COV_DCM_RTM_DEV_DEBUG).

\ID COV_DCM_RTM_UNREACHABLE_COMPLEXITY
  \REASON The justified object is uncovered, since it can be reached only in very rare situations (i.e. interrupt, multiple events in a specific sequence, etc.)!
          But at the same time it is part of the (sub-)component design and cannot be removed.

\ID COV_DCM_RTM_DESIGN_LOWER_LAYER
  \REASON The justified object is uncovered, since it is a basic (lower layer) functionality and is designed to behave in different situations.
          These situations are not necessarily supported by all configurations. Due to the high complexity of dependent functionalities
          the concrete usage of the uncovered functionality is left to the upper layers in DCM!

\ID COV_DCM_RTM_RUNTIME_CHECK
  \REASON This condition depends on a runtime check for an uninitialized pointer or remaining buffer size check.

\ID COV_DCM_RTM_DEPENDS_ON_UNSUPPORTED
  \REASON The justified object is uncovered, since it can vary only if a specific (non supported in SafeBSW context) configuration is activated.

\ID COV_DCM_RTM_DEPENDS_ON_CFG
  \ACCEPT TX
  \ACCEPT XF
  \REASON The justified expression evaluates always depending on the configuration either to true or false.

\ID COV_DCM_RTM_UNIT_TEST
  \REASON This condition is covered on unit test level.

COV_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 *********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_DCM_PARAM_PTR_WRITE
  \DESCRIPTION    In a function, data is written through a pointer passed as function argument.
  \COUNTERMEASURE \N [CM_DCM_N_PASS_PARAM_PTR]

\ID SBSW_DCM_LOCAL_ARRAY_WRITE
  \DESCRIPTION    In a function, data written to an array.
  \COUNTERMEASURE \N [CM_DCM_N_ARRAY_INDEX_WRITE]

\ID SBSW_DCM_GEN_PARAM_PTR_WRITE
  \DESCRIPTION    In a generated function, called only by DCM static code, data is written through a
                  pointer passed as function argument.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_PARAM_PTR_WRITE]

\ID SBSW_DCM_PARAM_PTR_FORWARD
  \DESCRIPTION    A pointer received in a function is passed to a function expecting a valid pointer.
  \COUNTERMEASURE \N [CM_DCM_N_PASS_PARAM_PTR]

\ID SBSW_DCM_GEN_PARAM_PTR_FORWARD
  \DESCRIPTION    A pointer received in a generated function, called only by DCM static code, is passed to
                  a function expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_PARAM_PTR_FORWARD]

\ID SBSW_DCM_PARAM_FUNCPTR_CALL
  \DESCRIPTION    In a function, a function pointer passed as function argument is called.
  \COUNTERMEASURE \N [CM_DCM_N_CALL_PARAM_FUNCPTR]

\ID SBSW_DCM_GLOBAL_PTR_WRITE
  \DESCRIPTION    In a function, data is written through a pointer taken from a global variable.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_FORWARD_STACK
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer. The called function does not store its
                  pointer parameter(s).
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_GEN_POINTER_FORWARD_STACK
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer (i.e. to return data through it).
                  The called function does not store its pointer parameter(s) for deferred writing.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_POINTER_FORWARD_STACK]

\ID SBSW_DCM_POINTER_FORWARD_GLOBAL
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer. The called function does not store its
                  pointer parameters.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_COMB_PTR_FORWARD
  \DESCRIPTION    A combination of pointers is passed to a function expecting valid pointers. The called function does
                  not store its pointer parameters.The pointer is taken from one of the following locations:
                     - global variable;
                     - local (stack) variable;
                     - is an argument of the caller function (refer also to CM_DCM_N_PASS_PARAM_PTR);
                     - is a pointer initialized with one of the following associated initialization function that
                       returns always a valid Pointer: Dcm_UtiGetDataContextBuffer(),
                                                       Dcm_UtiGetResData(),
                                                       Dcm_UtiGetResDataAt(),
                                                       Dcm_UtiInitDataContext(),
                                                       Dcm_DiagInitMsgContextBufferInfo(),
                                                       Dcm_DiagInitiateServiceProcessing(),
                                                       Dcm_NetGetBufferInfos(),
                                                       Dcm_NetGetTransportObject(),
                                                       Dcm_NetGetPeriodicTxObject(),
                                                       Dcm_Svc22GetDidInfoContext(),
                                                       Dcm_Svc2ASchdGetEntry(),
                                                       Dcm_Svc2CGetSrcItem(),
                                                       Dcm_GetThreadContext(),
                                                       Dcm_Svc31RedirectMsgContext(),
                                                       Dcm_Svc86_03GetDidRecord(),
                                                       Dcm_Svc86_07GetDidRecord(),
                                                       Dcm_DidMgrSecureCodingLockBuffer()
                     as such it is always valid.
\COUNTERMEASURE \M [CM_DCM_M_SIZEOF_BUFFERINFO]
                \R [CM_DCM_R_GET_BUFFERINFO]
                \R [CM_DCM_R_SET_DATA_CONTEXT_USAGE]

\ID SBSW_DCM_GEN_COMB_PARAM_PTR_FORWARD
  \DESCRIPTION    A combination of pointers is passed to a function expecting valid pointers. The called function does
                  not store its pointer parameters.The pointer is taken from one of the following locations:
                     - local (stack) variable;
                     - is an argument of the caller function (refer also to CM_DCM_S_GEN_PARAM_PTR_FORWARD);
                     - is a pointer initialized with one of the following associated initialization function that
                       returns always a valid Pointer: Dcm_UtiGetResDataAt()
                     as such it is always valid.
\COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER]
                \R [CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER]
                \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]
                \S [CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH]

\ID SBSW_DCM_GEN_RID_WRAPPER
  \DESCRIPTION    In a generated function, a pointer to a uint8/sint8 array routine signal is passed to a function
                  expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]
                  \S [CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH]

\ID SBSW_DCM_GEN_DID_WRAPPER
  \DESCRIPTION    In a generated function, a pointer to a uint8 array DID signal is passed to a function
                  expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_DID_WRAPPER_WRITE_DATA_LENGTH]
                  \S [CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_READ]
                  \S [CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_WRITE]

\ID SBSW_DCM_ROE_NV_ACCESS
  \DESCRIPTION    In a function, a pointer to a NvM data structure is passed to a function expecting valid pointers.
  \COUNTERMEASURE \S [CM_DCM_S_ROE_NV_BLOCK_ID_ACCESS]

\ID SBSW_DCM_SVC29_NV_ACCESS
  \DESCRIPTION    In a function, a pointer to a NvM data structure is passed to a function expecting valid pointers.
  \COUNTERMEASURE \S [CM_DCM_S_SVC29_NV_BLOCK_ID_ACCESS]

\ID SBSW_DCM_POINTER_WRITE_BUFFERINFO
  \DESCRIPTION    In an internal function, data is written through a pointer to a buffer info element, or the
                  pointer is passed to a function expecting a valid reference to a buffer info element.
                  The memory entry pointer is initialized by Dcm_NetGetBufferInfos() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_CONSISTENCY_BUFFERINFO]
                  \M [CM_DCM_M_SIZEOF_BUFFERINFO]
                  \R [CM_DCM_R_GET_BUFFERINFO]

\ID SBSW_DCM_POINTER_WRITE_COMMCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a ComM context element, or the
                  pointer is passed to a function expecting a valid reference to a ComM context element.
                  The memory entry pointer is initialized by Dcm_NetGetComMContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_COMMCONTEXT]
                  \R [CM_DCM_R_GET_COMMCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a buffer context element, or the
                  pointer is passed to a function expecting a valid reference to a buffer context element.
                  The memory entry pointer is initialized by Dcm_NetGetBufferContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_BUFFERCONTEXT]
                  \R [CM_DCM_R_GET_BUFFERCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetGetTransportObject() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]

\ID SBSW_DCM_POINTER_WRITE_TIMERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a timer context, or the
                  pointer is passed to a function expecting a valid reference to a timer context.
                  The memory entry pointer is initialized by Dcm_TmrGetTimerContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_TIMERCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_THREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a thread context, or the
                  pointer is passed to a function expecting a valid reference to a thread context.
                  The memory entry pointer is initialized by Dcm_GetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_THREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_DEBUGTHREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a debug thread context, or the
                  pointer is passed to a function expecting a valid reference to a debug thread context.
                  The memory entry pointer is initialized by Dcm_DebugGetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_DEBUGTHREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_SVC31THREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a Svc31 thread context, or the
                  pointer is passed to a function expecting a valid reference to a Svc31 thread context.
                  The memory entry pointer is initialized by Dcm_SVC31GetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_SVC31THREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_DiagGetQueuedTranspObj() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]

\ID SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetGetTranspObjOfConnection().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetAllocateOrGetTranspObject().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetLockConnection().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_PERTXOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a periodic transport ojbect, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetGetPeriodicTxObject() which will always
                  return a pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_PERTXOBJ]
                  \R [CM_DCM_R_SET_PERTXOBJ]

\ID SBSW_DCM_POINTER_WRITE_TASKCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a task context object.
                  The memory entry pointer is initialized by Dcm_TskGetTaskContext().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_SCHEDULERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a scheduler context object.
                  The memory entry pointer is initialized by Dcm_TskGetSchedulerContext().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_BITSET
  \DESCRIPTION    In an internal function, data is written to an array representing a bitset. The index within the
                  array is calculated by Dcm_UtiBitSetGetRowIdxSafe() which will always return a valid index within
                  the given bitset.
  \COUNTERMEASURE \R [CM_DCM_R_GET_BITSETROWINDEX]
                  \R [CM_DCM_R_BITSET_BASETYPE]

\ID SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2A scheduler table entry, or
                  the pointer is passed to a function expecting a valid reference to a service 0x2A scheduler table
                  entry. The memory entry pointer is initialized by Dcm_Svc2ASchdGetEntry() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2ASCHEDULERENTRY]

\ID SBSW_DCM_POINTER_WRITE_2CSRCITEM
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2C source item, or the
                  pointer is passed to a function expecting a valid reference to a service 0x2C source item. The memory
                  entry pointer is initialized by Dcm_Svc2CGetSrcItem() which will always return a pointer to a valid
                  memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CSRCITEM]

\ID SBSW_DCM_POINTER_WRITE_2CITEM
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2C item, or the pointer is
                  passed to a function expecting a valid reference to a service 0x2C item. The memory entry pointer is
                  initialized by Dcm_Svc2CGetItem() which will always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CITEM]

\ID SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a process context of a DID, or the
                  pointer is passed to a function expecting a valid reference to a process context. The memory entry
                  pointer is initialized by Dcm_Svc2CGetDDDidProcessContext() which will always return a pointer to a
                  valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CPROCESSCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_2FACTIVEIODIDS
  \DESCRIPTION    In an internal function, data is written through a pointer to a bitmask of active IODIDs, or the
                  pointer is passed to a function expecting a valid reference to a bitmask of active IODIDs. The memory
                  entry pointer is initialized by Dcm_Svc2FGetActiveIoDids() which will always return a pointer to a
                  valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2FACTIVEIODIDS]

\ID SBSW_DCM_POINTER_READ_86DTCRECORD
  \DESCRIPTION    In a function, a pointer to array Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRT[] and its size
                  descriptor Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRTLength is passed to a function expecting
                  a valid pointer to an array and a size descriptor which does not exceed the size of the passed array.
  \COUNTERMEASURE \N The pointer is only used for reading and taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_86DIDRECORD
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x86 DID record, or the
                  pointer is passed to a function expecting a valid reference to a service 0x86 DID record. The memory
                  entry pointer is initialized by Dcm_Svc86_03GetDidRecord() or Dcm_Svc86_07GetDidRecord() which will 
                  always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_86DIDRECORD]

\ID SBSW_DCM_POINTER_WRITE_VSGACTIVECFGVARIANTS
  \DESCRIPTION    In an internal function, data is written to the array Dcm_VarMgrSingletonContext.ActiveCfgVariants[]
                  using an iterator.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VSGACTIVECFGVARIANTS]

\ID SBSW_DCM_POINTER_WRITE_PERIODICTXBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to the tx buffer of a periodic message, or
                  the pointer is passed to a function expecting a valid reference to the tx buffer of a periodic
                  message. The memory entry pointer is initialized by Dcm_NetPeriodicMsgGetTxBuffer() which will always
                  return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_PERIODICTXBUFFER]

\ID SBSW_DCM_POINTER_WRITE_RXPDUINFO
  \DESCRIPTION    In an internal function, data is written through a pointer to a Rx-PduInfo, or the
                  pointer is passed to a function expecting a valid reference to a Rx-PduInfo element.
                  The memory entry pointer is initialized by Dcm_NetProvideRxBuffer() which will always return a
                  pointer to a valid memory entry if the return values is BUFREQ_OK.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RXPDUINFO]

\ID SBSW_DCM_POINTER_WRITE_DIDINFOCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a Did info context, or the
                  pointer is passed to a function expecting a valid reference to a Did info context.
                  The memory entry pointer is initialized by Dcm_Svc22GetDidInfoContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_DIDINFOCONTEXT]

\ID SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT
  \DESCRIPTION    The global message context variable is passed to Dcm_DiagInitMsgContextBufferInfo() for
                  initialization. That initalization function ensures that all pointer members are pointing to valid
                  memory entry pointer and the corresponding buffer size members are matching to the pointed memory
                  entry sizes.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_REQBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context request buffer,
                  or the pointer is passed to a function expecting a valid reference to a message context
                  request buffer. The caller ensures that the reqData member points to a valid memory entry pointer
                  and the reqBufSize member matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER]

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_RESBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context response buffer,
                  or the pointer is passed to a function expecting a valid reference to a message context
                  response buffer. The caller ensures that the resData member points to a valid memory entry pointer
                  and the resBufSize member matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER]

\ID SBSW_DCM_POINTER_WRITE_IO_CTRL_CEMR_SESSION_MASK
  \DESCRIPTION    In an internal function, data is written through a pointer to an IO control CEMR session mask, or the
                  pointer is passed to a function expecting a valid reference to an IO control CEMR session mask.
  \COUNTERMEASURE \R [CM_DCM_R_WRITE_IO_CTRL_CEMR_SESSION_MASK]

\ID SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCHANNELSTATE
  \DESCRIPTION    In an internal function, data is written through a pointer to a ComM channel state of the recovery
                  info, or the pointer is passed to a function expecting a valid reference to a ComM channel state of
                  the recovery info. The memory entry pointer is initialized by
                  Dcm_DiagGetRecoveryInfoComMChannelState() which will always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RECOVERYINFOCOMMCHANNELSTATE]

\ID SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCONTROLSTATE
  \DESCRIPTION    In an internal function, data is written through a pointer to the communication state of a ComM
                  channel of the recovery info, or the pointer is passed to a function expecting a valid reference to
                  the communication state of a ComM channel of the recovery info. The memory entry pointer is
                  initialized by Dcm_DiagGetRecoveryInfoCommControlState() which will always return a pointer to a
                  valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RECOVERYINFOCOMMCONTROLSTATE]

\ID SBSW_DCM_POINTER_INIT_DATA_CONTEXT
  \DESCRIPTION    The data context pointer is locally allocated and passed to Dcm_UtiInitDataContext() or 
                  Dcm_DidMgrSecureCodingLockBuffer() for initialization.
                  The caller ensures that the passed buffer pointer points to a valid memory entry
                  pointer and the passed buffer size matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_DATA_CONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a data context, or the
                  pointer is passed to a function expecting a valid reference to a data context.
                  The pointer is initialized by Dcm_UtiInitDataContext(), which will always initialize it to point
                  to a valid memory entry. A data context ensures that the sum of Usage and AvailLen member is always
                  equal to or smaller than the referenced buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DATA_CONTEXT_BUFFER]
                  \R [CM_DCM_R_SET_DATA_CONTEXT_USAGE]

\ID SBSW_DCM_POINTER_MEMCPY_BUFFER
  \DESCRIPTION    In an internal function, data is copied into a buffer through a pointer using memcpy, or the
                  buffer pointer with the appropriate buffer size are passed to a function expecting a valid pair
                  of buffer pointer and buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_MEMCPY_PASS_BUFFER]
                  \R [CM_DCM_R_MEMCPY_WRITE]

\ID SBSW_DCM_CALL_FUNCPTR_TASKINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_TskGetTaskInfo().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_TIMERINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_TmrGetTimerInfo() which will always return a valid function pointer.
  \COUNTERMEASURE \R [CM_DCM_R_GET_TIMERINFO]

\ID SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_DiagGetPostProcessorFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCPOSTPROCESSORS]
                  \R [CM_DCM_R_GET_SVCPOSTPROCESSORS]

\ID SBSW_DCM_CALL_FUNCPTR_SVCUPDATERS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_DiagGetUpdateFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCUPDATERS]
                  \R [CM_DCM_R_GET_SVCUPDATERS]

\ID SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_DiagGetCancelFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCCANCELLERS]
                  \R [CM_DCM_R_GET_SVCCANCELLERS]

\ID SBSW_DCM_CALL_FUNCPTR_MODEMGRRULES
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapModeMgrRulesGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_MODEMGRRULES]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_TERMINATED_LIST
  \DESCRIPTION    In an internal function, iteration over all elements of a given table is done.
                  Each element is a function pointer or a structure containing only function pointer members.
                  The end of iteration is marked by NULL_PTR.
  \COUNTERMEASURE \M [CM_DCM_M_TERMINATED_LIST]
                  \R [CM_DCM_R_TERMINATED_LIST]

\ID SBSW_DCM_LOOP
  \DESCRIPTION    In an internal function, iteration over all elements of a given context member is done.
  \COUNTERMEASURE \R [CM_DCM_R_LOOP]

\ID SBSW_DCM_POINTER_WRITE_TOBJ_ADDBUFFER
  \DESCRIPTION    In an internal function, data is written to array AddBuffer[] of a specific transport object using a
                  function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_TOBJ_ADDBUFFER]

\ID SBSW_DCM_POINTER_WRITE_COMCONTROLCHANNEL
  \DESCRIPTION    In an internal function, data is written to array Dcm_PbRamNetComCtrlChannels[] using a
                  function argument as index.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_COMCONTROLCHANNEL]
                  \R [CM_DCM_R_SET_COMCONTROLCHANNEL]

\ID SBSW_DCM_POINTER_WRITE_AUTHNVMDATA
  \DESCRIPTION    In an internal function, data is written to array Dcm_AuthMgrNvMData[] using a
                  function argument as index.
                  Alternatively, data is written to that array through a pointer.
                  In the latter case the memory entry pointer is initialized by Dcm_AuthMgrGetWLContext().
  \COUNTERMEASURE \R [CM_DCM_R_SET_AUTHNVMDATA]

\ID SBSW_DCM_ARRAY_WRITE_PENDINGROLEDATA
  \DESCRIPTION    In an external API, data is written to array Dcm_AuthMgrSingletonContext.PendingData.Role[].
  \COUNTERMEASURE \R [CM_DCM_R_SET_PENDINGROLEDATA]

\ID SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WLServiceContext.WLServiceElement[]
                  of a connection white list context, or the pointer is passed to a function expecting a valid reference
                  to a connection white list context.
                  The memory entry pointer is either initialized by Dcm_AuthMgrWLContextGetEntry(),
                  or is taken from global variable repeater context, and a loop iterator is used as index.
                  The caller ensures that the element length matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WLSERVICEELEMENT]

\ID SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WLDidContext.WLDidElement[]
                  of a connection white list context.
                  The memory entry pointer is either initialized by Dcm_AuthMgrWLContextGetEntry(),
                  or is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WLDIDELEMENT]

\ID SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WLRidContext.WLRidElement[]
                  of a connection white list context.
                  The memory entry pointer is either initialized by Dcm_AuthMgrWLContextGetEntry(),
                  or is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WLRIDELEMENT]

\ID SBSW_DCM_POINTER_WRITE_MEM_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WLMemContext.WLMemElement[]
                  of a connection white list context.
                  The memory entry pointer is either initialized by Dcm_AuthMgrWLContextGetEntry(),
                  or is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WLMEMELEMENT]

\ID SBSW_DCM_POINTER_WRITE_WL_ELEMENT
  \DESCRIPTION    The white list element pointer is passed to a function expecting a valid reference to a white list
                  element buffer. The caller ensures that the white list element points to a valid memory pointer and
                  the white list element length matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CERTIFICATE_DATA
  \DESCRIPTION    In an internal function, data is written through a pointer to a certificate data buffer,
                  or the certificate data pointer is passed to a function expecting a valid reference to a certificate
                  data buffer. The caller ensures that the certData member points to a valid memory entry pointer and
                  the certDataLength member matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CHALLENGE_SWAP_BUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to the challenge swap buffer,
                  or the challenge swap buffer pointer is passed to a function expecting a valid reference to
                  the challenge swap buffer. The caller ensures that the challenge swap buffer points to a valid memory
                  pointer and buffer length matches to the pointed memory size.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CERTIFICATE_ROLE
  \DESCRIPTION    The certificate role pointer is passed to a function expecting a valid reference to a certificate
                  role buffer. The caller ensures that the role element points to a valid memory pointer and
                  the role element length matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_TOBJHANDLE
  \DESCRIPTION    In an internal function, data is written to array Dcm_PbRamNetConnHdl2TObjMap[] using a
                  function argument as index.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TOBJHANDLE]
                  \R [CM_DCM_R_SET_TOBJHANDLE]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIMER
  \DESCRIPTION    In an internal function, data is written to array
                  Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[] using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIMER]

\ID SBSW_DCM_POINTER_WRITE_TIMER
  \DESCRIPTION    In an internal function, data is written to array Dcm_SingletonContext.TimerMgr.Timer[] using a
                  function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_TIMER]

\ID SBSW_DCM_POINTER_WRITE_START_UP_FBL_RES_BUFFER
  \DESCRIPTION    In an internal function, data is written to array Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_START_UP_FBL_RES_BUFFER]

\ID SBSW_DCM_POINTER_WRITE_ATTEMPTCOUNT
  \DESCRIPTION    In an internal function, data is written to array
                  Dcm_Svc27SingletonContext.AttemptCount[] using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_ATTEMPTCOUNT]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIME_SEC
  \DESCRIPTION    In an internal function, data is written to array Dcm_Svc27SingletonContext.DelayTime[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIME_SEC]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIME_AUTH
  \DESCRIPTION    In an internal function, data is written to array Dcm_AuthMgrSingletonContext.DelayTime[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIME_AUTH]

\ID SBSW_DCM_POINTER_WRITE_DIDLOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.DidLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DIDLOCKLIST]

\ID SBSW_DCM_POINTER_WRITE_RIDLOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.RidLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_RIDLOCKLIST]

\ID SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.ServiceLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_SERVICELOCKLIST]

\ID SBSW_DCM_CALL_FUNCPTR_SVC11SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc11SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC11SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC19SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc19SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC19SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC
  \DESCRIPTION    In an internal function, a function pointer is called.
                  The pointer is initialized by Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_DIDMGROPFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_DIDMGR_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_DIDMGR_OP_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDidMgrIoControlSenderReceiverHandlersGetEntry() which will always return a valid function
                  pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_IOCTRLSRHANDLERS]

\ID SBSW_DCM_CALL_FUNCPTR_SVC27SECLEVEL
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc27SecLevelInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC27SECLEVEL]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC27GETSEED
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc27SecLevelInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC27SECLEVEL]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SEED_FUNC_SIGNATURE]
                  \S [CM_DCM_S_GET_SEED_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapRidMgrOpInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC31OPFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC28SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc28SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC28SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC29SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc29SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC29SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC2CSUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgSvc2CSubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC2CSUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC85SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc85SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC85SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC86SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgSvc86SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC86SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_ROEMODESWITCH
  \DESCRIPTION    In an internal function, a function pointer is called. The index to the called function pointer
                  is a function argument.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_ROEEVENTMODESWITCHINFO]
                  \R [CM_DCM_R_GET_ROEEVENTMODESWITCHINFO]

\ID SBSW_DCM_CALL_FUNCPTR_AUTHMODESWITCH
  \DESCRIPTION    In an internal function, a function pointer is called. The index to the called function pointer
                  is a function argument.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_CFGAUTHMGRNETCONNINFO]
                  \R [CM_DCM_R_GET_CFGAUTHMGRNETCONNINFO]

\ID SBSW_DCM_CALL_FUNCPTR_SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_READMEMORY
  \DESCRIPTION    The callout Dcm_ReadMemory uses a write-able pointer of the DCM buffer, which is always valid. The
                  application always writes as many data as the caller expects.
  \COUNTERMEASURE \S [CM_DCM_S_READMEMORY_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC01SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc01ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC01SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC01SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC08SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc08ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC08SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC08SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC09SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc09ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC09SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC09SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC06MIDTIDINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc06MidTidInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC06MIDTIDINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_CONVERTTOARRAY
  \DESCRIPTION    In an internal function, data is copied into a buffer through a pointer, or the
                  buffer pointer with the appropriate buffer size are passed to a function expecting a valid pair
                  of buffer pointer and buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_CONVERTTOARRAY_PASS_BUFFER]
                  \R [CM_DCM_R_CONVERTTOARRAY_WRITE]
                  \S [CM_DCM_S_GEN_POINTER_FORWARD_STACK]

\ID SBSW_DCM_CALL_PROCESSREQUESTTRANSFEREXIT
  \DESCRIPTION    The callout Dcm_ProcessRequestTransferExit uses a write-able pointer of the DCM buffer, which 
                  is always valid. The amount of data which can be written is indicated by an function argument 
                  to the application.
  \COUNTERMEASURE \S [CM_DCM_S_PROCESSREQUESTTRANSFEREXIT_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_POINTER_SVC31_REDIRECT_MESSAGE_CONTEXT
  \DESCRIPTION    A local message context variable is passed to Dcm_Svc31RedirectMsgContext() for
                  initialization. That initalization function ensures that all pointer members are pointing to valid
                  memory entry pointer and the corresponding buffer size members are matching to the pointed memory
                  entry sizes.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

SBSW_JUSTIFICATION_END */

/*
\CM CM_DCM_N_PASS_PARAM_PTR
  The caller ensures the pointers passed to this function are valid.

\CM CM_DCM_N_ARRAY_INDEX_WRITE
  The function insures calculating the correct array size and writing in an array index within the array index limit.

\CM CM_DCM_S_GEN_PARAM_PTR_WRITE
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer
  passed as an argument, but only writes a new value to it.
  SMI-37386

\CM CM_DCM_S_GEN_PARAM_PTR_FORWARD
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer
  passed as an argument, but only forwards it to another function.
  SMI-37387

\CM CM_DCM_S_GEN_POINTER_FORWARD_STACK
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer taken
  from a local variable, and that the function receiving those pointer parameter(s) neither stores the addresses after
  return nor writes more data to the addesses than specified by the byte size of the local variable (especially in
  case of local arrays).
  SMI-37388

\CM CM_DCM_S_DIDMGR_OP_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated OpFunc functions in table Dcm_CfgDidMgrSignalOpClassInfo
  located in Dcm_Lcfg.c match the signature indicated by the FuncClass setting.
  SMI-106112, SMI-117436, SMI-126353, SMI-163630

\CM CM_DCM_S_GET_SEED_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated GetSeedFunc functions in table Dcm_CfgSvc27SecLevelInfo
  located in Dcm_Lcfg.c match the signature indicated by the GetSeedFuncClass setting.
  SMI-40607

\CM CM_DCM_S_GET_SEED_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated GetSeedFunc functions in table Dcm_CfgSvc27SecLevelInfo
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding SeedResLength setting.
  SMI-49478

\CM CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated OpFunc functions in table Dcm_CfgRidMgrOpInfo located
  in Dcm_Lcfg.c match the signature indicated by the OpType setting.
  SMI-64952

\CM CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgRidMgrOpInfo located
  in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResMaxLength setting.
  SMI-64953

\CM CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify all generated Dcm_RidMgr_<RID>_<Start|Stop|RequestResults> functions
  implemented in Dcm_Lcfg.c that for each Dcm_UtiGetResDataRel(pMsgContext, *<offset>*) usage on out signals the
  application does not write more data than (ResMaxLength - *<offset>*), specified by the corresponding ResMaxLength
  setting in table Dcm_CfgRidMgrOpInfo located in _Dcm_Lcfg.c.
  SMI-65597

\CM CM_DCM_S_DID_WRAPPER_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x00XX or 0x30XX, that are entirely
  or partially (e.g. through a wrapper function) implemented by the application, no more data will be written than
  specified by the corresponding MaxLength setting.
  SMI-105069

\CM CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_READ
  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMReadSignal() is intended to be used exclusively
  by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMReadSignal() has a size equals to MaxLength.

  This SMI is required to fulfill SMI-23 by NvM.

  OpFunc functions that require Nv Block ID are implemented in Dcm_Lcfg.c both of:
  - with a generic name Dcm_DidMgr_<SignalName>_Read,
  - and the corresponding FuncClass equals to 0x0002.

  The table Dcm_CfgDidMgrSignalOpClassInfo is located in Dcm_Lcfg.c.
  SMI-106815, SMI-106816

\CM CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_WRITE
  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMWriteSignal() is intended to be used exclusively
  by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMWriteSignal() has a size equals to MaxLength.

  This SMI is required to fulfill SMI-23 by NvM.

  OpFunc functions that require Nv Block ID are implemented in Dcm_Lcfg.c both of:
  - with a generic name Dcm_DidMgr_<SignalName>_Write,
  - and the corresponding FuncClass equals to 0x1002.

  The table Dcm_CfgDidMgrSignalOpClassInfo is located in Dcm_Lcfg.c.
  SMI-117437, SMI-117438

\CM CM_DCM_S_ROE_NV_BLOCK_ID_ACCESS
  The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by DCM_EXT_SVC_86_NVRAM_BLOCKID located
  in Dcm_Cfg.h is intended to be used exclusively by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by DCM_EXT_SVC_86_NVRAM_BLOCKID located
  in Dcm_Cfg.h has a size equals to the size of the structure data type Dcm_ExtSvc86NvMDataType.

  This SMI is required to fulfill SMI-23 by NvM.
  SMI-163518, SMI-163519

\CM CM_DCM_S_SVC29_NV_BLOCK_ID_ACCESS
  The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table
  Dcm_CfgAuthMgrNetConnInfo located in Dcm_Lcfg.c, is intended to be used exclusively by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table
  Dcm_CfgAuthMgrNetConnInfo located in Dcm_Lcfg.c, has a size of the structure data type Dcm_AuthMgrNvMDataType.

  This SMI is required to fulfill SMI-23 by NvM.
  SMI-440519, SMI-440520

\CM CM_DCM_S_READMEMORY_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that the implementation of the call-out function Dcm_ReadMemory() will not
  write more data than specified by the MemorySize parameter.
  SMI-74833

\CM CM_DCM_S_DIDMGR_OP_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x00XX or 0x30XX, that are entirely
  or partially (e.g. through a wrapper function) implemented by the application, no more data will be written than
  specified by the corresponding MaxLength setting.

  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x0003 no more data will be written
  than specified by DCM_DIDMGR_READ_PAGE_SIZE setting located in Dcm_Cfg.h.
  SMI-105069, SMI-105312

\CM CM_DCM_S_GET_SVC01SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc01ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-147940

\CM CM_DCM_S_GET_SVC08SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc08ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-143395

\CM CM_DCM_S_GET_SVC09SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc09ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-147938

\CM CM_DCM_S_PROCESSREQUESTTRANSFEREXIT_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that the implementation of the call-out function
  Dcm_ProcessRequestTransferExit() will not write more data than specified by the transferResponseParameterRecordSize
  parameter.
  SMI-579281

\CM CM_DCM_N_CALL_PARAM_FUNCPTR
  The caller ensures the pointers passed to this function are valid.

\CM CM_DCM_M_DEV_ERROR_DETECT
  Verify that development error detection is active in the configuration.

\CM CM_DCM_M_CONSISTENCY_BUFFERINFO
  Verify for each element in Dcm_CfgNetBufferInfo[] that the member 'Size' matches to the size of the array the
  member 'BufferPtr' is pointing to.

\CM CM_DCM_M_SIZEOF_BUFFERINFO
  Verify Dcm_PbCfgNetBufferInfo[] is defined for each id in range [0..Dcm_PbCfgNetNumBuffers[.

\CM CM_DCM_M_SIZEOF_COMMCONTEXT
  Verify Dcm_PbRamNetComMContext[] is defined for each id in range [0..Dcm_PbCfgNetNumComMChannels[.

\CM CM_DCM_M_SIZEOF_BUFFERCONTEXT
  Verify Dcm_PbRamNetBufferContext[] is defined for each id in range [0..Dcm_PbCfgNetNumBuffers[.

\CM CM_DCM_M_SIZEOF_TRANSPORTOBJECT
  Verify Dcm_PbRamNetTransportObject[] is defined for each id in range [0..Dcm_PbCfgNetNumTransportObjects[.

\CM CM_DCM_M_SIZEOF_COMCONTROLCHANNEL
  Verify Dcm_PbRamNetComCtrlChannels[] is defined for each id in range [0..Dcm_PbCfgNetNumComMChannels[.

\CM CM_DCM_M_SIZEOF_TOBJHANDLE
  Verify Dcm_PbRamNetConnHdl2TObjMap[] is defined for each id in range [0..Dcm_PbCfgNetNumConnections[.

\CM CM_DCM_M_SIZEOF_PERTXOBJ
  Verify Dcm_PbRamNetPeriodicTxObject[] is defined for each id in range [0..Dcm_PbCfgNetNumPerTxObjects[.

\CM CM_DCM_M_SIZEOF_SVCPOSTPROCESSORS
  Verify Dcm_CfgDiagSvcPostProcessors[] is defined for each id in range [0..DCM_CFGDIAGSVCPOSTPROCESSORS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCUPDATERS
  Verify Dcm_CfgDiagSvcUpdaters[] is defined for each id in range [0..DCM_CFGDIAGSVCUPDATERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCCANCELLERS
  Verify Dcm_CfgDiagSvcCancellers[] is defined for each id in range [0..DCM_CFGDIAGSVCCANCELLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_MODEMGRRULES
  Verify Dcm_CfgModeMgrRules[] is defined for each id in range [0..DCM_CFGMODEMGRRULES_SIZE[.

\CM CM_DCM_M_TERMINATED_LIST
  Verify that the last element of the given table is a NULL_PTR.

\CM CM_DCM_M_SIZEOF_SVC11SUBFUNC
  Verify Dcm_CfgSvc11SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC11SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC19SUBFUNC
  Verify Dcm_CfgSvc19SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC19SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_DIDMGROPFUNC
  Verify Dcm_CfgDidMgrSignalOpClassInfo[] is defined for each id in range [0..DCM_CFGDIDMGRSIGNALOPCLASSINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_IOCTRLSRHANDLERS
  Verify Dcm_CfgDidMgr_IOControlSenderReceiverHandlers[] is defined for each id in range
  [0..DCM_CFGDIDMGR_IOCONTROLSENDERRECEIVERHANDLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC27SECLEVEL
  Verify Dcm_CfgSvc27SecLevelInfo[] is defined for each id in range [0..DCM_CFGSVC27SECLEVELINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC31OPFUNC
  Verify Dcm_CfgRidMgrOpInfo[] is defined for each id in range [0..DCM_CFGRIDMGROPINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC28SUBFUNC
  Verify Dcm_CfgSvc28SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC28SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC29SUBFUNC
  Verify Dcm_CfgSvc29SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC29SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC2CSUBFUNC
  Verify Dcm_CfgSvc2CSubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC2CSUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC85SUBFUNC
  Verify Dcm_CfgSvc85SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC85SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC86SUBFUNC
  Verify Dcm_CfgSvc86SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC86SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_ROEEVENTMODESWITCHINFO
  Verify Dcm_CfgModeRoEEventModeSwitchInfoTable[] is defined for each id in range [0..DCM_CFGMODEROEEVENTMODESWITCHINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_CFGAUTHMGRNETCONNINFO
  Verify Dcm_CfgAuthMgrNetConnInfo[] is defined for each id in range [0..DCM_CFGAUTHMGRNETCONNINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SERVICEINFO
  Verify Dcm_CfgDiagServiceInfo[] is defined for each id in range [0..DCM_CFGDIAGSERVICEINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC01SERVICEINFO
  Verify Dcm_CfgSvc01ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC01SERVICEINFOTABLE_SIZE[.

 \CM CM_DCM_M_SIZEOF_SVC06MIDTIDINFO
  Verify Dcm_CfgSvc06MidTidInfoTable[] is defined for each id in range [0..DCM_CFGSVC06MIDTIDINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC08SERVICEINFO
  Verify Dcm_CfgSvc08ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC08SERVICEINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC09SERVICEINFO
  Verify Dcm_CfgSvc09ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC09SERVICEINFOTABLE_SIZE[.

\CM CM_DCM_R_CHECK_REMAINING_BUFFER
  Before writing data to a buffer, check that the buffer is large enough for the data written to it.

\CM CM_DCM_R_GET_BUFFERINFO
  Dcm_NetGetBufferInfo() will always return a valid buffer info element even when the passed memory index is
  not a handle to a buffer info element.

\CM CM_DCM_R_GET_COMMCONTEXT
  Dcm_NetGetComMContext() will always return a valid ComM context element even when the passed memory index is
  not a handle to a ComM context element.

\CM CM_DCM_R_GET_BUFFERCONTEXT
  Dcm_NetGetBufferContext() will always return a valid buffer context element even when the passed memory index is
  not a handle to a buffer context element.

\CM CM_DCM_R_GET_TRANSPORTOBJECT
  Dcm_NetGetTransportObject() will always return a valid transport object even when the passed memory index is
  not a handle to a transport object.

\CM CM_DCM_R_GET_TIMERCONTEXT
  Dcm_TmrGetTimerContext() will always return a valid transport object even when the passed memory index is
  not a handle to a timer context.

\CM CM_DCM_R_GET_BITSETROWINDEX
  Dcm_UtiBitSetGetRowIdxSafe() will return a index within the given bitset array, even when the passed bit position is
  not within the bitset.

\CM CM_DCM_R_BITSET_BASETYPE
  Verify that the base type passed to the bitset manipulation macro matches the base type of the respective bitset
  array.

\CM CM_DCM_R_GET_2ASCHEDULERENTRY
  Dcm_Svc2ASchdGetEntry() will return a valid entry of the 0x2A scheduler table, even when the passed memory index is
  not a handle to a scheduler table entry.

\CM CM_DCM_R_GET_2CSRCITEM
  Dcm_Svc2CGetSrcItem() will return a valid entry of the 0x2C source item table, even when the passed memory index is
  not a handle to a source item table entry.

\CM CM_DCM_R_GET_2CITEM
  Dcm_Svc2CGetItem() will return a valid entry of the 0x2C item table, even when the passed memory index is not a handle
  to an item table entry.

\CM CM_DCM_R_GET_2CPROCESSCONTEXT
  Dcm_Svc2CGetDDDidProcessContext() will return a valid process context of a DID, even when the passed memory index is
  not a handle to a process context.

\CM CM_DCM_R_GET_2FACTIVEIODIDS
  Dcm_Svc2FGetActiveIoDids() will return a valid bitmask of active IODIDs, even when the passed memory index is not a
  handle to a bitmask of active IODIDs.

\CM CM_DCM_R_GET_86DIDRECORD
  Dcm_Svc86_03GetDidRecord() or Dcm_Svc86_07GetDidRecord() will return a valid entry of the DID record table, even when
  the passed memory index is not a handle to a DID record table entry.

\CM CM_DCM_R_GET_VSGACTIVECFGVARIANTS
  Verify that the array index is in range before writing to Dcm_VarMgrSingletonContext.ActiveCfgVariants[].

\CM CM_DCM_R_GET_PERIODICTXBUFFER
  Dcm_NetPeriodicMsgGetTxBuffer() will return a valid tx buffer of a periodic message, even when the passed memory index
  is not a handle to a periodic tx object.

\CM CM_DCM_R_GET_RXPDUINFO
  Verify that the local pointer initialized as a NULL_PTR is set after calling Dcm_NetProvideRxBuffer().

\CM CM_DCM_R_GET_THREADCONTEXT
  Dcm_GetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_DEBUGTHREADCONTEXT
  Dcm_DebugGetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_SVC31THREADCONTEXT
  Dcm_Svc31GetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_TIMERINFO
  Dcm_TmrGetTimerInfo() will always return a valid timer info even when the passed memory index is
  not a handle to a timer info.

\CM CM_DCM_R_GET_SVCPOSTPROCESSORS
  Dcm_DiagGetPostProcessorFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a post-processor function.

\CM CM_DCM_R_GET_SVCUPDATERS
  Dcm_DiagGetUpdateFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to an update function.

\CM CM_DCM_R_GET_SVCCANCELLERS
  Dcm_DiagGetCancelFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a cancellation function.

\CM CM_DCM_R_TERMINATED_LIST
  Verify that iteration over all elements of a given table is only done until NULL_PTR element.

\CM CM_DCM_R_LOOP
  The for loop or while loop with constrains ensures that the index never exceeds the size of the given table.

\CM CM_DCM_R_SET_TOBJ_ADDBUFFER
  Verify that the array index is in range before writing to AddBfufer[] of a specific transport object.

\CM CM_DCM_R_SET_COMCONTROLCHANNEL
  Verify that the array index is in range before writing to Dcm_PbRamNetComCtrlChannels[].

\CM CM_DCM_R_SET_AUTHNVMDATA
  Verify that the array index is in range before writing to Dcm_AuthMgrNvMData[].

\CM CM_DCM_R_SET_PENDINGROLEDATA
  Verify that the array index is in range before writing to Dcm_AuthMgrSingletonContext.PendingData.Role[].

\CM CM_DCM_R_SET_WLSERVICEELEMENT
  Verify that the array index is in range before writing to pWLServiceContext->WLServiceElement[].

\CM CM_DCM_R_SET_WLDIDELEMENT
  Verify that the array index is in range before writing to pWLDidContext->WLDidElement[].

\CM CM_DCM_R_SET_WLRIDELEMENT
  Verify that the array index is in range before writing to pWLRidContext->WLRidElement[].

\CM CM_DCM_R_SET_WLMEMELEMENT
  Verify that the array index is in range before writing to pWLMemContext->WLMemElement[].

\CM CM_DCM_R_SET_TOBJHANDLE
  Verify that the array index is in range before writing to Dcm_PbRamNetConnHdl2TObjMap[].

\CM CM_DCM_R_SET_TIMER
  Verify that the array index is in range before writing to Dcm_SingletonContext.TimerMgr.Timer[].

\CM CM_DCM_R_SET_START_UP_FBL_RES_BUFFER
  Verify that the array index is in range before writing to Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[].

\CM CM_DCM_R_SET_ATTEMPTCOUNT
  Verify that the array index is in range before writing to Dcm_Svc27SingletonContext.AttemptCount[].

\CM CM_DCM_R_SET_DELAYTIME_SEC
  Verify that the array index is in range before writing to Dcm_Svc27SingletonContext.DelayTime[].

\CM CM_DCM_R_SET_DELAYTIME_AUTH
  Verify that the array index is in range before writing to Dcm_AuthMgrSingletonContext.DelayTime[].

\CM CM_DCM_R_SET_DIDLOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.DidLockList[].

\CM CM_DCM_R_SET_RIDLOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.RidLockList[].

\CM CM_DCM_R_SET_SERVICELOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.ServiceLockList[].

\CM CM_DCM_R_SET_PERTXOBJ
  Verify that the array index is in range before writing to Dcm_PbRamNetPeriodicTxObject[].

\CM CM_DCM_R_SET_DELAYTIMER
  Verify that the array index is in range before writing to Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[].

\CM CM_DCM_R_SET_DATA_CONTEXT_BUFFER
  Verify that the array index is in range [0..pDataContext->Size[ before writing to pDataContext->Buffer.

\CM CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER
  Verify that the array index is in range [0..pMsgContext->resBufSize[ before writing to pMsgContext->resData.

\CM CM_DCM_R_WRITE_IO_CTRL_CEMR_SESSION_MASK
  Verify that the write access to the bitmask Dcm_DidMgrIoControlCemrSessionMask does not exceed its
  size (DCM_DIDMGR_IODID_MAX_EXT_CEMR_SIZE).

\CM CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER
  Verify that the array index is in range [0..pMsgContext->reqBufSize[ before writing to pMsgContext->reqData.

\CM CM_DCM_R_SET_DATA_CONTEXT_USAGE
  Verify that the sum of pDataContext->Usage and pDataContext->AvailLen is always equal to or smaller as
  pDataContext->Size.

\CM CM_DCM_R_MEMCPY_PASS_BUFFER
  Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry and memory size.

\CM CM_DCM_R_MEMCPY_WRITE
  Verify that the array index is in range between 0 and the passed buffer size before writing to the passed
  buffer pointer.

\CM CM_DCM_R_GET_DIDINFOCONTEXT
  Dcm_Svc22GetDidInfoContext() will always return a valid Did info context even when the passed memory index is
  not a handle to a Did info context.

\CM CM_DCM_R_GET_CFGAUTHMGRNETCONNINFO
  Verify that the array index is in range before getting the mode switch function from Dcm_CfgAuthMgrNetConnInfo[].

\CM CM_DCM_R_GET_ROEEVENTMODESWITCHINFO
  Verify that the array index is in range before getting the mode switch function from Dcm_CfgModeRoEEventModeSwitchInfoTable[].

\CM CM_DCM_R_GET_RECOVERYINFOCOMMCHANNELSTATE
  Dcm_DiagGetRecoveryInfoComMChannelState() will return a valid ComM channel state of the recovery info, even when the
  passed memory index is not a handle to a ComM channel state of the recovery info.

\CM CM_DCM_R_GET_RECOVERYINFOCOMMCONTROLSTATE
  Dcm_DiagGetRecoveryInfoCommControlState() will return a valid communication state of a ComM channel of the recovery
  info, even when the passed memory index is not a handle to a communication state of a ComM channel of the recovery
  info.

\CM CM_DCM_R_CONVERTTOARRAY_PASS_BUFFER
  Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry and memory size.

\CM CM_DCM_R_CONVERTTOARRAY_WRITE
  Verify that the array index is in range between 0 and the passed buffer size before writing to the passed
  buffer pointer.

\CM CM_DCM_R_GET_VALID_POINTER
  The pointer concerned is initialized with a getter-function by memory index.
  The caller must explicitly verify that the initialization was successful before forwarding or writing to the pointer,
  unless when it can be guaranteed that the passed memory index is valid.

*/
/**********************************************************************************************************************
 *  END OF FILE: Dcm.c
 *********************************************************************************************************************/
