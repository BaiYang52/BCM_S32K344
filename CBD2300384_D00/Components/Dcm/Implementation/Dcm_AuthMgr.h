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
/**        \file  Dcm_AuthMgr.h
 *         \unit  AuthMgr
 *        \brief  Contains public inline function definitions of Authentication Manager unit.
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
#if !defined(DCM_AUTHMGR_H)
# define DCM_AUTHMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_AuthMgrTypes.h"
# if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_Tsk.h"
#  include "Dcm.h"
#  include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Authentication RID access rights validation support */
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON) && \
      (DCM_SVC_31_SUPPORT_ENABLED == STD_ON)
#   define DCM_AUTHMGR_RID_CHECK_ENABLED                             STD_ON
#  else
#   define DCM_AUTHMGR_RID_CHECK_ENABLED                             STD_OFF
#  endif

/*! Authentication DID access rights validation support */
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON) && \
      (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
#   define DCM_AUTHMGR_DID_CHECK_ENABLED                             STD_ON
#  else
#   define DCM_AUTHMGR_DID_CHECK_ENABLED                             STD_OFF
#  endif

#  if (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
#   define DCM_AUTHMGR_CERT_FUNCTION_OTHER                           (Dcm_AuthMgrCertFunctionType)0u
#   define DCM_AUTHMGR_CERT_FUNCTION_CODING                          (Dcm_AuthMgrCertFunctionType)1u
#  endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON) && \
      ((DCM_SVC_14_USER_MEM_ID_ENABLED == STD_ON) || (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMem()
 *********************************************************************************************************************/
/*! \brief          MemorySelection authentication checks
 *  \details        Checks if MemorySelection is allowed by MEM white list
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      stateRef                   Reference to the state precondition table
 *  \param[in]      memId                      The memory selection id
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMem(
  Dcm_ContextPtrType pContext,
  Dcm_CfgStateRefOptType stateRef,
  uint8 memId
  );
#  endif

#  if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerStart()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific connection idle timer.
 *  \details        Starts the base authentication idle connection timer.
 *  \param[in]      authInfoRef      The authentication info reference
 *  \param[in]      value            Timer value for the specific idle connection timer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerStart(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  );
#  endif

#  if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerReset()
 *********************************************************************************************************************/
/*! \brief          Resets the value of a specific delay timer.
 *  \details        -
 *  \param[in]      authInfoRef  The authentication info reference
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerReset(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif

#  if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDetectIdleConnection()
 *********************************************************************************************************************/
/*! \brief          Detects an idle connection and deauthenticates it if necessary
 *  \details        -
 *  \param[in]      oldSession    The old session
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrDetectIdleConnection(
  Dcm_CfgStateGroupOptType oldSession
  );
#  endif

#  if (DCM_AUTHMGR_RID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRid()
 *********************************************************************************************************************/
/*! \brief          RID operation authentication checks
 *  \details        Checks if RID operation is allowed by role or RID white list
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      rid                        The RID identifier
 *  \param[in]      ridSubfunc                 The routine control subfunction
 *  \param[in]      stateRef                   The operation execution precondition reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRid(
  Dcm_ContextPtrType pContext,
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgStateRefOptType stateRef
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRole()
 *********************************************************************************************************************/
/*! \brief          Authentication role check for execution preconditons reference
 *  \details        -
 *  \param[in]      stateRef                   The operation execution precondition reference
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRole(
  Dcm_CfgStateRefOptType stateRef,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertRole()
 *********************************************************************************************************************/
/*! \brief          Reads the role element of a certificate.
 *  \details        -
 *  \param[in]      opStatus                   The operation status
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertRole(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDispatcherLevel()
 *********************************************************************************************************************/
/*! \brief          Authentication access rights check on dispatcher level
 *  \details        -
 *  \param[in]      sid                        The service identifier
 *  \param[in]      stateRef                   The operation execution precondition reference
 *  \param[in,out]  pMsgContext                The message context
 *  \param[in]      authInfoRef                The authentication information reference
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDispatcherLevel(
  uint8 sid,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrServiceWLReadList()
 *********************************************************************************************************************/
/*! \brief          Reads the services white list element of a certificate.
 *  \details        -
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrServiceWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDidWLReadList()
 *********************************************************************************************************************/
/*! \brief          Reads the DID white list element of a certificate.
 *  \details        -
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrDidWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrMemWLReadList()
 *********************************************************************************************************************/
/*! \brief          Reads the memory selection white list element of a certificate.
 *  \details        -
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrMemWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetWLContext()
 *********************************************************************************************************************/
/*! \brief          Get white lists for a connection.
 *  \details        -
 *  \param[in]      authInfoRef     Index to white lists array
 *  \param[out]     pWLContext      White list entries for a connection
 *  \param[out]     ErrorCode       Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK        The operation was successful
 *  \return         DCM_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetWLContext(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  CONSTP2VAR(Dcm_AuthMgrWLContextPtrType, AUTOMATIC, AUTOMATIC) pWLContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrUpdateActiveRole()
 *********************************************************************************************************************/
/*! \brief          Authentication Manager update active role.
 *  \details        Sets the active role to a particular connection to the new authentication role argument.
 *  \param[in]      authInfoRef                Index to the ConnActiveRole array
 *  \param[in]      authRole                   The new authentication role
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrUpdateActiveRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType authRole
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetAuthentication()
 *********************************************************************************************************************/
/*! \brief          Performs an authentication state transition.
 *  \details        -
 *  \param[in]      authInfoRef    The authentication information reference
 *  \param[in]      authState      The new authentication state
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrSetAuthentication(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType authState
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_AuthMgrInit()
 *********************************************************************************************************************/
/*! \brief          State initialization.
 *  \details        Initialization of the authentication manager sub-module.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrInit(
  void
  );

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDeauthenticateIdleConn()
 *********************************************************************************************************************/
/*! \brief          Deauthenticates all idle connections
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrDeauthenticateIdleConn(
  void
  );
#  endif

#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetDeauthenticatedRole()
 *********************************************************************************************************************/
/*! \brief          Handles the event set by API Dcm_SetDeauthenticatedRole().
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrSetDeauthenticatedRole(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  );
#  endif

#  if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDid()
 *********************************************************************************************************************/
/*! \brief          DID operation authentication checks
 *  \details        -
 *  \param[in]      connHdl                    The connection identifier
 *  \param[in]      did                        The DID identifier
 *  \param[in]      didOpMask                  The operation (bit-wise)
 *  \param[in]      stateRef                   The operation execution precondition reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDid(
  Dcm_NetConnRefMemType connHdl,
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgStateRefOptType stateRef
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrAuthenticationState()
 *********************************************************************************************************************/
/*! \brief          Returns the authentication state
 *  \details        -
 *  \param[in]      lAuthInfoRef               Reference to the authentication info
 *  \return         authentication state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_AuthenticationStateType, DCM_CODE) Dcm_AuthMgrAuthenticationState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerExpireMaskSetBit()
 *********************************************************************************************************************/
/*! \brief          Sets the flag to store expired timers for authentication sessions
 *  \details        -
 *  \param[in]      lAuthInfoRef               Reference to the authentication info
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerExpireMaskSetBit(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrRidWLReadList()
 *********************************************************************************************************************/
/*! \brief          Reads the RID white list element of a certificate.
 *  \details        -
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrRidWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_AUTHMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_AuthMgr.h
 *********************************************************************************************************************/
