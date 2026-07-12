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
/**        \file  Dcm_RsrcMgr.c
 *         \unit  RsrcMgr
 *        \brief  Contains the implementation of Resource Manager unit.
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
#define DCM_RSRCMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_RsrcMgr.h"
#include "Dcm_Debug.h"

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
/*! Lock information of the resource manager */
struct DCM_RSRCMGRDIDLOCKTYPE_TAG
{
  uint16                       Did;    /*!< Identifier value of a resource */
  Dcm_RsrcMgrDidLockOwnerType  Owner;  /*!< Lock owner */
};
typedef struct DCM_RSRCMGRDIDLOCKTYPE_TAG Dcm_RsrcMgrDidLockType;

/*! Lock information of the resource manager */
struct DCM_RSRCMGRRIDLOCKTYPE_TAG
{
  uint16                       Rid;    /*!< Identifier value of a resource */
  Dcm_RsrcMgrRidLockOwnerType  Owner;  /*!< Lock owner */
};
typedef struct DCM_RSRCMGRRIDLOCKTYPE_TAG Dcm_RsrcMgrRidLockType;

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/*! Lock information of the resource manager */
struct DCM_RSRCMGRSERVICELOCKTYPE_TAG
{
  uint8                            Service;  /*!< Identifier value of a resource */
  Dcm_RsrcMgrServiceLockOwnerType  Owner;    /*!< Lock owner */
};
typedef struct DCM_RSRCMGRSERVICELOCKTYPE_TAG Dcm_RsrcMgrServiceLockType;
#endif

/*! Lock information of the resource manager */
struct DCM_RSRCMGRMEMLOCKTYPE_TAG
{
  Dcm_RsrcMgrMemLockOwnerType  Owner;  /*!< Lock owner */
};
typedef struct DCM_RSRCMGRMEMLOCKTYPE_TAG Dcm_RsrcMgrMemLockType;

/*! Context information of the resource manager */
struct DCM_RSRCMGRSINGLETONCONTEXTTYPE_TAG
{
  Dcm_RsrcMgrDidLockType      DidLockList[DCM_RSRCMGR_DID_LOCK_LIST_SIZE];          /*!< List of Dids which can be handled in parallel */
  Dcm_RsrcMgrRidLockType      RidLockList[DCM_RSRCMGR_RID_LOCK_LIST_SIZE];          /*!< List of Rids which can be handled in parallel */
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  Dcm_RsrcMgrServiceLockType  ServiceLockList[DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE];  /*!< List of Services which can be handled in parallel */
#endif
  Dcm_RsrcMgrMemLockType      MemLock;                                              /*!< Lock for the memory resource */
};
typedef struct DCM_RSRCMGRSINGLETONCONTEXTTYPE_TAG Dcm_RsrcMgrSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsMemLockFree()
 *********************************************************************************************************************/
/*! \brief          Checks whether the memory resource can be locked or not.
 *  \details        -
 *  \return         TRUE     Memory resource is free
 *  \return         FALSE    Memory resource is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsMemLockFree(
  void
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsDidFree()
 *********************************************************************************************************************/
/*! \brief          Checks whether a Did can be locked or not.
 *  \details        -
 *  \param[in]      did      Did to be locked
 *  \return         TRUE     Did is free
 *  \return         FALSE    Did is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsDidFree(
  uint16 did
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsRidFree()
 *********************************************************************************************************************/
/*! \brief          Checks whether a Rid can be locked or not.
 *  \details        -
 *  \param[in]      rid      Rid to be locked
 *  \return         TRUE     Rid is free
 *  \return         FALSE    Rid is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsRidFree(
  uint16 rid
  );

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsServiceFree()
 *********************************************************************************************************************/
