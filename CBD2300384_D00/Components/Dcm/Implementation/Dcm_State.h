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
/**        \file  Dcm_State.h
 *         \unit  State
 *        \brief  Contains public inline function definitions of State unit.
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
#if !defined(DCM_STATE_H)
# define DCM_STATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_StateTypes.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON) || \
     (DCM_STATE_SECURITY_EXT_SETTER_ENABLED == STD_ON)
#  define DCM_STATE_SECURITY_LOOKUP_ENABLED                          STD_ON
# else
#  define DCM_STATE_SECURITY_LOOKUP_ENABLED                          STD_OFF
# endif

# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON) || \
     (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) || \
     (DCM_DIDMGR_PERIODICDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) || \
     (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
#  define DCM_STATE_NOTIFY_SVC_PRO_ENABLED                           STD_ON
# else
#  define DCM_STATE_NOTIFY_SVC_PRO_ENABLED                           STD_OFF
# endif

/*! Represents the initial (default) value of the diagnostic state group "DiagnosticSession": "DefaultSession" */
# define DCM_STATE_VALUE_SESSION_INIT                                ((Dcm_CfgStateGroupMemType)0x01u)
/*! Represents the initial (default) value of the diagnostic state group "SecurityAccess": "Locked" */
# define DCM_STATE_VALUE_SECURITY_INIT                               ((Dcm_CfgStateGroupMemType)0x01u)
/*! Represents all of the values of the diagnostic state group "SecurityAccess" except "Locked" */
# define DCM_STATE_VALUE_SECURITY_ALL_BUT_INIT                       ((Dcm_CfgStateGroupMemType)(Dcm_UtiMaxValueOfUintType(Dcm_CfgStateGroupMemType) - 1u))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Synchronizes both pending and committed (active) diagnostic session to have the same state (i.e. the pending session becomes the state of the last valid diagnostic session */
# define Dcm_StateSyncPendingSession()                               (Dcm_StateSetPendingSession(Dcm_StateGetSession()))                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/*! Returns TRUE if the authentication role pre-condition of a diagnostic entity is met */
#  define Dcm_StateIsSupportedInRole(precondPckgRef, authInfoRef)    ((Dcm_AuthMgrActiveRole(authInfoRef) & (precondPckgRef).AuthRole) != 0u)        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! Returns the diagnostic state related pre-conditions of a diagnostic entity */
# define Dcm_StateGetPreconditionStates(precondPckgRef)              (Dcm_CfgWrapStatePreconditions((precondPckgRef)).States)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns the diagnostic session pre-condition(s) of a diagnostic entity */
# define Dcm_StateGetPreconditionSession(precondPckgRef)             (Dcm_StateGetPreconditionStates(precondPckgRef).Session)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/*! Returns the security access pre-condition(s) of a diagnostic entity */
#  define Dcm_StateGetPreconditionSecurity(precondPckgRef)           (Dcm_StateGetPreconditionStates(precondPckgRef).Security)                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! Returns TRUE if current (committed) diagnostic session is the Default-Session. Otherwise returns FALSE */
# define Dcm_StateIsDefaultSessionActive()                           ((Dcm_StateGetSession() & DCM_STATE_VALUE_SESSION_INIT) != 0u)                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns TRUE if pending diagnostic session is the Default-Session. Otherwise returns FALSE */
# define Dcm_StateIsDefaultSessionPending()                          ((Dcm_StateGetPendingSession() & DCM_STATE_VALUE_SESSION_INIT) != 0u)           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns TRUE if the diagnostic session precondition of a diagnostic entity is met */
# define Dcm_StateIsSupportedInSession(preconditionPackage)          ((Dcm_StateGetSession() & (preconditionPackage).Session) != 0u)                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns the diagnostic session ID of the diagnostic session state */
# define Dcm_StateGetSessionValue(stateIdx)                          (Dcm_CfgWrapStateSessionInfo(stateIdx)->Value)                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/*! Returns TRUE if current security level is the Locked-Level. Otherwise returns FALSE */
#  define Dcm_StateIsSecurityLocked()                                ((Dcm_StateGetSecurity() & DCM_STATE_VALUE_SECURITY_INIT) != 0u)                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns TRUE if the security access pre-condition of a diagnostic entity is met */
#  define Dcm_StateIsSupportedInSecurityLevel(preconditionPackage)   ((Dcm_StateGetSecurity() & (preconditionPackage).Security) != 0u)               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Returns the security access level ID of the security access state */
#  define Dcm_StateGetSecurityValue(stateIdx)                        ((Dcm_SecLevelType)(((stateIdx) == 0u)?0u:(Dcm_CfgWrapStateSecurityInfo((stateIdx)-1u))->Value)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

