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
/**        \file  Dcm_State.c
 *         \unit  State
 *        \brief  Contains the implementation of State unit.
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
#define DCM_STATE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_State.h"
#include "Dcm_Diag.h"
#include "Dcm_Tsk.h"
#include "Dcm_Uti.h"
#include "Rte_Dcm.h"
#include "Dcm_AuthMgr.h"
#include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Control data of the sub-component state manager */
struct DCM_STATECONTEXTTYPE_TAG
{
  Dcm_CfgStateDiagStateInfoType Preconditions;         /*!< Current (active) state of all diagnostic state related execution pre-conditions */
  Dcm_CfgStateGroupMemType      PendingSession;        /*!< Pending diagnostic session, still in transition, waiting for the diagnostic service processing confirmation */
  boolean                       PendingSecurityBypass; /*!< Pending security bypass, still in transition, waiting for idle diagnostic kernel */
};
typedef struct DCM_STATECONTEXTTYPE_TAG Dcm_StateSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateSetPendingSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Set the pending security bypass mode.
 *  \details        The security bypass mode will be updated once the diagnostic kernel is idle again.
 *  \param[in]      value    Specifies whether the security bypass mode shall be activated or not
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateSetPendingSecurityBypass(
  boolean value
  );
#endif

/**********************************************************************************************************************
 *  Dcm_StateExecOnStateChangeFunc()
 *********************************************************************************************************************/
/*! \brief          Executes all registered on state change functions if precondition no more fulfilled.
 *  \details        -
 *  \param[in]      notifList        The notification list
 *  \param[in]      formerStateId    The former state
 *  \param[in]      newStateId       The new state
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateExecOnStateChangeFunc(
  Dcm_CfgStateNotificationInfoPtrType notifList,
  uint8 formerStateId,
  uint8 newStateId
  );

#if (DCM_STATE_SECURITY_LOOKUP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateFindStateIndexFromSecLevel()
 *********************************************************************************************************************/
/*! \brief          Map an external state identifier to an internal state index.
 *  \details        -
 *  \param[in]      secLevel    The security level (incl. LOCKED)
 *  \return         -1          Not found
 *  \return         >= 0        Internal state index
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(sint8_least, DCM_CODE) Dcm_StateFindStateIndexFromSecLevel(
  Dcm_SecLevelType secLevel
  );
#endif

#if ((DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)  && (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_StateVsgGetSecurityFixedByteStateIdx()
 *********************************************************************************************************************/
/*! \brief         Returns referencs to active security fixed byte.
 *  \details        Returns a reference to a security fixed byte that is assigned to a active VSG
 *  \param[in,out]  pStateIndex      Index to security fixed byte VSG configuration in
 *                                   Dcm_CfgStateVsgMaskSecurityFixedByte
 *  \return         DCM_E_OK         Operation succeeded
 *  \return         DCM_E_NOT_OK     No active security fixed byte available
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_StateVsgGetSecurityFixedByteStateIdx(
  P2VAR(sint32_least, AUTOMATIC, DCM_APPL_DATA) pStateIndex
  );
#endif

#if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateGetSecurityLevelFixedBytes()
 *********************************************************************************************************************/
/*! \brief          Provides fixed bytes set up for a specific security level.
 *  \details        This function provides the fixed bytes for the requested security level.
 *                  A security level without configured fixed bytes will return E_OK and no bytes (bufferSize = 0)
 *  \param[in]      secLevel              The requested security level
 *  \param[out]     fixedBytes            Buffer to receive the fixed byte values
 *  \param[in,out]  bufferSize
 *                  IN                    The provided buffer size
 *                  OUT                   The number of fixed bytes for the requested level
 *  \return         E_OK                  The security bytes were copied to the provided buffer
 *  \return         DCM_E_NOT_OK          The security level is not configured
 *  \return         DCM_E_BUFFERTOOLOW    The buffer was too small, bufferSize will contain the number of bytes needed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_StateGetSecurityLevelFixedBytes(
  Dcm_SecLevelType secLevel,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) fixedBytes,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) bufferSize
  );
#endif

#if (DCM_STATE_NOTIFY_SVC_PRO_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateNotifyServiceProcessors()
 *********************************************************************************************************************/
