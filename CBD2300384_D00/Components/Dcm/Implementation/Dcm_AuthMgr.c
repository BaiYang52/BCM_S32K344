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
/**        \file  Dcm_AuthMgr.c
 *         \unit  AuthMgr
 *        \brief  Contains the implementation of Authentication Manager unit.
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
#define DCM_AUTHMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_AuthMgr.h"
#if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Debug.h"
# include "Dcm_Tsk.h"
# include "Dcm_Tmr.h"
# include "Dcm_Uti.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_AUTHMGR_CERT_ROLE_SIZE > 4)
#  error "Authentication role size greater 4 is not supported."
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Authentication session timers */
# define Dcm_AuthMgrGetTimerIdx(authInfoRef)                         (authInfoRef)                                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_AuthMgrTimerGet(authInfoRef)                            (Dcm_AuthMgrSingletonContext.DelayTime[Dcm_AuthMgrGetTimerIdx((authInfoRef))])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerActive(authInfoRef)                         ((boolean)(Dcm_AuthMgrTimerGet((authInfoRef)) != 0u))                           /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerExpireMaskReset()                           (Dcm_AuthMgrSingletonContext.TimerExpiredMask = 0u)                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrTimerExpireMaskGet()                             (Dcm_AuthMgrSingletonContext.TimerExpiredMask)                                  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_AuthMgrAnyTimerExpired()                                (Dcm_AuthMgrSingletonContext.TimerExpiredMask != 0u)
/*! Authentication role */
# define Dcm_AuthMgrActiveRole(authInfoRef)                          (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.ActiveRole)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Service white list operations */
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
#  define Dcm_AuthMgrIsGlobalAccessGranted()                         (Dcm_AuthMgrSingletonContext.GrantGlobalAccess == TRUE)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSetGlobalAccess(value)                          (Dcm_AuthMgrSingletonContext.GrantGlobalAccess = (value))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrNumSvcWLElements(authInfoRef)                   (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLServiceContext.NumServiceWLElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemLen(authInfoRef, iter)                 (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLServiceContext.WLServiceElement[iter].WLEntryLength) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, iter, pos)         (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLServiceContext.WLServiceElement[iter].WLData[pos]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsSID(authInfoRef, iter, sid)          (Dcm_AuthMgrSvcWLElemDataAt((authInfoRef), (iter), 0u) == (sid))                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs22(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_READ))         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs2E(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_WRITE))        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs2F(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), DCM_DIDMGR_SVC_IOCTRL))       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsDIDWRIO(authInfoRef, iter)           ((Dcm_AuthMgrSvcWLElemLen((authInfoRef), (iter)) == 3u) && (Dcm_AuthMgrSvcWLElemIs22((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIs2E((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIs2F((authInfoRef), (iter)))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIs31(authInfoRef, iter)                (Dcm_AuthMgrSvcWLElemIsSID((authInfoRef), (iter), 0x31u))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsRID(authInfoRef, iter)               ((Dcm_AuthMgrSvcWLElemLen((authInfoRef), (iter)) == 4u) && Dcm_AuthMgrSvcWLElemIs31((authInfoRef), (iter))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrSvcWLElemIsDSP(authInfoRef, iter)               (Dcm_AuthMgrSvcWLElemIsDIDWRIO((authInfoRef), (iter)) || Dcm_AuthMgrSvcWLElemIsRID((authInfoRef), (iter))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! DID white list operations */
# if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumDIDWLElements(authInfoRef)                   (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLDidContext.NumDidWLElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrDidWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLDidContext.WLDidElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! RID white list operations */
# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumRIDWLElements(authInfoRef)                   (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLRidContext.NumRidWLElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrRidWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLRidContext.WLRidElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! MEM white list operations */
# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
#  define Dcm_AuthMgrNumMemWLElements(authInfoRef)                   (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLMemContext.NumMemWLElements) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_AuthMgrMemWLElem(authInfoRef, iter)                    (Dcm_AuthMgrNvMData[(authInfoRef)].ConnContext.WLContext.WLMemContext.WLMemElement[(iter)]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
/*! Redirects the reading of the authenticaion roles */
# if (DCM_AUTHMGR_USER_SPECIFIC_ROLES_ENABLED == STD_ON)
#  define Dcm_AuthMgrCertElementGet                                  Dcm_GetAuthenticationRoles                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_AuthMgrCertElementGet                                  KeyM_CertElementGet                                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/*! Pending data for processing of Dcm_SetDeauthenticatedRole() */
struct DCM_AUTHMGRPENDINGROLETYPE_TAG
{
  uint32                    ConnectionMask;                        /*!< Bit mask for the connetionId for which Dcm_SetDeauthenticatedRole() was called */
  Dcm_CfgStateGroupMemType  Role[DCM_CFGAUTHMGRNETCONNINFO_SIZE];  /*!< Pending role from Dcm_SetDeauthenticatedRole() */
};
typedef struct DCM_AUTHMGRPENDINGROLETYPE_TAG Dcm_AuthMgrPendingRoleType;
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/*! Authentication manager context */
struct DCM_AUTHMGRSINGLETONCONTEXTTYPE_TAG
{
#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
  Dcm_AuthMgrPendingRoleType       PendingData;                              /*!< Pending data for processing of Dcm_SetDeauthenticatedRole() */
#  endif
  Dcm_AuthMgrTimerExpiredMaskType  TimerExpiredMask;                         /*!< A flag to store expired timers for authentication sessions */
#  if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
  Dcm_AuthMgrTimerType             DelayTime[DCM_AUTHMGR_NUM_DELAY_TIMERS];  /*!< Array of delay timers for authentication sessions */

#  endif
#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  boolean                          GrantGlobalAccess;                        /*!< Global access flag */
#  endif
};
typedef struct DCM_AUTHMGRSINGLETONCONTEXTTYPE_TAG Dcm_AuthMgrSingletonContextType;
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerSet()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific idle connection session timer.
 *  \details        -
 *  \param[in]      authInfoRef  The authentication info reference
 *  \param[in]      value        Timer value
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrTimerSet(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  );
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetConnAuthState()
 *********************************************************************************************************************/
