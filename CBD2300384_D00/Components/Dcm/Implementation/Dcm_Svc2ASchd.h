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
/**        \file  Dcm_Svc2ASchd.h
 *         \unit  Svc2ASchd
 *        \brief  Contains public inline function definitions of Service 0x2A Scheduler unit.
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
#if !defined(DCM_SVC2ASCHD_H)
# define DCM_SVC2ASCHD_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc2ASchdTypes.h"
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DCM_SVC2A_RATE_TYPE_SLOW                                   ((Dcm_Svc2ASchedRateMemType)0)
#  define DCM_SVC2A_RATE_TYPE_MED                                    ((Dcm_Svc2ASchedRateMemType)1)
#  define DCM_SVC2A_RATE_TYPE_FAST                                   ((Dcm_Svc2ASchedRateMemType)2)

#  define DCM_SVC2A_RATE_TYPE_MASK                                   ((Dcm_Svc2ASchedRateMemType)0x03u)

#  define DCM_SVC2A_RATE_TYPE_STOPPED                                ((Dcm_Svc2ASchedRateMemType)0x10u)
#  define DCM_SVC2A_RATE_TYPE_RESERVED                               ((Dcm_Svc2ASchedRateMemType)0x40u)

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
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopAll()
 *********************************************************************************************************************/
/*! \brief          Stops all scheduled DIDs.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopAll(
  void
  );

#  if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCancelReadByDid()
 *********************************************************************************************************************/
/*! \brief          Cancels the reading of a single scheduled item.
 *  \details        -
 *  \param[in]      did    The did
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdCancelReadByDid(
  uint16 did
  );
#  endif

#  if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2AGetConnectionOfDID()
 *********************************************************************************************************************/
/*! \brief          Returns the connection ID of scheduled DID.
 *  \details        -
 *  \param[in]      did    The DID
 *  \return         Valid connection ID (if DID is scheduled)
 *  \return         DCM_NET_INVALID_CONNHDL (if DID is not scheduled)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_Svc2AGetConnectionOfDID(
  uint16 did
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopItemByHandle()
 *********************************************************************************************************************/
/*! \brief          Stops a single scheduled item.
 *  \details        -
 *  \param[in]      schdHandle    The handle
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopItemByHandle(
  Dcm_Svc2ASchdItemHandleOptType schdHandle
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdStopItemByDid()
 *********************************************************************************************************************/
/*! \brief          Cancels the reading of a single scheduled item.
 *  \details        -
 *  \param[in]      did    The did
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdStopItemByDid(
  uint16 did
  );

/**********************************************************************************************************************
 *  Dcm_Service2ASchedulerInit()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        Initializes the periodic DID scheduler.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Service2ASchedulerInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdAddItemByDid()
 *********************************************************************************************************************/
/*! \brief          Inserts a new item to the scheduler without activating it.
 *  \details        -
 *  \param[in,out]  pDidInfoContext    The context of the DID
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdAddItemByDid(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdCommit()
 *********************************************************************************************************************/
/*! \brief          Commits already inserted items to be scheduled with a given rate.
 *  \details        -
 *  \param[in]      rate    The scheduling rate
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdCommit(
  Dcm_Svc2ASchedRateMemType rate
  );

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdDiscard()
 *********************************************************************************************************************/
/*! \brief          Discards all inserted items.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ASchdDiscard(
  void
  );

#  if (DCM_DIDMGR_PERIODICDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2AOnStateChanged()
 *********************************************************************************************************************/
/*! \brief          Stops all scheduled DIDs no more allowed by the new states.
 *  \details        Stops all scheduled DIDs that are nor more allowed after the state has been changed.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2AOnStateChanged(
  void
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_Svc2ASchdGetFreeSpace()
 *********************************************************************************************************************/
/*! \brief          Return Free space in the Scheduler.
 *  \details        -
 *  \return         Free space available
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2ASchdItemHandleMemType, DCM_CODE) Dcm_Svc2ASchdGetFreeSpace(
  void
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2ASCHD_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2ASchd.h
 *********************************************************************************************************************/