# if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)  &&  \
     (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
#  if (DCM_STATE_MULTIPLE_SECURITYFIXEDBYTES_ENABLED == STD_ON)
#   define Dcm_StateGetVsgMaskInfoIdx(stateIdx)                      (Dcm_CfgWrapStateVsgSecurityFixedByteInfo((stateIdx)))                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_StateGetNumberOfVsgs(stateIdx)                        ((Dcm_VsgSizeType)(Dcm_CfgWrapStateVsgSecurityFixedByteInfo((stateIdx) + 1u)) - (Dcm_VsgSizeType)(Dcm_CfgWrapStateVsgSecurityFixedByteInfo((stateIdx) + 0u))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#   define Dcm_StateGetVsgMaskInfoIdx(stateIdx)                      (stateIdx)                                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define Dcm_StateGetNumberOfVsgs(stateIdx)                        ((((Dcm_VsgSizeType)(Dcm_CfgWrapStateSecurityFixedByteInfo((stateIdx) + 1u)) - (Dcm_VsgSizeType)(Dcm_CfgWrapStateSecurityFixedByteInfo((stateIdx) + 0u))) > 0u)?1u:0u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Pointer to a execution precondition reference index located in the DCM ROM data */
typedef P2CONST(Dcm_CfgStateRefMemType, TYPEDEF, DCM_CONST)  Dcm_CfgStateRefMemPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_StateInit()
 *********************************************************************************************************************/
/*! \brief          State initialization.
 *  \details        Initialization of the state manager sub-module.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_StateInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_StateGetSession()
 *********************************************************************************************************************/
/*! \brief          Get the current session.
 *  \details        -
 *  \return         Session state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetSession(
  void
  );

/**********************************************************************************************************************
 *  Dcm_StateSetSession()
 *********************************************************************************************************************/
/*! \brief          Performs a session state transition.
 *  \details        -
 *  \param[in]      stateRef    The state order number.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_StateSetSession(
  Dcm_StateIndexOptType stateRef
  );

/**********************************************************************************************************************
 *  Dcm_StateGetPendingSession()
 *********************************************************************************************************************/
/*! \brief          Get the pending session.
 *  \details        -
 *  \return         Pending session
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetPendingSession(
  void
  );

/**********************************************************************************************************************
 *  Dcm_StateSetPendingSession()
 *********************************************************************************************************************/
/*! \brief          Set the pending session.
 *  \details        -
 *  \param[in]      session    The next pending session.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_StateSetPendingSession(
  Dcm_CfgStateGroupMemType session
  );

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateGetSecurity()
 *********************************************************************************************************************/
/*! \brief          Get the current security state.
 *  \details        -
 *  \return         Security state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetSecurity(
  void
  );
# endif

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateSetSecurity()
 *********************************************************************************************************************/
/*! \brief          Performs a security state transition.
 *  \details        -
 *  \param[in]      stateRef    The state order number.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_StateSetSecurity(
  Dcm_StateIndexOptType stateRef
  );
# endif

# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateSetSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Sets the security state in a bypass.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_StateSetSecurityBypass(
  void
  );
# endif

# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateIsSecurityBypassPending()
 *********************************************************************************************************************/
/*! \brief          Returns whether the security precondition checks are bypassed or going to be bypassed.
 *  \details        -
 *  \return         TRUE     The security precondition checks are bypassed or going to be bypassed
 *  \return         FALSE    The security precondition checks are not bypassed or going to be not bypassed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_StateIsSecurityBypassPending(
  void
  );
# endif

# if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateCheckDID()
 *********************************************************************************************************************/
/*! \brief          Performs a DID state rule check.
 *  \details        -
 *  \param[in]      connHdl                   The connection identifier
 *  \param[in]      did                       The DID identifier
 *  \param[in]      didOpMask                 The DID operation
 *  \param[in]      stateRef                  Reference to the state precondition table
 *  \param[out]     ErrorCode                 The NRC
 *  \return         DCM_E_OK                  State rule check was sucessful
 *  \return         DCM_E_NOT_OK              State rule check was not sucessful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheckDID(
  Dcm_NetConnRefMemType connHdl,
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateCheckRID()
 *********************************************************************************************************************/
/*! \brief          Performs a RID state rule check.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      rid                     The RID identifier
 *  \param[in]      ridSubfunc              The routine control subfunction
 *  \param[in]      stateRef                Reference to the state precondition table
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_OK                State rule check was sucessful
 *  \return         DCM_E_NOT_OK            State rule check was not sucessful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheckRID(
  Dcm_ContextPtrType pContext,
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

/**********************************************************************************************************************
*  Dcm_StateCheck()
*********************************************************************************************************************/
/*! \brief          Performs a state rule check.
*  \details        -
*  \param[in]      stateRef                  Reference to the state precondition table
*  \param[in]      chkLevel                  State level check
*  \param[out]     ErrorCode                 The NRC
*  \return         DCM_E_OK                  State rule check was sucessful
*  \return         DCM_E_NOT_OK              State rule check was not sucessful
*  \context        TASK
*  \reentrant      FALSE
*  \synchronous    TRUE
*  \pre            -
*********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheck(
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType ErrorCode
);
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_STATE_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_State.h
 *********************************************************************************************************************/