/*! \brief          Sets the authentication state of a connection of the given index.
 *  \details        -
 *  \param[in]      authInfoRef  Index to the ConnAuthStates array
 *  \param[in]      value        New authentication state of connection
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetConnAuthState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType value
  );
# endif

# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingRole()
 *********************************************************************************************************************/
/*! \brief          Sets the pending role coming from external API Dcm_SetDeauthenticatedRole().
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType pendingRole
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertWLElement()
 *********************************************************************************************************************/
/*! \brief          Reads a white list element of a certificate.
 *  \details        -
 *  \param[in]      certificateId              Certificate Id
 *  \param[in]      certElementId              Certificate element Id
 *  \param[out]     pElementData               Pointer to store the element data
 *  \param[in,out]  pElementDataLength         Element data length
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pRepContext                The repeater context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   No element left (end of list)
 *  \return         DCM_E_LOOP                 Element successfully read
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertWLElement(
  uint16 certificateId,
  uint16 certElementId,
  Dcm_MsgType pElementData,
  Dcm_Uint32VarDataPtrType pElementDataLength,
  Dcm_OpStatusType opStatus,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckServiceWLDispatcher()
 *********************************************************************************************************************/
/*! \brief          Authentication service white list check for DSD
 *  \details        Checks that the request matches first bytes of an element in service white list
 *  \param[in]      sid                        The service identifier
 *  \param[in,out]  pMsgContext                The message context
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckServiceWLDispatcher(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDidWL()
 *********************************************************************************************************************/
/*! \brief          Authentication DID white list check
 *  \details        Checks if DID and op matches element in DID white list
 *  \param[in]      did                        The DID identifier
 *  \param[in]      didOpMask                  The operation (bit-wise)
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDidWL(
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
#  endif
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
     (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRidWL()
 *********************************************************************************************************************/
/*! \brief          Authentication RID white list check
 *  \details        Checks if RID and op matches element in RID white list
 *  \param[in]      rid                        The RID identifier
 *  \param[in]      ridSubfunc                 The routine control subfunction
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRidWL(
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMemWL()
 *********************************************************************************************************************/
/*! \brief          Authentication MEM white list check
 *  \details        Checks if MemorySelection matches element in MEM white list
 *  \param[in]      memId                      The MemorySelection
 *  \param[in]      authInfoRef                The authentication information reference
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMemWL(
  uint8 memId,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Authentication manager data that can be read from and written to NvM (if NvM is configured) */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
VAR(Dcm_AuthMgrNvMDataType, DCM_VAR_NOINIT) Dcm_AuthMgrNvMData[DCM_CFGAUTHMGRNETCONNINFO_SIZE];
# endif

/*! Singleton context of AuthMgr unit */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
DCM_LOCAL VAR(Dcm_AuthMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_AuthMgrSingletonContext;
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrTimerSet(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  )
{
  uint8_least lTimerIdx = Dcm_AuthMgrGetTimerIdx(authInfoRef);

  if(Dcm_DebugDetectRuntimeError(lTimerIdx >= DCM_AUTHMGR_NUM_DELAY_TIMERS))                                                                         /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.DelayTime[lTimerIdx] = value;                                                                                        /* SBSW_DCM_POINTER_WRITE_DELAYTIME_AUTH */
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetConnAuthState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetConnAuthState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType value
  )
{
  if(Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_CFGAUTHMGRNETCONNINFO_SIZE))                                                                     /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrNvMData[authInfoRef].ConnContext.AuthState = value;                                                                                   /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
  }
}
# endif

# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetPendingRole()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_AuthMgrSetPendingRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType pendingRole
  )
{
  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_CFGAUTHMGRNETCONNINFO_SIZE))                                                                    /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrSingletonContext.PendingData.Role[authInfoRef] = pendingRole;                                                                         /* SBSW_DCM_ARRAY_WRITE_PENDINGROLEDATA */
  }
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckServiceWLDispatcher()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckServiceWLDispatcher(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  uint16_least lIter;

  /* Reset global access flag */
  Dcm_AuthMgrSetGlobalAccess(FALSE);

  /* If requesting connection is authenticated: */
  if(Dcm_AuthMgrAuthenticationState(authInfoRef) == DCM_AUTHENTICATED)
  {
    /* Loop through all service white list elements */
    for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter ++)
    {
      /* If service white list element should be processed in DSP: */
      if(!Dcm_AuthMgrSvcWLElemIsDSP(authInfoRef, lIter))
      {
        uint8_least lElemIter;
        boolean lFound = TRUE;
        /* If SID matches the first byte of element: */
        if(sid == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 0u))
        {
          /* Loop through the rest of the element bytes */
          for(lElemIter = 0u; lElemIter < ((uint8_least)Dcm_AuthMgrSvcWLElemLen(authInfoRef, lIter) - 1u); lElemIter ++)
          {
            /* If the iterator does not exceed request data length: */
            if(lElemIter < pMsgContext->reqDataLen)
            {
              /* If the element byte doesn't match corresponding request byte: */
              if(pMsgContext->reqData[lElemIter] != Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, (lElemIter + 1u)))
              {
                /* Set found boolean to false and skip element */
                lFound = FALSE;
                break;
              }
            }
            else
            {
              /* Element size needs to be less than or equal to the request length */
              lFound = FALSE;
            }
          }
        }
        else
        {
          /* SID doesn't match */
          lFound = FALSE;
        }

        /* If a matching element is found: */
        if(lFound == TRUE)
        {
          /* Grant global request access (no need to check subfunction or data) */
          Dcm_AuthMgrSetGlobalAccess(TRUE);
          lStdResult = DCM_E_OK;
          break;
        }
      }
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDidWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDidWL(
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint16_least lIter;

#   if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
  /* Loop through entire DID WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumDIDWLElements(authInfoRef); lIter++)
  {
    /* If we find the DID we're after */
    if(did == Dcm_AuthMgrDidWLElem(authInfoRef, lIter).DidNumber)
    {
      /* And the right DID op access */
      if(Dcm_UtiBitOpTest(uint8, Dcm_AuthMgrDidWLElem(authInfoRef, lIter).DidAccessMask, didOpMask))
      {
        lStdResult = DCM_E_OK;
        break;
      }
    }
  }
#   endif
#   if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* Check service white list for allowance */
  if(lStdResult == DCM_E_NOT_OK)                                                                                                                     /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    uint8 lSid = Dcm_DidMgrGetSidFromOp(didOpMask);
    if(lSid != DCM_DIDMGR_SVC_NONE)
    {
      /* Loop over all service WL elements */
      for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter ++)
      {
        if(Dcm_AuthMgrSvcWLElemIsDIDWRIO(authInfoRef, lIter))
        {
          /* Check the bytes of the white list element */
          if((lSid == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 0u))
           && (Dcm_UtiGetHiByte(did) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 1u))
           && (Dcm_UtiGetLoByte(did) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 2u)))
          {
            /* We have found a matching element to the request! */
            lStdResult = DCM_E_OK;
            /* No need to check further */
            break;
          }
        }
      }
    }
  }
#   endif

  /* Return search result */
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#  endif
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
     (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRidWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRidWL(
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint16_least lIter;

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
  /* Loop through entire RID WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumRIDWLElements(authInfoRef); lIter++)
  {
    /* If we find the RID we're after */
    if(rid == Dcm_AuthMgrRidWLElem(authInfoRef, lIter).RidNumber)
    {
      /* And the right RID op access */
      if(Dcm_UtiBitOpTest(uint8, Dcm_AuthMgrRidWLElem(authInfoRef, lIter).RidAccessMask, Dcm_RidMgrConvOpOfSubFunc(ridSubfunc)))
      {
        lStdResult = DCM_E_OK;
        break;
      }
    }
  }
#  endif
#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* Check service white list for allowance */
  if(lStdResult == DCM_E_NOT_OK)                                                                                                                     /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    /* Loop over all service WL elements */
    for(lIter = 0u; lIter < Dcm_AuthMgrNumSvcWLElements(authInfoRef); lIter++)
    {
      if(Dcm_AuthMgrSvcWLElemIsRID(authInfoRef, lIter))
      {
        /* Check the bytes of the white list element */
        if((ridSubfunc == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 1u))
         && (Dcm_UtiGetHiByte(rid) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 2u))
         && (Dcm_UtiGetLoByte(rid) == Dcm_AuthMgrSvcWLElemDataAt(authInfoRef, lIter, 3u)))
        {
          /* We have found a matching element to the request! */
          lStdResult = DCM_E_OK;
          /* No need to check further */
          break;
        }
      }
    }
  }
