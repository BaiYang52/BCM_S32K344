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
/**        \file  Dcm_RidMgr.h
 *         \unit  RidMgr
 *        \brief  Contains public inline function definitions of RID Manager unit.
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
#if !defined(DCM_RIDMGR_H)
# define DCM_RIDMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_RidMgrTypes.h"
# if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_Svc31.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! RID operation types */
#  define DCM_RIDMGR_OP_NONE                                         ((Dcm_RidMgrOpType)0x00u)
#  define DCM_RIDMGR_OP_START                                        ((Dcm_RidMgrOpType)0x01u)
#  define DCM_RIDMGR_OP_STOP                                         ((Dcm_RidMgrOpType)0x02u)
#  define DCM_RIDMGR_OP_REQRSLTS                                     ((Dcm_RidMgrOpType)0x04u)

/*! Routine operations function prototype descriptors */
#  define DCM_RIDMGR_OPTYPE_NONE                                     ((Dcm_RidMgrOpCallType)0u)
#  define DCM_RIDMGR_OPTYPE_REQ                                      ((Dcm_RidMgrOpCallType)1u)
#  define DCM_RIDMGR_OPTYPE_REQ_RES                                  ((Dcm_RidMgrOpCallType)2u)
#  define DCM_RIDMGR_OPTYPE_RES                                      ((Dcm_RidMgrOpCallType)3u)
#  define DCM_RIDMGR_OPTYPE_REQ_DYNLEN                               ((Dcm_RidMgrOpCallType)4u)
#  define DCM_RIDMGR_OPTYPE_REQ_DYNLEN_RES                           ((Dcm_RidMgrOpCallType)5u)
#  define DCM_RIDMGR_OPTYPE_RES_DYNLEN                               ((Dcm_RidMgrOpCallType)6u)
#  define DCM_RIDMGR_OPTYPE_REQ_DYNLEN_RES_DYNLEN                    ((Dcm_RidMgrOpCallType)7u)
#  define DCM_RIDMGR_OPTYPE_REQ_RES_DYNLEN                           ((Dcm_RidMgrOpCallType)8u)
#  define DCM_RIDMGR_OPTYPE_WRAPPER                                  ((Dcm_RidMgrOpCallType)9u)

#  define DCM_RIDMGR_ROUTINEINFOBYTE_IDX                             ((Dcm_DiagBufferIndexType)3u) /*!< Index of routine info byte in response message (skip SF and RID) */

/*! Routine input and output parameter integrity descriptors */
#  define DCM_RIDMGR_INTEGRITY_NONE                                  ((Dcm_RidMgrParamIntegrityType)0u)
#  define DCM_RIDMGR_INTEGRITY_IN                                    ((Dcm_RidMgrParamIntegrityType)1u)
#  define DCM_RIDMGR_INTEGRITY_OUT                                   ((Dcm_RidMgrParamIntegrityType)2u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Converts sub-function ID to the internal routine operation value */
#  define Dcm_RidMgrConvOpOfSubFunc(sf)                              ((Dcm_RidMgrOpType)(((sf) < 4u)?Dcm_RidMgrSubFunc2OpMap[(sf)]:0u))              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Access abstraction for RID information table */
#  define Dcm_CfgRidMgrRidInfoOpBaseIdx(idx)                         (Dcm_CfgWrapRidMgrRidInfo((idx)).OpBaseIdx)                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrRidInfoExecCondRef(idx)                       (Dcm_CfgWrapRidMgrRidInfo((idx)).ExecCondRef)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrRidInfoOperations(idx)                        (Dcm_CfgWrapRidMgrRidInfo((idx)).Operations)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrRidInfoRoutineInfoByte(idx)                   (Dcm_CfgWrapRidMgrRidInfo((idx)).RoutineInfoByte)                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Access abstraction for RID operation information table */
#  define Dcm_CfgRidMgrOpInfoOpFunc(idx)                             (Dcm_CfgWrapRidMgrOpInfo((idx)).OpFunc)                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrOpInfoReqMinLength(idx)                       (Dcm_CfgWrapRidMgrOpInfo((idx)).ReqMinLength)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrOpInfoReqMaxLength(idx)                       (Dcm_CfgWrapRidMgrOpInfo((idx)).ReqMaxLength)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrOpInfoResMinLength(idx)                       (Dcm_CfgWrapRidMgrOpInfo((idx)).ResMinLength)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrOpInfoOpType(idx)                             (Dcm_CfgWrapRidMgrOpInfo((idx)).OpType)                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_CfgRidMgrOpInfoParamIntegrity(idx)                     (Dcm_CfgWrapRidMgrOpInfo((idx)).ParamIntegrity)                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef P2VAR(Dcm_CfgRidMgrInfoHandleOptType, AUTOMATIC, DCM_VAR_NOINIT) Dcm_CfgRidMgrInfoHandlePtrType;
typedef P2VAR(Dcm_CfgRidMgrOpHandleOptType, TYPEDEF, DCM_VAR_NOINIT) Dcm_CfgRidMgrOpHandlePtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RidMgrInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the RID manager sub-component.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RidMgrInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_RidMgrGetOpInfo()
 *********************************************************************************************************************/
/*! \brief          Returns the operation descriptor of a RID operation.
 *  \details        -
 *  \param[in]      ridInfoIdx       Index to the RID info object
 *  \param[in]      ridOp            The requested RID operation
 *  \param[out]     pRidOpInfoIdx    Index to the RID operation information in the configuration table
 *  \return         DCM_E_OK         Returning descriptor is successful (pRidOpInfoIdx value shall be evaluated)
 *  \return         DCM_E_NOT_OK     Returning descriptor is not successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrGetOpInfo(
  Dcm_CfgRidMgrInfoHandleOptType ridInfoIdx,
  Dcm_RidMgrOpType ridOp,
  Dcm_CfgRidMgrOpHandlePtrType pRidOpInfoIdx
  );

/**********************************************************************************************************************
 *  Dcm_RidMgrExecuteRoutine()
 *********************************************************************************************************************/
/*! \brief          Executes a routine operation.
 *  \details        -
 *  \param[in]      opStatus               The operations status
 *  \param[in]      pMsgContext            Current message context
 *  \param[out]     ErrorCode              The NRC
 *  \param[in,out]  pRepContext            Current repeater proxy context
 *  \return         DCM_E_OK               Operation is done positive
 *  \return         DCM_E_NOT_OK           Operation is done negative
 *  \return         DCM_E_PENDING          Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP      Forces a RCR-RP response
 *  \return         DCM_E_LOOP             Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrExecuteRoutine(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_RidMgrRidLookUp()
 *********************************************************************************************************************/
/*! \brief          Looks for the RID to get its corresponding RID information reference.
 *  \details        -
 *  \param[in]      opStatus        The operations status
 *  \param[in]      rid             The RID to be found
 *  \param[in,out]  pRidInfoIdx     Index to the RID information in the configuration table
 *  \return         DCM_E_NOT_OK    No RID has been found
 *  \return         DCM_E_OK        Look up success
 *  \return         DCM_E_PENDING   Look up result is pending, try again
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrRidLookUp(
  Dcm_OpStatusType opStatus,
  uint16 rid,
  Dcm_CfgRidMgrInfoHandlePtrType pRidInfoIdx
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CONST_8
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! RID sub-function ID to operation type value conversion map */
extern CONST(Dcm_RidMgrOpType, DCM_CONST) Dcm_RidMgrSubFunc2OpMap[4];
#  define DCM_STOP_SEC_CONST_8
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_RIDMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_RidMgr.h
 *********************************************************************************************************************/
