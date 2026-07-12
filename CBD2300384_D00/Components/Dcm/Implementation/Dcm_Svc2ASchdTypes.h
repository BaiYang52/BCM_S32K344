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
/**        \file  Dcm_Svc2ASchdTypes.h
 *         \unit  Svc2ASchd
 *        \brief  Contains native types and forward declarations of Service 0x2A Scheduler unit.
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
#if !defined(DCM_SVC2ASCHDTYPES_H)
# define DCM_SVC2ASCHDTYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Types.h"
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_DidMgr.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
typedef uint8       Dcm_Svc2ASchedRateMemType;
#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
typedef uint8_least Dcm_Svc2ASchedRateOptType;
#  endif
typedef uint8       Dcm_Svc2ASchdItemHandleMemType;
typedef uint8_least Dcm_Svc2ASchdItemHandleOptType;

/*! Service 0x2A scheduler entry data */
struct DCM_SVC2ASCHDENTRYTYPE_TAG
{
           Dcm_DidMgrDidInfoContextType DidContext; /*!< Information about the scheduled DID */
  volatile Dcm_TmrTimerCntrMemType      Timer;      /*!< Timer for the periodic transmission */
           Dcm_Svc2ASchedRateMemType    Rate;       /*!< Requested transmission rate */
};
typedef struct DCM_SVC2ASCHDENTRYTYPE_TAG Dcm_Svc2ASchdEntryType;

typedef P2VAR(Dcm_Svc2ASchdEntryType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2ASchdEntryPtrType;
typedef P2CONST(Dcm_Svc2ASchdEntryType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2ASchdEntryConstPtrType;

/*! Service 0x2A scheduler data */
struct DCM_SVC2ASCHEDULERTYPE_TAG
{
  Dcm_Svc2ASchdEntryType          Table[DCM_SVC_2A_SCHEDULER_SIZE];        /*!< Table of scheduled DIDs */
  Dcm_DidMgrDidOpTypeContextType  DidOpTypeContext;                        /*!< Information about operations on current DID */
  Dcm_CfgNetBufferSizeMemType     WriteIndex;                              /*!< Number of bytes that were already read */
  Dcm_Svc2ASchdItemHandleMemType  NumActiveEntries;                        /*!< Number of active entries in scheduler table */
  Dcm_Svc2ASchdItemHandleMemType  CurrentEntryIndex;                       /*!< Scheduler table entry that is currently processed */
  Dcm_CfgNetPTxObjHandleMemType   MsgTxHandle;                             /*!< Index of current tx object */
  Dcm_OpStatusType                OpStatus;                                /*!< Current operation status */
};
typedef struct DCM_SVC2ASCHEDULERTYPE_TAG Dcm_Svc2ASchedulerType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2ASCHDTYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2ASchdTypes.h
 *********************************************************************************************************************/