#  endif

  /* Return search result */
  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMemWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMemWL(
  uint8 memId,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  uint16_least lIter;

  /* Loop through entire MEM WL */
  for(lIter = 0u; lIter < Dcm_AuthMgrNumMemWLElements(authInfoRef); lIter++)
  {
    /* If we find the MEM we're after */
    if(memId == Dcm_AuthMgrMemWLElem(authInfoRef, lIter).MemNumber)
    {
      lStdResult = DCM_E_OK;
      break;
    }
  }

  /* Return search result */
  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertWLElement()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertWLElement(
  uint16 certificateId,
  uint16 certElementId,
  Dcm_MsgType pElementData,
  Dcm_Uint32VarDataPtrType pElementDataLength,
  Dcm_OpStatusType opStatus,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lKeyMResult;

    /* If first element to be read: */
  if(opStatus == DCM_INITIAL)
  {
    /* Retrieve first element with KeyM_CertElementGetFirst, which sets up KeyM iterator struct */
    lKeyMResult = KeyM_CertElementGetFirst(certificateId
                                          ,certElementId
                                          ,&pRepContext->CertElementIterator
                                          ,pElementData
                                          ,pElementDataLength);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* Retrieve remeining elements with KeyM_CertElementGetNext */
    lKeyMResult = KeyM_CertElementGetNext(&pRepContext->CertElementIterator
                                         ,pElementData
                                         ,pElementDataLength);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If KeyM_CertElementGet[First/Next] returns E_OK: */
  if(lKeyMResult == E_OK)
  {
    lStdResult = DCM_E_LOOP;
  }
  /* Otherwise, if KeyM function returns E_NOT_OK: */
  else if(lKeyMResult == E_NOT_OK) /* End of list */
  {
    /* End of elements list, return success */
    lStdResult = DCM_E_OK;
  }
  /* Otherwise, send panic NRC */
  else
  {
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetDeauthenticatedRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetDeauthenticatedRole(
  uint16 connectionId,                                                                                                                               /* PRQA S 1330 */ /* MD_Dcm_RteSpecific_1330 */
  P2CONST(uint8, AUTOMATIC, RTE_DCM_APPL_DATA) deauthenticatedRole                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                         /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_NetConnRefMemType lConnHdl;

    lConnHdl = Dcm_NetGetConnHdlByConnectionId(connectionId);

    if (Dcm_DebugDetectError(lConnHdl == Dcm_NetGetInvalidConnHdl()))                                                                                /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
    {
      lErrorId = DCM_E_PARAM;
    }
    else if (Dcm_DebugDetectError(Dcm_NetGetAuthInfoRef(lConnHdl) == DCM_AUTHMGR_INVALID_AUTH_INFO_REF))                                             /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      /* ----- Implementation ---------------------------------------------- */
      uint8_least               lRoleIndex;
      Dcm_CfgStateGroupMemType  lRoleInLittleEndian = 0x00u;

      for (lRoleIndex = 0; lRoleIndex < DCM_AUTHMGR_CERT_ROLE_SIZE; lRoleIndex++)
      {
        lRoleInLittleEndian |= (Dcm_CfgStateGroupMemType)deauthenticatedRole[lRoleIndex]<<(lRoleIndex * 8u);
      }

      /* Enter critical section (Reason: API is reentrant) */
      Dcm_UtiEnterCriticalSection();
      /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
      Dcm_AuthMgrSetPendingRole(Dcm_NetGetAuthInfoRef(lConnHdl), lRoleInLittleEndian);
      Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask |= Dcm_UtiGetBitFromIndex(uint32, lConnHdl);
      /*=================================*
      END CRITICAL SECTION
      *=================================*/
      Dcm_UtiLeaveCriticalSection();

      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_SETROLE, DCM_THREAD_ID_BASIC);
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_DCMSETDEAUTHENTICATEDROLE, lErrorId);

  return DCM_E_OK;
}                                                                                                                                                    /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON) && \
     ((DCM_SVC_14_USER_MEM_ID_ENABLED == STD_ON) || (DCM_SVC_19_USER_MEM_ID_CHECK_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckMem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckMem(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgStateRefOptType stateRef,
  uint8 memId
  )
{
  Std_ReturnType lStdResult;

  Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(Dcm_DiagGetTranspObj(pContext)->ConnHdl);

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
  /* Check the MEM whitelist only in authenticated state */
  if((lStdResult == DCM_E_NOT_OK) && (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED))
  {
    lStdResult = Dcm_AuthMgrCheckMemWL(memId, lAuthInfoRef);
  }
#  else
  DCM_IGNORE_UNREF_PARAM(memId);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif

  return lStdResult;
}
# endif

# if (DCM_TMR_NUM_AUTHENTICATION > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutAuthentication()
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
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutAuthentication(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType       lReloadTick = 0u;
  Dcm_CfgNetAuthInfoRefOptType  lTmrIter;

  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Enter critical section (Reason: A timer can be modified from an ISR) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* For each software timer: */
  for(lTmrIter = 0; lTmrIter < DCM_AUTHMGR_NUM_DELAY_TIMERS; ++lTmrIter)
  {
    /* If the software timer is active: */
    if(Dcm_AuthMgrTimerActive(lTmrIter) == TRUE)                                                                                                     /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Decrement the timer value */
      Dcm_AuthMgrTimerSet((Dcm_CfgNetAuthInfoRefMemType)lTmrIter, (Dcm_AuthMgrTimerType)(Dcm_AuthMgrTimerGet(lTmrIter) - 1u));

      /* If the software timer is still active */
      if(Dcm_AuthMgrTimerActive(lTmrIter) == TRUE)                                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* Reload the hardware timer */
        lReloadTick = 1u;
      }
      else
      {
        /* Otherwise, update the global expired timer mask flag with position of expired timer */
        Dcm_AuthMgrTimerExpireMaskSetBit((Dcm_CfgNetAuthInfoRefMemType)lTmrIter);
      }
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  /* If at least one timer has expired */
  if(Dcm_AuthMgrAnyTimerExpired())                                                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    /* Set the event to deauthenticate idle connections in default session */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_IDLE_CONN, threadId);
  }

  return lReloadTick;
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerStart(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthMgrTimerType value
  )
{
  /* Start the appropriate authentication timer */
  Dcm_AuthMgrTimerSet(authInfoRef, value);

  if(!Dcm_TmrIsTimerRunning(DCM_TMR_ID_AUTHENTICATION))
  {
    /* Start the base authentication timer */
    Dcm_TmrStartTimer(DCM_TMR_ID_AUTHENTICATION, 1u);
  }
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerReset(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Dcm_AuthMgrTimerSet(authInfoRef, 0u);
}
# endif

# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDetectIdleConnection()
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
FUNC(void, DCM_CODE) Dcm_AuthMgrDetectIdleConnection(
  Dcm_CfgStateGroupOptType oldSession
  )
{
  /* If this is not a transition from default to default session: */
  if (oldSession != DCM_STATE_VALUE_SESSION_INIT)
  {
    Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(Dcm_NetGetSessionConnection());

    if (lAuthInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      boolean lIsTimerActive;

      /* Enter critical section (Reason: A timer can be modified from an ISR or high priority task) */
      Dcm_UtiEnterCriticalSection();
      /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
      lIsTimerActive = Dcm_AuthMgrTimerActive(lAuthInfoRef);                                                                                         /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      /*=================================*
      END CRITICAL SECTION
      *=================================*/
      /* Leave critical section */
      Dcm_UtiLeaveCriticalSection();

      if (lIsTimerActive == FALSE)
      {
        if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED)
        {
          /* Deauthenticate the connection */
          Dcm_AuthMgrUpdateActiveRole(lAuthInfoRef, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
          Dcm_AuthMgrSetAuthentication(lAuthInfoRef, DCM_DEAUTHENTICATED);
        }
      }
    }
  }
}
# endif

# if (DCM_AUTHMGR_RID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRid(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgStateRefOptType stateRef
  )
{
  Std_ReturnType lStdResult;

  Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(Dcm_DiagGetTranspObj(pContext)->ConnHdl);

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* If Role check fails, check RID WL if in authenticated state */
  if((lStdResult == DCM_E_NOT_OK) && (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED))
  {
    lStdResult = Dcm_AuthMgrCheckRidWL(rid, ridSubfunc, lAuthInfoRef);
  }
#  else
  DCM_IGNORE_UNREF_PARAM(rid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ridSubfunc);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckRole(
  Dcm_CfgStateRefOptType stateRef,
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* Check if authentication restriction is supported for this connection */
  if(authInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
  {
#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
    /* Check if global access has been granted through service whitelist at DSD */
    if(Dcm_AuthMgrIsGlobalAccessGranted())
    {
      lStdResult = DCM_E_OK;
    }
    else
#  endif
    {
      if(!Dcm_StateIsSupportedInRole(Dcm_StateGetPreconditionStates(stateRef), authInfoRef))
      {
        lStdResult = DCM_E_NOT_OK;
      }
    }
  }
  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetCertRole()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetCertRole(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lKeyMResult;
  Std_ReturnType lStdResult = DCM_E_OK;

  uint32                   lRoleElementLength = DCM_AUTHMGR_CERT_ROLE_SIZE;
  uint8                    lRoleReadElement[4] = { 0 };
  Dcm_CfgStateGroupMemType lRoleInLittleEndian;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Extract the role from the certificate */
  lKeyMResult = Dcm_AuthMgrCertElementGet(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                         ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertRoleId
                                         ,lRoleReadElement
                                         ,&lRoleElementLength);                                                                                      /* SBSW_DCM_POINTER_WRITE_CERTIFICATE_ROLE */


  /* If role extraction from certificate was successful */
  if(lKeyMResult == E_OK)
  {
    /* If the size of the read role is equal to configured role size */
    if(lRoleElementLength == DCM_AUTHMGR_CERT_ROLE_SIZE)
    {
      /* Store role in role context. Cast to Dcm_CfgStateGroupMemType to truncate unwanted upper bytes */
      lRoleInLittleEndian = ((Dcm_CfgStateGroupMemType)(Dcm_UtiMake32Bit(lRoleReadElement[3]
                                                                        ,lRoleReadElement[2]
                                                                        ,lRoleReadElement[1]
                                                                        ,lRoleReadElement[0])));

      pRepContext->ConnContext.ActiveRole = lRoleInLittleEndian;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      if(Dcm_DebugDetectError(lRoleElementLength > DCM_AUTHMGR_CERT_ROLE_SIZE))                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
      {
        /* Send panic NRC */
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
      }
      else
      {
        /* Otherwise return NRC 55 */
        *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
  }
  else
  {
    /* Role extraction wasn't successful, return Error */
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDispatcherLevel()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDispatcherLevel(
  uint8 sid,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  /* Reset global access flag */
  Dcm_AuthMgrSetGlobalAccess(FALSE);
#  endif

  /* Check authentication role */
  lStdResult = Dcm_AuthMgrCheckRole(stateRef, authInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  if(lStdResult == DCM_E_NOT_OK)
  {
    /* If role check fails, try whitelist */
    lStdResult = Dcm_AuthMgrCheckServiceWLDispatcher(sid, pMsgContext, authInfoRef);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#  else
  DCM_IGNORE_UNREF_PARAM(sid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif

  if(lStdResult == DCM_E_NOT_OK)
  {
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrServiceWLReadList()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrServiceWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_OpStatusType                   lOpStatus = DCM_INITIAL;
  Dcm_AuthMgrWLServiceContextPtrType pWLServiceContext = &pRepContext->ConnContext.WLContext.WLServiceContext;

  pWLServiceContext->NumServiceWLElements = 0u;                                                                                                      /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* Loop over all service white list elements until there are no more left */
  do
  {
    uint32 lElementLength = DCM_AUTHMGR_ELEMENT_SERVICE_SIZE;

    lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                            ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLServicesId
                                            ,&(pWLServiceContext->WLServiceElement[pWLServiceContext->NumServiceWLElements].WLData[0u])
                                            ,&lElementLength
                                            ,lOpStatus
                                            ,pRepContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If element successfully read: */
    if(lStdResult == DCM_E_LOOP)
    {
      /* If retrieved element length fits within allocated buffer: */
      if((lElementLength > 0u) && (lElementLength <= DCM_AUTHMGR_ELEMENT_SERVICE_SIZE))
      {
        /* Store length of service white list element */
        pWLServiceContext->WLServiceElement[pWLServiceContext->NumServiceWLElements].WLEntryLength = (uint8)lElementLength;                          /* SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT */

        /* Increment number of service white list entries for next iteration */
        pWLServiceContext->NumServiceWLElements++;                                                                                                   /* SBSW_DCM_GLOBAL_PTR_WRITE */
        lOpStatus = DCM_PENDING;

        /* If buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
        if(pWLServiceContext->NumServiceWLElements >= DCM_AUTHMGR_NUM_SERVICE_WL_ELEMENTS)
        {
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        /* Otherwise, if read element is bigger than expected: */
        if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_SERVICE_SIZE))                                                                  /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
        {
          /* Send panic NRC */
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        }
        else
        {
          /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    } /* Otherwise, all elements read or send prepared NRC */
  } while((pWLServiceContext->NumServiceWLElements < DCM_AUTHMGR_NUM_SERVICE_WL_ELEMENTS) && (lStdResult == DCM_E_LOOP));

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDidWLReadList()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrDidWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_OpStatusType               lOpStatus = DCM_INITIAL;
  Dcm_AuthMgrWLDidContextPtrType pWLDidContext = &pRepContext->ConnContext.WLContext.WLDidContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_DID_SIZE;
  uint8                          lDidReadElement[DCM_AUTHMGR_ELEMENT_DID_SIZE];

  pWLDidContext->NumDidWLElements = 0u;                                                                                                              /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* Loop over all service white list elements until there are no more left */
  do
  {
    lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                            ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLDidId
                                            ,&lDidReadElement[0u]
                                            ,&lElementLength
                                            ,lOpStatus
                                            ,pRepContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If element successfully read: */
    if(lStdResult == DCM_E_LOOP)
    {
      /* If retrieved element length is as expected: */
      if(lElementLength == DCM_AUTHMGR_ELEMENT_DID_SIZE)
      {
        /* Store DID element data into context (big endian format) */
        pWLDidContext->WLDidElement[pWLDidContext->NumDidWLElements].DidNumber = (Dcm_UtiMake16Bit(lDidReadElement[0u], lDidReadElement[1u]));       /* SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT */
        pWLDidContext->WLDidElement[pWLDidContext->NumDidWLElements].DidAccessMask = lDidReadElement[2u];                                            /* SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT */

        /* Increment number of DID white list entries for next iteration */
        pWLDidContext->NumDidWLElements++;                                                                                                           /* SBSW_DCM_GLOBAL_PTR_WRITE */
        lOpStatus = DCM_PENDING;

        /* If buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
        if(pWLDidContext->NumDidWLElements >= DCM_AUTHMGR_NUM_DID_WL_ELEMENTS)
        {
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        /* Otherwise, if read element is bigger than expected: */
        if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_DID_SIZE))                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
        {
          /* Send panic NRC */
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        }
        else
        {
          /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }/* Otherwise, all elements read or send prepared NRC */
  } while((pWLDidContext->NumDidWLElements < DCM_AUTHMGR_NUM_DID_WL_ELEMENTS) && (lStdResult == DCM_E_LOOP));

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrMemWLReadList()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrMemWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_OpStatusType               lOpStatus = DCM_INITIAL;
  Dcm_AuthMgrWLMemContextPtrType pWLMemContext = &pRepContext->ConnContext.WLContext.WLMemContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_MEM_SIZE;
  uint8                          lMemReadElement[DCM_AUTHMGR_ELEMENT_MEM_SIZE];

  pWLMemContext->NumMemWLElements = 0u;                                                                                                              /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* Loop over all service white list elements until there are no more left */
  do
  {

    lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                            ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLMemId
                                            ,&lMemReadElement[0u]
                                            ,&lElementLength
                                            ,lOpStatus
                                            ,pRepContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If element successfully read: */
    if(lStdResult == DCM_E_LOOP)
    {
      /* If retrieved element length is as expected: */
      if(lElementLength == DCM_AUTHMGR_ELEMENT_MEM_SIZE)
      {
        /* Store MEM element data into context */
        pWLMemContext->WLMemElement[pWLMemContext->NumMemWLElements].MemNumber = lMemReadElement[0u];                                                /* SBSW_DCM_POINTER_WRITE_MEM_WL_ELEMENT */

        /* Increment number of MEM white list entries for next iteration */
        pWLMemContext->NumMemWLElements++;                                                                                                           /* SBSW_DCM_GLOBAL_PTR_WRITE */
        lOpStatus = DCM_PENDING;

        /* If buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
        if(pWLMemContext->NumMemWLElements >= DCM_AUTHMGR_NUM_MEM_WL_ELEMENTS)
        {
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        /* Otherwise, if read element is bigger than expected: */
        if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_MEM_SIZE))                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
        {
          /* Send panic NRC */
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        }
        else
        {
          /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }/* Otherwise, all elements read or send prepared NRC */
  } while((pWLMemContext->NumMemWLElements < DCM_AUTHMGR_NUM_MEM_WL_ELEMENTS) && (lStdResult == DCM_E_LOOP));

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrGetWLContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrGetWLContext(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  CONSTP2VAR(Dcm_AuthMgrWLContextPtrType, AUTOMATIC, AUTOMATIC) pWLContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_CFGAUTHMGRNETCONNINFO_SIZE))                                                                    /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);

    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pWLContext = &Dcm_AuthMgrNvMData[authInfoRef].ConnContext.WLContext;                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrUpdateActiveRole()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrUpdateActiveRole(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_CfgStateGroupMemType authRole
  )
{

  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_CFGAUTHMGRNETCONNINFO_SIZE))                                                                    /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_AuthMgrNvMData[authInfoRef].ConnContext.ActiveRole = authRole;                                                                               /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
  }
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetAuthentication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrSetAuthentication(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  Dcm_AuthenticationStateType authState
  )
{
  /* Set Authentication flag for connection to new state */
  Dcm_AuthMgrSetConnAuthState(authInfoRef, authState);
  /* Rte mode switch to new state */
  Dcm_AuthMgrModeSwitchAuthentication(authInfoRef, authState);
}
# endif

/**********************************************************************************************************************
 *  Dcm_AuthMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrInit(
  void
  )
{
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  Dcm_NegativeResponseCodeType lErrorCode;
#  endif

#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  if (Dcm_ModeCheck(DCM_MODE_AUTH_PERSIST_STATE_MODERULE_REF, &lErrorCode) != DCM_E_OK)                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif
  {
    Dcm_CfgNetAuthInfoRefOptType lAuthInfoRef;

    for (lAuthInfoRef = 0u; lAuthInfoRef < DCM_CFGAUTHMGRNETCONNINFO_SIZE; lAuthInfoRef++)
    {
      Dcm_AuthMgrSetConnAuthState((Dcm_CfgNetAuthInfoRefMemType)lAuthInfoRef, DCM_DEAUTHENTICATED);
      Dcm_AuthMgrUpdateActiveRole((Dcm_CfgNetAuthInfoRefMemType)lAuthInfoRef, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
    }
  }

#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
  Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask = 0u;
#  endif
  Dcm_AuthMgrTimerExpireMaskReset();
# endif
}

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrDeauthenticateIdleConn()
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
FUNC(void, DCM_CODE) Dcm_AuthMgrDeauthenticateIdleConn(
  void
  )
{
  Dcm_CfgNetAuthInfoRefMemType    lTmrIter = 0u;
  Dcm_AuthMgrTimerExpiredMaskType lExpiredTimers;

  /* Enter critical section (Reason: A timer can be modified from an ISR) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
  BEGIN CRITICAL SECTION
  *=================================*/
  /* Retrieve all expired authentication timers atomically */
  lExpiredTimers = Dcm_AuthMgrTimerExpireMaskGet();

  /* Reset global expired timers flag */
  Dcm_AuthMgrTimerExpireMaskReset();
  /*=================================*
  END CRITICAL SECTION
  *=================================*/
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();

  DCM_UTI_LOOP_BIT_SCAN(lExpiredTimers)
  {
    if (Dcm_UtiBitOpTest(Dcm_AuthMgrTimerExpiredMaskType, lExpiredTimers, 0x01u))
    {
      /* If the connection is authenticated */
      if (Dcm_AuthMgrAuthenticationState(lTmrIter) == DCM_AUTHENTICATED)
      {
        if ( (Dcm_StateIsDefaultSessionActive())
           ||(lTmrIter != Dcm_NetGetAuthInfoRef(Dcm_NetGetSessionConnection())) )                                                                    /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        {
          Dcm_AuthMgrUpdateActiveRole(lTmrIter, DCM_AUTHMGR_DEAUTHENTICATED_ROLE);
          Dcm_AuthMgrSetAuthentication(lTmrIter, DCM_DEAUTHENTICATED);
        }
      }
    }

    ++lTmrIter;
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

# if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrSetDeauthenticatedRole()
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
FUNC(void, DCM_CODE) Dcm_AuthMgrSetDeauthenticatedRole(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If diagnostic kernel is still busy with a diagnostic job, retry next task activation */
  if (Dcm_DiagIsKernelBusy())
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SETROLE);                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    Dcm_NetConnRefMemType lConnHdl = 0u;
    uint32 lMask;

    /* Enter critical section (Reason: Protect against interrupts from Dcm_SetDeauthenticatedRole() API) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
     BEGIN CRITICAL SECTION
     *=================================*/
    lMask = Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask;

    DCM_UTI_LOOP_BIT_SCAN(lMask)
    {
      if (Dcm_UtiBitOpTest(uint32, lMask, 0x01u))
      {
        Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(lConnHdl);

        if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_DEAUTHENTICATED)
        {
          Dcm_AuthMgrUpdateActiveRole(lAuthInfoRef, Dcm_AuthMgrSingletonContext.PendingData.Role[lAuthInfoRef]);
        }
      }

      ++lConnHdl;
    }

    Dcm_AuthMgrSingletonContext.PendingData.ConnectionMask = 0u;
    /*=================================*
     END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }
}
# endif

# if (DCM_AUTHMGR_DID_CHECK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrCheckDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrCheckDid(
  Dcm_NetConnRefMemType connHdl,
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgStateRefOptType stateRef
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if(connHdl != Dcm_NetGetInvalidConnHdl())
  {
    Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetGetAuthInfoRef(connHdl);

    /* Check authentication role */
    lStdResult = Dcm_AuthMgrCheckRole(stateRef, lAuthInfoRef);

#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON) || \
      (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
    /* If Role check fails, check DID WL if in authenticated state */
    if((lStdResult == DCM_E_NOT_OK) && (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED))
    {
      lStdResult = Dcm_AuthMgrCheckDidWL(did, didOpMask, lAuthInfoRef);
    }
#  else
    DCM_IGNORE_UNREF_PARAM(did);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
    DCM_IGNORE_UNREF_PARAM(didOpMask);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
  }

  return lStdResult;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrAuthenticationState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_AuthenticationStateType, DCM_CODE) Dcm_AuthMgrAuthenticationState(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  return Dcm_AuthMgrNvMData[authInfoRef].ConnContext.AuthState;
}
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrTimerExpireMaskSetBit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrTimerExpireMaskSetBit(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef
  )
{
  Dcm_UtiBitOpSet(Dcm_AuthMgrTimerExpiredMaskType, Dcm_AuthMgrSingletonContext.TimerExpiredMask, Dcm_UtiGetBitFromIndex(uint8_least, Dcm_AuthMgrGetTimerIdx(authInfoRef)));
}
# endif

# if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrRidWLReadList()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthMgrRidWLReadList(
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_OpStatusType               lOpStatus = DCM_INITIAL;
  Dcm_AuthMgrWLRidContextPtrType pWLRidContext = &pRepContext->ConnContext.WLContext.WLRidContext;
  uint32                         lElementLength = DCM_AUTHMGR_ELEMENT_RID_SIZE;
  uint8                          lRidReadElement[DCM_AUTHMGR_ELEMENT_RID_SIZE];

  pWLRidContext->NumRidWLElements = 0u;                                                                                                              /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* Loop over all service white list elements until there are no more left */
  do
  {
    lStdResult = Dcm_AuthMgrGetCertWLElement(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ConnectionCertificateId
                                            ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLRidId
                                            ,&lRidReadElement[0u]
                                            ,&lElementLength
                                            ,lOpStatus
                                            ,pRepContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_POINTER_WRITE_WL_ELEMENT */ /* SBSW_DCM_COMB_PTR_FORWARD */

    /* If element successfully read: */
    if(lStdResult == DCM_E_LOOP)
    {
      /* If retrieved element length is as expected: */
      if(lElementLength == DCM_AUTHMGR_ELEMENT_RID_SIZE)
      {
        /* Store RID element data into context (big endian format) */
        pWLRidContext->WLRidElement[pWLRidContext->NumRidWLElements].RidNumber = (Dcm_UtiMake16Bit(lRidReadElement[0u], lRidReadElement[1u]));       /* SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT */
        pWLRidContext->WLRidElement[pWLRidContext->NumRidWLElements].RidAccessMask = lRidReadElement[2u];                                            /* SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT */

        /* Increment number of RID white list entries for next iteration */
        pWLRidContext->NumRidWLElements++;                                                                                                           /* SBSW_DCM_GLOBAL_PTR_WRITE */
        lOpStatus = DCM_PENDING;

        /* If buffer too small for the number of entries: Send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
        if(pWLRidContext->NumRidWLElements >= DCM_AUTHMGR_NUM_RID_WL_ELEMENTS)
        {
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
      else
      {
        /* Otherwise, if read element is bigger than expected: */
        if(Dcm_DebugDetectError(lElementLength > DCM_AUTHMGR_ELEMENT_RID_SIZE))                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
        {
          /* Send panic NRC */
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_BUFFER_OVERFLOW);
        }
        else
        {
          /* Otherwise, send NRC 0x55 (DCM_E_CVF_INVALIDCONTENT) */
          *ErrorCode = DCM_E_CVF_INVALIDCONTENT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }/* Otherwise, all elements read or send prepared NRC */
  } while((pWLRidContext->NumRidWLElements < DCM_AUTHMGR_NUM_RID_WL_ELEMENTS) && (lStdResult == DCM_E_LOOP));

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_AuthMgr.c
 *********************************************************************************************************************/