/*! \brief          Notifies all related service processors for a state change (of any state group).
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateNotifyServiceProcessors(
  void
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of State unit */
DCM_LOCAL VAR(Dcm_StateSingletonContextType, DCM_VAR_NOINIT) Dcm_StateSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! A map to get the negative response code corresponding to a specific diagnostic entity (i.e. service, sub-service or parameter identifier */
DCM_LOCAL CONST(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_StateSessionNrcs[DCM_DIAG_NUM_CHK_LVLS]=                                                 /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION /* on SID level */
 ,DCM_E_REQUESTOUTOFRANGE /* on Parameter (DID, RID, etc.) level */
 ,DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION /* on SubFunction level */
};
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  Dcm_StateSetPendingSecurityBypass()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateSetPendingSecurityBypass(
  boolean value
  )
{
  Dcm_StateSingletonContext.PendingSecurityBypass = value;
}
#endif

/**********************************************************************************************************************
 *  Dcm_StateExecOnStateChangeFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateExecOnStateChangeFunc(
  Dcm_CfgStateNotificationInfoPtrType notifList,                                                                                                     /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint8 formerStateId,
  uint8 newStateId
  )
{
  Dcm_CfgStateNotificationInfoPtrType lNotifList = notifList;
  /* Iterate over all with the diagnostic state group state change related notifications, as long as the EndOfList marker is not reached */
  while (lNotifList->OnChgFunc != NULL_PTR)
  {
    /* Invoke the configured notification function */
    lNotifList->OnChgFunc(formerStateId, newStateId);                                                                                                /* SBSW_DCM_PARAM_FUNCPTR_CALL */
    Dcm_UtiNextItemByPtr(lNotifList, 1u);/* take next function */
  }
}

#if (DCM_STATE_SECURITY_LOOKUP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateFindStateIndexFromSecLevel()
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
DCM_LOCAL_INLINE FUNC(sint8_least, DCM_CODE) Dcm_StateFindStateIndexFromSecLevel(
  Dcm_SecLevelType secLevel
  )
{
  sint8_least lResult;

  /* If "locked" level: */
  if(secLevel == DCM_SEC_LEV_LOCKED)
  {
    /* Return the only possible index - 0 */
    lResult = 0;
  }
  else
  {
    /* Otherwise locate the index of the security level value in the security info table */
    uint8_least lSecLvlIter;

    lResult = -1;
    for(lSecLvlIter = 0; lSecLvlIter < DCM_STATE_SECURITY_NUM_LEVELS; ++lSecLvlIter)
    {
      if ((Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->Value == secLevel)
      {
        /* Found the correct index into the table - compensate zero offset for locked level */
        lResult = (sint8_least)((sint16_least)lSecLvlIter + 1);
        break;
      }
    }
    /* If the security level is not found, return -1 */
  }
  return lResult;
}
#endif

#if ((DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)  &&  (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_StateVsgGetSecurityFixedByteStateIdx()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_StateVsgGetSecurityFixedByteStateIdx(
  P2VAR(sint32_least, AUTOMATIC, DCM_APPL_DATA) pStateIndex
  )
{
  Std_ReturnType  lStdResult;
  Dcm_VsgSizeType lNumberOfFixedBytes;
  sint32_least    lRefToVsg;

  lRefToVsg = (sint32) Dcm_StateGetVsgMaskInfoIdx(*pStateIndex);
  lStdResult = DCM_E_NOT_OK;

  /* If there is any Securtiy Level with more than 1 security fixed byte configured */
  lNumberOfFixedBytes = Dcm_StateGetNumberOfVsgs((uint16)(*pStateIndex));                                                                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  /* Iterate over all securtiy fixed bytes that are assigned to referenced security level */
  /* If lNumberOfFixedBytes is 0 -> no security fixed byte is configured for securtiy level */
  for (; lNumberOfFixedBytes > 0u; --lNumberOfFixedBytes)
  {
    if (Dcm_VarMgrIsEnabledInActiveVariants(Dcm_CfgWrapStateVsgMaskSecurityFixedByte(lRefToVsg)))                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    {
      *pStateIndex = lRefToVsg;                                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_OK;
      break;
    }

    ++lRefToVsg;                                                                                                                                     /* PRQA S 2983 */ /* MD_Dcm_Redundant_2983 */
  }

  return lStdResult;
}
#endif