/*! \brief          Checks whether a Service can be locked or not.
 *  \details        -
 *  \param[in]      service  Service to be locked
 *  \return         TRUE     Service is free
 *  \return         FALSE    Service is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsServiceFree(
  uint8 service
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of RsrcMgr unit */
DCM_LOCAL VAR(Dcm_RsrcMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_RsrcMgrSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsMemLockFree()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsMemLockFree(
  void
  )
{
  return Dcm_RsrcMgrHasMemLock(DCM_RSRCMGR_MEM_ACCESS_NONE);
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsDidFree()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsDidFree(
  uint16 did
  )
{
  boolean      lResult = TRUE;
  uint8_least  lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_DID_LOCK_LIST_SIZE; ++lIter)
  {
    if ( (Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner != DCM_RSRCMGR_DIDLOCK_OWNER_NONE)
       &&(Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Did == did))
    {
      lResult = FALSE;
      break;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsRidFree()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsRidFree(
  uint16 rid
  )
{
  boolean      lResult = TRUE;
  uint8_least  lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_RID_LOCK_LIST_SIZE; ++lIter)
  {
    if ( (Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner != DCM_RSRCMGR_RIDLOCK_OWNER_NONE)
       &&(Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Rid == rid))
    {
      lResult = FALSE;
      break;
    }
  }

  return lResult;
}

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrIsServiceFree()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_RsrcMgrIsServiceFree(
  uint8 service
  )
{
  boolean      lResult = TRUE;
  uint8_least  lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE; ++lIter)
  {
    if ( (Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner != DCM_RSRCMGR_SERVICELOCK_OWNER_NONE)
       &&(Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Service == service))
    {
      lResult = FALSE;
      break;
    }
  }

  return lResult;
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
 *  Dcm_RsrcMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RsrcMgrInit(
  void
  )
{
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_DID_LOCK_LIST_SIZE; ++lIter)
  {
    Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Did = 0u;                                                                                         /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */
    Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner = DCM_RSRCMGR_DIDLOCK_OWNER_NONE;                                                           /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */
  }

  for(lIter = 0; lIter < DCM_RSRCMGR_RID_LOCK_LIST_SIZE; ++lIter)
  {
    Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Rid = 0u;                                                                                         /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */
    Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner = DCM_RSRCMGR_RIDLOCK_OWNER_NONE;                                                           /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */
  }

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  for(lIter = 0; lIter < DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE; ++lIter)
  {
    Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Service = 0u;                                                                                 /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */
    Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner = DCM_RSRCMGR_SERVICELOCK_OWNER_NONE;                                                   /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */
  }
#endif

  Dcm_RsrcMgrSingletonContext.MemLock.Owner = DCM_RSRCMGR_MEM_ACCESS_NONE;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasMemLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasMemLock(
  Dcm_RsrcMgrMemLockOwnerType owner
  )
{
  boolean lResult = FALSE;

  if (Dcm_RsrcMgrSingletonContext.MemLock.Owner == owner)
  {
    lResult = TRUE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetMemLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetMemLock(
  Dcm_OpStatusType opStatus,
  Dcm_RsrcMgrMemLockOwnerType owner
  )
{
  Std_ReturnType lStdResult = DCM_E_PENDING;

  if(opStatus == DCM_CANCEL)
  {
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    if(Dcm_RsrcMgrIsMemLockFree() == TRUE)
    {
      Dcm_RsrcMgrSingletonContext.MemLock.Owner = owner;
      lStdResult = DCM_E_OK;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseMemLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseMemLock(
  void
  )
{
  Dcm_RsrcMgrSingletonContext.MemLock.Owner = DCM_RSRCMGR_MEM_ACCESS_NONE;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasDidLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasDidLock(
  Dcm_RsrcMgrDidLockOwnerType owner
  )
{
  boolean     lResult = FALSE;
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_DID_LOCK_LIST_SIZE; ++lIter)
  {
    if(Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner == owner)
    {
      lResult = TRUE;
      break;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetDidLock()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetDidLock(
  Dcm_OpStatusType opStatus,
  uint16 did,
  Dcm_RsrcMgrDidLockOwnerType owner
  )
{
  Std_ReturnType lStdResult = DCM_E_PENDING;
  uint8_least    lIter;

  if(opStatus == DCM_CANCEL)
  {
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    if (Dcm_RsrcMgrHasDidLock(owner) == FALSE)
    {
      if (Dcm_RsrcMgrIsDidFree(did) == TRUE)
      {
        for (lIter = 0; lIter < DCM_RSRCMGR_DID_LOCK_LIST_SIZE; ++lIter)                                                                             /* COV_DCM_RTM_UNREACHABLE TX */
        {
          if (Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner == DCM_RSRCMGR_DIDLOCK_OWNER_NONE)
          {
            Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Did = did;                                                                                /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */
            Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner = owner;                                                                            /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */

            lStdResult = DCM_E_OK;
            break;
          }
        }

        /* Assert that DidLockList is large enough */
        Dcm_DebugAssert((lStdResult == DCM_E_OK), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                            /* COV_DCM_RTM_DEV_DEBUG XF */
      }
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseDidLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseDidLock(
  Dcm_RsrcMgrDidLockOwnerType owner
  )
{
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_DID_LOCK_LIST_SIZE; ++lIter)
  {
    if(Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner == owner)
    {
      Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Did = 0u;                                                                                       /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */
      Dcm_RsrcMgrSingletonContext.DidLockList[lIter].Owner = DCM_RSRCMGR_DIDLOCK_OWNER_NONE;                                                         /* SBSW_DCM_POINTER_WRITE_DIDLOCKLIST */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetRidLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetRidLock(
  Dcm_OpStatusType opStatus,
  uint16 rid,
  Dcm_RsrcMgrRidLockOwnerType owner
  )
{
  Std_ReturnType lStdResult = DCM_E_PENDING;
  uint8_least    lIter;

  if(opStatus == DCM_CANCEL)
  {
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    if (Dcm_RsrcMgrIsRidFree(rid) == TRUE)
    {
      for (lIter = 0; lIter < DCM_RSRCMGR_RID_LOCK_LIST_SIZE; ++lIter)                                                                               /* COV_DCM_RTM_UNREACHABLE TX */
      {
        if (Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner == DCM_RSRCMGR_RIDLOCK_OWNER_NONE)
        {
          Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Rid = rid;                                                                                  /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */
          Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner = owner;                                                                              /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */

          lStdResult = DCM_E_OK;
          break;
        }
      }
      /* Assert that RidLockList is large enough */
      Dcm_DebugAssert((lStdResult == DCM_E_OK), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                              /* COV_DCM_RTM_DEV_DEBUG XF */
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseRidLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseRidLock(
  Dcm_RsrcMgrRidLockOwnerType owner
  )
{
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_RID_LOCK_LIST_SIZE; ++lIter)
  {
    if(Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner == owner)
    {
      Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Rid = 0u;                                                                                       /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */
      Dcm_RsrcMgrSingletonContext.RidLockList[lIter].Owner = DCM_RSRCMGR_RIDLOCK_OWNER_NONE;                                                         /* SBSW_DCM_POINTER_WRITE_RIDLOCKLIST */
      break;
    }
  }
}

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasServiceLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasServiceLock(
  Dcm_RsrcMgrServiceLockOwnerType owner
  )
{
  boolean lResult = FALSE;
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE; ++lIter)
  {
    if(Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner == owner)
    {
      lResult = TRUE;
      break;
    }
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetServiceLock()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetServiceLock(
  Dcm_OpStatusType opStatus,
  uint8 service,
  Dcm_RsrcMgrServiceLockOwnerType owner
  )
{
  Std_ReturnType lStdResult = DCM_E_PENDING;
  uint8_least    lIter;

  if(opStatus == DCM_CANCEL)
  {
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    if (Dcm_RsrcMgrIsServiceFree(service) == TRUE)
    {
      for (lIter = 0; lIter < DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE; ++lIter)                                                                           /* COV_DCM_RTM_UNREACHABLE TX */
      {
        if (Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner == DCM_RSRCMGR_SERVICELOCK_OWNER_NONE)
        {
          Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Service = service;                                                                      /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */
          Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner = owner;                                                                          /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */

          lStdResult = DCM_E_OK;
          break;
        }
      }
      /* Assert that ServiceLockList is large enough */
      Dcm_DebugAssert((lStdResult == DCM_E_OK), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                              /* COV_DCM_RTM_DEV_DEBUG XF */
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseServiceLock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseServiceLock(
  Dcm_RsrcMgrServiceLockOwnerType owner
  )
{
  uint8_least lIter;

  for(lIter = 0; lIter < DCM_RSRCMGR_SERVICE_LOCK_LIST_SIZE; ++lIter)
  {
    if(Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner == owner)
    {
      Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Service = 0u;                                                                               /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */
      Dcm_RsrcMgrSingletonContext.ServiceLockList[lIter].Owner = DCM_RSRCMGR_SERVICELOCK_OWNER_NONE;                                                 /* SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST */
      break;
    }
  }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_RsrcMgr.c
 *********************************************************************************************************************/