#if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateGetSecurityLevelFixedBytes()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_StateGetSecurityLevelFixedBytes(
  Dcm_SecLevelType secLevel,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) fixedBytes,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) bufferSize
  )
{
  Std_ReturnType  lResult;
  sint32_least    lStateIdx;

  lStateIdx = Dcm_StateFindStateIndexFromSecLevel(secLevel);
  if (lStateIdx <= 0)
  {
    /* Locked, or unsupported security level */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    uint8 lFixedByteSize;
    --lStateIdx; /* norm to zero index (excluded locked state offset) */

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
    lResult = Dcm_StateVsgGetSecurityFixedByteStateIdx(&lStateIdx);                                                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (lResult == DCM_E_NOT_OK)
    {
      lFixedByteSize = 0;
      lResult = E_OK;
    }
    else
# endif
    {
      lFixedByteSize = (uint8)(Dcm_CfgWrapStateSecurityFixedByteInfo(lStateIdx + 1)
                             - Dcm_CfgWrapStateSecurityFixedByteInfo(lStateIdx + 0));                                                                /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
      if (lFixedByteSize > *bufferSize)
      {
        lResult = DCM_E_BUFFERTOOLOW;
      }
      else
      {
        Dcm_UtiMemCopySafe(Dcm_CfgWrapStateSecurityFixedBytes(Dcm_CfgWrapStateSecurityFixedByteInfo(lStateIdx))
                          ,fixedBytes
                          ,0u
                          ,*bufferSize
                          ,lFixedByteSize);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        lResult = E_OK;
      }
    }
    *bufferSize = lFixedByteSize;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  return lResult;
}
#endif

#if (DCM_STATE_NOTIFY_SVC_PRO_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateNotifyServiceProcessors()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_StateNotifyServiceProcessors(
  void
  )
{
  /* now process session/security state updates:
    - on any session transition: security is always changed
    - on security state transition: security is always changed
  */
# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc27OnStateChanged();
# endif
# if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
  Dcm_Svc2COnStateChanged();
# endif
# if (DCM_DIDMGR_PERIODICDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
  Dcm_Svc2AOnStateChanged();
# endif
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
  Dcm_Svc2FOnStateChanged();
# endif
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_ResetToDefaultSession()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Just register an externally triggered request for changing back to default diagnostic session */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF, DCM_THREAD_ID_BASIC);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_RESET2DEFAULTSESSION, lErrorId);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_GetSesCtrlType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlType(
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(SesCtrlType == NULL_PTR))                                                                                             /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Return the diagnostic session ID of the currently (still) active (committed not pending) diagnostic session state */
    *SesCtrlType = Dcm_CfgWrapStateSessionInfo(Dcm_UtiGetBitPosition(Dcm_StateSingletonContext.Preconditions.Session))->Value;                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETSESCTRLTYPE, lErrorId);

  return DCM_E_OK;
}

#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetSecurityLevel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevel(
  P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_APPL_DATA) SecLevel
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(SecLevel == NULL_PTR))                                                                                                /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_DebugDetectError(Dcm_StateIsSecurityBypassPending()))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------ */
    uint8 stateIdx = Dcm_UtiGetBitPosition(Dcm_StateSingletonContext.Preconditions.Security);
    /* Return the security access level ID of the currently active security access level state */
    *SecLevel = Dcm_StateGetSecurityValue(stateIdx);                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETSECLEVEL, lErrorId);

  return DCM_E_OK;
}
#endif

#if (DCM_STATE_SECURITY_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSecurityLevel()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSecurityLevel(
  Dcm_SecLevelType SecLevel
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;
  sint8_least    lStateIdx;

  /* Convert security access level to internal zero based security access state handle */
  lStateIdx = Dcm_StateFindStateIndexFromSecLevel(SecLevel);/* Calling this API is safe since it does not depend on any internal DCM state (no initialization is needed!) */

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(lStateIdx < 0))                                                                                                      /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_StateSetSecurity((Dcm_StateIndexOptType)lStateIdx);

    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETSECLEVEL, lErrorId);

  return lResult;
}
#endif

#if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetSecurityLevelFixedBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevelFixedBytes(
  Dcm_SecLevelType SecLevel,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) FixedBytes,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) BufferSize
  )
{
  Std_ReturnType  lResult  = DCM_E_NOT_OK;
  uint8           lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (FixedBytes == NULL_PTR) || (BufferSize == NULL_PTR) ))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = Dcm_StateGetSecurityLevelFixedBytes(SecLevel, FixedBytes, BufferSize);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETSECLEVELFIXEDBYTES, lErrorId);

  return lResult;
}
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSecurityBypass()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSecurityBypass(
  boolean SecBypass
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  /*  Check that the module is initialized. */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  /* Check for valid parameter. */
  else if(Dcm_DebugDetectError((SecBypass != FALSE) && (SecBypass != TRUE)))                                                                         /* PRQA S 2992, 2996 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM;                                                                                                                          /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_StateSetPendingSecurityBypass(SecBypass);

    /* Just register security bypass mode */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS, DCM_THREAD_ID_BASIC);
    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETSECURITYBYPASS, lErrorId);

  return lResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_StateInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_StateInit(
  void
  )
{
  Dcm_StateSingletonContext.Preconditions.Session  = DCM_STATE_VALUE_SESSION_INIT;
#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  Dcm_StateSingletonContext.Preconditions.Security = DCM_STATE_VALUE_SECURITY_INIT;
#endif
  Dcm_StateSyncPendingSession();

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  Dcm_StateSingletonContext.PendingSecurityBypass  = FALSE;
#endif
}

/**********************************************************************************************************************
 *  Dcm_StateGetSession()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetSession(
  void
  )
{
  return Dcm_StateSingletonContext.Preconditions.Session;
}

/**********************************************************************************************************************
 *  Dcm_StateSetSession()
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
 */
FUNC(void, DCM_CODE) Dcm_StateSetSession(
  Dcm_StateIndexOptType stateRef
  )
{
  Dcm_CfgStateGroupMemType oldSession;

  oldSession = Dcm_StateSingletonContext.Preconditions.Session;

  /* Commit the new diagnostic session */
  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  Dcm_StateSingletonContext.Preconditions.Session = Dcm_UtiGetBitFromIndex(Dcm_CfgStateGroupMemType, stateRef);
  Dcm_StateSyncPendingSession();
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_SplitTaskLeaveCS();

  /* Notify application for a diagnostic session transition (also for self-transitions e.g. Default->Default): */
  (void)Rte_Switch_DcmDiagnosticSessionControl_DcmDiagnosticSessionControl(Dcm_CfgWrapStateSessionInfo(stateRef)->Mode);

  Dcm_StateExecOnStateChangeFunc(Dcm_CfgStateSessionNotificationInfo, Dcm_StateGetSessionValue(Dcm_UtiGetBitPosition(oldSession)), Dcm_StateGetSessionValue(stateRef)); /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  /* If the new session is the default one, reset all related internally handled diagnostic jobs (order irrelevant): */
  if(Dcm_StateIsDefaultSessionActive())
  {
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
    if(Dcm_NetGetKeepAlive() == FALSE)
#endif
    {
      Dcm_NetUnRegisterAllComMActivity();
    }

#if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc2ASchdStopAll();
#endif
#if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
    Dcm_Svc2FReturnControlToEcu();
#endif
#if(DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
    Dcm_AuthMgrDetectIdleConnection(oldSession);
#endif
  }

  /* On any diagnostic session transition (order irrelevant): */
#if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
  /* Enable DTCsetting in DEM (if ControlDTCsetting service is not supported in current session) */
  Dcm_Svc85OnChangeSession();
#endif
#if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
  /* Enable communication abilities of application and NM messages (if CommunicationControl service is not supported in current session) */
  Dcm_Svc28OnChangeSession();
#endif
#if (DCM_SVC_86_RST_ON_DSC_ENABLED == STD_ON)
  /* Notify the diagnostic service 0x86 processor (if diagnostic service 0x86 is handled by DCM) */
  Dcm_ExtSvc86OnChangeSession(oldSession, Dcm_StateSingletonContext.Preconditions.Session);
#endif

#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_StateIsSecurityBypassPending() == FALSE)
# endif
  {
    /* Reset the security access level back to "locked" (if diagnostic service 0x27 is supported by the ECU configuration) */
    Dcm_StateSetSecurity(0);
  }
#else
# if (DCM_STATE_NOTIFY_SVC_PRO_ENABLED == STD_ON)
  /* Notify all service processors that are interested in a state change */
  Dcm_StateNotifyServiceProcessors();
# endif
#endif
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_StateGetPendingSession()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetPendingSession(
  void
  )
{
  return Dcm_StateSingletonContext.PendingSession;
}

/**********************************************************************************************************************
 *  Dcm_StateSetPendingSession()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_StateSetPendingSession(
  Dcm_CfgStateGroupMemType session
  )
{
  Dcm_StateSingletonContext.PendingSession = session;
}

#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateGetSecurity()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgStateGroupMemType, DCM_CODE) Dcm_StateGetSecurity(
  void
  )
{
  return Dcm_StateSingletonContext.Preconditions.Security;
}
#endif

#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateSetSecurity()
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
FUNC(void, DCM_CODE) Dcm_StateSetSecurity(
  Dcm_StateIndexOptType stateRef
  )
{
  Dcm_CfgStateGroupMemType oldLevel;

  oldLevel = Dcm_StateSingletonContext.Preconditions.Security;

  /* #10 Commit the new security access level */
  Dcm_StateSingletonContext.Preconditions.Security = Dcm_UtiGetBitFromIndex(Dcm_CfgStateGroupMemType, stateRef);

  /* #20 Notify application for a security access level transition (only for non-self-transitions e.g. Locked->Level1): */
  if ((oldLevel & Dcm_StateSingletonContext.Preconditions.Security) == 0u) /* consider only non-self transitions (i.e. Locked->Locked) */
  {
    Dcm_SecLevelType               secLev;
# if (DCM_MODE_SECURITY_ACCESS_ENABLED == STD_ON)
    Rte_ModeType_DcmSecurityAccess newMode;

    newMode = (Rte_ModeType_DcmSecurityAccess)((stateRef == 0u) ? RTE_MODE_DcmSecurityAccess_LockedLevel
                                                                : (Dcm_CfgWrapStateSecurityInfo(stateRef - 1u))->Mode);

    /* Notify application */
    (void)Rte_Switch_DcmSecurityAccess_DcmSecurityAccess(newMode);
# endif

    if (oldLevel == (Dcm_UtiMaxValueOfUintType(Dcm_CfgStateGroupMemType) - 1u))  /* Check for transition back from bypass mode */
    {
      secLev = 0;
    }
    else
    {
      secLev = Dcm_UtiGetBitPosition(oldLevel); /* Local variable needed to avoid warnings for side-effects on macro Dcm_StateGetSecurityValue() */
    }
    Dcm_StateExecOnStateChangeFunc(Dcm_CfgStateSecurityNotificationInfo, Dcm_StateGetSecurityValue(secLev), Dcm_StateGetSecurityValue(stateRef));    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }
# if (DCM_STATE_NOTIFY_SVC_PRO_ENABLED == STD_ON)
  /* Notify all service processors that are interested in a state change */
  Dcm_StateNotifyServiceProcessors();
# endif
}
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  Dcm_StateIsSecurityBypassPending()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_StateIsSecurityBypassPending(
  void
  )
{
  return Dcm_StateSingletonContext.PendingSecurityBypass;
}
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateSetSecurityBypass()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_StateSetSecurityBypass(
  void
  )
{
  Dcm_StateSingletonContext.Preconditions.Security = DCM_STATE_VALUE_SECURITY_ALL_BUT_INIT;
}
#endif

#if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateCheckDID()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheckDID(
  Dcm_NetConnRefMemType connHdl,
  uint16 did,
  Dcm_DidOpType didOpMask,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  /* Get the precondition package for the related diagnostic entity */
  CONSTP2CONST(Dcm_CfgStateDiagStateInfoType, AUTOMATIC, DCM_CONST) pStatePreconditions = &Dcm_StateGetPreconditionStates(stateRef);

  /* First verify the diagnostic session pre-condition (UDS verification order) */
  if(!Dcm_StateIsSupportedInSession(*pStatePreconditions))                                                                                           /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
  {
    /* If not met, return appropriate to UDS NRC for the evaluated diagnostic entity (i.e. NRC 0x7F for services, 0x7E for sub-functions etc.) */
    *ErrorCode = Dcm_StateSessionNrcs[DCM_DIAG_CHK_LVL_PARAMETER];                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  /* Otherwise, if supported, verify the authentication requirement (UDS verification order) */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckDid(connHdl, did, didOpMask, stateRef) == DCM_E_NOT_OK)
  {
    /* If not met, do return NRC 0x34 */
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  /* Otherwise, if supported, verify the security access level pre-condition (UDS verification order) */
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(!Dcm_StateIsSupportedInSecurityLevel(*pStatePreconditions))
  {
    /* If not met, do return NRC 0x33 (UDS) */
    *ErrorCode = DCM_E_SECURITYACCESSDENIED;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  {
# if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
    /* Otherwise, if supported, get the the mode-related pre-condition expression(s) result */
    lStdReturn = Dcm_ModeCheck(Dcm_CfgStatePreconditions[stateRef].ModeRuleRef, ErrorCode);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
# else
    /* Otherwise, everything fine - return positive response code */
    lStdReturn = DCM_E_OK;
# endif
  }

  DCM_IGNORE_UNREF_PARAM(did);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(didOpMask);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(connHdl);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return lStdReturn;
}
#endif

#if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_StateCheckRID()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheckRID(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint16 rid,
  uint8 ridSubfunc,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  /* Get the precondition package for the related diagnostic entity */
  CONSTP2CONST(Dcm_CfgStateDiagStateInfoType, AUTOMATIC, DCM_CONST) pStatePreconditions = &Dcm_StateGetPreconditionStates(stateRef);

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* First verify the diagnostic session pre-condition (UDS verification order) */
  if(!Dcm_StateIsSupportedInSession(*pStatePreconditions))                                                                                           /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
  {
    /* If not met, return appropriate to UDS NRC for the evaluated diagnostic entity (i.e. NRC 0x7F for services, 0x7E for sub-functions etc.) */
    *ErrorCode = Dcm_StateSessionNrcs[DCM_DIAG_CHK_LVL_PARAMETER];                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  /* Otherwise, if supported, verify the authentication requirement (UDS verification order) */
# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckRid(pContext, rid, ridSubfunc, stateRef) == DCM_E_NOT_OK)                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    /* If not met, do return NRC 0x34 */
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  /* Otherwise, if supported, verify the security access level pre-condition (UDS verification order) */
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(!Dcm_StateIsSupportedInSecurityLevel(*pStatePreconditions))
  {
    /* If not met, do return NRC 0x33 (UDS) */
    *ErrorCode = DCM_E_SECURITYACCESSDENIED;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  {
# if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
    /* Otherwise, if supported, get the the mode-related pre-condition expression(s) result */
    lStdReturn = Dcm_ModeCheck(Dcm_CfgStatePreconditions[stateRef].ModeRuleRef, ErrorCode);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
# else
    /* Otherwise, everything fine - return positive response code */
    lStdReturn = DCM_E_OK;
# endif
  }

  DCM_IGNORE_UNREF_PARAM(rid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ridSubfunc);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return lStdReturn;
}
#endif

/**********************************************************************************************************************
 *  Dcm_StateCheck()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_StateCheck(
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  /* Get the precondition package for the related diagnostic entity */
  CONSTP2CONST(Dcm_CfgStateDiagStateInfoType, AUTOMATIC, DCM_CONST) pStatePreconditions = &Dcm_StateGetPreconditionStates(stateRef);

  /* First verify the diagnostic session pre-condition (UDS verification order) */
  if(!Dcm_StateIsSupportedInSession(*pStatePreconditions))                                                                                           /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
  {
    /* If not met, return appropriate to UDS NRC for the evaluated diagnostic entity (i.e. NRC 0x7F for services, 0x7E for sub-functions etc.) */
    *ErrorCode = Dcm_StateSessionNrcs[chkLevel];                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  /* Otherwise, if supported, verify the security access level pre-condition (UDS verification order) */
#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(!Dcm_StateIsSupportedInSecurityLevel(*pStatePreconditions))
  {
    /* If not met, do return NRC 0x33 (UDS) */
    *ErrorCode = DCM_E_SECURITYACCESSDENIED;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
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
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_State.c
 *********************************************************************************************************************/
