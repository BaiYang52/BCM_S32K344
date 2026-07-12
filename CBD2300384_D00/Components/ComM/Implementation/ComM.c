/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!         \file  ComM.c
 *         \brief  Communication Manager ASR4
 *
 *       \details  Implementation of the Autosar Communication Manager
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CCL_ASR_COMM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
/* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ComM_Private_Cfg.h"

#if (COMM_DCM_INDICATION == STD_ON)
# include "Dcm_Cbk.h"
# include "ComM_Dcm.h"
#endif

#if (COMM_NVM_SUPPORT == STD_ON)
# include "NvM.h"
#endif

#include "BswM_ComM.h"
#include "SchM_ComM.h"

#include "ComM_EcuMBswM.h"
#include "ComM_BusSM.h"
#include "ComM_Nm.h"

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
# include "Nm.h"
#endif

#if (COMM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

#if (COMM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
# include "CanSM.h"
# include "CanSM_ComM.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of ComM.h */
#if (  (COMM_SW_MAJOR_VERSION != (0x16u)) \
    || (COMM_SW_MINOR_VERSION != (0x01u)) \
    || (COMM_SW_PATCH_VERSION != (0x00u)) )
# error "Version numbers of ComM.h are inconsistent!"
#endif

/* Check the version of ComM_BusSM.h */
#if (  (COMM_BUSSM_MAJOR_VERSION != (0x16u)) \
    || (COMM_BUSSM_MINOR_VERSION != (0x01u)) \
    || (COMM_BUSSM_PATCH_VERSION != (0x00u)) )
# error "Version numbers of ComM_BusSM.h are inconsistent!"
#endif

/* Check the version of ComM_Dcm.h */
#if (COMM_DCM_INDICATION == STD_ON)
# if (  (COMM_DCM_MAJOR_VERSION != (0x16u)) \
     || (COMM_DCM_MINOR_VERSION != (0x01u)) \
     || (COMM_DCM_PATCH_VERSION != (0x00u)) )
#  error "Version numbers of ComM_Dcm.h are inconsistent!"
# endif
#endif

/* Check the version of ComM_EcuMBswM.h */
#if (  (COMM_ECUMBSWM_MAJOR_VERSION != (0x16u)) \
    || (COMM_ECUMBSWM_MINOR_VERSION != (0x01u)) \
    || (COMM_ECUMBSWM_PATCH_VERSION != (0x00u)) )
# error "Version numbers of ComM_EcuMBswM.h are inconsistent!"
#endif

/* Check the version of ComM_Nm.h */
#if (  (COMM_NM_MAJOR_VERSION != (0x16u)) \
    || (COMM_NM_MINOR_VERSION != (0x01u)) \
    || (COMM_NM_PATCH_VERSION != (0x00u)) )
# error "Version numbers of ComM_Nm.h are inconsistent!"
#endif

/* Check the version of ComM_Types.h */
#if (  (COMM_TYPES_MAJOR_VERSION != (0x16u)) \
    || (COMM_TYPES_MINOR_VERSION != (0x01u)) \
    || (COMM_TYPES_PATCH_VERSION != (0x00u)) )
# error "Version numbers of ComM_Types.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  COMPATIBILITY / DEFAULTS
 *********************************************************************************************************************/

#if (  (COMM_CONFIGURATION_VARIANT != COMM_CONFIGURATION_VARIANT_PRECOMPILE) \
    && (COMM_CONFIGURATION_VARIANT != COMM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) )
# error "ComM currently supports configuration variants PreCompile and PostBuild Loadable only"
#endif

#if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
# if ((COMM_MINFULLCOMTIMEOFCHANNEL == STD_OFF) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_OFF))
#  error "Generated data is invalid: COMM_MINFULLCOMTIMEOFCHANNEL and COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL must be STD_ON"
# endif
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Bit mask to set the Prevent Wake-up status */
#define COMM_WAKEUP_INHIBITION_MASK                (uint8)0x01u
/*! Bit mask to clear the Prevent Wake-up status */
#define COMM_WAKEUP_INHIBITION_CLEAR_MASK          (uint8)0xfeu
/*! Bit mask to set the Mode Limitation status */
#define COMM_MODE_LIMITATION_MASK                  (uint8)0x02u
/*! Bit mask to clear the Mode Limitation status */
#define COMM_MODE_LIMITATION_CLEAR_MASK            (uint8)0xfdu

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/*! Bit mask to set PNC Routing Limitation status of a channel to activated (limited) */
#  define COMM_PNCROUTING_LIMITED                  (uint8)0x00
/*! Bit mask to set PNC Routing Limitation status of a channel to de-activated (partly) because Nm is in NM_REPEAT_MESSAGE state */
#  define COMM_PNCROUTING_NM_REPEAT_MESSAGE        (uint8)0x01
/*! Bit mask to set PNC Routing Limitation status of a channel to de-activated (default) */
#  define COMM_PNCROUTING_NOT_LIMITED              (uint8)0x02

/*! Bit mask to clear PNC Routing Limitation status de-activated (partly) */
#  define COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR  (uint8)0xfe
/*! Bit mask to clear PNC Routing Limitation status de-activated */
#  define COMM_PNCROUTING_NOT_LIMITED_CLEAR        (uint8)0xfd
# endif

/*! Send PNC signals on channels with Gateway Type ACTIVE */
# define COMM_SEND_ACTIVE_SIG_ONLY                  (uint8)(0u)
/*! Send PNC signals on channels with Gateway Type PASSIVE */
# define COMM_SEND_PASSIVE_SIG_ONLY                 (uint8)(1u)
/*! Send PNC signals on all channels */
# define COMM_SEND_ALL_SIG                          (uint8)(2u)
#endif

/*! Used to initialize and check the user request bit field */
#define COMM_NO_REQUESTS                           (uint8)0x00u

/*! Default value of the state change status */
#define COMM_NO_STATE_CHANGE                       (uint8)0x10u

#if (COMM_MULTIPARTITION == STD_ON)
# define COMM_PARTITION_TYPE_ONLY                    CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx
# define COMM_PARTITION_TYPE_LAST                    , CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx
# define COMM_PARTITION_PARA_ONLY                    (ComM_PartitionConfigIdxOfChannelType)(partitionIdx)
# define COMM_PARTITION_PARA_LAST                    , (ComM_PartitionConfigIdxOfChannelType)(partitionIdx)
# define COMM_MASTER_PARTITION_PARA_LAST             , COMM_MASTER_PARTITION_IDX
# define PARTITION_IDX                               (partitionIdx)
#else
# define COMM_PARTITION_TYPE_ONLY                    void
# define COMM_PARTITION_TYPE_LAST
# define COMM_PARTITION_PARA_ONLY
# define COMM_PARTITION_PARA_LAST
# define COMM_MASTER_PARTITION_PARA_LAST
# define PARTITION_IDX                               COMM_MASTER_PARTITION_IDX
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* Initialization APIs */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# define ComM_IsPreInitialized()                    ComM_PreInitialized
# define ComM_SetPreInitialized(value)              ComM_PreInitialized = (value)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Compiler.h for AUTOSAR 4.0.3 won't have the 'static' keyword  */
#if !defined (COMM_LOCAL) /* COV_MSR_COMPATIBILITY */
# define COMM_LOCAL                                static
#endif

#if !defined (COMM_LOCAL_INLINE) /* COV_MSR_COMPATIBILITY */
# define COMM_LOCAL_INLINE                         LOCAL_INLINE
#endif

/* type definition of the ComM transition function call table */
typedef P2FUNC(void, COMM_CODE, ComM_TransitionFunctionType) (NetworkHandleType Channel COMM_PARTITION_TYPE_LAST);

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES OF STATE TRANSITION FUNCTIONS (TF)
 *********************************************************************************************************************/

#define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
  ComM_TF_NoCom_NetReq()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from No Communication to Network Requested for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-47933, SPEC-5081, SPEC-47933, SPEC-5456, SPEC-5414
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_NoCom_NetReq( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_NoCom_FullReadySleep()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from No Communication to Ready Sleep for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5081, SPEC-5361, SPEC-5487, SPEC-5488
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_NoCom_FullReadySleep( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_Full_SiCom()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from Full to Silent Communication for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5097
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_Full_SiCom( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_ReadyS_NetReq()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from Ready Sleep to Network Requested for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5478
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_ReadyS_NetReq( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_SiCom_NoCom()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from Full or Silent to No Communication for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      CREQ-116030
 *  \trace      SPEC-5083, SPEC-5370
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_SiCom_NoCom( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_NetReq_ReadyS()
**********************************************************************************************************************/
/*! \brief      Performs the state transition from Network Requested to Ready Sleep for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5477, SPEC-5086
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_NetReq_ReadyS( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_No_Action()
**********************************************************************************************************************/
/*! \brief      Special state transition function for transitions that do not require any actions, needed as a placeholder
 *              in the state transition table.
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_No_Action( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_TF_No_Transition()
**********************************************************************************************************************/
/*! \brief      Special state transition function for unsupported transitions to report development errors
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_No_Transition( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST );

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
  ROM DATA
**********************************************************************************************************************/
#define COMM_START_SEC_CONST_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/*! ComM transition table [requested][active], the element COMM_MAX_NUMBER_OF_STATES means that no transition is defined for the active/requested combination. */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
COMM_LOCAL CONST(ComM_StateType, COMM_CONST) ComM_TransitionTable[COMM_MAX_NUMBER_OF_STATES][COMM_MAX_NUMBER_OF_STATES] =    /* [row] [column] */
{/* COMM_NO_COM_NO_PENDING_REQUEST |COMM_NO_COM_REQUEST_PENDING     |COMM_FULL_COM_NETWORK_REQUESTED |COMM_FULL_COM_READY_SLEEP       |COMM_SILENT_COM                   requested mode: */
  {
    COMM_MAX_NUMBER_OF_STATES,      COMM_NO_COM_NO_PENDING_REQUEST,  COMM_FULL_COM_READY_SLEEP,       COMM_SILENT_COM,                 COMM_NO_COM_NO_PENDING_REQUEST  /* COMM_NO_COM_NO_PENDING_REQUEST */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES       /* COMM_NO_COM_REQUEST_PENDING */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_FULL_COM_NETWORK_REQUESTED, COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_NETWORK_REQUESTED, COMM_FULL_COM_NETWORK_REQUESTED /* COMM_FULL_COM_NETWORK_REQUESTED */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_FULL_COM_READY_SLEEP,       COMM_FULL_COM_READY_SLEEP,       COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_READY_SLEEP       /* COMM_FULL_COM_READY_SLEEP */
  },
  {
    COMM_MAX_NUMBER_OF_STATES,      COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_READY_SLEEP,       COMM_SILENT_COM,                 COMM_MAX_NUMBER_OF_STATES       /* COMM_SILENT_COM */
  }
};

#define COMM_STOP_SEC_CONST_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

#define COMM_START_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/*! ComM transition action table [requested][active], the element ComM_TF_No_Action means that no transition is defined for the active/requested combination. */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
COMM_LOCAL CONST(ComM_TransitionFunctionType, COMM_CONST) ComM_TransitionFctTable[COMM_MAX_NUMBER_OF_STATES][COMM_MAX_NUMBER_OF_STATES] =
{/* COMM_NO_COM_NO_PENDING_REQUEST |COMM_NO_COM_REQUEST_PENDING  |COMM_FULL_COM_NETWORK_REQUESTED |COMM_FULL_COM_READY_SLEEP |COMM_SILENT_COM                  requested mode: */
  {
    ComM_TF_No_Transition,          ComM_TF_No_Action,            ComM_TF_NetReq_ReadyS,           ComM_TF_Full_SiCom,        ComM_TF_SiCom_NoCom           /* COMM_NO_COM_NO_PENDING_REQUEST */
  },
  {
    ComM_TF_No_Action,              ComM_TF_No_Transition,        ComM_TF_No_Transition,           ComM_TF_No_Transition,     ComM_TF_No_Transition         /* COMM_NO_COM_REQUEST_PENDING */
  },
  {
    ComM_TF_No_Action,              ComM_TF_NoCom_NetReq,         ComM_TF_No_Transition,           ComM_TF_ReadyS_NetReq,     ComM_TF_NoCom_NetReq          /* COMM_FULL_COM_NETWORK_REQUESTED */
  },
  {
    ComM_TF_No_Action,              ComM_TF_NoCom_FullReadySleep, ComM_TF_NetReq_ReadyS,           ComM_TF_No_Transition,     ComM_TF_NoCom_FullReadySleep  /* COMM_FULL_COM_READY_SLEEP */
  },
  {
    ComM_TF_No_Transition,          ComM_TF_No_Transition,        ComM_TF_NetReq_ReadyS,           ComM_TF_Full_SiCom,        ComM_TF_No_Transition         /* COMM_SILENT_COM */
  }
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#if (COMM_DEV_ERROR_DETECT == STD_ON)
# define COMM_START_SEC_VAR_INIT_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"
/*! Pre-Initialization state of ComM */
COMM_LOCAL VAR( ComM_PreInitializedType, COMM_VAR_CLEARED )   ComM_PreInitialized = FALSE;
# define COMM_STOP_SEC_VAR_INIT_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"
#endif

#if (COMM_USE_INIT_POINTER == STD_ON)
# define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/*! Pointer to the current configuration */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
P2CONST(ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA)       ComM_ConfigPtr = NULL_PTR; /* UREQ00035484 */

# define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"
#endif /* (COMM_USE_INIT_POINTER == STD_ON) */

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
  ComM_CalculateHighestComMode()
 **********************************************************************************************************************/
/*! \brief      Calculates the target highest communication mode of a channel by using the 'highest win' strategy
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \param[out] internalRequest         set to TRUE if an internal request via user, Dcm or coordinated PNC exists on the channel
 *  \return     The target highest communication mode of a channel
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5379, SPEC-5255, SPEC-5462, SPEC-19856, SPEC-19846, SPEC-5413, SPEC-5454, SPEC-5475, SPEC-5457,
 *  \trace      SPEC-5426, SPEC-5111, SPEC-5138, SPEC-5080, SPEC-5085
 *********************************************************************************************************************/
COMM_LOCAL FUNC(ComM_StateType, COMM_CODE) ComM_CalculateHighestComMode( NetworkHandleType channel, boolean *internalRequest );

/**********************************************************************************************************************
  ComM_ChannelStateTransition()
 **********************************************************************************************************************/
/*! \brief      Performs a state transition on the given channel according to the target (highest requested) mode.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \param[in]  highestComModeReq       the target state of the transition, the highest requested mode
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5483, SPEC-5472, SPEC-5479, SPEC-5463, SPEC-5469, SPEC-5474, SPEC-5397
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelStateTransition( NetworkHandleType channel, ComM_StateType highestComModeReq );

#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_ProcessSynchronousWakeUp()
 **********************************************************************************************************************/
/*! \brief      Handles synchronous wakeup for all possible passive wakeup events.
 *  \details    Calculates the bus-site requested communication mode and ensures that no passive wakeup event is
 *              overwritten by any triggered shutdown event.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_ProcessSynchronousWakeUp( void );
#endif

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/**********************************************************************************************************************
  ComM_ProcessPassiveWakeUp()
 **********************************************************************************************************************/
/*! \brief      Stores the states requested by bus (externally) after a Passive Wake-up occurred
 *  \details    -
 *  \param[in]  channel                 local channel handle where the wake-up event occurred
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_ProcessPassiveWakeUp( NetworkHandleType channel COMM_PARTITION_TYPE_LAST );
#endif

#if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_IsChannelUserRequestingChannel()
 **********************************************************************************************************************/
/*! \brief      Determines if a channel user is requesting the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \return     TRUE                    a channel user requests FULL_COM or a PNC user requests FULL_COM (if PNC coordination is disabled).
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(boolean, COMM_CODE) ComM_IsChannelUserRequestingChannel( NetworkHandleType channel COMM_PARTITION_TYPE_LAST );
#endif

#if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON )
# if(COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_IsUserRequestingChannelOnSatellite()
 **********************************************************************************************************************/
/*! \brief      Determines if a user on a satellite partition is requesting the provided channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    a user on a specific partition index is requesting FULL_COM for the given channel
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingChannelOnSatellite( NetworkHandleType channel );
# endif

# if((COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON))
#  if(COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
/**********************************************************************************************************************
  ComM_IsUserRequestingPncOnSatellite()
 **********************************************************************************************************************/
/*! \brief      Determines if a user on a satellite partition is requesting the provided PNC.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \return     TRUE                    a user on a specific partition index is requesting FULL_COM for the given PNC
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingPncOnSatellite( ComM_PncIterType pncIndex );
#  endif
# endif
#endif

/**********************************************************************************************************************
  ComM_ChannelIsRequestPending()
 **********************************************************************************************************************/
/*! \brief      Determines if a pending communication request exists on the channel. The function checks all communication
 *              requests that can occur in NO_COM mode: user requests and passive wake-ups. EIRA/ERA and Dcm requests are
 *              not relevant because they can occur only if communication is on.
 *  \details    The function is used by EcuM fixed and by BswM to evaluate the channel state before a sleep/shutdown.
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    a passive wake-up occurred or a channel or PNC is requesting FULL_COM
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(boolean, COMM_CODE) ComM_ChannelIsRequestPending(NetworkHandleType channel);

#if ((COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON) || (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
/**********************************************************************************************************************
  ComM_ChannelHandleTimers()
 **********************************************************************************************************************/
/*! \brief      Handles Minimum FULL_COM mode timer and Nm Light timer of the channel
 *  \details    -
 *  \param[in]  channel                 local channel handler
 *  \param[in]  internalRequest         TRUE an internal request via user, Dcm or coordinated PNC exists on the channel
 *                                      FALSE otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-47932, SPEC-5395, SPEC-5396
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelHandleTimers( NetworkHandleType channel, boolean internalRequest );
#endif

#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
/**********************************************************************************************************************
  ComM_ChannelGetMaxStateAfterExtRamCheck()
 **********************************************************************************************************************/
/*! \brief      Provides the highest possible state according to the result of Extended RAM Check.
 *  \details    -
 *  \param[in]  channel                 local channel handler
 *  \param[in]  targetState             target communication state
 *  \return     COMM_NO_COM_NO_PENDING_REQUEST if Extended RAM Check status was CANSM_DISABLE_COMMUNICATION
 *  \return     COMM_NO_COM_REQUEST_PENDING if Extended RAM Check status was CANSM_DISABLE_COMMUNICATION_REPEAT
 *  \return     targetState otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(ComM_StateType, COMM_CODE) ComM_ChannelGetMaxStateAfterExtRamCheck( NetworkHandleType channel, ComM_StateType targetState );
#endif

#if (COMM_PNC_ACTIVE_COORD == STD_ON)
/**********************************************************************************************************************
  ComM_PncGetGwChannelRequest()
 **********************************************************************************************************************/
/*! \brief      Calculates the requested state of the PN gateway channel depending on its Gateway Type and the highest requested mode of the PNCs
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     The requested state of the PN gateway channel
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelRequest( NetworkHandleType channel );
#endif

#if (COMM_PNC_ACTIVE_COORD == STD_ON)
/**********************************************************************************************************************
  ComM_PncGetGwChannelState()
 **********************************************************************************************************************/
/*! \brief      Calculates the target state of a PN gateway channel depending on its Gateway Type and the requested PNC mode
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \param[in]  highestPNComModeReq     the requested PNC mode
 *  \return     The target state of the PN gateway channel
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelState( NetworkHandleType channel, ComM_PncModeType highestPNComModeReq );
#endif

#if ((COMM_PNC_SUPPORT == STD_ON) && (COMM_PNCCHANNELMAPPING == STD_ON))
/**********************************************************************************************************************
  ComM_PncInFullComWakesUpTheChannel()
 **********************************************************************************************************************/
/*! \brief      Determines if at least one PNC on the channel is in PNC_FULL_COMMUNICATION mode.
 *  \details    The function shall not be used if Partial Network Coordination Limitation is enabled, it does not take
 *              the limitation status of the channel into account.
 *  \param[in]  channel                 local channel handle
 *  \param[in]  highestRequest   the current highest channel request
 *  \return     COMM_FULL_COM_READY_SLEEP if the current highest request is COMM_NO_COM_NO_PENDING_REQUEST and at least
 *              one PNC on this channel is in PNC_FULL_COMMUNICATION mode. Otherwise highestRequest (no changes).
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(ComM_StateType, COMM_CODE) ComM_PncInFullComWakesUpTheChannel(NetworkHandleType channel, ComM_StateType highestRequest);
#endif

#if (COMM_WAKEUP_INHIBITION == STD_ON)
/**********************************************************************************************************************
  ComM_PreventWakeUpActive()
 **********************************************************************************************************************/
/*! \brief      Determines whether Prevent Wake-up is active on the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    Prevent Wake-up is active on the channel
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \trace      SPEC-5140, SPEC-5131
 *********************************************************************************************************************/
COMM_LOCAL FUNC(boolean, COMM_CODE) ComM_PreventWakeUpActive( NetworkHandleType channel );
#endif

#if ((COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_MODE_LIMITATION == STD_ON) && ( COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF ))
/**********************************************************************************************************************
  ComM_NoComLimitationActive()
 **********************************************************************************************************************/
/*! \brief      Determines whether Mode Limitation NO_COM is active on the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    Mode Limitation to NO_COM is active on the channel
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(boolean, COMM_CODE) ComM_NoComLimitationActive( NetworkHandleType channel );
#endif

#if ((COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON)))
/**********************************************************************************************************************
  ComM_ChannelIsLimited()
 **********************************************************************************************************************/
/*! \brief      Determines whether Mode Limitation NO_COM or Prevent Wake-up is active on the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    Mode Limitation NO_COM or Prevent Wake-up is active on the channel
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE) ComM_ChannelIsLimited( NetworkHandleType channel );
#endif

#if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
/**********************************************************************************************************************
  ComM_ChannelResetAfterForcingNoComm()
 **********************************************************************************************************************/
/*! \brief       Initiates a reset after all channels entered NO_COM and at least one channel has an active Mode Limitation
 *               and Reset After Forcing NO_COM enabled.
 *  \details     -
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-116030
 *  \trace       SPEC-2818113
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelResetAfterForcingNoComm( COMM_PARTITION_TYPE_ONLY );
#endif

#if ( COMM_USERMODENOTIFUNC == STD_ON )
# if ( COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF )
/**********************************************************************************************************************
  ComM_StateChangeNotification()
 **********************************************************************************************************************/
/*! \brief      Called if BusSM mode has changed on the channel. It notifies users about a mode change if applicable.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \trace      CREQ-1269
 *  \trace      SPEC-5098, SPEC-19802, SPEC-5266
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_StateChangeNotification( NetworkHandleType channel );
# endif
#endif

#if( COMM_FULL_COMM_REQUEST_NOTIF == STD_ON )
/*********************************************************************************************************************
  ComM_CurrentChannelRequestUpdate()
 *********************************************************************************************************************/
/*! \brief      This function updates the set of ComM users currently requesting FULL_COM for the channel.
 *  \details    The set is stored in the static variable ComM_FullComRequesters[]. Then it calls
 *              ComM_CurrentChannelRequestNotification which detects if the set has changed and notifies RTE via the
 *              optional sender-receiver interface 'ComM_CurrentChannelRequest'.
 *  \param[in]  channel                 local channel handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_CurrentChannelRequestUpdate( NetworkHandleType channel );
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
  ComM_PncInit()
 **********************************************************************************************************************/
/*! \brief      Initialization of PNC related data.
 *  \details    -
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19845, SPEC-19781
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncInit( COMM_PARTITION_TYPE_ONLY );

# if (COMM_PNCCHANNELMAPPING == STD_ON)
/**********************************************************************************************************************
  ComM_PncModeArbitration()
 **********************************************************************************************************************/
/*! \brief      Determines if a PNC mapped to the channel has to perform a state transition or not and initiates
 *              a state transition if so.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19808
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeArbitration( NetworkHandleType channel );
# endif

# if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
#  if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
/**********************************************************************************************************************
  ComM_PncModeArbitrationWithoutChannels()
 **********************************************************************************************************************/
/*! \brief      Determines if a PNC without channels has to perform a state transition or not and initiates
 *              a state transition if so.
 *  \details    -
 *  \param[in]  -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeArbitrationWithoutChannels( void );
#  endif
# endif

/**********************************************************************************************************************
  ComM_PncStateTransition()
 **********************************************************************************************************************/
/*! \brief      Performs state transitions to reach the target requested PNC state.
 *  \details    -
 *  \param[in]  ComM_CurrentPn          local PNC handle
 *  \param[in]  highestComModeReq       the target requested PNC state
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19784, SPEC-19803, SPEC-19878, SPEC-19865, SPEC-19815
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncStateTransition( ComM_PncIterType ComM_CurrentPn, ComM_PncModeType highestComModeReq );

# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_PncIsOnlySystemUserRequesting()
 **********************************************************************************************************************/
/*! \brief      Determines whether only the system user is requesting the PNC (via ERA) or regular PNC user.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \param[in]  byteIndex               byte index of UserReqPncFullCom
 *  \return     TRUE                    if in the given request byte only the system user request is stored and no other PNC user request.
 *  \return     FALSE                   if at least one PNC user (not the system user) stored the request in the given byte.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE) ComM_PncIsOnlySystemUserRequesting( ComM_PncIterType pncIndex, ComM_UserReqPncFullComIterType byteIndex );

#  if (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON)
/**********************************************************************************************************************
  ComM_PncIsEraReceivedOnActiveChannel()
 **********************************************************************************************************************/
/*! \brief      Determines whether a request is received via ERA on a channel with GW type ACTIVE. This is needed to
 *              calculate the target PNC state.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \return     TRUE                    if at least one request is received via ERA on a channel with GW type ACTIVE
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE) ComM_PncIsEraReceivedOnActiveChannel( ComM_PncIterType pncIndex );
#  endif
# endif

/**********************************************************************************************************************
  ComM_CalculateHighestPNComMode()
 **********************************************************************************************************************/
/*! \brief      Calculates the highest communication mode request for the PNC.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \param[in]  pncCanBeRequested       The parameter is only used if PNC Routing Limitation feature is active
 *                                      TRUE: PNC is allowed to enter PNC_REQUESTED state, Routing Limitation is disabled on at least one channel
 *                                      FALSE: otherwise, ComM inhibits all FULL_COM requests to the PNC
 *  \return     Highest requested PNC mode
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-2818028, SPEC-2818290, SPEC-19872, SPEC-19786, SPEC-19866, SPEC-19806, SPEC-19857, SPEC-2818042,
 *  \trace      SPEC-19838, SPEC-19785, SPEC-19853
 *********************************************************************************************************************/
COMM_LOCAL FUNC(ComM_PncModeType, COMM_CODE) ComM_CalculateHighestPNComMode( ComM_PncIterType pncIndex, boolean pncCanBeRequested );

# if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON)
/**********************************************************************************************************************
  ComM_SynchronizedPncShutdownHandling()
 **********************************************************************************************************************/
/*! \brief      Determines if a Synchronized PNC Shutdown is possible while the affected PNC transitions to PNC_READY_SLEEP.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SynchronizedPncShutdownHandling( ComM_PncIterType ComM_CurrentPn );
# endif

# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/**********************************************************************************************************************
  ComM_PncRoutingLimitationHandleChannels()
 **********************************************************************************************************************/
/*! \brief      Special handling (attaching and detaching) of channels while PNC is in state PNC_REQUESTED.
 *  \details    If the feature 'PNC To Channel Routing Limitation' is activated, channels can be attached or detached
 *              while PNC is in state PNC_REQUESTED by de-activating or activating the limitation. Here we ensure that:
 *              1) if the channel has been attached to PNC, EIRA with PNC bit = 1 is sent out or
 *              2) if the channel has been detached from PNC, EIRA with PNC bit = 0 is sent out on the channel.
 *              Note: attaching when entering NM_REPEAT_MESSAGE state is handled in ComM_Nm_StateChangeNotification().
 *              Note: function ComM_PncModeSendSignal() cannot be used because it handles all PNC channels.
 *  \param[in]  InternalPncId           local PNC handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncRoutingLimitationHandleChannels( ComM_PncIterType InternalPncId );

/**********************************************************************************************************************
  ComM_PncIsEraRequestReceivedOnChannel()
 **********************************************************************************************************************/
/*! \brief      Determines if at least one PNC is requested via ERA_RX signal received on the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \return     TRUE                    if at least one PNC is requested via ERA_RX signal received on the given channel
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC( boolean, COMM_CODE ) ComM_PncIsEraRequestReceivedOnChannel( NetworkHandleType Channel );

/**********************************************************************************************************************
  ComM_PncRoutingLimitationSetState()
 **********************************************************************************************************************/
/*! \brief      Updates the PNC Routing Limitation state depending on input parameters highestPNComModeReq and pncRequestIsPending
 *  \details    -
 *  \param[in]  channel              -   local channel handle
 *  \param[in]  highestPNComModeReq  -   the highest requested mode of Pncs mapped to the channel
 *  \param[in]  pncRequestIsPending  -   TRUE - at least one FULL_COM request for a PNC mapped to the channel exists
 *                                      FALSE - otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncRoutingLimitationSetState( CONST(NetworkHandleType, AUTOMATIC) channel, CONST(ComM_PncModeType, AUTOMATIC) highestPNComModeReq, CONST(boolean, AUTOMATIC) pncRequestIsPending );
# endif

# if (COMM_PNC_PS_TIMER > 0u)
/**********************************************************************************************************************
  ComM_HandlePncPrepareSleepTimer()
 **********************************************************************************************************************/
/*! \brief      Decrements the PNC Prepare Sleep Timer for all PNCs and updates the bus-side request if the timer has expired.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      CREQ-109081
 *  \trace      SPEC-19889
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_HandlePncPrepareSleepTimer( void );
# endif

# if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON )
#  if (COMM_PNC_PS_TIMER > 0u)
/**********************************************************************************************************************
  ComM_ActivateAllPncs()
 **********************************************************************************************************************/
/*! \brief       Activates all PNCs at least for the duration of the PNC Prepare Sleep Timer.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_ActivateAllPncs( void );
#  endif
# endif

# if (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON)
/**********************************************************************************************************************
  ComM_PncModeSendSignal()
 **********************************************************************************************************************/
/*! \brief      Processing of EIRA_TX transmission requests. The function is available if at least one EIRA_TX signal is present.
 *  \details    -
 *  \param[in]  PncHandle               internal PNC index to send EIRA TX signals for
 *  \param[in]  activeState             Defines the value of PNC bit in the COM signal to be sent:
 *                                      TRUE - PNC entered PNC_REQUESTED state, PNC bit = 1
 *                                      FALSE - PNC left PNC_REQUESTED state, PNC bit = 0
 *  \param[in]  SigSendType             Defines the target GW type of channels where COM signal shall be sent:
 *                                      COMM_SEND_ACTIVE_SIG_ONLY - send only on channels with GW type ACTIVE
 *                                      COMM_SEND_PASSIVE_SIG_ONLY - send only on channels with GW type PASSIVE
 *                                      COMM_SEND_ALL_SIG - send on all channels, this is to be used if PNC Gateway feature is disabled.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19783, SPEC-19817, SPEC-19801
 *********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeSendSignal( ComM_PncIterType PncHandle, boolean activeState, uint8 SigSendType );
# endif

# if ( COMM_USERMODENOTIFUNC == STD_ON )
/***********************************************************************************************************************
 *  ComM_PncStateChangeNotification()
 **********************************************************************************************************************/
/*! \brief      Notifies ComM user about a PNC main state change. If more than one PNC is assigned to a user, notify the lowest
 *              PNC state if it has changed since the last notification.
 *  \details    -
 *  \return     E_OK                    user notification is successful
 *  \return     E_NOT_OK                otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      CREQ-1269
 **********************************************************************************************************************/
COMM_LOCAL FUNC(Std_ReturnType, COMM_CODE) ComM_PncStateChangeNotification( void );
# endif

/**********************************************************************************************************************
  ComM_PncSetBitInSignal()
 **********************************************************************************************************************/
/*! \brief      Sets PNC bit in the signal to 1.
 *  \details    -
 *  \param[in]  InternalSignalId        internal signal handle where to set the PNC bit
 *  \param[in]  InternalPncId           internal PNC handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncSetBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_PncClearBitInSignal()
 **********************************************************************************************************************/
/*! \brief      Sets PNC bit in the signal to 0.
 *  \details    -
 *  \param[in]  InternalSignalId        internal signal handle where to set the PNC bit
 *  \param[in]  InternalPncId           internal PNC handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncClearBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId COMM_PARTITION_TYPE_LAST );

/**********************************************************************************************************************
  ComM_PncClearWakeup()
 **********************************************************************************************************************/
/*! \brief      Clears the PNC wakeup flag.
 *  \details    -
 *  \param[in]  ComM_CurrentPn        Current PNC
 *  \param[in]  partitionIdx          identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
*********************************************************************************************************************/
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncClearWakeup(ComM_PncIterType ComM_CurrentPn COMM_PARTITION_TYPE_LAST );
#endif

#if (COMM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  ComM_InitCheckGeneratedData()
 **********************************************************************************************************************/
/*! \brief      Checks generated data for correctness.
 *  \details    -
 *  \param[in]  partitionIdx                identification of the internal partition index
 *  \return     COMM_E_NO_ERROR             check is successful
 *  \return     COMM_E_WRONG_GENERATED_DATA otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(uint8, COMM_CODE) ComM_InitCheckGeneratedData( COMM_PARTITION_TYPE_ONLY );
#endif

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_UpdateDataFromSatellites()
 *********************************************************************************************************************/
/*! \brief       Update and synchronize data from satellite partitions
 *  \details     Checks if an event occured on any satellite partition since the last synchronization.
 *               Update the new data from the satellite partitions and synchronize the synchronization counters.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateDataFromSatellites( void );

/**********************************************************************************************************************
  ComM_UpdateMasterSyncCounterAndStates()
 *********************************************************************************************************************/
/*! \brief          Updates the master sync counter and the corresponding new state, if available.
 *  \details        Checks if the state has changed on the satellite partition by comparing the master sync counter to the
 *                  satellite sync counter. If the state has changed, update the master sync counter and copy the new state
 *                  to the master partition.
 *  \param[in]      satSyncCounterPtr      Pointer to the satellite sync counter for this variable
 *  \param[in,out]  masterSyncCounterPtr   Pointer to the master sync counter for this variable
 *  \param[in]      satStatePtr            Pointer to the satellite state value of this variable
 *  \param[in,out]  masterStatePtr         Pointer to the master state value of this variable
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndStates( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satStatePtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterStatePtr );

# if ((COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) && (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON))
/**********************************************************************************************************************
  ComM_UpdateMasterSyncCounterAndTimers()
 *********************************************************************************************************************/
/*! \brief          Updates the master sync counter and the corresponding new timer value, if available.
 *  \details        Checks if the state has changed on the satellite partition by comparing the master sync counter to the
 *                  satellite sync counter. If the state has changed, update the master sync counter and set the timer on
 *                  the master partition to the value given by satState.
 *  \param[in]      satSyncCounterPtr      Pointer to the satellite sync counter for this variable
 *  \param[in,out]  masterSyncCounterPtr   Pointer to the master sync counter for this variable
 *  \param[in]      satTimer               The satellite state value of this variable
 *  \param[in,out]  masterTimerPtr         Pointer to the master state value of this variable
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndTimers( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 CONST( ComM_MinFullComTimeOfChannelType, AUTOMATIC ) satTimer, volatile P2VAR( ComM_MinFullComModeTimerType, AUTOMATIC, COMM_APPL_VAR ) masterTimerPtr );
# endif

#if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_SyncUserFullComRequests()
 *********************************************************************************************************************/
 /*! \brief      Synchronizes the channel and PNC user requests.
 *  \details     Checks every satellite partition if a user request was performed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncUserFullComRequests( void );
#endif

# if (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNC_ACTIVE_COORD == STD_ON)
#  if ( (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON) || (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON) )
/**********************************************************************************************************************
  ComM_SyncPncUserRequests()
 *********************************************************************************************************************/
/*! \brief       Synchronizes the user requests received via PNC requests.
 *  \details     Checks every satellite partition if a user request has to be performed due to a PNC request.
 *  \param[in]   partitionIdx                identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
*********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncPncUserRequests( COMM_PARTITION_TYPE_ONLY );
#  endif
# endif

# if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF))
/**********************************************************************************************************************
  ComM_SyncBusComModeRequests()
 *********************************************************************************************************************/
/*! \brief          Synchronizes the bus-site requested communication mode of all partitions.
 *  \details        Checks every satellite partition for a new bus-site requested communication mode.
 *  \param[in]      channel                            local channel handle
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusComModeRequests( CONST( NetworkHandleType, AUTOMATIC ) channel
#  if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON )
                                                                       , P2CONST( ComM_StateType, AUTOMATIC, COMM_CONST ) lHighestCalculatedBusComMode
#  endif
  );
# endif

# if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_SyncBusPncComModeRequests()
 *********************************************************************************************************************/
/*! \brief       Synchronizes the bus-site requested PNC communication mode of all partitions.
 *  \details     Checks every satellite partition for a new bus-site requested PNC communication mode.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusPncComModeRequests( void );
# endif

# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
/**********************************************************************************************************************
  ComM_SyncSynchronousWakeUpIndications()
 *********************************************************************************************************************/
/*! \brief       Synchronizes the synchronous wakeup indications for all partitions.
 *  \details     Checks if a passive wakeup event occurred due to Nm (RestartIndication or NetworkStartIndication) or EcuM
 *               and wakes up all channels accordingly which are not yet in FULL_COMMUNICATION.
 *  \param[in]   channel                     local channel handle
 *  \param[in]   partitionIdx                identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncSynchronousWakeUpIndications( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) lHighestCalculatedBusComMode COMM_PARTITION_TYPE_LAST );
# endif

/**********************************************************************************************************************
  ComM_IsRequestPendingOnSatellite()
 *********************************************************************************************************************/
/*! \brief       Checks if a request for the given channel is pending on any satellite partition.
 *  \details     -
 *  \param[in]   channel                     local channel handle
 *  \param[out]  busComModeReq               calculated requested bus communication mode
 *  \param[out]  busComModeReqFrShutdown     calculated requested bus communication mode for FR
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        Called by ComM_GetState()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_IsRequestPendingOnSatellite( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReq
#  if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  , P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReqFrShutdown
#  endif
    );

/**********************************************************************************************************************
  ComM_InitMasterAndSatelliteChannelSyncCounter()
 *********************************************************************************************************************/
/*! \brief       Initializes the channel related synchronization counters
 *  \details     -
 *  \param[in]   partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for the different partitions
 *  \synchronous TRUE
 *  \note        Called by ComM_Init()
*********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_InitMasterAndSatelliteChannelSyncCounter(CONST(NetworkHandleType, AUTOMATIC) channel COMM_PARTITION_TYPE_LAST );

# if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/**********************************************************************************************************************
  ComM_CheckSyncCounterAndForwardFunction()
 *********************************************************************************************************************/
/*! \brief       Checks the synchronization counters and forwards the function calls, if needed.
 *  \details     -
 *  \param[in]   syncCounterPtr            pointer to the synchronization counter on the master partition
 *  \param[in]   syncCounterCurrPartPtr    pointer to the synchronization counter on the current satellite partition
 *  \param[in]   forwardingFunction        pointer to the function to be executed
 *  \param[in]   networkHandle             local channel handle
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for the different partitions
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction_Satellite()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_CheckSyncCounterAndForwardFunction( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) syncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) syncCounterCurrPartPtr,
                                                                                   ComM_ForwardingFunctionPtrType forwardingFunction, CONST( NetworkHandleType, AUTOMATIC ) networkHandle );
# endif
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COMM_PNC_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncStateTransition()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncStateTransition( ComM_PncIterType ComM_CurrentPn, ComM_PncModeType highestComModeReq )
{
  /* #10 If the highest requested PNC mode differs from the currently active PNC mode, perform as many
   * state transitions as needed to reach the highest requested PNC mode.
   *     1) Send COM signal if entering or leaving PNC Requested state,
   *     2) Set the new PNC state,
   *     3) Notify BswM about the new PNC state (BusSM states of channels are not considered),
   *     4) Handle PNC Prepare Sleep Timer if applicable,
   *     5) Store a flag to notify PNC users about the new state if entering or leaving PNC Full Communication mode. */
  ComM_PncModeType localhighestComModeReq = highestComModeReq;
  do /* FETA_COMM_PNC_STATE_TRANSITION */
  {
    /* #20 PNC shall stay in Prepare Sleep state if PNC Prepare Sleep timer is running. */
# if (COMM_PNC_PS_TIMER > 0u)
    if ((ComM_GetPncPSleepTimer(ComM_CurrentPn, COMM_MASTER_PARTITION_IDX) > 0u) && (localhighestComModeReq <= COMM_PNC_PREPARE_SLEEP))
    {
      localhighestComModeReq = COMM_PNC_PREPARE_SLEEP;
    }
# endif

    if (localhighestComModeReq != ComM_GetActivePncComMode(ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ))
    {
      switch (ComM_GetActivePncComMode(ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ))
      {
      /* #30 State transition from COMM_PNC_NO_COMMUNICATION state. */
      case COMM_PNC_NO_COMMUNICATION:
        switch (localhighestComModeReq)
        {
        /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
# if (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON)
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
# endif

          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
# if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif
          ComM_PncClearWakeup(ComM_CurrentPn COMM_MASTER_PARTITION_PARA_LAST );
          break;

        /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_REQUESTED_PASSIVE */
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
#  if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  endif
          ComM_PncClearWakeup(ComM_CurrentPn COMM_MASTER_PARTITION_PARA_LAST );
          break;
# endif

        /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_READY_SLEEP */
        case COMM_PNC_READY_SLEEP:
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_READY_SLEEP);
# if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif
          break;

        /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_PREPARE_SLEEP */
# if ((COMM_PNC_PS_TIMER > 0u) && ((COMM_SYNCHRONOUS_WAKE_UP == STD_ON) || (COMM_WAKEUPENABLEDOFPNC == STD_ON)))
        case COMM_PNC_PREPARE_SLEEP:
          /* Note: it is ensured that transition is only performed if COMM_PNC_PS_TIMER > 0 and
           * (COMM_SYNCHRONOUS_WAKE_UP == STD_ON or COMM_WAKEUPENABLEDOFPNC == STD_ON) */
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_PREPARE_SLEEP);
          ComM_SetPncPSleepTimer(ComM_CurrentPn, COMM_PNC_PS_TIMER, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  endif
          break;
# endif
        default: /* COV_COMM_MISRA */
          break;
        }
        break;

      /* #40 State transition from COMM_PNC_PREPARE_SLEEP state. */
# if (COMM_PNC_PS_TIMER > 0u)
      case COMM_PNC_PREPARE_SLEEP:
        switch (localhighestComModeReq)
        {
        /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
#  if (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON)
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
#  endif
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
          break;

        /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_REQUESTED_PASSIVE */
#  if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
          break;
#  endif

        /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_READY_SLEEP */
        case COMM_PNC_READY_SLEEP:
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_READY_SLEEP);
          break;

        /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_NO_COMMUNICATION */
        case COMM_PNC_NO_COMMUNICATION:
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_NO_COMMUNICATION);
#  if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  endif
          break;

        default: /* COV_COMM_MISRA */
          break;
        }

        /* When leaving COMM_PNC_PREPARE_SLEEP state, always stop the timer and reset ComM_BusPncComModeReq[]
        if no EIRA = 1 signal received for the PNC. Needed e.g. in case PNC is not considered by Nm filter mask.
        Possible scenario: Synchronous Wake-up, FULL_COM request, NO_COM request. In this case highestComModeReq
        shall be COMM_PNC_NO_COMMUNICATION and PNC shall enter COMM_PNC_PREPARE_SLEEP. (ESCAN00077600). */
        ComM_SetPncPSleepTimer(ComM_CurrentPn, 0, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_PncClearWakeup(ComM_CurrentPn COMM_MASTER_PARTITION_PARA_LAST );
        break;
# endif

      /* #50 State transition from COMM_PNC_READY_SLEEP state. */
      case COMM_PNC_READY_SLEEP:
        switch (localhighestComModeReq)
        {
        /* COMM_PNC_READY_SLEEP -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
# if (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON)
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
# endif
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
          break;

        /* COMM_PNC_READY_SLEEP -> COMM_PNC_REQUESTED_PASSIVE, if COMM_PNC_ACTIVE_COORD == STD_ON */
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_REQUESTED);
          break;
# endif

        case COMM_PNC_NO_COMMUNICATION:
# if(COMM_PNC_PS_TIMER > 0u)
        case COMM_PNC_PREPARE_SLEEP:
# endif
          /* Always indicate COMM_PNC_PREPARE_SLEEP to BswM, even if the state is omitted in case COMM_PNC_PS_TIMER == 0. */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_PREPARE_SLEEP);

# if (COMM_PNC_PS_TIMER == 0u)
          /* Special case if COMM_PNC_PREPARE_SLEEP == 0: COMM_PNC_READY_SLEEP -> COMM_PNC_NO_COMMUNICATION (ESCAN00067357) */
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_NO_COMMUNICATION);
#   if ( COMM_USERMODENOTIFUNC == STD_ON )
          ComM_SetNotifyPncStateChange(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#   endif
# else  /* COMM_PNC_PS_TIMER > 0 */
          /* COMM_PNC_READY_SLEEP -> COMM_PNC_PREPARE_SLEEP */
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          ComM_SetPncPSleepTimer(ComM_CurrentPn, COMM_PNC_PS_TIMER, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif /* COMM_PNC_PS_TIMER */
          break;

        default: /* COV_COMM_MISRA */
          break;
        }
        break;

# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
      /* #60 State transition from COMM_PNC_REQUESTED_PASSIVE state. */
      case COMM_PNC_REQUESTED_PASSIVE:
        if(localhighestComModeReq == COMM_PNC_REQUESTED)
        {
          /* COMM_PNC_REQUESTED_PASSIVE -> COMM_PNC_REQUESTED, if COMM_PNC_ACTIVE_COORD == STD_ON */
          ComM_PncModeSendSignal(ComM_CurrentPn, TRUE, COMM_SEND_PASSIVE_SIG_ONLY );
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
        else
        {
          /* COMM_PNC_REQUESTED_PASSIVE -> COMM_PNC_READY_SLEEP, if COMM_PNC_ACTIVE_COORD == STD_ON.
          Note: there are only two possible transitions from COMM_PNC_REQUESTED_PASSIVE state, therefore using if..else is ok */
          ComM_PncModeSendSignal(ComM_CurrentPn, FALSE, COMM_SEND_ACTIVE_SIG_ONLY );
#  if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON)
          ComM_SynchronizedPncShutdownHandling( ComM_CurrentPn );
#  endif
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_READY_SLEEP);
        }
        break;
# endif

      /* #70 State transition from COMM_PNC_REQUESTED state. */
      case COMM_PNC_REQUESTED:
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        if(localhighestComModeReq == COMM_PNC_REQUESTED_PASSIVE)
        {
          /* COMM_PNC_REQUESTED -> COMM_PNC_REQUESTED_PASSIVE, if COMM_PNC_ACTIVE_COORD == STD_ON */
          ComM_PncModeSendSignal(ComM_CurrentPn, FALSE, COMM_SEND_PASSIVE_SIG_ONLY );
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
        else
# endif
        {
          /* COMM_PNC_REQUESTED -> COMM_PNC_READY_SLEEP
          Note: there are only two possible transitions from COMM_PNC_REQUESTED state, therefore using if..else is ok */
# if (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON)
          ComM_PncModeSendSignal(ComM_CurrentPn, FALSE, COMM_SEND_ALL_SIG );
# endif

# if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON)
          ComM_SynchronizedPncShutdownHandling( ComM_CurrentPn );
# endif
          ComM_SetActivePncComMode(ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          BswM_ComM_CurrentPNCMode(ComM_GetPncIdOfPnc(ComM_CurrentPn), COMM_PNC_READY_SLEEP);
        }
        break;

      default: /* COV_COMM_MISRA */
        break;
      }
    }
  } while (localhighestComModeReq != ComM_GetActivePncComMode(ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ));
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNCCHANNELMAPPING == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncModeArbitration()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeArbitration(NetworkHandleType channel )
{
  ComM_PncModeType highestComModeReq;
#  if (COMM_PNC_ACTIVE_COORD == STD_ON)
  ComM_PncChannelMappingEndIdxOfPncPbType pncChIndex;
  boolean allCoordinatedChannelsAreInFullCom;
#   if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  boolean pncCanBeRequested;
#   endif
#  endif
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
  ComM_PncIterType pncIndex;

  for (pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb(channel); /* FETA_COMM_CSL_USE_CASE */
       pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb(channel);
       pncPbIndIter++)
  {
    pncIndex = (ComM_PncIterType)ComM_GetPncPbInd(pncPbIndIter);

    /* For each PNC, the boolean value for allCoordinatedChannelsAreInFullCom and pncCanBeRequested must be recalculated. */
#  if (COMM_PNC_ACTIVE_COORD == STD_ON)
    allCoordinatedChannelsAreInFullCom = TRUE;
#   if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    pncCanBeRequested = FALSE;
#   endif

    /* #10 If a PNC has channels, a potential state change of the PNC can be performed in context of current channel if
    *     1) the PNC is mapped to the current channel and
    *     2) all coordinated PNC channels are in FULL_COM mode in case PNC Gateway functionality is enabled.
    *     If the feature PNC to Channel Routing Limitation is enabled, only channels without limitation are considered.
    *     E.g. channels with limitation can be in NO_COM mode but PNC can change its state nevertheless. */
    for (pncChIndex = ComM_GetPncChannelMappingStartIdxOfPncPb(pncIndex); /* FETA_COMM_CSL_USE_CASE */
         pncChIndex < ComM_GetPncChannelMappingEndIdxOfPncPb(pncIndex);
         pncChIndex++)
    {
#   if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
      /* Example: assume PNC is on CAN1 and CAN2 and routing is limited on CAN1. If PNC is requested, it changes to PNC_REQUESTED
       * as soon as CAN2 is in FULL_COM mode. The mode of CAN1 is not evaluated. */
      if( ComM_GetPncToChannelRoutingState(ComM_GetPncChannelMapping(pncChIndex), COMM_MASTER_PARTITION_IDX) != COMM_PNCROUTING_LIMITED )
      {
        /* Note: found a channel without Routing Limitation, it will be passed to ComM_CalculateHighestPNComMode() later */
        pncCanBeRequested = TRUE;
#   endif
        if( (ComM_GetActiveComMode( ComM_GetPncChannelMapping(pncChIndex), COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u ) /*lint !e539 positive indentation is intended */
        {
          allCoordinatedChannelsAreInFullCom = FALSE;
        }
#   if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
      }
#   endif
    }

    /* #20 If a PNC can perform a state transition in the context of the current channel task, calculate the highest requested mode
     *     of the PNC and perform the corresponding state transition. */
    highestComModeReq = COMM_PNC_NO_COMMUNICATION;
    if (allCoordinatedChannelsAreInFullCom == TRUE)
#  endif
    {
      /* Note: highestComModeReq shall be COMM_PNC_NO_COMMUNICATION (default) as long as at least one channel is not in FULL_COMM yet.
       * This is to ensure that a state transition (activation) of a coordinated PNC is performed AFTER all assigned channels have entered FULL_COMM. */
#  if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
      highestComModeReq = ComM_CalculateHighestPNComMode(pncIndex, pncCanBeRequested);
#  else
      highestComModeReq = ComM_CalculateHighestPNComMode(pncIndex, TRUE); /* 2nd parameter is not used */
#  endif
    }

    if (highestComModeReq != ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX ))
    {
      ComM_PncStateTransition(pncIndex, highestComModeReq);
    }
#  if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    else if(ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX ) > COMM_PNC_READY_SLEEP)
    {
      ComM_PncRoutingLimitationHandleChannels( pncIndex );
    }
    else
    {
      /* nothing to do */
    }
#  endif
  } /* Iteration over PNCs */

#  if (COMM_ISDEF_PNCPBINDSTARTIDXOFCHANNELPB == STD_ON)
  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
}
# endif
#endif

#if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
# if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncModeArbitrationWithoutChannels()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeArbitrationWithoutChannels( void )
{
  ComM_PncIterType pncIndex;
  ComM_PncModeType highestComModeReq = COMM_PNC_NO_COMMUNICATION;

  /* #10 If Extended PNC Functionality is enabled, a PNC may have no channels assigned. A potential state change of the PNC
   *     is always to be performed in context of Channel 0.
   *     Note: The function is called for Channel 0 even if it is disabled in the current variant. However, it is not necessary
   *     to check ComM_IsEnabledOfChannel, because no PNC will be assigned to such a disabled channel. */
  for (pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++)
  {
# if (COMM_PNCCHANNELMAPPING == STD_ON)
    if (ComM_GetPncChannelMappingStartIdxOfPncPb(pncIndex) == COMM_NO_PNCCHANNELMAPPINGSTARTIDXOFPNCPB)
# endif
    {
      highestComModeReq = ComM_CalculateHighestPNComMode(pncIndex, TRUE); /* 2nd parameter is not used */

      if (highestComModeReq != ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ))
      {
        ComM_PncStateTransition(pncIndex, highestComModeReq);
      }
    }
  }
}
# endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncRoutingLimitationHandleChannels()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncRoutingLimitationHandleChannels( ComM_PncIterType InternalPncId )
{
  ComM_PncSignalIndEndIdxOfPncPbType ComM_PncSignalMappingIndex;
  ComM_SizeOfPncSignalType ComM_SignalIndex;
  ComM_SizeOfChannelType ComM_ChannelIndex;

  for(ComM_PncSignalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb(InternalPncId); ComM_PncSignalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb(InternalPncId); ComM_PncSignalMappingIndex++) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SignalIndex = ComM_GetPncSignalInd(ComM_PncSignalMappingIndex);
    if (ComM_GetTypeOfPncSignal(ComM_SignalIndex) == COMM_EIRA_TX_TYPEOFPNCSIGNAL)
    {
      ComM_ChannelIndex = ComM_GetChannelIdxOfPncSignal(ComM_SignalIndex);

      /* #10 Attaching: PNC routing is not limited and PNC bit = 0 was sent on the channel, send PNC bit = 1 now. */
      if( ((ComM_GetPncToChannelRoutingState(ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX) & COMM_PNCROUTING_NOT_LIMITED) != 0x00u) &&
          (!ComM_IsSignalStoredPncBitSet( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX )) )
      {
#  if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        if( (ComM_GetGwTypeOfChannel(ComM_ChannelIndex) == COMM_GATEWAY_TYPE_PASSIVE) && (ComM_GetActivePncComMode(InternalPncId, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_REQUESTED_PASSIVE) )
        {
          /* PNC is requested via ERAp=1 from PASSIVE channel, do not keep the channel awake and do not mirror the request back */
        }
        else
#  endif
        {
          ComM_PncSetBitInSignal( ComM_SignalIndex, InternalPncId COMM_MASTER_PARTITION_PARA_LAST );
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          if ( ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
#   endif
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
            ComM_SetForwardComSendSignalSyncCounter( ComM_ChannelIndex, (uint8)(ComM_GetForwardComSendSignalSyncCounter( ComM_ChannelIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            ComM_SetForwardingComSendSignalIndex( ComM_ChannelIndex, ComM_SignalIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
          }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          {
            (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_SignalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_SignalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */
          }
#  endif

#  if (COMM_PNCNMREQUESTOFCHANNEL == STD_ON)
          if ( ComM_IsPncNmRequestOfChannel( ComM_ChannelIndex ) ) /* COV_COMM_SUBSET_OF_ELEMENTS */
          {
#   if (COMM_MULTIPARTITION == STD_ON)
            if ( ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_PNCNMREQUEST */
            {
              ComM_SetForwardNmNetworkRequestSyncCounter( ComM_ChannelIndex, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( ComM_ChannelIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
            else
#   endif
            {
              (void)Nm_NetworkRequest( ComM_ChannelIndex );
            }
          }
#  endif
        }
      }
      /* #20 Detaching: PNC routing is limited and PNC bit = 1 was sent on the channel, send PNC bit = 0 now. Valid for any GW type. */
      else if( (ComM_GetPncToChannelRoutingState(ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED) &&
               (ComM_IsSignalStoredPncBitSet( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX )) )
      {
        ComM_PncClearBitInSignal( ComM_SignalIndex, InternalPncId COMM_MASTER_PARTITION_PARA_LAST );
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        if ( ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
#   endif
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
          ComM_SetForwardComSendSignalSyncCounter( ComM_ChannelIndex, (uint8)(ComM_GetForwardComSendSignalSyncCounter( ComM_ChannelIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          ComM_SetForwardingComSendSignalIndex( ComM_ChannelIndex, ComM_SignalIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
        }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        {
          (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_SignalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_SignalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */
        }
#  endif
      }
      else
      {
        /* nothing to do */
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncSetBitInSignal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncSetBitInSignal(ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId COMM_PARTITION_TYPE_LAST )
{
  /* #10 Calculate the new value of PNC signal byte by setting the corresponding PNC bit to 1. */
  ComM_SizeOfPncSignalValuesType signalByteIndex = (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal(InternalSignalId) + ComM_GetSignalByteIndexOfPncPb(InternalPncId)); /* PRQA S 2985 */ /* MD_ComM_2985 */

  /* PRQA S 2995, 2991 1 */ /* MD_ComM_2995_2991 */
  if ( (signalByteIndex < ComM_GetSizeOfPncSignalValues( PARTITION_IDX ))
# if (COMM_MULTIPARTITION == STD_ON)
    && (PARTITION_IDX < ComM_GetSizeOfPartitionIdentifiers())
# endif
    ) /* COV_COMM_GENDATA_CHECK TX tx tx */
  {
    /* #20 Calculate the initial value of the signalByte from the master partition.
     *     In case of MultiPartition, assign the PNC byte from the signal with the new value in the satellite partition data.
     *     If the signalByte was previously calculated on the satellite partition,
     *     get the signalValue from the satellite and reevaluate the signalByte value.
     */
    ComM_PncSignalValuesType signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfPnc( InternalPncId )); /* PRQA S 2842, 2843 */ /* MD_ComM_2842_2843 */
# if (COMM_MULTIPARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
      if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, PARTITION_IDX ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
      {
        signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, PARTITION_IDX ) | ComM_GetSetMaskOfPnc( InternalPncId )); /* PRQA S 2842, 2843 */ /* MD_ComM_2842_2843 */
        ComM_SetPncSignalValues( signalByteIndex, signalByte, PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
      else
      {
        ComM_SetPncSignalValues( signalByteIndex, signalByte, PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        ComM_SetPncSignalValuesSyncCounter( signalByteIndex, (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_SYNCCOUNTER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
    }
    else
# endif
    {
      ComM_SetPncSignalValues( signalByteIndex, signalByte, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
  }

# if (COMM_ACTIVE_PNC == 1u)
  COMM_DUMMY_STATEMENT_CONST(InternalPncId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

# if (COMM_ISDEF_PNCSIGNALVALUESSTARTIDXOFPNCSIGNAL == STD_ON)
  COMM_DUMMY_STATEMENT_CONST(InternalSignalId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncClearBitInSignal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncClearBitInSignal( ComM_SizeOfPncSignalType InternalSignalId, ComM_PncIterType InternalPncId COMM_PARTITION_TYPE_LAST )
{
  /* #10 Calculate the new value of PNC signal byte by setting the corresponding PNC bit to 0. */
  ComM_SizeOfPncSignalValuesType signalByteIndex = (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal(InternalSignalId) + ComM_GetSignalByteIndexOfPncPb(InternalPncId)); /* PRQA S 2985 */ /* MD_ComM_2985 */

  /* PRQA S 2995, 2991 1 */ /* MD_ComM_2995_2991 */
  if ( (signalByteIndex < ComM_GetSizeOfPncSignalValues( COMM_MASTER_PARTITION_IDX ))
# if (COMM_MULTIPARTITION == STD_ON)
    && (PARTITION_IDX < ComM_GetSizeOfPartitionIdentifiers())
# endif
    ) /* COV_COMM_GENDATA_CHECK TX tx tx */ /* PRQA S 2995, 2991 */ /* MD_ComM_2995_2991 */
  {
    /* #20 Calculate the initial value of the signalByte from the master partition.
     *     In case of MultiPartition, assign the PNC byte from the signal with the new value in the satellite partition data.
     *     If the signalByte was previously calculated on the satellite partition,
     *     get the signalValue from the satellite and reevaluate the signalByte value.
     */
    ComM_PncSignalValuesType signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfPnc( InternalPncId )); /* PRQA S 2842, 2843 */ /* MD_ComM_2842_2843 */
# if (COMM_MULTIPARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
      if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, PARTITION_IDX ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
      {
        signalByte = (ComM_PncSignalValuesType)(ComM_GetPncSignalValues( signalByteIndex, PARTITION_IDX ) & ComM_GetClearMaskOfPnc( InternalPncId )); /* PRQA S 2842, 2843 */ /* MD_ComM_2842_2843 */
        ComM_SetPncSignalValues( signalByteIndex, signalByte, PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
      else
      {
        ComM_SetPncSignalValues( signalByteIndex, signalByte, PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        ComM_SetPncSignalValuesSyncCounter( signalByteIndex, (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_SYNCCOUNTER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
    }
    else
# endif
    {
      ComM_SetPncSignalValues( signalByteIndex, signalByte, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
  }

# if (COMM_ACTIVE_PNC == 1u)
  COMM_DUMMY_STATEMENT_CONST(InternalPncId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

# if (COMM_ISDEF_PNCSIGNALVALUESSTARTIDXOFPNCSIGNAL == STD_ON)
  COMM_DUMMY_STATEMENT_CONST(InternalSignalId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_PNC_PS_TIMER > 0u)
/***********************************************************************************************************************
 *  ComM_HandlePncPrepareSleepTimer()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_HandlePncPrepareSleepTimer( void )
{
  /* #10 For each PNC check the PNC Prepare Sleep Timer. If the timer is started, decrement it. If the timer is expired
   *     and PNC is not requested, set the bus requested mode to COMM_PNC_NO_COMMUNICATION to allow PNC to enter COMM_PNC_NO_COMMUNICATION. */
  ComM_PncIterType pncIndex;

# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
  if (ComM_HasPncPb())
# endif
  {
    for(pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++)
    {
      if(ComM_GetPncPSleepTimer(pncIndex, COMM_MASTER_PARTITION_IDX) > 0u) /* PNC state is always COMM_PNC_PREPARE_SLEEP in this case, no need to check it. */
      {
        ComM_DecPncPSleepTimer(pncIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        if( (ComM_GetBusPncComModeReq(pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_PREPARE_SLEEP) && (ComM_GetPncPSleepTimer(pncIndex, COMM_MASTER_PARTITION_IDX ) == 0u) )
        {
          ComM_SetBusPncComModeReq(pncIndex, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
      }
    }
  }
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON )
#  if (COMM_PNC_PS_TIMER > 0u)
/***********************************************************************************************************************
 *  ComM_ActivateAllPncs()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_ActivateAllPncs( void )
{
  ComM_PncIterType pncIndex;

  /* #10 For each PNC check the current state. If the PNC is not yet in FULL_COMMUNICATION, activate the PNC by setting
   *     the bus-site requested communication mode to COMM_PNC_PREPARE_SLEEP. */
#   if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
  if ( ComM_HasPncPb() )
#   endif
  {
    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ )
    {
      if ( (ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) && (ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) )
      {
        ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
    }
  }
}
#  endif
# endif
#endif

#if (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
/***********************************************************************************************************************
*  ComM_InternalBusSmModeIndication()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalBusSmModeIndication( NetworkHandleType Channel, ComM_ModeType ComMode )
{
  ComM_SetSimulatedSmMode( Channel, ComMode, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  ComM_BusSM_ModeIndication( Channel, &ComMode ); /* SBSW_COMM_INTERNAL_BUSSM_CALL_WITH_PTR_PARAM */ /* PRQA S 1339 */ /* MD_ComM_1339 */
  return E_OK;
}

/***********************************************************************************************************************
*  ComM_InternalGetCurrentBusSmMode()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalGetCurrentBusSmMode( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  *ComMode = ComM_GetSimulatedSmMode( Channel, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PASSED_PTR_WRITE_ACCESS */
  return E_OK;
}
#endif

#if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/*********************************************************************************************************************
 *  ComM_LimitPncToChannelRouting()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitPncToChannelRouting( PNCHandleType Pnc, NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_PncIterType pncIndex;
  ComM_SizeOfPncLimitationType index = 0u;
  ComM_PncLimitationType value = 0u;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
#  if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
#  endif
  /* #30 Check parameter 'Channel': it must be valid and PNC routing limitation must be supported on the channel. */
  /* Note: it is ensured by the code generator that only active channels having GW type != NONE support routing limitation. */
  else if ((Channel >= ComM_GetSizeOfChannelPb()) || (!ComM_IsPncLimitationUsedOfChannelPb( Channel ))) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* Get the internal PNC index from the given PNC identifier */
    for( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ )
    {
      if (Pnc == ComM_GetPncIdOfPnc(pncIndex))
      {
        break;
      }
    }

    /* #40 Check parameter 'Pnc': it must be a valid PNC identifier. */
    if( pncIndex < ComM_GetSizeOfPnc() )
    {
      /* #50 If parameter checks are successful, store the PNC to Channel Routing Limitation status. */
      if( Status == TRUE )
      {
        /* Limitation is activated */
        value = (ComM_PncLimitationType)(ComM_GetPncLimitation((ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb(Channel) + ComM_GetSignalByteIndexOfPncPb(pncIndex)), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfPnc(pncIndex));
      }
      else
      {
        /* Limitation is de-activated */
        value = (ComM_PncLimitationType)(ComM_GetPncLimitation((ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb(Channel) + ComM_GetSignalByteIndexOfPncPb(pncIndex)), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfPnc(pncIndex));
      }

      index = (ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb(Channel) + ComM_GetSignalByteIndexOfPncPb(pncIndex));

      if( index < ComM_GetSizeOfPncLimitation(COMM_MASTER_PARTITION_IDX) ) /* COV_COMM_GENDATA_CHECK */
      {
        ComM_SetPncLimitation( index, value, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        retVal = E_OK;
      }
# if (COMM_DEV_ERROR_REPORT == STD_ON)
      else
      {
        errorId = COMM_E_WRONG_GENERATED_DATA;
      }
# endif
    }
# if (COMM_DEV_ERROR_REPORT == STD_ON)
    else
    {
      errorId = COMM_E_WRONG_PARAMETERS;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report errors if applicable. */
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_LIMITPNCTOCHANNELROUTING, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if ((COMM_PNC_SUPPORT == STD_ON) && (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON))
/**********************************************************************************************************************
 *  ComM_PncModeSendSignal()
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
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncModeSendSignal( ComM_PncIterType PncHandle, boolean activeState, uint8 SigSendType )
{
  ComM_PncSignalIndEndIdxOfPncPbType signalMappingIndex;
  ComM_SizeOfPncSignalType signalIndex;
# if ( (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON) || (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON) || (COMM_MULTIPARTITION == STD_ON) || (COMM_PNCNMREQUESTOFCHANNEL == STD_ON) )
  NetworkHandleType channel;
# endif
# if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
#  if (COMM_PNCNMREQUESTOFCHANNEL == STD_ON)
  ComM_PncChannelMappingEndIdxOfPncPbType pncChannelMappingIndex;
#  endif
# endif

  /* #10 Iterate over EIRA_TX signals mapped to the given PNC. Send the corresponding COM signal if
   *     1) the GW type of the corresponding channel matches the given SigSendType or
   *     2) all EIRA_TX signals have to be sent.
   *     The signal value (PNC bit) is set according to the PNC activation given by activeState.
   *     If PNC Routing Limitation is enabled, PNC activation can be set only if routing is not limited on the channel. */
  for(signalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb(PncHandle); signalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb(PncHandle); signalMappingIndex++) /* FETA_COMM_CSL_USE_CASE */
  {
    signalIndex = ComM_GetPncSignalInd(signalMappingIndex);
# if (COMM_EXISTS_ERA_RX_TYPEOFPNCSIGNAL == STD_ON)
    if (ComM_GetTypeOfPncSignal(signalIndex) == COMM_EIRA_TX_TYPEOFPNCSIGNAL) /* COV_COMM_SUBSET_OF_ELEMENTS */ /* Signal Type can be EIRA_TX or ERA_RX */
# endif
    {
# if ( (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON) || (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON) || (COMM_MULTIPARTITION == STD_ON) || (COMM_PNCNMREQUESTOFCHANNEL == STD_ON) )
      channel = ComM_GetChannelIdxOfPncSignal( signalIndex ); /* PRQA S 2983 */ /* MD_ComM_2983 */
# endif
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
      if ((SigSendType == ComM_GetGwTypeOfChannel( channel )) || (SigSendType == COMM_SEND_ALL_SIG))
# endif
      {
        if(activeState == TRUE)
        {
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
          if(ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) != COMM_PNCROUTING_LIMITED )
# endif
          {
            ComM_PncSetBitInSignal( signalIndex, PncHandle COMM_MASTER_PARTITION_PARA_LAST );
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
#   endif
            {
              SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
              ComM_SetForwardComSendSignalSyncCounter( channel, (uint8)(ComM_GetForwardComSendSignalSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetForwardingComSendSignalIndex( channel, signalIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
            }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            {
              (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */
            }
#  endif
          }
        }
        else
        {
          ComM_PncClearBitInSignal( signalIndex, PncHandle COMM_MASTER_PARTITION_PARA_LAST );
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
#   endif
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
            ComM_SetForwardComSendSignalSyncCounter( channel, (uint8)(ComM_GetForwardComSendSignalSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            ComM_SetForwardingComSendSignalIndex( channel, signalIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
          }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
          {
            (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */
          }
#  endif
        }

        /* #20 Call Nm_NetworkRequest() if PNC is activated and the parameter 'PNC Nm Request' is enabled on the channel.
         *   In case PNC Extended Functionality is enabled, only call Nm_NetworkRequest() if PNC is mapped to the channel.
         *   In case PNC Routing Limitation is enabled, only call Nm_NetworkRequest() if Routing is not limited on the channel.
         */
# if (COMM_PNCNMREQUESTOFCHANNEL == STD_ON)
        if (activeState == TRUE)
        {
#  if (COMM_ISDEF_PNCNMREQUESTOFCHANNEL == STD_OFF)
          if (ComM_IsPncNmRequestOfChannel( channel )) /* COV_COMM_SUBSET_OF_ELEMENTS */
#  endif
          {
#  if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
            for (pncChannelMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb(PncHandle); /* FETA_COMM_CSL_USE_CASE */
                 pncChannelMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb(PncHandle);
                 pncChannelMappingIndex++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
            {
              if (ComM_GetPncChannelMapping(pncChannelMappingIndex) == channel ) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
              {
#   if (COMM_MULTIPARTITION == STD_ON)
#    if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
                if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_PNCNMREQUEST */
#    endif
                {
                  ComM_SetForwardNmNetworkRequestSyncCounter( channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel )) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
                }
#    if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
                else
#    endif
#   endif
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
                {
                  (void)Nm_NetworkRequest( channel );
                }
#   endif
                break;
              }
            }
#  else
            /* Note: In normal PNC functionality, additional PNC signals without a PNC - Channel assignment are only allowed for preparing
             * for a PB-L usecase. In this case, the additional signals are automatically filtered in the generator, so we don't need
             * to verify the PNC - Channel assignment. The generator guarantees that PNC Signals exist only if there is a corresponding
             * PNC - Channel assignment. */
#   if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
            /* This is to prevent calling Nm_NetworkRequest() if Limitation is de-activated temporarily because Nm is in state NM_STATE_REPEAT_MESSAGE. */
            if( (ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED) != 0x00u )
#   endif
            {
#   if (COMM_MULTIPARTITION == STD_ON)
#    if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
              if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_PNCNMREQUEST */
#    endif
              {
                ComM_SetForwardNmNetworkRequestSyncCounter( channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              }
#    if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
              else
#    endif
#   endif
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
              {
                (void)Nm_NetworkRequest( channel );
              }
#   endif
            }
#  endif
          }
        }
# endif
      }
    }
  }

# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_OFF)
  COMM_DUMMY_STATEMENT_CONST(SigSendType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_EXISTS_EIRA_RX_TYPEOFPNCSIGNAL == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncProcessRxSignalEira()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PncProcessRxSignalEira( ComM_SizeOfPncSignalType InternalSignalId, const uint8 *SignalDataRx
#  if (COMM_MULTIPARTITION == STD_ON)
                                                     , ComM_PartitionConfigIdxOfChannelType partitionIdx
#  endif
  )
{
#  if (COMM_PNC_MAX_NUMBER_OF_EIRA_RX > 1u)
  uint8 SigValuesArb;
  ComM_PncSignalIterType signalIndex;
#  endif
  ComM_SizeOfPncType pncIndex;

  /* #10 Iterate over PNCs and update the target PNC state. */
  for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ )
  {
    if ( (SignalDataRx[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) != 0u )
    {
      /* #110 If a PNC is requested via EIRA RX, set the target state to COMM_PNC_READY_SLEEP. */
#  if ( COMM_MULTIPARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
        ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_READY_SLEEP, PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        /* Increment sync counter for ComM_BusPncComModeReq by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusPncComModeReqSyncCounter( pncIndex, /* PRQA S 2985 */ /* MD_ComM_2985 */ /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
                                             (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( PARTITION_IDX ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u),
                                             PARTITION_IDX );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
      }
      else
#  endif
      {
        ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_READY_SLEEP, PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }

      ComM_PncSetBitInSignal( InternalSignalId, pncIndex COMM_PARTITION_PARA_LAST );
    }
    else
    {
      /* #120 If a PNC is released via EIRA RX, check if it is requested on other bus types.
       *      If it is released on all other bus types, set the target state to COMM_PNC_NO_COMMUNICATION. */
      ComM_PncClearBitInSignal( InternalSignalId, pncIndex COMM_PARTITION_PARA_LAST );
#  if (COMM_PNC_MAX_NUMBER_OF_EIRA_RX > 1u) /* If only one EIRA RX (bus type), no need to loop over signals */
      SigValuesArb = 0u;
      for ( signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++ )
      {
        if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_RX_TYPEOFPNCSIGNAL ) /* COV_COMM_SUBSET_OF_ELEMENTS */
        {
          SigValuesArb |= ComM_GetPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex ), PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */
        }
      }

      if ( (SigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u )
#  endif
      {
#  if ( COMM_MULTIPARTITION == STD_ON )
        if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
          /* Increment sync counter for ComM_BusPncComModeReq by 1 (relative to the corresponding Master-Counter) */
          ComM_SetBusPncComModeReqSyncCounter( pncIndex, /* PRQA S 2985 */ /* MD_ComM_2985 */ /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
                                               (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( PARTITION_IDX ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u),
                                               PARTITION_IDX );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
        }
        else
#  endif
        {
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }

      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_ACTIVE_COORD == STD_ON)
#  if ( (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON) || (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON) )
/**********************************************************************************************************************
 *  ComM_PncProcessRxSignalEra()
 **********************************************************************************************************************/
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
FUNC(void, COMM_CODE) ComM_PncProcessRxSignalEra(ComM_SizeOfPncSignalType InternalSignalId, const uint8 *SignalDataRx)
{
  uint8 SigValuesArb;
  ComM_SizeOfPncType pncIndex;
  ComM_PncSignalIndEndIdxOfPncPbType ComM_PncSignalMappingIndex;
  ComM_PncSignalIterType signalIndex;
  ComM_UserHandleType systemUser = 0u;
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex = 0u;
  ComM_UserReqPncFullComType userReqPncFullCom = 0u;
  ComM_UserByteMaskIterType iByteMaskUserIndex = 0u;
  ComM_UserReqFullComType userReqFullCom = 0u;
# if ( COMM_DEV_ERROR_REPORT == STD_ON )
  uint8 errorId = COMM_E_NO_ERROR;
# endif
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( InternalSignalId ) );
#  endif


  /* #10 Iterate over PNCs and request the system user accordingly. */
  for(pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++)
  {
#  if (COMM_ISDEF_COORDINATEDOFPNC == STD_OFF)
    if(ComM_IsCoordinatedOfPnc(pncIndex)) /* COV_COMM_SUBSET_OF_ELEMENTS */ /*lint !e506 */
#  endif
    {
      /* #110 If Era is received on channel with GW type NONE, the request is ignored.
       *      Note: Generator ensures that either all channels of the coordinated PNC have GW type NONE or no channel has GW type NONE.
              Managed Channels, mapped to a PNC, always have GW type NONE and thus cannot be coordinated, and therfore do not contribute to the coordination of the PNC. */
#   if (COMM_PNC_EXISTS_GATEWAY_TYPE_NONE == STD_ON)
      if ( ComM_GetGwTypeOfChannel( ComM_GetChannelIdxOfPncSignal( InternalSignalId ) ) != COMM_GATEWAY_TYPE_NONE )
#   endif
      {
        systemUser = ComM_GetSystemUserOfPnc( pncIndex );
        if ( systemUser < ComM_GetSizeOfUser() ) /* PRQA S 2995, 2991 */ /* MD_ComM_2995_2991 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
        {
          if ( (SignalDataRx[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) > 0u ) /* PRQA S 2823 1 */ /* MD_ComM_2823 */
          {
            /* #120 If a PNC is requested via ERA, request the corresponding system user. */
            ComM_PncSetBitInSignal( InternalSignalId, pncIndex COMM_PARTITION_PARA_LAST );

            /* It is insured by the validator that the generated System User of the affected Pnc is always on the COMM_MASTER_PARTITION_IDX */
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
            for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( systemUser ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( systemUser ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));
              ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }

            for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( systemUser ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( systemUser ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
          }
          else
          {
            /* #130 If a PNC is released on all related ERA signals, release the corresponding system user. */
            ComM_PncClearBitInSignal( InternalSignalId, pncIndex COMM_PARTITION_PARA_LAST );

            SigValuesArb = 0u;
            for ( ComM_PncSignalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb( pncIndex ); ComM_PncSignalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb( pncIndex ); ComM_PncSignalMappingIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              signalIndex = ComM_GetPncSignalInd( ComM_PncSignalMappingIndex );
              if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_ERA_RX_TYPEOFPNCSIGNAL ) /* COV_COMM_SUBSET_OF_ELEMENTS */
              {
#   if (COMM_MULTIPARTITION == STD_ON)
                SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
                if ( ComM_GetPncSignalValuesSyncCounter( (ComM_SizeOfPncSignalValuesSyncCounterType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985 */ /* MD_ComM_2985 */
                  ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( signalIndex ) ) )
                  == (uint8)(ComM_GetPncSignalValuesSyncCounter( (ComM_SizeOfPncSignalValuesSyncCounterType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985 */ /* MD_ComM_2985 */
                    COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
                {
                  SigValuesArb |= ComM_GetPncSignalValues( (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985, 2823 */ /* MD_ComM_2985, MD_ComM_2823 */
                    ComM_GetPartitionConfigIdxOfChannel( ComM_GetChannelIdxOfPncSignal( signalIndex ) ) );
                }
                else
#   endif
                {
                  SigValuesArb |= ComM_GetPncSignalValues( (ComM_SizeOfPncSignalValuesType)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), /* PRQA S 2985, 2823 */ /* MD_ComM_2985, MD_ComM_2823 */
                    COMM_MASTER_PARTITION_IDX );
                }
#   if (COMM_MULTIPARTITION == STD_ON)
                SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
#   endif
              }
            }
            if ( (SigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u ) /* COV_COMM_PNC_EXTENDED_SINGLE_ERA */
            {
              SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
              for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( systemUser ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( systemUser ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
              {
                userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));
                ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              }

              for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( systemUser ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( systemUser ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
              {
                userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));
                ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
              }
              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
            }
          }
        }
#    if ( COMM_DEV_ERROR_REPORT == STD_ON )
        else
        {
          errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
          break;
        }
#    endif
      }
    }
  }

#   if ( COMM_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != COMM_E_NO_ERROR ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_COMCBK, errorId ); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
#   endif

} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#  endif
# endif
#endif

#if (( COMM_PNC_SUPPORT == STD_ON ) && ( COMM_USERMODENOTIFUNC == STD_ON ))
/**********************************************************************************************************************
 *  ComM_PncStateChangeNotification()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC(Std_ReturnType, COMM_CODE) ComM_PncStateChangeNotification( void )
{
  ComM_SizeOfUserType userIndex;
  ComM_ModeType lowestUserMode;
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskIndex;
  ComM_PncIterType pncIndex;
  Std_ReturnType retValueUser = RTE_E_OK;
  Std_ReturnType retValueOverall = E_OK;

  /* #10 Iterate over users that have configured User Mode (state change) notification and are mapped to PNC. */
  for (userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++)
  {
    if (ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX ) != NULL_PTR) /* COV_COMM_SUBSET_OF_ELEMENTS */
    {
# if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
      if (ComM_IsPncUserOfUser(userIndex))
# endif
      {
        /* #20 Calculate the lowest mode of all PNCs assigned to the current user. If at least one PNC
         *     is in COMM_PNC_NO_COMMUNICATION mode, the lowest mode is COMM_NO_COMMUNICATION. */
        lowestUserMode = COMM_FULL_COMMUNICATION;

        for (iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser(userIndex); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser(userIndex); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
        {
          for (pncIndex = 0u; (lowestUserMode == COMM_FULL_COMMUNICATION) && (pncIndex < ComM_GetSizeOfPnc()); pncIndex++) /* FETA_COMM_CSL_USE_CASE */
          {
# if (COMM_ISDEF_PNCIDOFPNC == STD_OFF) /* STD_ON: only one PNC exists, no run-time check is needed */
            if (ComM_GetPncIdOfPnc(pncIndex) == ComM_GetPncIDOfUserPncByteMask(iByteMaskIndex)) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
# endif
            {
              if (ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION)
              {
                lowestUserMode = COMM_NO_COMMUNICATION;
              }
            }
          }
        }

        /* #30 If a new mode is available for the current user, notify it using the Mode Switch Interface (sender-receiver) 'ComM_CurrentMode'. */
        if (lowestUserMode != ComM_GetLastStateChange(userIndex, COMM_MASTER_PARTITION_IDX ))
        {
          if (userIndex < ComM_GetSizeOfUserModeNotiFunc( COMM_MASTER_PARTITION_IDX )) /* COV_COMM_GENDATA_CHECK */ /* PRQA S 2995, 2991 */ /* MD_ComM_2995_2991 */
          {
            switch (lowestUserMode)
            {
            case COMM_FULL_COMMUNICATION:
              retValueUser = ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX )(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION); /* SBSW_COMM_UserModeNotiFunc_PointerCall */
              break;
            case COMM_NO_COMMUNICATION:
              retValueUser = ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX )(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION);   /* SBSW_COMM_UserModeNotiFunc_PointerCall */
              break;
              /* PRQA S 2018 1 */ /* MD_ComM_2018 */
            default: /* COV_COMM_MISRA */
              break;
            }
          }

          /* #40 If notification to the current user was successful, update the mode to ensure that the same one will not be notified again.
           *     Otherwise the notification will be processed again in the next task. */
          if (retValueUser == RTE_E_OK)
          {
            ComM_SetLastStateChange(userIndex, lowestUserMode, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_USER_CSL02 */ /*lint !e661 ComM_LastStateChange[] has a size relation to ComM_User[] */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
          else
          {
            retValueOverall = E_NOT_OK;
          }
        }
      }
    }
  }

  return retValueOverall;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  ComM_PncClearWakeup()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_PncClearWakeup( ComM_PncIterType ComM_CurrentPn COMM_PARTITION_TYPE_LAST )
{
# if((COMM_PNC_PS_TIMER > 0u) && ((COMM_SYNCHRONOUS_WAKE_UP == STD_ON) || (COMM_WAKEUPENABLEDOFPNC == STD_ON)))
    /* #10 Clear pnc wakeup flag when a wakeup has occurred before. */
  if(ComM_GetBusPncComModeReq(ComM_CurrentPn, PARTITION_IDX ) == COMM_PNC_PREPARE_SLEEP)
  {
    ComM_SetBusPncComModeReq(ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
# else
  COMM_DUMMY_STATEMENT( ComM_CurrentPn ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif
}
#endif

#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
/**********************************************************************************************************************
 *  ComM_ProcessSynchronousWakeUp()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_ProcessSynchronousWakeUp( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType channel;

  /* ----- Implementation ----------------------------------------------- */
  /* ESCAN00108491: Ensure that all channels, which are not yet in FULL_COMMUNICATION, are woken up by the synchronous passive wakeup event.
   *                A wakeup event shall be handled with higher priority than a shutdown event, i.e. a wakeup event shall not be overwritten by
   *                a sleep command, e.g. ComM_Nm_BusSleepMode(), happening at the same time. */

  /* #10 Check if an EcuM WakeUp - Indication occurred. */
  if ( ComM_IsEcuMWakeUpIndicated( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    /* #11 Wake up every channel which is not in FULL_COMMUNICATION yet by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ )
    {
      if ( ((ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u)
         && (ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX ) != ComM_GetWakeupStateOfChannel( channel )) )
      {
        ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

        /* #12 If NM Gateway Extension is enabled notify Nm_PassiveStartUp() for each channel with Nm. */
# if( COMM_NM_GW_EXT_ENABLED == STD_ON )
#  if (COMM_ISDEF_NMSUPPORTOFCHANNEL == STD_OFF)
        if ( ComM_IsNmSupportOfChannel( channel ) ) /* COV_COMM_SUBSET_OF_ELEMENTS */
#  endif
        {
          (void)Nm_PassiveStartUp( channel );
        }
# endif
      }
    }

    ComM_SetEcuMWakeUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }

  /* #20 Check if an Nm WakeUp-Indication occurred. */
  if ( ComM_IsNmStartUpIndicated( 0u, COMM_MASTER_PARTITION_IDX ) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
  {
    /* #21 Wake up every channel which is not yet in FULL_COMMUNICATION by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ )  /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
    {
# if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
      if ( ComM_IsFrShutdown( channel, ComM_GetPartitionConfigIdxOfChannel(channel) ) )  /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
      {
        /* #22 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
        *      because shutdown on FlexRay shall not be interrupted.
        *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
        ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      else
# endif
      {
        if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )  /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
        {
          ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
      }
    }

    ComM_SetNmStartUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
}
#endif

/**********************************************************************************************************************
 *  ComM_CalculateHighestComMode()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC( ComM_StateType, COMM_CODE ) ComM_CalculateHighestComMode( NetworkHandleType channel, boolean *internalRequest )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_StateType lHighestRequest = COMM_NO_COM_NO_PENDING_REQUEST; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  *internalRequest = FALSE; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */

  /*lint -e661 */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

#if( COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON )
  /* #10 The highest requested mode is COMM_FULL_COM_NETWORK_REQUESTED if the following conditions are true:
   *   Communication shutdown on FlexRay is finished. (This is to counteract the so-called 'clique problem' on FlexRay) and:
   *     1) If Communication Inhibition is not active and at least one user mapped to the channel directly or via PNC requests FULL_COM or
   *     2) Dcm has requested communication (it does not take Communication Inhibition into account). */
  lHighestRequest = COMM_NO_COM_NO_PENDING_REQUEST;
# if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
  /* ComM postpones the execution of communication requests until FrSM indicates entering NO_COM mode.  */
#  if (COMM_MULTIPARTITION == STD_ON)
  if ( !ComM_IsFrShutdown( channel, ComM_GetPartitionConfigIdxOfChannel(channel) ) )
#  else
  if ( !ComM_IsFrShutdown( channel, COMM_MASTER_PARTITION_IDX ) )
#  endif
# endif
  {
# if (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON))
    if (ComM_ChannelIsLimited(channel) == FALSE)
# endif
    {
      if (ComM_IsChannelUserRequestingChannel(channel COMM_MASTER_PARTITION_PARA_LAST) == TRUE)
      {
        lHighestRequest = COMM_FULL_COM_NETWORK_REQUESTED;
        *internalRequest = TRUE; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    /* #110 If Nm variant LINSLAVE is present and a user is requesting the channel, the BusSleepMode flag is cleared for the corresponding channel. */
# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
        /* No runtime check for linslave channel needed, because variable is initialized with FALSE for each channel and only set to TRUE for linslave channels */
        ComM_SetBusSleepModeIndicated( channel, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      else
    /* #120 If no user is requesting the channel and current channel is Nm variant LINSLAVE and the active mode is COMM_FULL_COM_NETWORK_REQUESTED,
          the highest requested mode is COMM_FULL_COM_READY_SLEEP and the externally requested state is set to COMM_NO_COM_NO_PENDING_REQUEST.
          (This ensures that the channel remains in state COMM_FULL_COM_READY_SLEEP after a passive wake-up event and does not transition to COMM_FULL_COM_NETWORK_REQUEST again.) */
      {
        if ( (ComM_GetActiveComMode(channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED)
#  if ( COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF )
          && (ComM_GetNmTypeOfChannel(channel) == COMM_LINSLAVE_NMTYPEOFCHANNEL)
#  endif
           )
        {
          lHighestRequest = COMM_FULL_COM_READY_SLEEP;
          ComM_SetBusComModeReq(channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
# endif
      }
    }

# if (COMM_DCM_INDICATION == STD_ON)
    /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
    if( ComM_IsDcmRequestActive(channel, COMM_MASTER_PARTITION_IDX ) ) /* COV_COMM_NM_LINSLAVE_ONLY XF */
    {
      lHighestRequest = COMM_FULL_COM_NETWORK_REQUESTED;
      *internalRequest = TRUE; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    }
# endif
    /*lint +e443 */
  }
#endif

#if ((COMM_PNC_SUPPORT == STD_ON) && (COMM_PNCCHANNELMAPPING == STD_ON))
  /* #20 If PN feature is enabled and the channel is not requested by a user or Dcm, the requested mode is the highest one of
   *     the mode resulting from the Gateway Type of the channel and the highest activation of PNCs mapped to it or
   *     COMM_FULL_COM_READY_SLEEP if at least one PNC mapped to the channel is in PNC_FULL_COMMUNICATION state. */
# if (COMM_ISDEF_PNCPBINDUSEDOFCHANNELPB == STD_OFF) /* there are some channels without PNCs */
  if (ComM_IsPncPbIndUsedOfChannelPb(channel)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */ /* COV_COMM_SUBSET_OF_ELEMENTS */
# endif
  {
# if (COMM_PNC_ACTIVE_COORD == STD_ON)
    if( lHighestRequest == COMM_NO_COM_NO_PENDING_REQUEST )
    {
      lHighestRequest = ComM_PncGetGwChannelRequest( channel );
      if (lHighestRequest == COMM_FULL_COM_NETWORK_REQUESTED)
      {
        *internalRequest = TRUE; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
      }
    }

#  if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    if( lHighestRequest == COMM_FULL_COM_NETWORK_REQUESTED )
    {
      /* Enter the state COMM_PNCROUTING_NOT_LIMITED because channel is kept awake by a ComM user. */
      ComM_SetPncToChannelRoutingState(channel, ComM_GetPncToChannelRoutingState(channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NOT_LIMITED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
    else
    {
      /* nothing to do */
    }
#  endif
# endif

    /* This to consider all PNCs including not coordinated ones */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    if (ComM_GetPncToChannelRoutingState(channel, COMM_MASTER_PARTITION_IDX ) != COMM_PNCROUTING_LIMITED)
# endif
    {
      lHighestRequest = ComM_PncInFullComWakesUpTheChannel( channel, lHighestRequest ); /* PRQA S 2982 */ /* MD_ComM_2982 */
    }
  }
#endif

  /* #30 The requested mode shall be at least as high as requested via bus.
   *     Bus communication requests shall always be granted according to the 'highest win' strategy. */
#if( COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON )
  if ((lHighestRequest == COMM_NO_COM_NO_PENDING_REQUEST)
# if ((COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) || (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON))
     || (ComM_GetBusComModeReq(channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED)
# endif
     )
#endif
  {
    lHighestRequest = ComM_GetBusComModeReq(channel, COMM_MASTER_PARTITION_IDX );
  }

  /*lint +e661 */

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

  return lHighestRequest;
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

#if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
# if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserMappedToPncWithoutChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(boolean, COMM_CODE) ComM_IsUserMappedToPncWithoutChannels(ComM_UserHandleType User)
{
#  if (COMM_PNCCHANNELMAPPING == STD_ON)
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
  ComM_PncIdxOfUserPncByteMaskType pncIdx = 0u;
#  endif
  boolean isUserMappedToPncWithoutChannels = FALSE;
  if (ComM_IsPncUserOfUser(User))
  {
#  if (COMM_PNCCHANNELMAPPING == STD_ON)
    /* #10 Check special usecase: this PNC user is mapped to PNCs without channels. */
    for (iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser(User); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */ /* COV_COMM_FOR_LOOP_OPTIMIZATION */
    {
      /* #110 Get the internal PNC index from the given PNC identifier. */
      pncIdx = ComM_GetPncIdxOfUserPncByteMask(iByteMaskIndex);
      /* #120 The flag is set to true if PNC has no channels. */
      if (ComM_GetPncChannelMappingStartIdxOfPncPb(pncIdx) == COMM_NO_PNCCHANNELMAPPINGSTARTIDXOFPNCPB) /* PRQA S 2843 */ /* MD_ComM_2842_2843 */ /*lint !e661 */
      {
        isUserMappedToPncWithoutChannels = TRUE; /* PRQA S 2982 */ /* MD_ComM_2982 */
        break;
      }
    }
#  else
    /* #20 The flag is set to true, if the PNC does not have a Channel mapping. */
    isUserMappedToPncWithoutChannels = TRUE;
#  endif
  }
  return isUserMappedToPncWithoutChannels;
}
# endif
#endif

#if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
# if(COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserRequestingChannelOnSatellite()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingChannelOnSatellite( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx = 0u;
  boolean isRequestPending = FALSE;
  ComM_SizeOfUserType user;
  ComM_UserByteMaskStartIdxOfUserType userByteMaskIndex;

  /* ----- Implementation ---------------------------------------------- */
  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
    {
      /* #10 Check if a user request event occurred on a satellite partition for the provided channel and return the pending request status accordingly. */
      if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == channel ) /* COV_COMM_ONLY_ONE_CHANNEL_ASSIGNED_TO_ALL_USERS */
      {
        partitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
        if ( ComM_GetForwardUserReqSyncCounter( user, PARTITION_IDX ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          if ( ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), PARTITION_IDX ) != COMM_NO_REQUESTS ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
          {
            isRequestPending = TRUE;
            break;
          }
        }
        else
        {
          if ( ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
          {
            isRequestPending = TRUE;
            break;
          }
        }
      }
    }
    if ( isRequestPending == TRUE )
    {
      break;
    }
  }

  COMM_DUMMY_STATEMENT_CONST( channel );

  return isRequestPending;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

# if(( COMM_PNC_SUPPORT == STD_ON ) && ( COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON ))
#  if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserRequestingPncOnSatellite()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingPncOnSatellite( ComM_PncIterType pncIndex )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
  ComM_SizeOfUserType user;
  ComM_PartitionIdentifiersIterType partitionIdx = 0u;
  boolean isRequestPending = FALSE;

  /* ----- Implementation ---------------------------------------------- */
  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    partitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
    for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
          userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( pncIndex );
          userReqPncFullComIndex++ )
    {
      if ( userReqPncFullComIndex == ComM_GetUserReqPncFullComIdxOfUserPncByteMask( ComM_GetUserPncByteMaskStartIdxOfUser( user ) ) )
      {
        /* #10 Check if a user request event occurred on a satellite partition for the provided PNC and return the pending request status accordingly. */
        if ( ComM_GetForwardUserReqSyncCounter( user, PARTITION_IDX ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, PARTITION_IDX ) != COMM_NO_REQUESTS ) /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
          {
            isRequestPending = TRUE;
            break;
          }
        }
        else
        {
          if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
      }
    }
    if ( isRequestPending == TRUE )
    {
      break;
    }
  }

  COMM_DUMMY_STATEMENT_CONST( pncIndex );

  return isRequestPending;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#  endif
# endif
#endif

#if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsChannelUserRequestingChannel()
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
COMM_LOCAL_INLINE FUNC(boolean, COMM_CODE) ComM_IsChannelUserRequestingChannel( NetworkHandleType channel COMM_PARTITION_TYPE_LAST )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isUserRequesting = FALSE;
  ComM_UserReqFullComEndIdxOfChannelPbType userReqFullComIndex;
# if (COMM_PNC_ACTIVE_COORD == STD_ON)
  ComM_UserByteMaskEndIdxOfUserType userByteMaskIndex;
  ComM_SizeOfUserType user;
#  if ((COMM_MULTIPARTITION == STD_ON) && (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON))
  ComM_PartitionIdentifiersIterType userPartitionIdx = 0u;
#  endif
# endif

  /* #10 PNC Coordinator is enabled and at least one channel or PNC user is requesting FULL_COM:
   *     Return TRUE if the channel has no PNCs assigned, otherwise iterate over channel users to check if one of them is requesting FULL_COM.
   *     Note that PNC users are handled separately because of dependency to the Gateway Type of channel. */

  /* #20 PNC Coordinator is disabled and at least one channel or PNC user is requesting FULL_COM:
   *     Return TRUE, because there is no dependency to the Gateway Type of channel. */

  /*lint -e443 */
  /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
  for (userReqFullComIndex = ComM_GetUserReqFullComStartIdxOfChannelPb(channel); userReqFullComIndex < ComM_GetUserReqFullComEndIdxOfChannelPb(channel); userReqFullComIndex++) /* FETA_COMM_CSL_USE_CASE */
  {
    /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
    if ((ComM_GetUserReqFullCom(userReqFullComIndex, PARTITION_IDX ) != COMM_NO_REQUESTS) && (isUserRequesting == FALSE)) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
    {
      /* At least one user assigned to this channel requests FULL_COM. The 2nd condition is because there is no downgrade of highest request. */
# if (COMM_PNC_ACTIVE_COORD == STD_ON)
#  if (COMM_ISDEF_PNCPBINDUSEDOFCHANNELPB == STD_OFF) /* there are some channels without PNCs */
      if (!ComM_IsPncPbIndUsedOfChannelPb(channel)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */ /* COV_COMM_SUBSET_OF_ELEMENTS */
      {
        isUserRequesting = TRUE;
      }
      else
#  endif
      {
        for (user = 0u; user < ComM_GetSizeOfUser(); user++)
        {
          if (!ComM_IsPncUserOfUser(user))
          {
            for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
            {
              if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == channel )
              {
#  if ((COMM_MULTIPARTITION == STD_ON) && (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON))
                userPartitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
                if ( ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
                {
                  if ( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), userPartitionIdx ) &
                       ComM_GetSetMaskOfUserByteMask( userByteMaskIndex )) != COMM_NO_REQUESTS ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
                  {
                    isUserRequesting = TRUE;
                    break;
                  }
                }
                else
#  endif
                {
                  if ( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) &
                        ComM_GetSetMaskOfUserByteMask( userByteMaskIndex )) != COMM_NO_REQUESTS )
                  {
                    isUserRequesting = TRUE;
                    break;
                  }
                }
              }
            }
          }
        }
      }
# else
      isUserRequesting = TRUE;
# endif
    }
  }

  /* if ((COMM_CONFIGURATION_VARIANT == COMM_CONFIGURATION_VARIANT_PRECOMPILE) && (ComM_GetSizeOfChannel() == 1u)) */
  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return isUserRequesting;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  ComM_ChannelIsRequestPending()
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
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_ChannelIsRequestPending( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isRequestPending = FALSE;
#if((COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_OFF))
  ComM_UserReqFullComEndIdxOfChannelPbType userReqFullComIndex;
#endif
#if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  ComM_StateType busComModeReqFrShutdown = COMM_NO_COM_NO_PENDING_REQUEST;
#endif
  ComM_StateType busComModeReq = COMM_NO_COM_NO_PENDING_REQUEST;

#if( COMM_PNC_SUPPORT == STD_ON )
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  boolean isUserRequesting = FALSE;
#  if ( COMM_MULTIPARTITION == STD_ON )
#   if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
  ComM_PartitionIdentifiersIterType partitionIdx = 0u;
#   endif
  ComM_SizeOfUserType user;
#  endif
# endif

# if( COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON )
#  if( COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON )
  ComM_PncIterType pncIndex = 0u;
#   if ( COMM_MULTIPARTITION == STD_OFF )
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex = 0u;
#   endif
#  endif
# endif
#endif

  /* #10 If a passive wake-up occurred on the channel, return a pending request. */
#if ( COMM_MULTIPARTITION == STD_ON )

# if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  ComM_IsRequestPendingOnSatellite( channel, &busComModeReq, &busComModeReqFrShutdown ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
# else
  ComM_IsRequestPendingOnSatellite( channel, &busComModeReq ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
# endif
#else
# if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown(channel, COMM_MASTER_PARTITION_IDX);
# endif
  busComModeReq = ComM_GetBusComModeReq(channel, COMM_MASTER_PARTITION_IDX);
#endif

  if(busComModeReq != COMM_NO_COM_NO_PENDING_REQUEST )
  {
    isRequestPending = TRUE;
  }

  /* #20 If a passive wake-up occurred on a FlexRay channel during shutdown, return a pending request. */
#if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  else if(busComModeReqFrShutdown != COMM_NO_COM_NO_PENDING_REQUEST)
  {
    isRequestPending = TRUE;
  }
#endif

  /* #30 If at least one user is requesting the channel and the channel is not limited, return a pending request. */
#if( COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON )
  else
  {
# if( ( (COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF) ) || (COMM_WAKEUP_INHIBITION == STD_ON) )
    if ( ComM_ChannelIsLimited( channel ) == FALSE )
# endif
    {
# if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON )
      isRequestPending = ComM_IsUserRequestingChannelOnSatellite( channel );
# else
      for ( userReqFullComIndex = ComM_GetUserReqFullComStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
            userReqFullComIndex < ComM_GetUserReqFullComEndIdxOfChannelPb( channel );
            userReqFullComIndex++ )
      {
        if ( ComM_GetUserReqFullCom( userReqFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
        {
          isRequestPending = TRUE;
          break;
        }
      }
# endif
    }

    /* If PNC Routing Limitation is enabled, reject the request if no channel user requests FULL_COM and the channel has limited routing */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    if ( isRequestPending == TRUE )
    {
#  if ( COMM_MULTIPARTITION == STD_ON )
      for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
      {
#   if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
        partitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
        if ( ComM_GetForwardUserReqSyncCounter( user, PARTITION_IDX ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          isUserRequesting = ComM_IsChannelUserRequestingChannel( channel COMM_PARTITION_PARA_LAST );
        }
        else
#   endif
        {
          isUserRequesting = ComM_IsChannelUserRequestingChannel( channel COMM_MASTER_PARTITION_PARA_LAST );
        }
      }
#  else
      isUserRequesting = ComM_IsChannelUserRequestingChannel( channel COMM_MASTER_PARTITION_PARA_LAST );
#  endif

      if ( (isUserRequesting == FALSE) && (ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED) )
      {
        isRequestPending = FALSE;
      }
    }
# endif
  }
#endif

    /* #40 If Extended PNC Functionality is enabled, a PNC may have no channels assigned. If a user is requesting such a PNC,
     *     return a pending request for each channel to ensure ECU staying in RUN mode. */
#if( COMM_PNC_SUPPORT == STD_ON )
# if( COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON )
#  if( COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON )
  for( pncIndex = 0u; /* FETA_COMM_CSL_USE_CASE */
       (pncIndex < ComM_GetSizeOfPncPb()) && (isRequestPending == FALSE); /* break the loop as soon as a pending request is detected */
       pncIndex++ )
  {
#   if( COMM_PNCCHANNELMAPPING == STD_ON )
    /* The condition is true if PNC has no channels */
    if (ComM_GetPncChannelMappingStartIdxOfPncPb(pncIndex) == COMM_NO_PNCCHANNELMAPPINGSTARTIDXOFPNCPB)
#   endif
    {
#   if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
      isRequestPending = ComM_IsUserRequestingPncOnSatellite( pncIndex );
#   else
      for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
            userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( pncIndex );
            userReqPncFullComIndex++ )
      {
        if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
        {
          isRequestPending = TRUE;
          break;
        }
      }
#   endif
    }
  }
#  endif
# endif
#endif

  return isRequestPending;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (COMM_PNC_ACTIVE_COORD == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncGetGwChannelRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelRequest( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncIterType pncIndex;
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
  ComM_StateType gwChannelComMode = COMM_NO_COM_NO_PENDING_REQUEST;
  ComM_PncModeType highestPNComModeReq = COMM_PNC_NO_COMMUNICATION;
  ComM_PncModeType tmpPNComModeReq = COMM_NO_COM_NO_PENDING_REQUEST;
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  boolean eraRequestReceivedOnChannel = FALSE;
  boolean pncRequestIsPending = FALSE;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over PNCs mapped to the channel and calculate the highest requested mode of the PNCs.
   *     Break the iteration once the highest possible mode (COMM_PNC_REQUESTED) is detected. */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  if(ComM_PncIsEraRequestReceivedOnChannel(channel) == TRUE)
  {
    /* Routing to this channel is not limited because an ERA signal containing a PNC request is received on the channel */
    eraRequestReceivedOnChannel = TRUE;
  }
# endif

  for (pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb(channel); /* FETA_COMM_CSL_USE_CASE */
      (pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb(channel)) && (highestPNComModeReq != COMM_PNC_REQUESTED);
       pncPbIndIter++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
  {
    pncIndex = (ComM_PncIterType)ComM_GetPncPbInd(pncPbIndIter);

# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
    /* Parameter pncCanBeRequested is TRUE because we first need the potentially highest PNC request and will consider the channel' limitation state later. */
    tmpPNComModeReq = ComM_CalculateHighestPNComMode(pncIndex, TRUE);
    if (tmpPNComModeReq >= COMM_PNC_REQUESTED)
    {
      pncRequestIsPending = TRUE;
      /* PNC mode may control the channel mode only if Routing Limitation state of the channel is COMM_PNCROUTING_NOT_LIMITED,
       * i.e. if routing of this PNC is not limited on the channel or limitation is de-activated due to reception of ERA=1. Otherwise ignore the PNC mode.
       * Note: PNC mode does not control the channel mode if limitation is de-activated because Nm is in state NM_REPEAT_MESSAGE or
       * if PNC is woken up by EIRA=1 (not ERA). */
      if((!ComM_IsPncLimitation(channel, pncIndex, COMM_MASTER_PARTITION_IDX )) || (eraRequestReceivedOnChannel == TRUE)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        highestPNComModeReq = tmpPNComModeReq;
      }
    }
# else
    /* Found a PN assigned to the current channel. Calculate the highest PN requested mode of all PNs on the channel */
    tmpPNComModeReq = ComM_CalculateHighestPNComMode(pncIndex, TRUE); /* the 2nd parameter is not evaluated */
    if (tmpPNComModeReq >= COMM_PNC_REQUESTED)
    {
      highestPNComModeReq = tmpPNComModeReq;
    }
# endif
  }

  /* #20 Get the requested channel state depending on the highest requested mode of the PNCs and the Gateway Type of the channel. */
  gwChannelComMode = ComM_PncGetGwChannelState(channel, highestPNComModeReq);

  /* #30 If PNC Routing Limitation is enabled, update the limitation state of the channel. */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  ComM_PncRoutingLimitationSetState( channel, highestPNComModeReq, pncRequestIsPending );
# endif

  return gwChannelComMode;
}
#endif

#if (COMM_PNC_ACTIVE_COORD == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncGetGwChannelState()
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
COMM_LOCAL_INLINE FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelState( NetworkHandleType channel, ComM_PncModeType highestPNComModeReq )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
   * Therefore the function is not allowed to change the module' state i.e. write global variables.
   */

  /* ----- Local Variables ---------------------------------------------- */
  ComM_StateType gwChannelComMode = COMM_NO_COM_NO_PENDING_REQUEST;
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
#  if (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON)
  ComM_GwTypeOfChannelType gwTypeOfChannel = ComM_GetGwTypeOfChannel(channel);
#  endif
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The requested channel state depends on the highest requested mode of the PNCs and the Gateway Type of the channel:
   *     COMM_FULL_COM_NETWORK_REQUESTED if at least one PNC on the channel
   *      1) is requested internally by a PNC user or externally via ERA that is originated from a channel with Gateway Type ACTIVE.
   *      2) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel is a managed channel.
   *      3) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel has Gateway Type ACTIVE.
   *      4) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel has Gateway Type PASSIVE, if the PNC-0-Vector Avoidance feature is enabled.
   *     COMM_FULL_COM_READY_SLEEP otherwise and if at least one PNC on the channel is requested externally via ERA that
   *     is originated from a channel with Gateway Type PASSIVE and the current channel has Gateway Type PASSIVE.
   *     COMM_NO_COM_NO_PENDING_REQUEST otherwise. */
  if( highestPNComModeReq == COMM_PNC_REQUESTED )
  {
    gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
  }
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
  else if( highestPNComModeReq == COMM_PNC_REQUESTED_PASSIVE )
  {
#  if (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON)
    if( gwTypeOfChannel == COMM_GATEWAY_TYPE_ACTIVE ) /* COV_COMM_SUBSET_OF_ELEMENTS */
    {
      /* Channels with GW type ACTIVE shall be requested actively. */
      gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
    }
    else
#  endif
#  if (COMM_PNC_EXISTS_COORD_PNC_WITH_GW_PASSIVE_AND_MANAGED_CHANNELS== STD_ON)
      /* Note: A GW type check is not needed as a managed channel always has GW type NONE. */
    if ( ComM_GetManagingChannelIdOfChannel( channel ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL )
    {
      /* Managed Channels with GW type NONE shall be requested actively. */
      gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
    }
    else
#  endif
    {
   /* If PNC-0-Vector Avoidance feature is enabled
     * Channels with GW type PASSIVE shall not be requested actively. Note: no check for Gw type needed because it is ensured
     * per validation rule that a coordinated channel always has GW type ACTIVE or PASSIVE and never has Gw type NONE.
     * Managed channels always have GW type NONE and do not contribute to the coordination of the PNC. */
#  if (COMM_PNC_0_VECTOR_AVOIDANCE == STD_ON)
      gwChannelComMode = COMM_FULL_COM_READY_SLEEP;
#  else
    /* Cannels with GW type PASSIVE shall be requested actively if the feature PNC-0-Vector Avoidance is not enabled. */
      gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
#  endif
    }
  }
# endif
  else
  {
    gwChannelComMode = COMM_NO_COM_NO_PENDING_REQUEST;
  }

# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_OFF)
  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# else
#  if (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_OFF)
  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif

  return gwChannelComMode;
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncRoutingLimitationSetState()
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
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncRoutingLimitationSetState( CONST(NetworkHandleType, AUTOMATIC) channel, CONST(ComM_PncModeType, AUTOMATIC) highestPNComModeReq, CONST(boolean, AUTOMATIC) pncRequestIsPending )
{
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;

  if( highestPNComModeReq >= COMM_PNC_REQUESTED )
  {
    /* #10 Enter the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED if
     *     1) ERA=1 is received on the channel or
     *     2) ComM user requests FULL_COM for a PNC, which is not limited on the channel. */
    ComM_SetPncToChannelRoutingState(channel, ComM_GetPncToChannelRoutingState(channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NOT_LIMITED, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
  else if( pncRequestIsPending == TRUE )
  {
    /* #20 Exit the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED if
     *     1) All ComM users assigned to the channel request NO_COM and
     *     2) ERA=0 is received on the channel and
     *     3) All PNCs being in state PNC_REQUESTED are limited on the channel.
     *     Clear the COMM_PNCROUTING_NOT_LIMITED flag, note that Nm can be in state NM_STATE_REPEAT_MESSAGE. */
    ComM_SetPncToChannelRoutingState(channel, ComM_GetPncToChannelRoutingState(channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED_CLEAR, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
  else
  {
    /* #30 Otherwise and if at least one PNC on the channel is limited, exit the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED. */
    /* This is needed e.g. for the following scenario: a PNC is limited on all channels, requesting the PNC via user shall not activate it. */
    for (pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb(channel); /* FETA_COMM_CSL_USE_CASE */
         pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb(channel);
         pncPbIndIter++)
    {
      if (ComM_IsPncLimitation(channel, ComM_GetPncPbInd(pncPbIndIter), COMM_MASTER_PARTITION_IDX )) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        ComM_SetPncToChannelRoutingState(channel, ComM_GetPncToChannelRoutingState(channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED_CLEAR, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        break;
      }
    }
  }
}
# endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncIsEraRequestReceivedOnChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC( boolean, COMM_CODE ) ComM_PncIsEraRequestReceivedOnChannel( NetworkHandleType  Channel )
{
  boolean isEraRequestReceivedOnChannel = FALSE;
  ComM_PncSignalValuesEndIdxOfPncSignalType pncSignalByteIndex = 0u;
  ComM_SizeOfPncSignalType pncSignalIndex = 0u;

  for(pncSignalIndex = 0u; pncSignalIndex < ComM_GetSizeOfPncSignal(); pncSignalIndex++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
  {
    if((ComM_GetTypeOfPncSignal(pncSignalIndex) == COMM_ERA_RX_TYPEOFPNCSIGNAL) && (ComM_GetChannelIdxOfPncSignal(pncSignalIndex) == Channel))
    {
      for(pncSignalByteIndex = ComM_GetPncSignalValuesStartIdxOfPncSignal(pncSignalIndex); pncSignalByteIndex < ComM_GetPncSignalValuesEndIdxOfPncSignal(pncSignalIndex); pncSignalByteIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if(ComM_GetPncSignalValues( pncSignalByteIndex, COMM_MASTER_PARTITION_IDX ) != 0x00u)
        {
          /* #10 Return TRUE if at least one PNC is requested via ERA_RX signal on the given channel. Return FALSE otherwise. */
          isEraRequestReceivedOnChannel = TRUE;
          break;
        }
      }
      break; /* Because there is one ERA_RX signal per channel at most. */
    }
  }

  return isEraRequestReceivedOnChannel;
}
# endif
#endif

#if ((COMM_PNC_SUPPORT == STD_ON) && (COMM_PNCCHANNELMAPPING == STD_ON))
/**********************************************************************************************************************
 *  ComM_PncInFullComWakesUpTheChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(ComM_StateType, COMM_CODE) ComM_PncInFullComWakesUpTheChannel(NetworkHandleType channel, ComM_StateType highestRequest)
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncIterType pncIndex;
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
  ComM_StateType localhighestRequest = highestRequest;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return COMM_FULL_COM_READY_SLEEP if current highest request is COMM_NO_COM_NO_PENDING_REQUEST and
   *   at least one PNC on this channel is in PNC_FULL_COMMUNICATION mode.
   *   Otherwise return the current highest request (no changes). */

  /* Note: if COMM_FULL_COM_READY_SLEEP ComM ensures that the channel is at least in COMM_FULL_COM_READY_SLEEP state. */
  /* ESCAN00066290: There is no downgrade of highest request. If detected COMM_FULL_COM_NETWORK_REQUESTED, then no need to check PNCs anymore */
  if (localhighestRequest == COMM_NO_COM_NO_PENDING_REQUEST)
  {
    for (pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb(channel); /* FETA_COMM_CSL_USE_CASE */
         pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb(channel);
         pncPbIndIter++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
    {
      pncIndex = (ComM_PncIterType)ComM_GetPncPbInd(pncPbIndIter);

      if(ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX) != COMM_PNC_NO_COMMUNICATION)
      {
        localhighestRequest = COMM_FULL_COM_READY_SLEEP;
        break;
      }
    }
  }

# if (COMM_ISDEF_PNCPBINDSTARTIDXOFCHANNELPB == STD_ON)
  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return localhighestRequest;
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncIsOnlySystemUserRequesting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE) ComM_PncIsOnlySystemUserRequesting( ComM_PncIterType pncIndex, ComM_UserReqPncFullComIterType byteIndex )
{
  boolean retValue = FALSE;
  ComM_UserPncByteMaskStartIdxOfUserType uSysUserByteMaskIndex = 0u;

  /* #10 If the provided byteIndex corresponds to the system user, check whether a request from another PNC user (not the system user)
   *     is stored in this byte. Return TRUE if only a request of the system user is stored in the request byte.
   *     Note: if PNC is synchronized, there is exactly one system user and one byte index where the system user' request is stored. */
#  if (COMM_ISDEF_COORDINATEDOFPNC == STD_OFF)
  if(ComM_IsCoordinatedOfPnc(pncIndex)) /* COV_COMM_SUBSET_OF_ELEMENTS */
#  endif
  {
    uSysUserByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser(ComM_GetSystemUserOfPnc(pncIndex));
    if (byteIndex == ComM_GetUserReqPncFullComIdxOfUserPncByteMask(uSysUserByteMaskIndex))
    {
      if (((ComM_GetUserReqPncFullCom(byteIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask(uSysUserByteMaskIndex)) == COMM_NO_REQUESTS))
      {
        retValue = TRUE;
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST(pncIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  return retValue;
}
# endif
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
# if ( (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON) && (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON) )
/**********************************************************************************************************************
 *  ComM_PncIsEraReceivedOnActiveChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE) ComM_PncIsEraReceivedOnActiveChannel( ComM_PncIterType pncIndex )
{
  ComM_PncSignalIndEndIdxOfPncPbType pncSignalIndIter;
  ComM_SizeOfPncSignalType  pncSignalInd;
  boolean retValue = FALSE;
  ComM_GwTypeOfChannelType gwTypeOfChannel = COMM_GATEWAY_TYPE_NONE;

  /* #10 Return TRUE if at least one PNC request is received via ERA on a channel with GW type ACTIVE. Otherwise return FALSE. */
  /* Note: if PNC is coordinated if an ERA signal is mapped to it */
  for (pncSignalIndIter = ComM_GetPncSignalIndStartIdxOfPncPb(pncIndex); /* FETA_COMM_CSL_USE_CASE */
       pncSignalIndIter < ComM_GetPncSignalIndEndIdxOfPncPb(pncIndex);
       pncSignalIndIter++)
  {
    pncSignalInd = ComM_GetPncSignalInd(pncSignalIndIter);
    if (ComM_GetTypeOfPncSignal(pncSignalInd) == COMM_ERA_RX_TYPEOFPNCSIGNAL) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
    {
      gwTypeOfChannel = ComM_GetGwTypeOfChannel(ComM_GetChannelIdxOfPncSignal(pncSignalInd));
      if ( (gwTypeOfChannel == COMM_GATEWAY_TYPE_ACTIVE) && (ComM_IsSignalStoredPncBitSet(pncSignalInd, pncIndex, COMM_MASTER_PARTITION_IDX )) ) /* PRQA S 2985 */ /* MD_ComM_2985 */ /* COV_COMM_PNC_EXTENDED_NO_ERA_ON_ACTIVE_CHANNEL */
      {
        retValue = TRUE;
        break;
      }
    }
  }

#  if (COMM_ACTIVE_PNC == 1u)
  COMM_DUMMY_STATEMENT_CONST(pncIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
  return retValue;
}
# endif
#endif

#if ((COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON)))
/**********************************************************************************************************************
 *  ComM_ChannelIsLimited()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(boolean, COMM_CODE) ComM_ChannelIsLimited( NetworkHandleType channel )
{
  boolean retValue = FALSE;

  /* #10 Return TRUE if Mode Limitation or Prevent Wake-up is active on the channel.
   *     Otherwise return FALSE. */
  if (
# if ((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF))
     /* PRQA S 3415 1 */ /* MD_ComM_3415 */
     (ComM_NoComLimitationActive(channel) == TRUE)
#  if (COMM_WAKEUP_INHIBITION == STD_ON)
      ||
#  endif
# endif
# if (COMM_WAKEUP_INHIBITION == STD_ON)
     /* PRQA S 3415 1 */ /* MD_ComM_3415 */
     (ComM_PreventWakeUpActive(channel) == TRUE)
# endif
     )
  {
    retValue = TRUE;
  }

  return retValue;
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_CalculateHighestPNComMode()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC( ComM_PncModeType, COMM_CODE) ComM_CalculateHighestPNComMode( ComM_PncIterType pncIndex, boolean pncCanBeRequested )
{
  ComM_PncModeType lHighestRequest = COMM_PNC_NO_COMMUNICATION; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  ComM_UserReqPncFullComEndIdxOfPncPbType  userReqPncFullComIndex;
# if ( (COMM_PNCCHANNELMAPPING == STD_ON) && \
      ((COMM_MODE_LIMITATION == STD_ON) || (COMM_WAKEUP_INHIBITION == STD_ON)) )
#  if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
  ComM_PncChannelMappingEndIdxOfPncPbType pncChMappingIndex;
#  endif
# endif

  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  /* The guard below is needed to inhibit FULL_COM request to a PNC if all its channels have
   * Routing Limitation state COMM_PNCROUTING_LIMITED (pncCanBeRequested == FALSE). */
  if( pncCanBeRequested == TRUE )
# endif
  {
    /* #10 Iterate through PNC user request bytes in ComM_UserReqPncFullCom[] for the given PN.
     *     If at least one user request is detected, set the requested mode to
     *     COMM_PNC_REQUESTED_PASSIVE if only system user is requesting and Passive Gw Type is supported or
     *     COMM_PNC_REQUESTED otherwise. */
    /* PRQA S 2823 2 */ /* MD_ComM_2823 */
    for (userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb(pncIndex); /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
         userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb(pncIndex);
         userReqPncFullComIndex++)
    {
      /* PRQA S 2842, 2823 */ /* MD_ComM_2842_2843, MD_ComM_2823 */
      if (ComM_GetUserReqPncFullCom(userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX) != COMM_NO_REQUESTS)
      {
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
        lHighestRequest = COMM_PNC_REQUESTED_PASSIVE;
        if (ComM_PncIsOnlySystemUserRequesting(pncIndex, userReqPncFullComIndex) == FALSE)
# endif
        {
          lHighestRequest = COMM_PNC_REQUESTED; /* PRQA S 2982 */ /* MD_ComM_2982 */
          break; /* Highest requested mode found */
        }
      }
    }

    /* #20 If Passive Gw Type is supported and PNC activation is received via ERA from an Active channel,
     *     set the requested mode to COMM_PNC_REQUESTED. */
# if (COMM_PNC_PASSIVE_GW_SUPPORT == STD_ON)
#  if (COMM_PNC_EXISTS_GATEWAY_TYPE_ACTIVE == STD_ON)
    if (lHighestRequest == COMM_PNC_REQUESTED_PASSIVE)
    {
      if (ComM_PncIsEraReceivedOnActiveChannel(pncIndex) == TRUE) /* COV_COMM_PNC_EXTENDED_NO_ERA_ON_ACTIVE_CHANNEL */
      {
        lHighestRequest = COMM_PNC_REQUESTED;
      }
    }
#  endif
# endif

    /* #30 If Node Limitation NO_COM or Prevent Wake-up is active on at least one PNC channel, ComM shall inhibit FULL_COM requests. */
# if ( (COMM_PNCCHANNELMAPPING == STD_ON) && \
       ((COMM_MODE_LIMITATION == STD_ON) || (COMM_WAKEUP_INHIBITION == STD_ON)) )
#  if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
    /* PRQA S 2823 2 */ /* MD_ComM_2823 */
    for (pncChMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb(pncIndex); /* FETA_COMM_CSL_USE_CASE */
         pncChMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb(pncIndex);
         pncChMappingIndex++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
    {
      if (ComM_ChannelIsLimited(ComM_GetPncChannelMapping(pncChMappingIndex)) == TRUE) /* COV_COMM_PNC_ON_CHANNELS_NM_PASSIVE */
      {
        lHighestRequest = COMM_PNC_NO_COMMUNICATION;
        break;
      }
    }
#  else
    {
      lHighestRequest = COMM_PNC_NO_COMMUNICATION;
    }
#  endif
# endif
  }

  /* Do not allow PNC activation if pncCanBeRequested == FALSE, but always allow PNC de-activation */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  if ((pncCanBeRequested == TRUE) || (ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX ) != COMM_PNC_NO_COMMUNICATION))
# endif
  {
    /* #40 The requested mode shall be at least as requested via bus (EIRA). */
    if (ComM_GetBusPncComModeReq(pncIndex, COMM_MASTER_PARTITION_IDX ) > lHighestRequest)
    {
      lHighestRequest = ComM_GetBusPncComModeReq(pncIndex, COMM_MASTER_PARTITION_IDX );
    }
# if (COMM_PNC_PS_TIMER > 0u)
    else if ((ComM_GetPncPSleepTimer(pncIndex, COMM_MASTER_PARTITION_IDX ) > 0u) && (lHighestRequest <= COMM_PNC_PREPARE_SLEEP)) /* PRQA S 2995 */ /* MD_ComM_2995_2991 */ /* COV_COMM_PNC_ON_CHANNELS_NM_PASSIVE */
    {
      lHighestRequest = COMM_PNC_PREPARE_SLEEP;
    }
    else
    {
      /* Nothing to do. Statement is needed for MISRA-C:2004 compliance (Rule 14.10). */
    }
# endif
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_OFF)
  COMM_DUMMY_STATEMENT_CONST(pncCanBeRequested); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return lHighestRequest;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  ComM_SynchronizedPncShutdownHandling()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SynchronizedPncShutdownHandling( ComM_PncIterType ComM_CurrentPn )
{
  ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;

  /* #10 Determines if a Synchronized PNC Shutdown should be forwarded towards the Nm for the provided PNC, if an ECU is
   *      1) in the role of an intermediate PNC Coordinator
   *      2) in the role of a Top-Level PNC Coordinator */

# if ( COMM_INTERMEDIATEPNCCOORDINATOROFPNC == STD_ON )
#  if ( COMM_ISDEF_INTERMEDIATEPNCCOORDINATOROFPNC == STD_OFF )
  if ( ComM_IsIntermediatePncCoordinatorOfPnc( ComM_CurrentPn ) )
#  endif
  {
    if ( ComM_IsForwardSynchronizedPncShutdownIndicated( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) )
    {
      for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( ComM_CurrentPn ); /* FETA_COMM_CSL_USE_CASE */
            pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( ComM_CurrentPn ); pncChIdx++ )
      {
        if ( ComM_GetGwTypeOfChannel( ComM_GetPncChannelMapping( pncChIdx ) ) == COMM_GATEWAY_TYPE_ACTIVE )
        {
          (void)Nm_RequestSynchronizedPncShutdown( ComM_GetPncChannelMapping( pncChIdx ), ComM_GetPncIdOfPnc( ComM_CurrentPn ) );
        }
      }
      ComM_SetForwardSynchronizedPncShutdownIndicated( ComM_CurrentPn, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
  }
#  if ( COMM_ISDEF_INTERMEDIATEPNCCOORDINATOROFPNC == STD_OFF )
  else
#  endif
# endif

# if ( COMM_TOPLEVELPNCCOORDINATOROFPNC == STD_ON )
#  if ( COMM_ISDEF_TOPLEVELPNCCOORDINATOROFPNC == STD_OFF )
  if ( ComM_IsTopLevelPncCoordinatorOfPnc( ComM_CurrentPn ) )
#  endif
  {
    for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( ComM_CurrentPn ); /* FETA_COMM_CSL_USE_CASE */
          pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( ComM_CurrentPn ); pncChIdx++ )
    {
      (void)Nm_RequestSynchronizedPncShutdown( ComM_GetPncChannelMapping( pncChIdx ), ComM_GetPncIdOfPnc( ComM_CurrentPn ) );
    }
  }
#  if ( COMM_ISDEF_TOPLEVELPNCCOORDINATOROFPNC == STD_OFF )
  else
#  endif
# endif
  {
    /* Intentionally left empty.*/
  }

}
#endif

#if (COMM_WAKEUP_INHIBITION == STD_ON)
/*********************************************************************************************************************
 *  ComM_PreventWakeUpActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(boolean, COMM_CODE) ComM_PreventWakeUpActive( NetworkHandleType channel )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
   * Therefore the function is not allowed to change the module' state i.e. write global variables.
   */

  boolean retValue = FALSE;

  /* #10 Return TRUE if Prevent Wake-up
   *     1) is allowed by ECU Group Classification and
   *     2) is active on the channel and
   *     3) the channel is not in FULL_COM mode. */
  if( (ComM_GetInhibition(COMM_MASTER_PARTITION_IDX).ComM_ECUGroupClassification & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
  {
    /* check if prevent wake up is active */
    if( (ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[channel] & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
    {
      if((ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u)
      {
        /* The inhibition shall not get active in FULL_COM mode */
        retValue = TRUE;
      }
    }
  }

  return retValue;
}
#endif

#if ((COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_MODE_LIMITATION == STD_ON) && ( COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF ))
/*********************************************************************************************************************
 *  ComM_NoComLimitationActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(boolean, COMM_CODE) ComM_NoComLimitationActive( NetworkHandleType channel )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
   * Therefore the function is not allowed to change the module' state i.e. write global variables.
   */

  boolean retValue = FALSE;

  /* #10 Return TRUE if Mode Limitation to NO_COM
   *     1) is active because of NmType PASSIVE or
   *     2) is allowed by ECU Group Classification and
   *     3) is active on the channel */

# if (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON)
  if (ComM_GetNmTypeOfChannel(channel) == COMM_PASSIVE_NMTYPEOFCHANNEL)
  {
    retValue = TRUE;
  }
  else
# endif
  {
    if( (ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_ECUGroupClassification & COMM_MODE_LIMITATION_MASK) == COMM_MODE_LIMITATION_MASK )
    {
      if( (ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[channel] & COMM_MODE_LIMITATION_MASK) == COMM_MODE_LIMITATION_MASK )
      {
        retValue = TRUE;
      }
    }
  }

  return retValue;
}
#endif


#if (COMM_FULL_COMM_REQUEST_NOTIF == STD_ON)
/*********************************************************************************************************************
 *  ComM_CurrentChannelRequestUpdate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_CurrentChannelRequestUpdate( NetworkHandleType channel )
{
  ComM_SizeOfFullComRequestersType currentNumOfRequesters = 0u; /* Number of users requesting the channel */
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
  ComM_SizeOfUserType userIndex;

  /* #10 Initialize the list with the default user handle. */
  for (userIndex = 0u; userIndex < ComM_GetSizeOfFullComRequesters( COMM_MASTER_PARTITION_IDX ); userIndex++)
  {
    ComM_SetFullComRequesters( userIndex, 0xFFu, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }

  /* #20 If communication inhibition is active all FULL_COM user requests for this channel are rejected. */
# if ( ( (COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF) ) || (COMM_WAKEUP_INHIBITION == STD_ON) )
  if (ComM_ChannelIsLimited(channel) == FALSE)
# endif
  {
    /* #30 Otherwise store handles of users that are mapped to the current channel and requesting FULL_COM. */
    for(userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++)
    {
# if ( COMM_PNC_SUPPORT == STD_ON )
      if (!ComM_IsPncUserOfUser(userIndex))
# endif
      {
        for(iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser(userIndex); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser(userIndex); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
        {
          if (
# if (ComM_GetSizeOfChannel() > 1u ) /* If only one channel exists, every channel user is always mapped to it. ComM_GetSizeOfChannel represents the superset number of channels over all variants. */
              (ComM_GetChannelOfUserByteMask(iByteMaskIndex) == channel) &&
# endif
              ((ComM_GetUserReqFullCom(ComM_GetUserReqFullComIdxOfUserByteMask(iByteMaskIndex), COMM_MASTER_PARTITION_IDX) & ComM_GetSetMaskOfUserByteMask(iByteMaskIndex)) != COMM_NO_REQUESTS))
          {
            if( currentNumOfRequesters < ComM_GetSizeOfFullComRequesters( COMM_MASTER_PARTITION_IDX ) ) /* COV_COMM_GENDATA_CHECK */
            {
              ComM_SetFullComRequesters(currentNumOfRequesters, userIndex, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* Justification is applicable because of the run-time check above */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              currentNumOfRequesters++;
            }
          }
        }
      }
    }
  }

  /* #40 Trigger notification via Sender-Receiver interface. */
  /* Note: The function below will evaluate ComM_FullComRequesters[] and if detected a change, it will notify RTE via
   * the optional sender-receiver interface ComM_CurrentChannelRequest */
  ComM_CurrentChannelRequestNotification( channel, currentNumOfRequesters );
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/*********************************************************************************************************************
 *  ComM_TF_NoCom_NetReq()
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
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_NoCom_NetReq( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
#if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
# if ( (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) )
  switch ( ComM_GetNmTypeOfChannel( Channel ) )
  {
    /* #10 Nm variant NONE: once communication requested, ComM shall not trigger shutdown on the channel. */
#  if (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON)
  case COMM_NONE_NMTYPEOFCHANNEL:
    ComM_SetBusComModeReq( Channel, COMM_FULL_COM_NETWORK_REQUESTED, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

    /* For channels with NmType NONE, this transition function can only be called from the mater partition.
       No further check for the partiton index and no sync counter increase needed here. */
    break;
#  endif

    /* #20 Nm variant LIGHT: cancel the LIGHT SILENT timer (ComM_NmLightTimer variable is re-used). */
#  if (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON)
  case COMM_LIGHT_NMTYPEOFCHANNEL:
#   if (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON)
    ComM_SetNmLightTimer( Channel, 0, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#   endif

    /* #30 Nm variant LIGHT: start the timer for ComMTMinFullComModeDuration. */
    ComM_SetMinFullComModeTimer( Channel, ComM_GetMinFullComTimeOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    break;
#  endif

  /* #40 Nm variant FULL: notify Nm about network request. */
#  if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
  case COMM_FULL_NMTYPEOFCHANNEL:

    ComM_SetBusComModeReq(Channel, ComM_GetWakeupStateOfChannel(Channel), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* ESCAN00106564 */
#   if (COMM_MULTIPARTITION == STD_ON)
#    if (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION */
    {
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */

      /* Leave critical section. It was entered previously in ComM_Nm_NetworkMode(). */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#    endif

    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL */
    {
      ComM_SetForwardNmNetworkRequestSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
    else
#   endif
    {
      (void)Nm_NetworkRequest( Channel );
    }

    /* #50 Nm variant FULL: start the timer for ComMTMinFullComModeDuration. */
    /* ESCAN00081330 */
#   if (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
#    if ( COMM_MULTIPARTITION == STD_ON )
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION */
    {
      /* Increment sync counter for ComM_MinFullComModeTimer by 1 (relative to the corresponding Master-Counter) */
      ComM_SetMinFullComModeTimerSyncCounter( Channel, (uint8)(ComM_GetMinFullComModeTimerSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
    }
    else
#    endif
    {
      ComM_SetMinFullComModeTimer( Channel, ComM_GetMinFullComTimeOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#   endif
    break;
#  endif
    /* PRQA S 2018 1 */ /* MD_ComM_2018 */
  default: /* COV_COMM_MISRA */
    break;
  }
# endif

  /* #60 Request COMM_FULL_COMMUNICATION from the state manager. */
# if (COMM_MULTIPARTITION == STD_ON)
  if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION */
  {
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
    /* Channel is not assigned to current partition: Transition function is called from master main function, channel is not assigned to master partition. */
    ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    ComM_SetForwardingBusSmMode( Channel, COMM_FULL_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
  }
  else
# endif
  {
    /* Channel is assigned to current partition: Transition function is called either from master main function (-> current channel is assigned to master partition)
    or from interrupt context on satellite partition (-> current channel is assigned to current satellite partition). */
    if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, COMM_FULL_COMMUNICATION) == E_NOT_OK ) /* SBSW_COMM_BusSMMode_PointerCall */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION XX */
    {
# if (COMM_DEV_ERROR_REPORT == STD_ON)
      (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE );
# endif
    }
  }

#else
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif
}

/*********************************************************************************************************************
 *  ComM_TF_NoCom_FullReadySleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_NoCom_FullReadySleep( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
  /* #10 If Nm is supported on the channel, request FULL_COM from BusSM and notify Nm about passive start-up. */
#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
# if (COMM_ISDEF_NMSUPPORTOFCHANNEL == STD_OFF)
  if (ComM_IsNmSupportOfChannel(Channel)) /* COV_COMM_SUBSET_OF_ELEMENTS */
# endif
  {
    ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* ESCAN00106564 */

#  if ( COMM_MULTIPARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
    {
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */

      /* Leave critical section. It was entered previously in ComM_Nm_NetworkMode(). */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }

    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingBusSmMode( Channel, COMM_FULL_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
    }
    else
# endif
    {
      if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, COMM_FULL_COMMUNICATION) == E_NOT_OK ) /* SBSW_COMM_BusSMMode_PointerCall */
      {
# if (COMM_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE );
# endif
      }
    }

    /* ComM shall call Nm_PassiveStartUp() when entering Ready Sleep for robustness reasons and ignore E_NOT_OK return value ESCAN00068132 */
#  if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
    {
      ComM_SetForwardNmPassiveStartUpSyncCounter( Channel, (uint8)(ComM_GetForwardNmPassiveStartUpSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
    else
#  endif
    {
      (void)Nm_PassiveStartUp( Channel );
    }
  }

#else
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif

  /* Note: the function cannot be called for channels with Nm Variant LIGHT because Minimum Full Com Duration
   * always exists for LIGHT and this results that channels enters Network Requested state and not Ready Sleep. */
}

/*********************************************************************************************************************
 *  ComM_TF_Full_SiCom()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_Full_SiCom( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
#if (COMM_SILENTSUPPORTOFCHANNEL == STD_ON)
# if (COMM_ISDEF_SILENTSUPPORTOFCHANNEL == STD_OFF)
  if (ComM_IsSilentSupportOfChannel(Channel))
# endif
  {
# if (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON)
#  if (COMM_ISDEF_NMLIGHTSILENTDURATIONOFCHANNEL == STD_OFF)
    if (ComM_GetNmLightSilentDurationOfChannel(Channel) > 0x00u)
#  endif
    {
      /* #10 If Nm Variant is LIGHT and the LIGHT SILENT timer is configured, start the timer. */
      ComM_SetNmLightTimer(Channel, ComM_GetNmLightSilentDurationOfChannel(Channel), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
# endif

    /* #20 Request COMM_SILENT_COMMUNICATION from BusSM. Silent mode is only available on CAN (except J1939Nm) or ETH with Nm Variants FULL or PASSIVE (ESCAN00069043) or LIGHT SILENT timer.
     *     Note: EthSM will ignore the mode request, but ComM shall trigger it for the sake of consistency because UdpNm indicates Prepare Bus Sleep Mode. */
# if (COMM_MULTIPARTITION == STD_ON)
#  if (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LIGHT */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingBusSmMode( Channel, COMM_SILENT_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
    }
    else
#  endif
# endif
    {
      if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, COMM_SILENT_COMMUNICATION) == E_NOT_OK ) /* SBSW_COMM_BusSMMode_PointerCall */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LIGHT XX */
      {
# if (COMM_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE );
# endif
      }
    }
  }

# if ( COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_OFF )
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#else
  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif
}

/*********************************************************************************************************************
 *  ComM_TF_ReadyS_NetReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_ReadyS_NetReq( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
#if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)

  /* #10 If Nm Type is LIGHT, cancel the light timer. */
# if (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON)
  ComM_SetNmLightTimer(Channel, 0u, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif

  /* #20 If Nm Type is FULL, notify Nm about network requested. */
# if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
#  if( COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF )
  if (ComM_GetNmTypeOfChannel(Channel) == COMM_FULL_NMTYPEOFCHANNEL) /* COV_COMM_SUBSET_OF_ELEMENTS */
#  endif
  {
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL */
#   endif
    {
      ComM_SetForwardNmNetworkRequestSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    {
      (void)Nm_NetworkRequest( Channel );
    }
#  endif
  }
# endif

  /* #30 If Nm Type is LINSLAVE, notify LinSM and request FULL_COMMUNICATION. */
# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
#  if ( COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF )
  if (ComM_GetNmTypeOfChannel(Channel) == COMM_LINSLAVE_NMTYPEOFCHANNEL)
#  endif
  {
#  if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingBusSmMode( Channel, COMM_FULL_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
    }
    else
#  endif
    {
      if (ComM_GetRequestBusSMModeApiOfChannel(Channel)(Channel, COMM_FULL_COMMUNICATION) == E_NOT_OK) /* SBSW_COMM_BusSMMode_PointerCall */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE XX */
      {
  # if (COMM_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE);
  # endif
      }
    }
  }
# endif

# if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_OFF) && (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_OFF) && (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_OFF))
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif

#else
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif
}

/*********************************************************************************************************************
 *  ComM_TF_SiCom_NoCom()
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
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_SiCom_NoCom( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
#if ( (COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF) )
  ComM_UserByteMaskStartIdxOfUserType userByteMaskIndex;
  ComM_UserIterType user;
# if ( (COMM_PNC_SUPPORT == STD_ON) && (COMM_PNCCHANNELMAPPING == STD_ON) )
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
# endif
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionIdentifiersIterType userPartitionIdx = 0u;
# endif
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set channel mode to NO_COM before requesting NO_COM from the BusSM to ensure accepting bus wake-ups. */
  ComM_SetActiveComMode( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */ /* ESCAN00091475 */

#  if ((COMM_MULTIPARTITION == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON))
#   if (COMM_ISDEF_NMSUPPORTOFCHANNEL == STD_OFF)
  if ( ComM_IsNmSupportOfChannel( Channel ) )
#   endif
  {
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
    {
      /* Increment sync counter for ComM_ActiveComMode by 1 (relative to the corresponding Master-Counter) */
      ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */

      /* Leave critical section. It was entered previously in BusSleepMode indication from Nm. */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
  }
#  endif

#if ( COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF )
  switch (ComM_GetNmTypeOfChannel(Channel))
  {
  /* #20 If the channel is Nm LINSLAVE, set Bus Sleep Mode indicated Flag to FALSE. The Flag is handled in the next ComM main function. */
# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
  case COMM_LINSLAVE_NMTYPEOFCHANNEL:
    ComM_SetBusSleepModeIndicated(Channel, FALSE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    break;
# endif
  /* #30 If the channel is Nm FULL, PASSIVE or LIGHT, request NO_COM from BusSM. */
# if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) )
#  if ( COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON )
  case COMM_FULL_NMTYPEOFCHANNEL:
#  endif
#  if ( COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON )
  case COMM_PASSIVE_NMTYPEOFCHANNEL:
#  endif
#  if ( COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON )
  case COMM_LIGHT_NMTYPEOFCHANNEL:
#   if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LIGHT */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingBusSmMode( Channel, COMM_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
    }
    else
#   endif
#  endif
    {
      if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, COMM_NO_COMMUNICATION) == E_NOT_OK ) /* SBSW_COMM_BusSMMode_PointerCall */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LIGHT XX */ /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
      {
# if (COMM_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE );
# endif
      }
    }
    break;
# endif
  /* PRQA S 2018 1 */ /* MD_ComM_2018 */
  default: /* COV_COMM_MISRA */
    break;
  }
#endif

  /* #40 If the channel is limited to NO_COM, clear all FULL_COM requests for channel and PNC users.
   *     In case of MultiPartition, update the data from the satellite partitions to prevent from having unexpected wakeUps.*/
#if ( (COMM_MODE_LIMITATION == STD_ON) && ( COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF ) )
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
  if (ComM_NoComLimitationActive(Channel) == TRUE)
# endif
  {
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
    {
# if (COMM_MULTIPARTITION == STD_ON)
      userPartitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
# endif
      for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == Channel ) /* COV_COMM_ONLY_ONE_CHANNEL_ASSIGNED_TO_ALL_USERS */
        {
# if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
          if ( userPartitionIdx != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
          {
            ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_NO_REQUESTS, userPartitionIdx ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            ComM_SetForwardUserReqSyncCounter( user, (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u), userPartitionIdx ); /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            ComM_SetForwardingUserReqMode( user, COMM_NO_COMMUNICATION, userPartitionIdx ); /* SBSW_COMM_USER_CHECK */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
          else
# endif
          {
            ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_NO_REQUESTS, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
        }
      }

# if (( COMM_PNC_SUPPORT == STD_ON ) && ( COMM_PNCCHANNELMAPPING == STD_ON ))
      for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( Channel ); /* FETA_COMM_CSL_USE_CASE */
            pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( Channel );
            pncPbIndIter++ )
      {
        for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( ComM_GetPncPbInd( pncPbIndIter ) ); /* FETA_COMM_CSL_USE_CASE */
              userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( ComM_GetPncPbInd( pncPbIndIter ) );
              userReqPncFullComIndex++ )
        {
#  if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
          if ( userPartitionIdx != COMM_MASTER_PARTITION_IDX )
          {
            ComM_SetUserReqPncFullCom( userReqPncFullComIndex, COMM_NO_REQUESTS, userPartitionIdx ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
          else
#  endif
          {
            ComM_SetUserReqPncFullCom( userReqPncFullComIndex, COMM_NO_REQUESTS, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
        }
      }
# endif
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
  }
#endif /* COMM_MODE_LIMITATION */

} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/*********************************************************************************************************************
 *  ComM_TF_NetReq_ReadyS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NetReq_ReadyS( NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
#if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)

  /* #10 If Nm Light timer is configured, start it. It is ensured by generator that Nm Type of the channel is LIGHT. */
# if (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON)
#  if (COMM_ISDEF_NMLIGHTDURATIONOFCHANNEL == STD_OFF)
  if ( ComM_GetNmLightDurationOfChannel( Channel ) > 0u )
#  endif
  {
    ComM_SetNmLightTimer( Channel, ComM_GetNmLightDurationOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
# endif

  /* #20 If Nm Type is FULL, notify Nm about network release. */
# if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
#  if (COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF)
#   if (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON || COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON || COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON)
  if ( ComM_GetNmTypeOfChannel( Channel ) == COMM_FULL_NMTYPEOFCHANNEL )
#   endif
#  endif
  {
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL */
#   endif
    {
      ComM_SetForwardNmNetworkReleaseSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkReleaseSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    {
      (void)Nm_NetworkRelease( Channel );
    }
#  endif
  }
# endif

  /* #30 If Nm Type is LINSLAVE, notify LinSm and directly request NO_COMMUNICATION. */
# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
#  if ( COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF )
  if (ComM_GetNmTypeOfChannel(Channel) == COMM_LINSLAVE_NMTYPEOFCHANNEL)
#  endif
  {
#  if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingBusSmMode( Channel, COMM_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
    }
    else
#  endif
    {
      if (ComM_GetRequestBusSMModeApiOfChannel(Channel)(Channel, COMM_NO_COMMUNICATION) == E_NOT_OK) /* SBSW_COMM_BusSMMode_PointerCall */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE XX */
      {
  # if (COMM_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE);
  # endif
      }
    }
  }
# endif

# if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_OFF) && (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_OFF) && (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_OFF))
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
# endif
#else
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif
}

/*********************************************************************************************************************
 *  ComM_TF_No_Action()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_No_Action(NetworkHandleType Channel COMM_PARTITION_TYPE_LAST )
{
  /* #10 There is no action for this sub-state transition. Use cases:
   * COMM_NO_COM_REQUEST_PENDING to COMM_NO_COM_NO_PENDING_REQUEST (communication was never allowed and user/DCM request is canceled)
   * COMM_NO_COM_NO_PENDING_REQUEST to COMM_NO_COM_REQUEST_PENDING (communication is requested and ComM waits for allowed notification)
   */
  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}

/*********************************************************************************************************************
 *  ComM_TF_No_Transition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC(void, COMM_CODE) ComM_TF_No_Transition(NetworkHandleType Channel COMM_PARTITION_TYPE_LAST)
{
  /* #10 This is an invalid state transition, report a Det error. */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, COMM_E_NOSUPPORTED_MODECHANGE );
#endif

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}

#if (COMM_USERMODENOTIFUNC == STD_ON)
# if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
/**********************************************************************************************************************
 *  ComM_StateChangeNotification()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC(void, COMM_CODE) ComM_StateChangeNotification( NetworkHandleType channel )
{
  ComM_SizeOfUserType userIndex;
  ComM_ModeType ComM_CalcMode = COMM_NO_COMMUNICATION;
  Std_ReturnType retValue = RTE_E_OK;
  Std_ReturnType retValueCumulated = RTE_E_OK;
# if ( ComM_GetSizeOfChannel() > 1u ) /* ComM_GetSizeOfChannel represents the superset number of channels over all variants. */
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
  boolean bNotify;
# endif

  /* #10 Iterate over users that have configured User Mode (state change) notification and are mapped to channels (not PNC). */
  for( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ )
  {
    if (ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX ) != NULL_PTR) /* COV_COMM_SUBSET_OF_ELEMENTS */
    {
# if ( COMM_PNC_SUPPORT == STD_ON )
      if (!ComM_IsPncUserOfUser(userIndex))
# endif
      {
        /* #20 In single-channel configurations notify the BusSM mode to the users */
        ComM_CalcMode = ComM_GetBusSmState(channel, COMM_MASTER_PARTITION_IDX );

# if ( ComM_GetSizeOfChannel() > 1u ) /* ComM_GetSizeOfChannel represents the superset number of channels over all variants. */
        /* #30 In multi-channel configurations only notify a new user mode if
         *     1) the user is mapped to the current channel where the BusSM mode has changed and
         *     2) the lowest mode of all channels mapped to the user has changed since the last mode notification. */
        bNotify = FALSE;

        for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( userIndex ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( userIndex ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          if ( ComM_GetChannelOfUserByteMask( iByteMaskIndex ) == channel )
          {
            bNotify = TRUE;
          }

          if ( ComM_GetBusSmState( ComM_GetChannelOfUserByteMask( iByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) < ComM_CalcMode )
          {
            ComM_CalcMode = ComM_GetBusSmState( ComM_GetChannelOfUserByteMask( iByteMaskIndex ), COMM_MASTER_PARTITION_IDX );
          }
        }

        if( bNotify == TRUE ) /* COV_COMM_COMMON_USER_WITH_MODE_NOTIFICATION */
# endif
        {
          if( ComM_CalcMode != ComM_GetLastStateChange(userIndex, COMM_MASTER_PARTITION_IDX ) )
          {
            if( userIndex < ComM_GetSizeOfUserModeNotiFunc( COMM_MASTER_PARTITION_IDX ) ) /* COV_COMM_GENDATA_CHECK */ /* PRQA S 2995, 2991 */ /* MD_ComM_2995_2991 */
            {
              /* #40 If a new mode is available for the current user notify it using the Mode Switch Interface (sender-receiver) 'ComM_CurrentMode' */
              switch(ComM_CalcMode)
              {
              case COMM_FULL_COMMUNICATION:
                retValue = ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX )(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION);   /* SBSW_COMM_UserModeNotiFunc_PointerCall */
                break;
              case COMM_SILENT_COMMUNICATION:
                retValue = ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX )(RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION); /* SBSW_COMM_UserModeNotiFunc_PointerCall */
                break;
              case COMM_NO_COMMUNICATION:
                retValue = ComM_GetUserModeNotiFunc(userIndex, COMM_MASTER_PARTITION_IDX )(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION);     /* SBSW_COMM_UserModeNotiFunc_PointerCall */
                break;
                /* PRQA S 2018 1 */ /* MD_ComM_2018 */
              default: /* COV_COMM_MISRA */
                break;
              }
            }

            if( retValue == RTE_E_OK )
            {
              /* #400 If notification to the current user was successful, update the mode to ensure that the same one will not be notified again. */
              ComM_SetLastStateChange(userIndex, ComM_CalcMode, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_USER_CSL02 */ /*lint !e661 ComM_LastStateChange[] has a size relation to ComM_User[] */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
            else
            {
              /* #410 If at least one user notification was not successful, the state change on this channel will be processed again in the next task. */
              retValueCumulated = E_NOT_OK;
            }
          }
        }
      }
    }
  }

  if( retValueCumulated == RTE_E_OK )
  {
    /* #50 If all users were notified successfully, mark processing of this state change as completed. */
    ComM_SetStateChange(channel, COMM_NO_STATE_CHANGE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
# endif
#endif

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  ComM_ProcessPassiveWakeUp()
 **********************************************************************************************************************/
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
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_ProcessPassiveWakeUp( NetworkHandleType channel COMM_PARTITION_TYPE_LAST )
{
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  uint8 channelIndex;
#  endif
# endif

# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  /* #10 If Synchronous Wake-up is enabled, notify passive wake-up to all ComM channels being in NO_COM or SI_COM state.
   *     This is done by setting the bus-side requested state to FULL_COM (Network Requested or Ready Sleep). */
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#   endif
  {
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

    ComM_SetNmStartUpIndicationSyncCounter( channel, (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
  }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  {
    ComM_SetNmStartUpIndicated( 0u, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
    {
#   if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
      if ( ComM_IsFrShutdown( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
      {
        /* #100 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
         *      because shutdown on FlexRay shall not be interrupted.
         *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
        ComM_SetBusComModeReqFrShutdown( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      else
#   endif
      {
        if ( (ComM_GetActiveComMode( channelIndex, PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT XX */
        {
          ComM_SetBusComModeReq( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
      }
    }

    /* #20 If Synchronous Wake-up is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#   if (COMM_PNC_SUPPORT == STD_ON)
#    if (COMM_PNC_PS_TIMER > 0u) /* ESCAN00067357 */
    ComM_ActivateAllPncs();
#    endif
#   endif
  }
#  endif

  COMM_DUMMY_STATEMENT_CONST(channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

# else /* #if ( COMM_SYNCHRONOUS_WAKE_UP == STD_OFF ) */

  /* #30 If Synchronous Wake-up is disabled, notify passive wake-up to the channel by setting the bus-side
   *     requested state to FULL_COM (Network Requested or Ready Sleep). Ignore the wake-up in FULL_COM mode. */
#  if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
  if(ComM_IsFrShutdown(channel, PARTITION_IDX))
  {
    /* #300 If communication shutdown is ongoing on FlexRay channel, delay the notification until FrSM indicates NO_COM
     *      because shutdown on FlexRay shall not be interrupted. */
#  if ( COMM_MULTIPARTITION == STD_ON)
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUSTYPE_FR */
#   endif
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

      ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      /* Increment sync counter for ComM_BusComModeReqFrShutdown by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqFrShutdownSyncCounter( channel, (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    else
#   endif
#  endif
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    {
      ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#  endif
  }
  else
#  endif
  {
    if((ComM_GetActiveComMode(channel, COMM_MASTER_PARTITION_IDX) & COMM_FULL_COMMUNICATION) == 0u) /* ESCAN00091475 */
    {
#  if ( COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#   endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

        ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusComModeReqSyncCounter( channel, (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      else
#   endif
#  endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      {
        ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
    }
  }
# endif /* #if ( COMM_SYNCHRONOUS_WAKE_UP ) */

} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
/**********************************************************************************************************************
 *  ComM_ChannelGetMaxStateAfterExtRamCheck()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(ComM_StateType, COMM_CODE) ComM_ChannelGetMaxStateAfterExtRamCheck( NetworkHandleType channel, ComM_StateType targetState )
{
  ComM_StateType maxState = COMM_NO_COM_NO_PENDING_REQUEST;
  Std_ReturnType ramCheckStatus = E_OK;

  maxState = targetState;

  /* #10 Provide the maximum communication state on CAN channels according to the result of Extended RAM Check:
   *     COMM_NO_COM_NO_PENDING_REQUEST if check result is Disable,
   *     COMM_NO_COM_REQUEST_PENDING if check result is Disable Repeat,
   *     the targetState (no changes) otherwise. */
# if (COMM_ISDEF_BUSTYPEOFCHANNEL == STD_OFF)
  if (ComM_GetBusTypeOfChannel(channel) == COMM_BUS_TYPE_CAN)
# endif
  {
    ramCheckStatus = CanSM_RamCheckStatus(channel);

    if (ramCheckStatus == CANSM_DISABLE_COMMUNICATION)
    {
      maxState = COMM_NO_COM_NO_PENDING_REQUEST;
    }
    else if (ramCheckStatus == CANSM_DISABLE_COMMUNICATION_REPEAT)
    {
      maxState = COMM_NO_COM_REQUEST_PENDING;
    }
    else
    {
      /* no changes */
    }
  }

  return maxState;
}
#endif

#if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  ComM_ChannelResetAfterForcingNoComm()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelResetAfterForcingNoComm( COMM_PARTITION_TYPE_ONLY )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_SizeOfChannelType channelIndex;
  boolean allChannelsAreInNoComm = TRUE;
  boolean aChannelNeedsResetAfterForcingNoComm = FALSE;
# if ( COMM_MULTIPARTITION == STD_ON ) && ( COMM_NMSUPPORTOFCHANNEL == STD_ON )
  ComM_StateType activeComMode = COMM_NO_COM_REQUEST_PENDING;
# endif

  /* ----- Implementation ----------------------------------------------- */
  for (channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++)
  {
# if (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON)
    if (ComM_GetNmTypeOfChannel(channelIndex) != COMM_NONE_NMTYPEOFCHANNEL)
# endif
    {
      /* #10 Check for each channel except NM NONE: BusSm indicated NO_COM and the channel is in NO_COM (ignore possible passive wake-ups). */
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_NMSUPPORTOFCHANNEL == STD_ON )
      /* Check if an event occurred on a satellite which has changed the active com mode since last master sync. */
      /* No spinlock needed, since for any event on the satellite partition the channel state is written BEFORE the sync counter.
       * The value is always updated in the beginning of the master main function. In the worst case - where the value on the satellite
       * has changed but the sync counter is not yet increased - this value is read from the master partition. It is not possible that a
       * satellite event sets the ActiveComMode to a FULL_COM state, i.e. if the ActiveComMode is already NO_COM on the master, the satellite
       * cannot change to higher than NO_COM.
       */
      if ( ComM_GetActiveComModeSyncCounter( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) == (uint8)(ComM_GetActiveComModeSyncCounter( channelIndex, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
      {
        activeComMode = ComM_GetActiveComMode( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) );
      }
      else
      {
        activeComMode = ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX );
      }
      if ( (ComM_GetBusSmState( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) != COMM_NO_COMMUNICATION) || (activeComMode > COMM_NO_COM_REQUEST_PENDING))
#  else
      if ( (ComM_GetBusSmState( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) != COMM_NO_COMMUNICATION) || (ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) > COMM_NO_COM_REQUEST_PENDING) )
#  endif
# else
      if ( (ComM_GetBusSmState( channelIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COMMUNICATION) || (ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) > COMM_NO_COM_REQUEST_PENDING) )
# endif
      {
        allChannelsAreInNoComm = FALSE;
      }

      /* #20 Check that at least one channel has an active Mode Limitation and has Reset After Forcing NoCom enabled. */
# if (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON)
      if (ComM_IsResetAfterForcingNoCommOfChannel(channelIndex)) /* Needed only for channels with Nm PASSIVE */
# endif
      {
        /* #21 The reset shall not occur in case the mode limitation is not actually activated (was not forced from COMM_FULL_COMM_NETWORK_REQUESTED to COMM_FULL_COMM_READY_SLEEP)*/
        if ((ComM_NoComLimitationActive(channelIndex) == TRUE) && (ComM_IsResetRequired(channelIndex, COMM_MASTER_PARTITION_IDX)))
        {
          aChannelNeedsResetAfterForcingNoComm = TRUE;
        }
      }
    }
  }

  /* #30 If the conditions above are true, call BswM_ComM_InitiateReset(). */
  if (allChannelsAreInNoComm == TRUE)
  {
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#  endif
    {
      if ( aChannelNeedsResetAfterForcingNoComm == TRUE )
      {
        /* Increment sync counter on current satellite partition only. BswM notification about InitiateReset and Synchronization to other channels is done in next master main function. */
        ComM_SetInitiateResetSyncCounter( (uint8)(ComM_GetInitiateResetSyncCounter( COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
    }
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    else
#  endif
# endif
# if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    {
      for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ )
      {
        ComM_SetResetRequired( channelIndex, FALSE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      if ( aChannelNeedsResetAfterForcingNoComm == TRUE )
      {
        BswM_ComM_InitiateReset();
      }
    }
# endif
  }
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_GetInternalPartitionIdx()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( ComM_PartitionConfigIdxOfChannelType, COMM_CODE ) ComM_GetInternalPartitionIdx( CONST( ApplicationType, AUTOMATIC ) applicationId )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all ComM partition instances and return the partition index of the ComM partition instance which belongs to the current application. */
  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ )
  {
    /* #11 If no ComM partition belongs to the current application, return SizeOfPartitionIdentifiers as an invalid partition index. */
    if ( applicationId == ComM_GetPartitionSNVOfPartitionIdentifiers( partitionIdx ) )
    {
      break;
    }
  }

  return (ComM_PartitionConfigIdxOfChannelType)partitionIdx;
}
#endif

#if (COMM_MULTIPARTITION == STD_ON)
# if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  ComM_CheckSyncCounterAndForwardFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_CheckSyncCounterAndForwardFunction( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) syncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) syncCounterCurrPartPtr,
                                                                                   ComM_ForwardingFunctionPtrType forwardingFunction, CONST( NetworkHandleType, AUTOMATIC ) networkHandle )
{
  /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the
  current partition (i.e. on the partition the channel belongs to). */
  if ( *syncCounterPtr == (uint8)( *syncCounterCurrPartPtr + 1u ) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */ /* PRQA S 0404 */ /* MD_ComM_0404 */
  {
    /* #20 Increment synchronization counter on current partition and forward call to the BusNm. */
    (*syncCounterCurrPartPtr) = (*syncCounterCurrPartPtr) + 1u; /* SBSW_COMM_FORWARDING_FCT_PTR_WRITE */
    (void)forwardingFunction( networkHandle ); /* SBSW_COMM_FORWARDING_FCT_PTR_CALL */
  }
}
# endif

/**********************************************************************************************************************
 *  ComM_ForwardRequestBusSmMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_ForwardRequestBusSmMode( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONST( ComM_ModeType, AUTOMATIC ) Mode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request the new mode from BusSm by forwarding a call of <BusSM>_RequestComMode to the lower layer. */
  if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, Mode) == E_NOT_OK ) /* SBSW_COMM_BusSMMode_PointerCall */
  {
    errorId = COMM_E_ERROR_IN_PROV_SERVICE;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION_SATELLITE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif /* END: COMM_MULTIPARTITION == STD_ON */

#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_UpdateDataFromSatellites()
 *********************************************************************************************************************/
/*!
 *
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
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateDataFromSatellites( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx = 0u;
  uint8 channel;
# if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
  boolean initiateReset = FALSE;
# endif
# if (COMM_PNC_SUPPORT == STD_ON)
  ComM_PncSignalValuesEndIdxOfPncSignalType signalByteIndex;
#  if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON ) && ( COMM_INTERMEDIATEPNCCOORDINATOROFPNC == STD_ON )
  ComM_PncIterType pncIndex;
#  endif
# endif
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  ComM_StateType lHighestCalculatedBusComMode[ComM_GetSizeOfChannel()] = { 0u };
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each channel, check if an event occured on the channel related partition since last synchronization. */
  for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ )
  {
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );

# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
# endif
    {
      if ( ComM_GetGlobalPartitionSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
      {
        ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, PARTITION_IDX ) + 1u), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

        /* ------- User Variables ------------------------------------------------- */


        /* ------- Channel Variables ---------------------------------------------- */
        /* #20 Synchronize data for all channel related variables:
                  If on any satellite partition the synchronization counter of the corresponding variable was incremented
                  by 1 (compared to the corresponding counter on the master partition), update the data.*/

# if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) )
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
# endif

        /* Update ComM_ActiveComMode */
# if ( COMM_NMSUPPORTOFCHANNEL == STD_ON )
        ComM_UpdateMasterSyncCounterAndStates( &ComM_GetActiveComModeSyncCounter( channel, PARTITION_IDX ), &ComM_GetActiveComModeSyncCounter( channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE */
                                               &ComM_GetActiveComMode( channel, PARTITION_IDX ),            &ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) );
# endif

        /* Update ComM_BusComModeReqFrShutdown */
# if ( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
        ComM_UpdateMasterSyncCounterAndStates( &ComM_GetBusComModeReqFrShutdownSyncCounter( channel, PARTITION_IDX ), &ComM_GetBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE */
                                               &ComM_GetBusComModeReqFrShutdown( channel, PARTITION_IDX ),            &ComM_GetBusComModeReqFrShutdown( channel, COMM_MASTER_PARTITION_IDX ) );
# endif

        /* Update ComM_BusComModeReq */
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
#  if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) )
        ComM_SyncBusComModeRequests( channel, lHighestCalculatedBusComMode ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
#  endif

        ComM_SyncSynchronousWakeUpIndications( channel, lHighestCalculatedBusComMode COMM_PARTITION_PARA_LAST ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
# else
        ComM_SyncBusComModeRequests( channel );
# endif

# if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) )
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
# endif

        /* Update ComM_StateChange */
# if (COMM_USERMODENOTIFUNC == STD_ON)
#  if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_UpdateMasterSyncCounterAndStates( &ComM_GetStateChangeSyncCounter( channel, PARTITION_IDX ), &ComM_GetStateChangeSyncCounter( channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE */
                                               &ComM_GetStateChange( channel, PARTITION_IDX ),            &ComM_GetStateChange( channel, COMM_MASTER_PARTITION_IDX ) );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
#  endif
# endif

        /* Update ComM_MinFullComModeTimer */
# if ((COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) && (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON))
        ComM_UpdateMasterSyncCounterAndTimers( &ComM_GetMinFullComModeTimerSyncCounter( channel, PARTITION_IDX ), &ComM_GetMinFullComModeTimerSyncCounter( channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE */
                                               ComM_GetMinFullComTimeOfChannel( channel ),                        &ComM_GetMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX ) );
# endif

        /* Update ComM_NmLightTimer */
        /* No sync needed. ComM_NmLightTimer is only accessed on master partition */

        /* Update ComM_DcmRequestActive */
# if ((COMM_DCM_INDICATION) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON))
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_UpdateMasterSyncCounterAndStates( &ComM_GetDcmRequestActiveSyncCounter( channel, PARTITION_IDX ),           &ComM_GetDcmRequestActiveSyncCounter( channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE */
                                               &(ComM_GetDcmRequestActiveOfPCPartitionConfig( PARTITION_IDX )[channel]), &(ComM_GetDcmRequestActiveOfPCPartitionConfig( COMM_MASTER_PARTITION_IDX )[channel]) );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
# endif

        /* Update ComM_BusSleepModeIndicated */
# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
        if ( ComM_GetBusSleepModeIndicatedSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetBusSleepModeIndicatedSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
        {
          /* No Spinlock needed, since in case of a new event on a satellite the UserReqFullCom is always set to NO_COM. */
          ComM_SetBusSleepModeIndicated( channel, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          ComM_SetBusSleepModeIndicatedSyncCounter( channel, (ComM_GetBusSleepModeIndicatedSyncCounter( channel, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
# endif

        /* Update ComM_FrShutdown */
        /* No sync needed. ComM_FrShutdown is only written on satellite partition. */

        /* Update ComM_LastStateChange */
        /* No sync needed. ComM_LastStateChange is only accessed on master partition. */

        /* Update ComM_NvMStoreReq */
        /* No sync needed. Current restriction only allows access on master partition. */

        /* Update ComM_CommunicationAllowed */
        /* No sync needed. Current restriction only allows access on master partition. */

        /* Update ComM_Inhibition */
        /* No sync needed. Current restriction only allows write access on master partition */

        /* Update ComM_FullComRequesters */
        /* No sync needed. Current restriction only allows write access on master partition */


        /* ------- Partial Network Variables ------------------------------------- */
        /* #30 Synchronize data for all partial network related variables:
                  If on any satellite partition the synchronization counter of the corresponding variable was incremented
                  by 1 (compared to the corresponding counter on the master partition), update the data.*/

# if (COMM_PNC_SUPPORT == STD_ON)
        /* Update ComM_PncToChannelRoutingState */
#  if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
        if ( ComM_GetPncToChannelRoutingStateSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
          if ( (ComM_GetPncToChannelRoutingState( channel, PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE) != 0x00u )
          {
            ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
          else
          {
            ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
          ComM_SetPncToChannelRoutingStateSyncCounter( channel, (ComM_GetPncToChannelRoutingStateSyncCounter( channel, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
        }
#  endif

        /* Update ComM_ForwardSyncPncShutdown. */
#  if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON) && ( COMM_INTERMEDIATEPNCCOORDINATOROFPNC == STD_ON)
        if ( ComM_GetGwTypeOfChannel( channel ) == COMM_GATEWAY_TYPE_PASSIVE ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_GWTYPE_PASSIVE */
        {
          for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_GWTYPE_PASSIVE XX */
          {
            /* If the Forward Synchronized PNC Shutdown indication has changed on a satellite partition, set the Forward Synchronized PNC Shutdown indication on the master partition accordingly. */
            if ( ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, PARTITION_IDX ) == (uint8)(ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_GWTYPE_PASSIVE XX */
            {
              ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, (ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
          }
        }
#  endif

        /* Update ComM_NotifyPncStateChange */
        /* No sync needed. ComM_NotifyPncStateChange is only accessed on master partition. */

        /* Update ComM_ActivePncComMode */
        /* No sync needed. ComM_ActivePncComMode is only written on master partition. */

        /* Update ComM_PncPSleepTimer */
        /* No sync needed. ComM_PncPSleepTimer is only accessed on master partition. */

# endif
      } /* END: ComM_GlobalPartitionSyncCounter != (ComM_GlobalPartitionSyncCounter + 1) */

      /* Update ComM_BusSmState */
      /* Sync is only needed for BswM mode notification to be done on master partition. ComM_BusSmState is only written on satellite partition. */
      if ( ComM_GetBusSmState( channel, COMM_MASTER_PARTITION_IDX ) != ComM_GetBusSmState( channel, PARTITION_IDX ) )
      {
        /* No Spinlock needed, since value is directly read from the satellite partition (no synchronization needed). */
        ComM_SetBusSmState( channel, ComM_GetBusSmState( channel, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        BswM_ComM_CurrentMode( channel, ComM_GetBusSmState( channel, COMM_MASTER_PARTITION_IDX ) );
      }

    } /* END: PARTITION_IDX != COMM_MASTER_PARTITION_IDX */
  } /* END: for-loop */

  /* Update ComM_UserReqFullCom and ComM_UserReqPncFullCom */
#if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
  ComM_SyncUserFullComRequests();
#endif

# if (COMM_PNC_SUPPORT == STD_ON) || (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ )
    {
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#  endif
    {
#  if (COMM_PNC_SUPPORT == STD_ON)
      /* Update ComM_PncSignalValues */
      for ( signalByteIndex = 0u; signalByteIndex < ComM_GetSizeOfPncSignalValues( COMM_MASTER_PARTITION_IDX ); signalByteIndex++ )
      {
        if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, PARTITION_IDX ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
          ComM_SetPncSignalValues( signalByteIndex, ComM_GetPncSignalValues( signalByteIndex, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
          ComM_SetPncSignalValuesSyncCounter( signalByteIndex, ComM_GetPncSignalValuesSyncCounter( signalByteIndex, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
        }
      }
#  endif

      /* Update ComM_ResetRequired */
#  if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
      if ( ComM_GetInitiateResetSyncCounter( PARTITION_IDX ) == (uint8)(ComM_GetInitiateResetSyncCounter( COMM_MASTER_PARTITION_IDX ) + 1u) ) /*lint !e564 */
      {
        /* No Spinlock needed, since in case of a new event on a satellite only the sync counter is updated and a local flag is set. */
        ComM_SetInitiateResetSyncCounter( (ComM_GetInitiateResetSyncCounter( PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        initiateReset = TRUE;
      }
#  endif
    }
  }

#  if (COMM_PNC_SUPPORT == STD_ON)
  /* Update ComM_BusPncComModeReq */
  ComM_SyncBusPncComModeRequests(); /* PRQA S 2987 */ /* MD_ComM_2987 */
#  endif
# endif

# if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
  if ( initiateReset == TRUE )
  {
    for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ )
    {
      /* Reset the flag in case init function is not called due to BswM_ComM_InitiateReset(). */
      ComM_SetResetRequired( channel, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
    BswM_ComM_InitiateReset();
  }
# endif
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  ComM_UpdateMasterSyncCounterAndStates()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndStates( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satStatePtr      , volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterStatePtr )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
  if ( *satSyncCounterPtr == (uint8)(*masterSyncCounterPtr + 1u) ) /* COV_COMM_NM_LINSLAVE_ONLY XF */ /* PRQA S 0404 */ /* MD_ComM_0404 */
  {
    /* #20 Increment the synchronization counter on the master partition and update the state on the master partition
           with the value of the satellite partition. */
    *masterStatePtr = *satStatePtr; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    *masterSyncCounterPtr = *satSyncCounterPtr; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }
}

# if ((COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) && (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON))
/**********************************************************************************************************************
 *  ComM_UpdateMasterSyncCounterAndTimers()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndTimers( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr     , volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 CONST( ComM_MinFullComTimeOfChannelType, AUTOMATIC ) satTimer, volatile P2VAR( ComM_MinFullComModeTimerType, AUTOMATIC, COMM_APPL_VAR ) masterTimerPtr )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
  if ( *satSyncCounterPtr == (uint8)(*masterSyncCounterPtr + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION */ /* PRQA S 0404 */ /* MD_ComM_0404 */
  {
    /* #20 Increment the synchronization counter on the master partition and update the state on the master partition
           with the value of the satellite partition. */
    *masterTimerPtr = satTimer; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    *masterSyncCounterPtr = *satSyncCounterPtr; /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }
}
# endif

#if ( COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
/**********************************************************************************************************************
 *  ComM_SyncUserFullComRequests()
 *********************************************************************************************************************/
 /*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncUserFullComRequests( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_SizeOfUserType user;
  ComM_PartitionIdentifiersIterType partitionIdx = 0u;
  ComM_UserReqFullComType userReqFullCom = 0u;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskUserIndex;
#if (COMM_PNC_SUPPORT == STD_ON)
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex;
  ComM_UserReqPncFullComType userReqPncFullCom = 0u;
#endif

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    partitionIdx = ComM_GetPartitionConfigIdxOfUser( user );
    /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
    if ( ComM_GetForwardUserReqSyncCounter( user, PARTITION_IDX ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      /* #20 Update the state on the master partition with the value of the satellite partition.
       *     The userReqFullCom data is calculated from the master partition based on the state which is indicated on the satellite partition.
       *     The userReqFullCom data on the satellite remains as it is later on requested by ComM_GetRequestedComMode and ComM_GetState.
       */
      if ( ComM_GetForwardingUserReqMode( user, PARTITION_IDX ) == COMM_FULL_COMMUNICATION )
      {
        for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
        if ( ComM_HasPncPb() )
# endif
        {
          if ( ComM_IsPncUserOfUser( user ) ) /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
          {
            for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( user ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( user ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
            {
              userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
          }
        }
#endif
      }
      else
      {
        for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
        if ( ComM_HasPncPb() )
# endif
        {
          if ( ComM_IsPncUserOfUser( user ) ) /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
          {
            for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( user ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( user ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
            {
              userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
          }
        }
#endif
      }
      /* #30 Increment the synchronization counter on the master partition. */
      ComM_SetForwardUserReqSyncCounter( user, (ComM_GetForwardUserReqSyncCounter( user, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
  }
  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

# if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF) )
/**********************************************************************************************************************
 *  ComM_SyncBusComModeRequests()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusComModeRequests( CONST(NetworkHandleType, AUTOMATIC) channel
#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
                                                                     , P2CONST(ComM_StateType, AUTOMATIC, COMM_CONST) lHighestCalculatedBusComMode
#  endif
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the bus-site requested communication mode for this channel has changed on a satellite partition: */
  if ( ComM_GetBusComModeReqSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_ONLY_ONE_SATELLITE_CHANNEL */
  {
#  if ( (COMM_NMSUPPORTOFCHANNEL == STD_OFF) && (COMM_BUS_TYPE_FR_PRESENT == STD_OFF) && (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_OFF) )
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
#  endif

    /* #20 It has to be ensured that a wakeup event is not overwritten by a shutdown event. This means, if the channel was woken up on a satellite
            (bus requested FULL_COM, i.e. WakeupStateOfChannel), the wakeup state shall not be overwritten. */
#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
    if ( lHighestCalculatedBusComMode[channel] != ComM_GetWakeupStateOfChannel( channel ) ) /* COV_COMM_MP_ONLY_ONE_SATELLITE_CHANNEL */
#  endif
    {
      /* #30 Update the sync counter and the requested bus communication mode on the master partition. */
      ComM_SetBusComModeReq( channel, ComM_GetBusComModeReq( channel, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  if ( COMM_MANAGINGCHANNELOFCHANNEL == STD_ON )
      if ( ComM_GetManagingChannelIdOfChannel( channel ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL ) /* COV_COMM_MP_NO_OR_ALL_MANAGED_CHANNELS_ON_MASTER */
      {
        if ( (ComM_GetActiveComMode( ComM_GetManagingChannelIdOfChannel( channel ), COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u ) /* COV_COMM_MP_NO_OR_ALL_MANAGED_CHANNELS_ON_MASTER XX */
        {
          ComM_SetBusComModeReq( ComM_GetManagingChannelIdOfChannel( channel ), ComM_GetWakeupStateOfChannel( ComM_GetManagingChannelIdOfChannel( channel ) ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_MANAGING_CHANNEL_CHECK */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
      }
#  endif
    }
    ComM_SetBusComModeReqSyncCounter( channel, (ComM_GetBusComModeReqSyncCounter( channel, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

#  if ( (COMM_NMSUPPORTOFCHANNEL == STD_OFF) && (COMM_BUS_TYPE_FR_PRESENT == STD_OFF) && (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_OFF) )
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
#  endif
  }
}
# endif

# if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_SyncBusPncComModeRequests()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusPncComModeRequests( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx;
  ComM_PncIterType pncIndex;
#  if (COMM_PNC_MAX_NUMBER_OF_EIRA_RX > 1u)
  uint8 sigValuesArb;
  ComM_PncSignalIterType signalIndex;
#  endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the bus-site requested communication mode for a PNC has changed on a satellite partition: */
  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ )
  {
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#  endif
    {
      for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ )
      {
        if ( ComM_GetBusPncComModeReqSyncCounter( pncIndex, PARTITION_IDX ) == (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( PARTITION_IDX ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u) ) /* PRQA S 2985 1 */ /* MD_ComM_2985 */ /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();

          /* #20 If the bus-site requested communication mode has changed on a satellite partition, set the bus-site requested communication mode on the master partition accordingly. */
          if ( ComM_GetBusPncComModeReq( pncIndex, PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
          {
#  if (COMM_PNC_MAX_NUMBER_OF_EIRA_RX > 1u) /* If only one EIRA RX (bus type), no need to loop over signals */
            sigValuesArb = 0u;
            for ( signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++ )
            {
              if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_RX_TYPEOFPNCSIGNAL ) /* COV_COMM_SUBSET_OF_ELEMENTS */
              {
                sigValuesArb |= ComM_GetPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex ), COMM_MASTER_PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */
              }
            }

            if ( (sigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u ) /* COV_COMM_MP_ONLY_ONE_SATELLITE_WITH_ERA_OR_EIRA_RX_SIGNAL TX */
#  endif
            {
              ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
          }
          else
          {
            ComM_SetBusPncComModeReq( pncIndex, ComM_GetBusPncComModeReq( pncIndex, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }

          /* #30 Update the sync counter for ComM_BusPncComModeReq. */
          if ( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex) < ComM_GetBusPncComModeReqMasterSyncCounterEndIdxOfPncPartitionIndirection( partitionIdx ) ) /* PRQA S 2985 */ /* MD_ComM_2985 */ /* COV_COMM_MASTER_SYNCCOUNTER_IND */
          {
            ComM_SetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( PARTITION_IDX ) + pncIndex), (ComM_GetBusPncComModeReqSyncCounter( pncIndex, PARTITION_IDX )), COMM_MASTER_PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */ /* SBSW_COMM_MASTER_SYNCCOUNTER_IND */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }

          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
        }
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
/**********************************************************************************************************************
 *  ComM_SyncSynchronousWakeUpIndications()
 *********************************************************************************************************************/
/*!
 *
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
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncSynchronousWakeUpIndications( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) lHighestCalculatedBusComMode COMM_PARTITION_TYPE_LAST )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 syncWakeUpChannelIterator;

  /* ----- Implementation ----------------------------------------------- */
  /* Update ComM_EcuMWakeUpIndicationSyncCounter */
  /* #10 Check if an EcuM WakeUp-Indication occurred on a satellite partition. */
  if ( ComM_GetEcuMWakeUpIndicationSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    /* #11 Wake up every channel which is not in FULL_COMMUNICATION yet by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( syncWakeUpChannelIterator = 0u; syncWakeUpChannelIterator < ComM_GetSizeOfChannel(); syncWakeUpChannelIterator++ )
    {
      if ( (ComM_GetActiveComMode( syncWakeUpChannelIterator, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
      {
        ComM_SetBusComModeReq( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      lHighestCalculatedBusComMode[syncWakeUpChannelIterator] = ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    }
    ComM_SetEcuMWakeUpIndicationSyncCounter( channel, ComM_GetEcuMWakeUpIndicationSyncCounter( channel, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */


    /* #12 If PNC Support is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#  if (COMM_PNC_SUPPORT == STD_ON)
#   if (COMM_PNC_PS_TIMER > 0u)
    ComM_ActivateAllPncs();
#   endif
#  endif
  }

  /* Update ComM_NmStartUpIndicationSyncCounter */
  /* #20 Check if an Nm WakeUp-Indication occurred on a satellite partition. */
#  if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
  if ( ComM_GetNmStartUpIndicationSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    /* #21 Wake up every channel which is not yet in FULL_COMMUNICATION by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( syncWakeUpChannelIterator = 0u; syncWakeUpChannelIterator < ComM_GetSizeOfChannel(); syncWakeUpChannelIterator++ )
    {
#   if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
      if ( ComM_IsFrShutdown( syncWakeUpChannelIterator, ComM_GetPartitionConfigIdxOfChannel( syncWakeUpChannelIterator ) ) )
      {
        /* #211 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
         *      because shutdown on FlexRay shall not be interrupted.
         *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
        ComM_SetBusComModeReqFrShutdown( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      else
#   endif
      {
        if ( (ComM_GetActiveComMode( syncWakeUpChannelIterator, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
        {
          ComM_SetBusComModeReq( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
        lHighestCalculatedBusComMode[syncWakeUpChannelIterator] = ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
      }
    }
    ComM_SetNmStartUpIndicationSyncCounter( channel, ComM_GetNmStartUpIndicationSyncCounter( channel, PARTITION_IDX ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

    /* #22 If PNC Support is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#   if (COMM_PNC_SUPPORT == STD_ON)
#    if (COMM_PNC_PS_TIMER > 0u) /* ESCAN00067357 */
    ComM_ActivateAllPncs();
#    endif
#   endif
  }
#  endif
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
# endif

/**********************************************************************************************************************
 *  ComM_IsRequestPendingOnSatellite()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_IsRequestPendingOnSatellite( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_VAR) busComModeReq
#  if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  , P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReqFrShutdown
#  endif
  )
{
  /* ----- Local Variables ---------------------------------------------- */
#  if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF) )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );
#  endif
#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  NetworkHandleType channelIterator;
#  endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a wakeup event occurred on a satellite partition and set the value of the provided busComModeReq and busComModeReqFrShutdown accordingly. */
#  if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  if ( ComM_GetBusComModeReqFrShutdownSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUS_TYPE_FR */
  {
    *busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown( channel, PARTITION_IDX ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }
  else
  {
    *busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown( channel, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }
#  endif
#  if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF) )
  if ( ComM_GetBusComModeReqSyncCounter( channel, PARTITION_IDX ) == (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NM_LINSLAVE_ONLY_AND_SYNC_WUP_ENABLED XF */
  {
    *busComModeReq = ComM_GetBusComModeReq( channel, PARTITION_IDX ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }
  else
#  endif
  {
    *busComModeReq = ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
  }

#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  for ( channelIterator = 0u; channelIterator < ComM_GetSizeOfChannel(); channelIterator++ )
  {
    if ( (ComM_GetEcuMWakeUpIndicationSyncCounter( channelIterator, ComM_GetPartitionConfigIdxOfChannel( channelIterator ) ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channelIterator, COMM_MASTER_PARTITION_IDX ) + 1u))
#   if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
      || (ComM_GetNmStartUpIndicationSyncCounter( channelIterator, ComM_GetPartitionConfigIdxOfChannel( channelIterator ) ) == (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channelIterator, COMM_MASTER_PARTITION_IDX ) + 1u))
#   endif
      )
    {
      *busComModeReq = ComM_GetWakeupStateOfChannel( channel ); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
      break;
    }
  }
#  endif
}

/**********************************************************************************************************************
 *  ComM_InitMasterAndSatelliteChannelSyncCounter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_InitMasterAndSatelliteChannelSyncCounter(CONST(NetworkHandleType, AUTOMATIC) channel COMM_PARTITION_TYPE_LAST )
{
  ComM_SetGlobalPartitionSyncCounter( channel, 0u, PARTITION_IDX );                                  /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

  ComM_SetBusSmStateSyncCounter( channel, 0u, PARTITION_IDX );                                       /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

  ComM_SetForwardNmPassiveStartUpSyncCounter( channel, 0u, PARTITION_IDX );                          /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  ComM_SetForwardRequestBusSmModeSyncCounter( channel, 0u, PARTITION_IDX );                          /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  ComM_SetForwardingBusSmMode( channel, 0u, PARTITION_IDX );                                         /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

# if ( COMM_NMSUPPORTOFCHANNEL == STD_ON )
  ComM_SetActiveComModeSyncCounter( channel, 0u, PARTITION_IDX );                                    /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if ( (COMM_NMSUPPORTOFCHANNEL == STD_ON) || (COMM_BUS_TYPE_FR_PRESENT == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF) )
  ComM_SetBusComModeReqSyncCounter(channel, 0u, PARTITION_IDX);                                      /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
  ComM_SetBusSleepModeIndicatedSyncCounter(channel, FALSE, PARTITION_IDX);                           /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
  ComM_SetBusComModeReqFrShutdownSyncCounter(channel, 0u, PARTITION_IDX);                            /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) && (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
  ComM_SetMinFullComModeTimerSyncCounter(channel, 0x00u, PARTITION_IDX);                             /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
  ComM_SetNmLightTimerSyncCounter(channel, 0x00u, PARTITION_IDX);                                    /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
  ComM_SetInitiateResetSyncCounter( 0u, PARTITION_IDX );                                             /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if ((COMM_DCM_INDICATION) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON))
  ComM_SetDcmRequestActiveSyncCounter(channel, 0u, PARTITION_IDX);                                   /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if (COMM_USERMODENOTIFUNC == STD_ON)
#  if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
  ComM_SetStateChangeSyncCounter(channel, 0u, PARTITION_IDX);                                        /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  endif
# endif

# if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) )
  ComM_SetForwardNmNetworkReleaseSyncCounter(channel, 0u, PARTITION_IDX);                            /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  ComM_SetForwardNmNetworkRequestSyncCounter(channel, 0u, PARTITION_IDX);                            /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if ( COMM_PNC_SUPPORT == STD_ON )
  ComM_SetForwardComSendSignalSyncCounter(channel, 0u, PARTITION_IDX);                               /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  ComM_SetForwardingComSendSignalIndex(channel, 0u, PARTITION_IDX);                                  /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif

# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
#  if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
  ComM_SetNmStartUpIndicationSyncCounter( channel, 0u, PARTITION_IDX );                              /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  endif
  ComM_SetEcuMWakeUpIndicationSyncCounter( channel, 0u, PARTITION_IDX );                             /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_PreInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PreInit( P2CONST( ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA ) ConfigPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx;

#if ( COMM_USE_INIT_POINTER == STD_ON )
  uint8 errorId = COMM_E_NO_ERROR;

  /* #10 In the post-build variant: Save the pointer to configuration. */
  ComM_ConfigPtr = ConfigPtr;

  /* ----- Development Error Checks ------------------------------------- */
  if ( ComM_ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#else
  COMM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ )
    {
      ComM_SetInitStatus( COMM_UNINIT, partitionIdx ); /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }

#if ( COMM_MULTIPARTITION == STD_ON )
    if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u ) /* PRQA S 2992, 2996 */ /* MD_ComM_2996_2992 */ /*lint !e650 */ /* COV_COMM_GENDATA_CHECK */
#endif
    {
      ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }

#if ( COMM_DEV_ERROR_DETECT == STD_ON )
    ComM_SetPreInitialized( TRUE );
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( COMM_USE_INIT_POINTER == STD_ON )
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_PREINIT, errorId );
  }
# else
  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif
}


/**********************************************************************************************************************
 *  ComM_PostInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PostInit( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionIdentifiersIterType partitionIdx;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#if ( COMM_MULTIPARTITION == STD_ON )
  if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u ) /* PRQA S 2992, 2996 */ /* MD_ComM_2996_2992 */ /*lint !e650 */ /* COV_COMM_GENDATA_CHECK */
#endif
  {
    ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ )
    {
      if ( ComM_GetInitStatus( partitionIdx ) != COMM_INIT )
      {
        errorId = COMM_E_NOT_INITED;
        break;
      }
    }

    if ( errorId == COMM_E_NO_ERROR )
    {
      ComM_SetPostInitialized( 0u, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_POSTINIT, errorId );
  }
# endif
}

/***********************************************************************************************************************
 *  ComM_Init()
 **********************************************************************************************************************/
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
FUNC(void, COMM_CODE) ComM_Init(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  uint8 ComM_ChannelIndex;
  ComM_SizeOfUserReqFullComType ComM_ByteIndex;
#if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#endif

#if ( (COMM_USERMODENOTIFUNC == STD_ON) || (COMM_FULL_COMM_REQUEST_NOTIF == STD_ON) || (COMM_MULTIPARTITION == STD_ON))
  ComM_SizeOfUserType userIndex;
#endif

#if (COMM_NVM_SUPPORT == STD_ON)
  NvM_RequestResultType requestResult = NVM_REQ_NOT_OK;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if ( COMM_USE_INIT_POINTER == STD_ON )
  /* #10 Check that configuration pointer is valid. */
  if ( ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
# if ( COMM_USE_ECUM_BSW_ERROR_HOOK == STD_ON )
    EcuM_BswErrorHook( (uint16)COMM_MODULE_ID, (uint8)ECUM_BSWERROR_NULLPTR );
# endif
  }
  else
#endif
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the received partition index is valid. */
  if ( PARTITION_IDX >= ComM_GetSizeOfPartitionIdentifiers() )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  else
# endif
  /* #30 Check that module is pre-initialized. */
  if ( ComM_IsPreInitialized() != TRUE )
  {
    errorId = COMM_E_NO_PREINIT;
  }
  else
  /* #40 Check that module is not already initialized. */
  if (ComM_GetInitStatus(PARTITION_IDX) == COMM_INIT)
  {
    errorId = COMM_E_ALREADY_INITIALIZED;
  }
  else
#endif
#if ( COMM_USE_INIT_POINTER == STD_ON )
# if ( COMM_FINALMAGICNUMBER == STD_ON )
  /* #50 Check the magic number. */  /* SPEC-35483 */
  if (ComM_GetFinalMagicNumber() != COMM_FINAL_MAGIC_NUMBER) /* COV_COMM_MAGICNUMBER_CHECK */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
    EcuM_BswErrorHook((uint16) COMM_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Restore the NvM data. */
#if (COMM_NVM_SUPPORT == STD_ON)
    if( NvM_GetErrorStatus((NvM_BlockIdType)COMM_NVM_BLOCK_ID, &requestResult) != E_OK )                 /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */
    {
      errorId = COMM_E_ERROR_IN_PROV_SERVICE;
    }

    if(requestResult != NVM_REQ_OK) /* The last NvM read request has failed, initialize with configured data. */
#endif
    {
      ComM_GetInhibition(PARTITION_IDX).ComM_ECUGroupClassification = ComM_ECUGroupClassInit;  /* initialize the ECU group classification with the configured value */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      ComM_GetInhibition(PARTITION_IDX).ComM_InhibitCnt             = 0x00u;                   /* reset the inhibition counter */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }

    /* #70 Initialize ComM variables. */
#if ( COMM_FULL_COMM_REQUEST_NOTIF == STD_ON )
    ComM_CurrentChannelRequestInit();

    for( userIndex = 0u; userIndex < ComM_GetSizeOfFullComRequesters(PARTITION_IDX); userIndex++)
    {
      ComM_SetFullComRequesters(userIndex, 0xFFu, PARTITION_IDX);                                         /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
#endif

#if ( COMM_USERMODENOTIFUNC == STD_ON )
# if (COMM_MULTIPARTITION == STD_ON )
    if ( PARTITION_IDX == COMM_MASTER_PARTITION_IDX )
# endif
    {
      for( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++)
      {
        ComM_SetLastStateChange(userIndex, COMM_NO_COMMUNICATION, PARTITION_IDX);                         /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
    }
#endif

    for(ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++)
    {
      ComM_SetBusComModeReq(ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX);            /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

#if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
      ComM_SetBusSleepModeIndicated(ComM_ChannelIndex, FALSE, PARTITION_IDX);                             /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

#if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
      ComM_SetBusComModeReqFrShutdown(ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX);  /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      ComM_SetFrShutdown(ComM_ChannelIndex, FALSE, PARTITION_IDX);                                        /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

      ComM_SetActiveComMode(ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX);            /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      ComM_SetBusSmState(ComM_ChannelIndex, COMM_NO_COMMUNICATION, PARTITION_IDX);                        /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

#if (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
      ComM_SetMinFullComModeTimer(ComM_ChannelIndex, 0x00u, PARTITION_IDX);                               /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

#if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
      ComM_SetNmLightTimer(ComM_ChannelIndex, 0x00u, PARTITION_IDX);                                      /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

      ComM_SetComAllowed(ComM_ChannelIndex, FALSE, PARTITION_IDX);                                        /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

#if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
      if ( PARTITION_IDX == COMM_MASTER_PARTITION_IDX )
# endif
      {
        ComM_SetResetRequired( ComM_ChannelIndex, FALSE, PARTITION_IDX );                                 /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
#endif

#if ((COMM_DCM_INDICATION) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON))
      ComM_SetDcmRequestActive(ComM_ChannelIndex, FALSE, PARTITION_IDX);                                  /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

#if (COMM_USERMODENOTIFUNC == STD_ON)
# if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
      ComM_SetStateChange(ComM_ChannelIndex, COMM_NO_STATE_CHANGE, PARTITION_IDX);                        /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif
#endif

#if (COMM_NVM_SUPPORT == STD_ON)
      if(requestResult == NVM_REQ_OK)
      {
        /* Successfully restored non-volatile data, ComM_InhibitionStatus[] now contains the restored status of Wake-up Inhibition.
        * Now add the configured value of ComMNoCom (No Com Mode Limitation) which is not stored non-volatile. */
        ComM_GetInhibition(PARTITION_IDX).ComM_InhibitionStatus[ComM_ChannelIndex] |= (uint8)(ComM_GetInhibitionInitValueOfChannel(ComM_ChannelIndex) & COMM_WAKEUP_INHIBITION_CLEAR_MASK); /* PRQA S 2985 */ /* MD_ComM_2985 */ /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }
      else
#endif
      {
        /* Mode Limitation and Wake-up Inhibition are disabled or couldn't restore non-volatile data, initialize with configured values. */
        ComM_GetInhibition(PARTITION_IDX).ComM_InhibitionStatus[ComM_ChannelIndex] = ComM_GetInhibitionInitValueOfChannel(ComM_ChannelIndex); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      }

#if ( COMM_MULTIPARTITION == STD_ON )
      ComM_InitMasterAndSatelliteChannelSyncCounter(ComM_ChannelIndex, PARTITION_IDX);
#endif

#if (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
    ComM_SetSimulatedSmMode( ComM_ChannelIndex, COMM_NO_COMMUNICATION, PARTITION_IDX);                    /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif
    }

    for(ComM_ByteIndex = 0u; ComM_ByteIndex < ComM_GetSizeOfUserReqFullCom(PARTITION_IDX); ComM_ByteIndex++)
    {
      ComM_SetUserReqFullCom(ComM_ByteIndex, COMM_NO_REQUESTS, PARTITION_IDX);                            /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }

#if ( COMM_MULTIPARTITION == STD_ON )
    for (userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++)
    {
      ComM_SetForwardUserReqSyncCounter(userIndex, 0u, PARTITION_IDX);                                    /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      ComM_SetForwardingUserReqMode(userIndex, 0u, PARTITION_IDX);                                        /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
#endif

#if (COMM_NVM_SUPPORT == STD_ON)
    ComM_SetNvMStoreReq(FALSE, PARTITION_IDX);                                                            /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#endif

#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
    if ( PARTITION_IDX == COMM_MASTER_PARTITION_IDX )
# endif
    {
      ComM_SetEcuMWakeUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );                                /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
      ComM_SetNmStartUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );                                 /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
#endif

#if (COMM_PNC_SUPPORT)
    ComM_PncInit( COMM_PARTITION_PARA_ONLY ); /* PRQA S 2987 */ /* MD_ComM_2987 */
#endif

    /* #80 Check generated data and if successful set the initialization status to initialized. */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
    if (ComM_InitCheckGeneratedData(COMM_PARTITION_PARA_ONLY) == COMM_E_NO_ERROR) /* COV_COMM_GENDATA_CHECK */ /* PRQA S 2995, 2991 */ /* MD_ComM_2995_2991 */
#endif
    {
      ComM_SetInitStatus( COMM_INIT, PARTITION_IDX );                                                     /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    }
  }

#if ( COMM_USE_INIT_POINTER == STD_OFF )
  COMM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_INIT, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if (COMM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  ComM_InitCheckGeneratedData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL_INLINE FUNC( uint8, COMM_CODE ) ComM_InitCheckGeneratedData( COMM_PARTITION_TYPE_ONLY )
{
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_ChannelIterType channelIndex;
  ComM_ChannelIterType sizeOfChannel = ComM_GetSizeOfChannel();
# if (COMM_PNC_SUPPORT)
  ComM_PncIterType sizeOfPnc = ComM_GetSizeOfPnc();
# endif
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_PNC_SUPPORT == STD_ON )
  ComM_PncSignalValuesIterType sizeOfPncSignalValues = ComM_GetSizeOfPncSignalValues( PARTITION_IDX );
#  endif
# endif
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  ComM_EcuMWakeUpIndicatedIterType sizeOfEcuMWakeUpIndicated = ComM_GetSizeOfEcuMWakeUpIndicated( COMM_MASTER_PARTITION_IDX );
  ComM_NmStartUpIndicatedIterType sizeOfNmStartUpIndicated = ComM_GetSizeOfNmStartUpIndicated( COMM_MASTER_PARTITION_IDX );
# endif

  if ( sizeOfChannel != ComM_GetSizeOfChannelPb() ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# if (COMM_PNC_SUPPORT)
  else if ( sizeOfPnc != ComM_GetSizeOfPncPb() ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_PNC_SUPPORT == STD_ON )
  else if ( sizeOfPncSignalValues != ComM_GetSizeOfPncSignalValuesSyncCounter( PARTITION_IDX ) ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
#  endif
# endif
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  else if ( sizeOfEcuMWakeUpIndicated == 0u ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else if ( sizeOfNmStartUpIndicated == 0u ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif
  else
  {
    for (channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++) /* PRQA S 0771 */ /* MD_ComM_0771 */
    {
      uint8 maxNumberOfStates = ComM_GetWakeupStateOfChannel(channelIndex);
      /* Check the target wake-up state of the channel because it is used as index in ComM_TransitionFctTable */
      if (maxNumberOfStates >= COMM_MAX_NUMBER_OF_STATES) /* COV_COMM_GENDATA_CHECK */ /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /*lint !e506 */
      {
        errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
        break;
      }

# if (COMM_MANAGINGCHANNELOFCHANNEL == STD_ON)
      if ( (ComM_GetManagingChannelIdOfChannel( channelIndex ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL) && (ComM_GetManagingChannelIdOfChannel( channelIndex ) >= sizeOfChannel) ) /* COV_COMM_GENDATA_CHECK XF tf xf */ /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /*lint !e506 */
      {
        errorId = COMM_E_WRONG_GENERATED_DATA; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
        break;
      }
# endif
    }
  }

# if ( COMM_MULTIPARTITION == STD_ON )
  COMM_DUMMY_STATEMENT_CONST( PARTITION_IDX ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return errorId;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_PncInit( COMM_PARTITION_TYPE_ONLY )
{
  ComM_UserReqPncFullComIterType userReqPncFullComIndex;
  ComM_PncIterType pncIndex;
  ComM_PncSignalValuesIterType pncSignalValueIndex;
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PncPartitionIndirectionIterType partitionConfig;
# endif

# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  ComM_ChannelIterType channelIndex;
  ComM_PncLimitationIterType pncLimitationIndex;
# endif

  /* #10 Initialize all PNC related data. */
  for (userReqPncFullComIndex = 0u; userReqPncFullComIndex < ComM_GetSizeOfUserReqPncFullCom(PARTITION_IDX); userReqPncFullComIndex++)
  {
    ComM_SetUserReqPncFullCom(userReqPncFullComIndex, COMM_NO_REQUESTS, PARTITION_IDX);            /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  }

  for (pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++)
  {
    ComM_SetPncPSleepTimer(pncIndex, 0x00u, PARTITION_IDX);                                        /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    ComM_SetActivePncComMode(pncIndex, COMM_PNC_NO_COMMUNICATION, PARTITION_IDX);                  /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    ComM_SetBusPncComModeReq(pncIndex, COMM_PNC_NO_COMMUNICATION, PARTITION_IDX);                  /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetBusPncComModeReqSyncCounter(pncIndex, 0u, PARTITION_IDX);                              /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
    if ( PARTITION_IDX == COMM_MASTER_PARTITION_IDX )
    {
      for ( partitionConfig = 0u; partitionConfig < ComM_GetSizeOfPncPartitionIndirection(); partitionConfig++ )
      {
        if ( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionConfig ) + pncIndex) < ComM_GetBusPncComModeReqMasterSyncCounterEndIdxOfPncPartitionIndirection( partitionConfig ) ) /* PRQA S 2985 */ /* MD_ComM_2985 */ /* COV_COMM_MASTER_SYNCCOUNTER_IND */
        {
          ComM_SetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionConfig ) + pncIndex), 0u, COMM_MASTER_PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */ /* SBSW_COMM_MASTER_SYNCCOUNTER_IND */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }
      }
    }
# endif

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON ) && ( COMM_INTERMEDIATEPNCCOORDINATOROFPNC== STD_ON )
    ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, FALSE, PARTITION_IDX );           /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, 0u, PARTITION_IDX );           /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  endif
# endif
  }

  /* #20 Clear stored signal values (EIRA_RX, ERA_RX, EIRA_TX) */
  for (pncSignalValueIndex = 0u; pncSignalValueIndex < ComM_GetSizeOfPncSignalValues(PARTITION_IDX); pncSignalValueIndex++)
  {
    ComM_SetPncSignalValues(pncSignalValueIndex, 0x00, PARTITION_IDX);                             /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetPncSignalValuesSyncCounter(pncSignalValueIndex, 0u, PARTITION_IDX);                    /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif
  }

  /* #30 If PNC Routing Limitation is enabled, set channel status to not limited. */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
  for (channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++)
  {
    ComM_SetPncToChannelRoutingState(channelIndex, COMM_PNCROUTING_NOT_LIMITED, PARTITION_IDX);    /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetPncToChannelRoutingStateSyncCounter(channelIndex, 0u, PARTITION_IDX);                  /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
#  endif
  }

  for (pncLimitationIndex = 0u; pncLimitationIndex < ComM_GetSizeOfPncLimitation(PARTITION_IDX); pncLimitationIndex++)
  {
    ComM_SetPncLimitation(pncLimitationIndex, (uint8)0x00u, PARTITION_IDX);                        /* SBSW_COMM_CSL01 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
  }
# endif

# if (COMM_USERMODENOTIFUNC == STD_ON)
  ComM_SetNotifyPncStateChange(FALSE, PARTITION_IDX);                                              /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif
} /* PRQA S 6010, 6080 */ /* MD_MSR_STPTH, MD_MSR_STMIF */
#endif /* COMM_PNC_SUPPORT == STD_ON */

/*********************************************************************************************************************
 *  ComM_DeInit()
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
FUNC(void, COMM_CODE) ComM_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_ChannelIterType channelIndex;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 If Multi-Partition is supported, check if the service is called from the correct partition context */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if ( COMM_MULTIPARTITION == STD_ON )
  if ( ComM_GetInternalPartitionIdx( GetCurrentApplicationID() ) != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  else
# endif
# if ( COMM_USE_INIT_POINTER == STD_ON )
  if ( ComM_ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_NOT_INITED;
  }
  else
# endif
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #20 Trigger storage of non-volatile values. Only store the Wake-up Inhibition bit, clear the No Com Mode Limitation bit,
     *     it shall not be stored to NvM. */
#if (COMM_NVM_SUPPORT == STD_ON)
    if ( ComM_IsNvMStoreReq( COMM_MASTER_PARTITION_IDX ) )
    {
      for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ )
      {
        ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[channelIndex] &= COMM_WAKEUP_INHIBITION_MASK; /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }

      if ( NvM_SetRamBlockStatus( (NvM_BlockIdType)COMM_NVM_BLOCK_ID, TRUE ) != E_OK )
      {
        errorId = COMM_E_ERROR_IN_PROV_SERVICE;
      }

      ComM_SetNvMStoreReq( FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#endif

    /* #30 Iterate over the channels and check if all are in state COMM_NO_COM_NO_PENDING_REQUEST.
     *     If so, set the initialization status to COMM_UNINIT. */
    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ )
    {
      if ( ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COM_NO_PENDING_REQUEST )
      {
        break; /* De-Initialization impossible */
      }
    }

    if ( channelIndex == ComM_GetSizeOfChannel() ) /* All channels are in state COMM_NO_COM_NO_PENDING_REQUEST because break above was not reached */
    {
#if ( COMM_MULTIPARTITION == STD_ON )
      if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u ) /* PRQA S 2992, 2996 */ /* MD_ComM_2996_2992 */ /*lint !e650 */ /* COV_COMM_GENDATA_CHECK */
#endif
      {
        ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
      ComM_SetInitStatus( COMM_UNINIT, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#if (COMM_MULTIPARTITION == STD_OFF)
      ComM_InitMemory(); /* PRQA S 2987 */ /* MD_ComM_2987 */
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_DEINIT, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/*********************************************************************************************************************
 *  ComM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_InitMemory(void)
{
  /* #10 Set pre-initialization status of ComM to FALSE. */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  ComM_SetPreInitialized( FALSE );
#endif

#if (COMM_USE_INIT_POINTER == STD_ON)
  /* #20 In the post-build variant: set config pointer to NULL. */
  ComM_ConfigPtr = NULL_PTR;
#endif
}

/**********************************************************************************************************************
 *  ComM_GetState()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_GetState( NetworkHandleType Channel, P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_VAR) State)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = COMM_MASTER_PARTITION_IDX;
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
# endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'State': it must be a valid pointer. */
  else if (State == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the current active state of the channel by default. */
# if ( COMM_MULTIPARTITION == STD_ON ) && ( COMM_NMSUPPORTOFCHANNEL == STD_ON )
    if (ComM_GetActiveComModeSyncCounter(Channel, ComM_GetPartitionConfigIdxOfChannel(Channel)) == (uint8)(ComM_GetActiveComModeSyncCounter(Channel, COMM_MASTER_PARTITION_IDX) + 1u)) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
    {
      partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
    }
# endif
    *State = ComM_GetActiveComMode(Channel, PARTITION_IDX); /* SBSW_COMM_PARAMETER_PTR_WRITE */

    /* #50 If the active state is COMM_NO_COM_NO_PENDING_REQUEST and there is a pending request (external or internal),
     *     the current state depends on Extended RAM Check functionality:
     *     1) if Extended RAM Check functionality is used, the ComM channel is allowed to enter COMM_NO_COM_REQUEST_PENDING state depending on the check result.
     *     2) if Extended RAM Check functionality is not used, store COMM_NO_COM_REQUEST_PENDING.
     *     Note: COMM_NO_COM_REQUEST_PENDING ensures that the request can be considered and ECU stays in RUN mode. */
    if (ComM_GetActiveComMode(Channel, PARTITION_IDX ) == COMM_NO_COM_NO_PENDING_REQUEST)
    {
      if (ComM_ChannelIsRequestPending(Channel) == TRUE)
      {
#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
        *State = ComM_ChannelGetMaxStateAfterExtRamCheck( Channel, COMM_NO_COM_REQUEST_PENDING ); /* SBSW_COMM_PARAMETER_PTR_WRITE */
#else
        *State = COMM_NO_COM_REQUEST_PENDING; /* SBSW_COMM_PARAMETER_PTR_WRITE */
#endif
      }
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETSTATE, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/*********************************************************************************************************************
 *  ComM_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus( P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_VAR) Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'Status': it must be a valid pointer. */
  retVal = E_NOT_OK;
  if (Status == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If parameter checks are successful, store the initialization state of ComM. */
# if ( COMM_USE_INIT_POINTER == STD_ON )
    if ( ComM_ConfigPtr == NULL_PTR )
    {
      *Status = COMM_UNINIT; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }
    else
# endif
    {
      *Status = ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) ? COMM_INIT : COMM_UNINIT; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETSTATUS, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_RequestComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskUserIndex;
  ComM_UserReqFullComType userReqFullCom = 0u;
#if (COMM_PNC_SUPPORT == STD_ON)
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex;
  ComM_UserReqPncFullComType userReqPncFullCom = 0u;
#endif
#if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfUserType partitionIdx = ComM_GetPartitionConfigIdxOfUser( User ); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User'. */
  else if ( User >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode'. */
  /* If called with ComMode != COMM_NO_COMMUNICATION && ComMode != COMM_FULL_COMMUNICATION (ESCAN00068871) */
  else if ( (ComMode != COMM_NO_COMMUNICATION) && (ComMode != COMM_FULL_COMMUNICATION) )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #40 Check that the received partition index is valid. */
  else if ( PARTITION_IDX >= ComM_GetSizeOfPartitionIdentifiers() ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_INVALID_PARTITION; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = E_OK; /* Can be set to COMM_E_MODE_LIMITATION later. */

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    /* #50 If COMM_NO_COMMUNICATION is requested, clear the user request in each related byte of UserReqFullCom and UserReqPncFullCom. */
    if ( ComMode == COMM_NO_COMMUNICATION )
    {
#if (COMM_MULTIPARTITION == STD_ON)
# if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#  if ( COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
#  endif
      {
        ComM_SetForwardUserReqSyncCounter( User, (uint8)(ComM_GetForwardUserReqSyncCounter( User, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        ComM_SetForwardingUserReqMode( User, COMM_NO_COMMUNICATION, PARTITION_IDX ); /* SBSW_COMM_USER_CHECK */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
# endif
#endif

      for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), PARTITION_IDX ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));
#if ( COMM_MULTIPARTITION == STD_ON)
# if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#  if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
#  endif
        {
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }
#  if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        else
#  endif
# endif
#endif
#if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        {
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }
#endif
      }

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
      if ( ComM_HasPncPb() )
# endif
      {
        if ( ComM_IsPncUserOfUser( User ) )
        {
          for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));
# if ( COMM_MULTIPARTITION == STD_ON)
#  if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#   if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )  /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
#   endif
            {
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
#   if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            else
#   endif
#  endif
# endif
# if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            {
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
# endif
          }
        }
      }
#endif
    }
    else
    {
      /* #60 If COMM_FULL_COMMUNICATION is requested, set the user request in each related byte of UserReqFullCom and UserReqPncFullCom. */
#if (COMM_MULTIPARTITION == STD_ON)
# if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#  if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
#  endif
      {
        ComM_SetForwardUserReqSyncCounter( User, (uint8)(ComM_GetForwardUserReqSyncCounter( User, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_USER_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        ComM_SetForwardingUserReqMode( User, COMM_FULL_COMMUNICATION, PARTITION_IDX ); /* SBSW_COMM_USER_CHECK */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
# endif
#endif

      for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( User ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( User ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), PARTITION_IDX ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));
#if (COMM_MULTIPARTITION == STD_ON)
# if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#  if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER */
#  endif
        {
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }
#  if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        else
#  endif
# endif
#endif
#if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
        {
          ComM_SetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex ), userReqFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        }
#endif

        /* #70 If Mode Limitation NO_COM or Prevent Wake-up is active, ComM stores the FULL_COM request but does not perform it. */
#if (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON))
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
        if ( ComM_ChannelIsLimited( ComM_GetChannelOfUserByteMask( iByteMaskUserIndex ) ) == TRUE )
# endif
        {
          retVal = COMM_E_MODE_LIMITATION;
        }
#endif
      }

#if (COMM_PNC_SUPPORT == STD_ON)
# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
      if ( ComM_HasPncPb() )
# endif
      {
        if ( ComM_IsPncUserOfUser( User ) )
        {
          for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( User ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( User ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), PARTITION_IDX ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));
# if (COMM_MULTIPARTITION == STD_ON)
#  if (COMM_EXISTS_USER_ON_SATELLITE_PARTITION == STD_ON)
#   if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE */
#   endif
            {
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
#   if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            else
#   endif
#  endif
# endif
# if (COMM_EXISTS_USER_ON_MASTER_PARTITION == STD_ON)
            {
              ComM_SetUserReqPncFullCom( ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex ), userReqPncFullCom, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
            }
# endif
          }
        }
      }
#endif
    }

    /* #80 If FULL_COM request is inhibited, increment the inhibition counter and store it in NvM. */
#if (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON))
    if ( retVal == COMM_E_MODE_LIMITATION )
    {
      if ( ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitCnt < 65535u )
      {
        ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitCnt++; /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# if (COMM_NVM_SUPPORT == STD_ON)
        ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
# endif
      }
    }
#endif
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_REQUESTCOMMODE, errorId );
  }
#else
  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if ((COMM_ISDEF_USERBYTEMASKSTARTIDXOFUSER == STD_ON) && (COMM_PNC_SUPPORT == STD_OFF))
  COMM_DUMMY_STATEMENT( User ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif

  return retVal;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/*********************************************************************************************************************
 *  ComM_GetMaxComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_GetMaxComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
#if (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON))
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
#endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'ComMode': it must be a valid pointer. */
  else if (ComMode == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'User': it must be valid. */
  else if (User >= ComM_GetSizeOfUser())
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, get the maximum allowed communication mode of the user:
     *     NO_COM if Mode Limitation NO_COM or Prevent Wake-up is active on at least one channel mapped to the user,
     *     FULL_COM otherwise. */
    *ComMode = (ComM_ModeType)COMM_FULL_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */ /* PRQA S 2982 */ /* MD_ComM_2982 */

#if (((COMM_MODE_LIMITATION == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)) || (COMM_WAKEUP_INHIBITION == STD_ON))
    for (iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser(User); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
    {
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
      if (ComM_ChannelIsLimited(ComM_GetChannelOfUserByteMask(iByteMaskIndex)) == TRUE)
# endif
      {
        *ComMode = (ComM_ModeType)COMM_NO_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
      }
    }
#endif
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETMAXCOMMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  COMM_DUMMY_STATEMENT_CONST(User); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/*********************************************************************************************************************
 *  ComM_GetRequestedComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_GetRequestedComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
#if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfUserType partitionIdx = ComM_GetPartitionConfigIdxOfUser( User ); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'ComMode'. */
  else if (ComMode == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'User'. */
  else if (User >= ComM_GetSizeOfUser())
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #40 Check that the received partition index is valid. */
  else if (PARTITION_IDX >= ComM_GetSizeOfPartitionIdentifiers())  /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_INVALID_PARTITION; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 If parameter checks are successful, get the requested mode of the user:
     *     NO_COM if at least one channel mapped to the user is not requested or
     *       if this PNC user is mapped to PNCs without channels (special case) and at least one PNC is not requested.
     *     FULL_COM otherwise. */
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    /* Set the default value to NO_COM to consider users having no channels and no PNCs, in this case the function does not change it. ESCAN00092198 */
    *ComMode = COMM_NO_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
#if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
# if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
    if (ComM_IsUserMappedToPncWithoutChannels(User) == TRUE)
    {
      /* Special case: this PNC user is mapped to PNCs without channels */
      for(iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser(User); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
      {
        if( (ComM_GetUserReqPncFullCom(ComM_GetUserReqPncFullComIdxOfUserPncByteMask(iByteMaskIndex), PARTITION_IDX) & ComM_GetSetMaskOfUserPncByteMask(iByteMaskIndex)) != COMM_NO_REQUESTS)
        {
          *ComMode = COMM_FULL_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
        }
        else
        {
          *ComMode = COMM_NO_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
          break;
        }
      }
    }
    else
# endif
#endif
    {
      for(iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser(User); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
      {
        if( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask(iByteMaskIndex), PARTITION_IDX ) & ComM_GetSetMaskOfUserByteMask(iByteMaskIndex)) != COMM_NO_REQUESTS)
        {
          *ComMode = COMM_FULL_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
        }
        else
        {
          *ComMode = COMM_NO_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
          break;
        }
      }
    }

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETREQUESTEDCOMMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT(User); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return retVal;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/*********************************************************************************************************************
 *  ComM_GetCurrentComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
  ComM_ModeType tmpComMode;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if (User >= ComM_GetSizeOfUser())
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer. */
  else if (ComMode == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, get the lowest BusSM mode of all channels mapped to the user. */
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    /* Set the default value to NO_COM to consider users having no channels and no PNCs, in this case the function does not change it. ESCAN00092198 */
    *ComMode = COMM_NO_COMMUNICATION; /* PRQA S 2982 */ /* MD_ComM_2982 */ /* SBSW_COMM_PARAMETER_PTR_WRITE */
    for(iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser(User); iByteMaskIndex = ComM_GetUserByteMaskEndIdxOfUser(User)) /* FETA_COMM_CSL_USE_CASE */
    {
      /* If user has channels or PNCs, set the default value to FULL_COM by executing this loop exactly once. */
      *ComMode = COMM_FULL_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }

    for(iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser(User); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser(User); iByteMaskIndex++) /* FETA_COMM_CSL_USE_CASE */
    {
      retVal = ComM_GetGetCurrentBusSMModeApiOfChannel(ComM_GetChannelOfUserByteMask(iByteMaskIndex))(ComM_GetChannelOfUserByteMask(iByteMaskIndex), &tmpComMode); /* SBSW_COMM_BusSMMode_PointerCall */

      if (retVal == E_NOT_OK)
      {
        errorId = COMM_E_ERROR_IN_PROV_SERVICE;
        break;
      }
      else
      {
        if (tmpComMode < *ComMode)
        {
          *ComMode = tmpComMode; /* SBSW_COMM_PARAMETER_PTR_WRITE */
        }
      }
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

    if (errorId == COMM_E_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETCURRENTCOMMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  if (COMM_ISDEF_USERBYTEMASKSTARTIDXOFUSER == STD_ON)
  COMM_DUMMY_STATEMENT(User); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (COMM_PNC_SUPPORT == STD_ON)
/*********************************************************************************************************************
 *   ComM_GetCurrentPNCComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentPNCComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode)
{
  /* ----------------------- Local Variables ----------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_EXISTS_USER_WITH_SINGLE_PNC == STD_ON)
  ComM_PncIdxOfUserPncByteMaskType pncIdx;
# endif
  /* ------------------- Development Error Checks ------------------------ */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if (!ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX))
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if (User >= ComM_GetSizeOfUser())
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer. */
  else if (ComMode == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ------------------------ Implementation ------------------------------- */
    /* Set the default communication mode value to NO_COM.*/
    *ComMode = COMM_NO_COMMUNICATION; /* PRQA S 2982 */ /* MD_ComM_2982 */ /* SBSW_COMM_PARAMETER_PTR_WRITE */

    /* #40 Check if 'User' is a PNC user. */
    if (!ComM_IsPncUserOfUser(User))
    {
      retVal = E_NO_PNC_ASSIGNED;
    }
    else
# if (COMM_EXISTS_USER_WITH_SINGLE_PNC == STD_ON)
    /* #50 Check if 'User' is assigned to multiple PNCs. */
    if ( ComM_GetUserPncByteMaskLengthOfUser(User) > 1u) /* COV_COMM_EXISTS_ONLY_SINGLE_PNC_USER */
    {
      retVal = E_MULTIPLE_PNC_ASSIGNED;
    }
    else
    {
      retVal = E_OK;
      pncIdx = ComM_GetPncIdxOfUserPncByteMask(ComM_GetUserPncByteMaskStartIdxOfUser(User));
      /* #60 If there is exactly one PNC assigned to 'User', set the current communication mode of the PNC. */
      if (ComM_GetActivePncComMode(pncIdx, COMM_MASTER_PARTITION_IDX) != COMM_PNC_NO_COMMUNICATION) /* PRQA S 2843 */ /* MD_ComM_2842_2843 */ /*lint !e661 */
      {
        *ComMode = COMM_FULL_COMMUNICATION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
      }
    }
# else
    {
      retVal = E_MULTIPLE_PNC_ASSIGNED;
    }
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETCURRENTPNCCOMMODE, errorId);
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  ComM_GetInhibitionStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetInhibitionStatus( NetworkHandleType Channel, P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_VAR) Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Status': it must be valid pointer. */
  else if (Status == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the inhibition status of the channel. */
    *Status = ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[Channel]; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETINHIBITIONSTATUS, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}


/*********************************************************************************************************************
 *  ComM_PreventWakeUp()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp( NetworkHandleType Channel, boolean Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
# endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful and the feature Prevent Wake-up is enabled, process the request. */
#if ( COMM_WAKEUP_INHIBITION == STD_ON )
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
  if(Status == TRUE)
  {
    /* #50 Only accept the request if ECU Group Classification supports Prevent Wake-up. */
    /* ESCAN00068902 */
    if( (ComM_GetInhibition(COMM_MASTER_PARTITION_IDX).ComM_ECUGroupClassification & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
    {
      ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[Channel] |= COMM_WAKEUP_INHIBITION_MASK; /* SBSW_COMM_CHANNEL_CSL02 */
# if (COMM_NVM_SUPPORT == STD_ON)
      ComM_SetNvMStoreReq(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #60 Always accept releasing the Prevent Wake-up. */
    ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[Channel] &= COMM_WAKEUP_INHIBITION_CLEAR_MASK; /* SBSW_COMM_CHANNEL_CSL02 */
# if (COMM_NVM_SUPPORT == STD_ON)
    ComM_SetNvMStoreReq(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif
    retVal = E_OK;
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#else
    /* #70 Reject the request if the feature Prevent Wake-up is disabled. */
    /* ESCAN00068896 */
    retVal = E_NOT_OK;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
# if ( COMM_WAKEUP_INHIBITION == STD_ON )
  if (errorId != COMM_E_NO_ERROR)
# endif
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_PREVENTWAKEUP, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if ( COMM_WAKEUP_INHIBITION == STD_OFF )
  COMM_DUMMY_STATEMENT_CONST(Status); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if ( COMM_DEV_ERROR_DETECT == STD_OFF )
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/*********************************************************************************************************************
 *  ComM_LimitChannelToNoComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitChannelToNoComMode( NetworkHandleType Channel, boolean Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
# endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful and the feature Mode Limitation to NO_COM is enabled and
     *     at least one channel has Nm Type other than PASSIVE, process the request. */
#if ((COMM_MODE_LIMITATION == STD_ON) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON))
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    if (Status == TRUE)
    {
      /* #50 Accept a request for Mode Limitation to NO_COM if
       *     1) the channel has Nm Type other than NONE (because ComM does not shutdown communication on NONE) and
       *     2) ECU Group Classification supports Mode Limitation NO_COM. */
      /* ESCAN00068896 */ /* ESCAN00106344 */
# if (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF)
#  if (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON)
      if (ComM_GetNmTypeOfChannel(Channel) != COMM_NONE_NMTYPEOFCHANNEL)
#  endif
      {
        if (((ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_ECUGroupClassification) & (COMM_MODE_LIMITATION_MASK)) == COMM_MODE_LIMITATION_MASK )
        {
          ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[Channel] |= COMM_MODE_LIMITATION_MASK; /* SBSW_COMM_CHANNEL_CSL02 */
          retVal = E_OK;
#  if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
          if (ComM_GetActiveComMode(Channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED)
          {
            ComM_SetResetRequired(Channel, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
#  endif
        }
      }
# endif
    }
    else
    {
      /* #60 Accept releasing the Mode Limitation to NO_COM if the channel has Nm Type other than PASSIVE. */
      /* Note: a possible use case for NONE is when limitation has been set initially in the configuration. */
# if (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON)
      if (ComM_GetNmTypeOfChannel(Channel) != COMM_PASSIVE_NMTYPEOFCHANNEL)
# endif
      {
        ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitionStatus[Channel] &= COMM_MODE_LIMITATION_CLEAR_MASK; /* SBSW_COMM_CHANNEL_CSL02 */
        retVal = E_OK;
# if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
        ComM_SetResetRequired(Channel, FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# endif
      }
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
#endif
    /* #70 Reject the request if the feature Mode Limitation to NO_COM is disabled or all channels have Nm Type PASSIVE. */
    /* ESCAN00068896 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_LIMITCHANNELTONOCOMMODE, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if ((COMM_MODE_LIMITATION == STD_OFF) || (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_OFF) || (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_ON))
  COMM_DUMMY_STATEMENT_CONST(Status); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# if (COMM_DEV_ERROR_DETECT == STD_OFF)
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
#endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */


/*********************************************************************************************************************
 *  ComM_LimitECUToNoComMode()
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
FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode(boolean Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
#if ( (COMM_MODE_LIMITATION == STD_ON) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF) )
  NetworkHandleType ComM_ChannelIndex;
#endif
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
    retVal = E_NOT_OK;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful and the feature Mode Limitation to NO_COM is enabled and
     *     at least one channel has Nm Type other than PASSIVE and at least one channel has Nm Type other than NONE, request mode limitation for each channel. */
#if ( (COMM_MODE_LIMITATION == STD_ON) && (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_OFF) )
    retVal = E_OK;
    for(ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++)
    {
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
      if(ComM_IsEnabledOfChannel(ComM_ChannelIndex))
# endif
      {
        if( ComM_LimitChannelToNoComMode(ComM_ChannelIndex, Status) != E_OK )
        {
          /* #40 If the request failed for at least one channel, the overall status shall be E_NOT_OK as well. */
          /* ESCAN00068896 */
          retVal = E_NOT_OK;
        }
      }
    }
#else
    /* #50 Reject the request if the feature Mode Limitation to NO_COM is disabled or all channels have Nm Type PASSIVE or all channels have Nm Type NONE. */
    /* (ESCAN00068896) */
    retVal = E_NOT_OK;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_LIMITECUTONOCOMMODE, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

#if ( ((COMM_MODE_LIMITATION == STD_OFF) || (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_OFF)) || (COMM_EXISTS_ONLY_NONE_NMTYPEOFCHANNEL == STD_ON) )
  COMM_DUMMY_STATEMENT_CONST(Status); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/*********************************************************************************************************************
 *  ComM_ReadInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ReadInhibitCounter(P2VAR(uint16, AUTOMATIC, COMM_APPL_VAR) CounterValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'CounterValue': it must be a valid pointer. */
  else if (CounterValue == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the inhibition counter. */
    *CounterValue = ComM_GetInhibition(COMM_MASTER_PARTITION_IDX).ComM_InhibitCnt; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_READINHIBITCOUNTER, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}


/*********************************************************************************************************************
 *  ComM_ResetInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
    /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
    retVal = E_NOT_OK;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, set the inhibition counter to 0 and mark NvM data to be stored if NvM support is enabled. */
    ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitCnt = (uint16)0x00; /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

#if (COMM_NVM_SUPPORT == STD_ON)
    ComM_SetNvMStoreReq(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#endif
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_RESETINHIBITCOUNTER, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}


/*********************************************************************************************************************
 *  ComM_SetECUGroupClassification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  retVal = E_NOT_OK;
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Status': it must have a value between 0 and 3. */
  else if (Status > 0x03u)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, set ECU group classification and mark NvM data to be stored if NvM is supported. */
    ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_ECUGroupClassification = Status; /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */

#if (COMM_NVM_SUPPORT == STD_ON)
    ComM_SetNvMStoreReq(TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#endif
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_SETECUGROUPCLASSIFICATION, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}


#if (COMM_VERSION_INFO_API == STD_ON)
/*********************************************************************************************************************
 *  ComM_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, COMM_APPL_VAR) Versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check Versioninfo for NULL pointer. Note: no initialization check is needed. */
  if (Versioninfo == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set Versioninfo with corresponding macros from component header. */
    Versioninfo->vendorID = COMM_VENDOR_ID; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    Versioninfo->moduleID = COMM_MODULE_ID; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    Versioninfo->sw_major_version = (uint8)COMM_SW_MAJOR_VERSION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    Versioninfo->sw_minor_version = COMM_SW_MINOR_VERSION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    Versioninfo->sw_patch_version = COMM_SW_PATCH_VERSION; /* SBSW_COMM_PARAMETER_PTR_WRITE */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETVERSIONINFO, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

/**********************************************************************************************************************
 *  ComM_CommunicationAllowed()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_CommunicationAllowed(NetworkHandleType Channel, boolean Allowed)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
#  if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
#  endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the Communication Allowed status of the Channel. */
    ComM_SetComAllowed(Channel, Allowed, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_COMMUNICATIONALLOWED, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}

#if (COMM_DCM_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  ComM_GetDcmRequestStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetDcmRequestStatus(NetworkHandleType Channel, P2VAR(boolean, AUTOMATIC, COMM_APPL_VAR) Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON) && (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = COMM_MASTER_PARTITION_IDX;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Status': it must be a valid pointer. */
  else if (Status == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Store TRUE if Dcm indicated active diagnostic. Store FALSE otherwise. */
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
# if ( COMM_MULTIPARTITION == STD_ON )
    if (ComM_GetDcmRequestActiveSyncCounter(Channel, ComM_GetPartitionConfigIdxOfChannel(Channel)) == (uint8)(ComM_GetDcmRequestActiveSyncCounter(Channel, COMM_MASTER_PARTITION_IDX) + 1u)) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
    {
      partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
    }
# endif
    if (ComM_IsDcmRequestActive(Channel, PARTITION_IDX )) /* COV_COMM_NM_LINSLAVE_ONLY XF */
    {
      *Status = TRUE; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }
    else
# endif
    {
      *Status = FALSE; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETDCMREQUESTSTATUS, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}
#endif

#if (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  ComM_GetMinFullComModeTimerStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, COMM_CODE) ComM_GetMinFullComModeTimerStatus(NetworkHandleType Channel, P2VAR(boolean, AUTOMATIC, COMM_APPL_VAR) Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Status': it must be a valid pointer. */
  else if (Status == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Store TRUE if MinFullComModeTimer is running. Store FALSE otherwise. */
    if (ComM_GetMinFullComModeTimer(Channel, COMM_MASTER_PARTITION_IDX) > 0x00u)
    {
      *Status = TRUE; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }
    else
    {
      *Status = FALSE; /* SBSW_COMM_PARAMETER_PTR_WRITE */
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_GETMINFULLCOMMODETIMERSTATUS, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}
#endif

#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_MainFunction_Satellite()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_MainFunction_Satellite( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_PartitionIdentifiersIterType partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
  ComM_ModeType forwardingMode;
# if ( (COMM_PNC_SUPPORT == STD_ON) && ( (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON) || (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON) ) )
  P2CONST( ComM_PncSignalValuesType, AUTOMATIC, COMM_VAR_NO_INIT) pncSignalValuesPtr;
# endif

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check parameter 'Channel': it must be valid and active in the current variant. */
  if ( Channel >= ComM_GetSizeOfChannel() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  else if ( PARTITION_IDX >= ComM_GetSizeOfPartitionIdentifiers() ) /* PRQA S 2996, 2992 */ /* MD_ComM_2996_2992 */ /* COV_COMM_GENDATA_CHECK */ /*lint !e506 */
  {
    errorId = COMM_E_INVALID_PARTITION; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (COMM_ENABLEDOFCHANNEL == STD_ON)
    /* If the channel is de-activated in the current variant, the satellite main function shall do nothing. */
    if ( ComM_IsEnabledOfChannel( Channel ) ) /* COV_COMM_MP_ALL_PB_VARIANT_CHANNELS_ON_MASTER */
#endif
    {
      /* #30 Check forwarding-flags on each partition and forward calls to corresponding BusNm/SM on the current partition, if needed. */

      /* Check BusSmRequestComMode-Flag */
      if ( ComM_GetForwardRequestBusSmModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) == (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, PARTITION_IDX ) + 1u) )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
        ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)ComM_GetForwardRequestBusSmModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        forwardingMode = ComM_GetForwardingBusSmMode( Channel, COMM_MASTER_PARTITION_IDX );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();
        ComM_ForwardRequestBusSmMode( Channel, forwardingMode );
      }

# if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
      /* Check NetworkRequest-Flag */
      ComM_CheckSyncCounterAndForwardFunction( &ComM_GetForwardNmNetworkRequestSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CHECKSYNCCOUNTER_PTR_CALL */
                                                                                            &ComM_GetForwardNmNetworkRequestSyncCounter( Channel, PARTITION_IDX ),
                                                                                            Nm_NetworkRequest, Channel );

      /* Check NetworkRelease-Flag */
      ComM_CheckSyncCounterAndForwardFunction( &ComM_GetForwardNmNetworkReleaseSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CHECKSYNCCOUNTER_PTR_CALL */
                                                                                            &ComM_GetForwardNmNetworkReleaseSyncCounter( Channel, PARTITION_IDX ),
                                                                                            Nm_NetworkRelease, Channel );
# endif

# if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
      /* Check PassiveStartUp-Flag */
      ComM_CheckSyncCounterAndForwardFunction( &ComM_GetForwardNmPassiveStartUpSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), /* SBSW_COMM_CHECKSYNCCOUNTER_PTR_CALL */
                                                                                            &ComM_GetForwardNmPassiveStartUpSyncCounter( Channel, PARTITION_IDX ),
                                                                                            Nm_PassiveStartUp, Channel );
# endif

# if ( (COMM_PNC_SUPPORT == STD_ON) && ( (COMM_EXISTS_EIRA_TX_TYPEOFPNCSIGNAL == STD_ON) || (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON) ) )
      /* Check ComSendSignal-Flag */
      if ( ComM_GetForwardComSendSignalSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) == (uint8)(ComM_GetForwardComSendSignalSyncCounter( Channel, PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS */
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_6();
        ComM_SetForwardComSendSignalSyncCounter( Channel, (uint8)ComM_GetForwardComSendSignalSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
        pncSignalValuesPtr = ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_GetForwardingComSendSignalIndex( Channel, COMM_MASTER_PARTITION_IDX ) ), COMM_MASTER_PARTITION_IDX );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_6();

        (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_GetForwardingComSendSignalIndex( Channel, COMM_MASTER_PARTITION_IDX ) ), pncSignalValuesPtr ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */
      }
# endif

    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION_SATELLITE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  ComM_MainFunction()
 **********************************************************************************************************************/
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
FUNC(void, COMM_CODE) ComM_MainFunction(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  errorId = COMM_E_NO_ERROR;
  ComM_StateType  highestComModeReq;     /* The highest Com mode request */
  boolean  internalRequest;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that module is initialized. If not, the function returns immediately without performing any functionality and without raising any errors. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    /* no error reporting */
  }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if (Channel >= ComM_GetSizeOfChannel())
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (COMM_MULTIPARTITION == STD_ON)
# if (COMM_ISDEF_MAINFUNCTIONNEEDSSYNCOFCHANNEL == STD_OFF)
    if ( ComM_IsMainFunctionNeedsSyncOfChannel( Channel ) )
# endif
    {
      ComM_UpdateDataFromSatellites();
    }
#endif

#if (COMM_ENABLEDOFCHANNEL == STD_ON)
    /* If the channel is de-activated in the current variant, the main function shall do nothing
     * besides handling of PN prepare sleep timer. */
    if (ComM_IsEnabledOfChannel(Channel))
#endif
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();

#if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) && (COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      ComM_ProcessSynchronousWakeUp(); /* PRQA S 2987 */ /* MD_ComM_2987 */
#endif

      /* #30 If parameter checks are successful and the channel is active in the current variant, get the highest
       *     requested communication mode and perform a state transition accordingly. */
      highestComModeReq = ComM_CalculateHighestComMode(Channel, &internalRequest); /* SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE */

      /* #31 If Extended RAM Check functionality is used and communication needs to be started on the channel,
       *     limit the highest requested mode according to the RAM Check result. */
#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
      if ((ComM_GetActiveComMode(Channel, COMM_MASTER_PARTITION_IDX ) <= COMM_NO_COM_REQUEST_PENDING) && (highestComModeReq > COMM_NO_COM_REQUEST_PENDING))
      {
        highestComModeReq = ComM_ChannelGetMaxStateAfterExtRamCheck( Channel, highestComModeReq );
      }
#endif

      ComM_ChannelStateTransition(Channel, highestComModeReq);

      /* #40 If User Mode Notification is enabled and BusSM indicated a state change on the channel or the last notification failed,
       *     notify the mode change of channel users via RTE Mode Switch Interface.
       *     Note: do it outside of the exclusive area to avoid that calls to RTE extend the interrupt lock time. */
#if (COMM_USERMODENOTIFUNC == STD_ON)
# if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
      if (ComM_GetStateChange(Channel, COMM_MASTER_PARTITION_IDX ) != COMM_NO_STATE_CHANGE)
      {
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
        ComM_StateChangeNotification(Channel);
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();
      }
# endif
#endif

      /* #50 If Notification of Channel Requesters is enabled, update the data via RTE Sender-Receiver Interface if applicable.
       *     Note: do it outside of the exclusive area to avoid that calls to RTE extend the interrupt lock time. */
#if( COMM_FULL_COMM_REQUEST_NOTIF == STD_ON )
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
      ComM_CurrentChannelRequestUpdate( Channel ); /* PRQA S 2987 */ /* MD_ComM_2987 */
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();
#endif

      /* #60 If Minimum FULL_COM Mode or Nm Light timer are enabled, handle the timers. */
#if ((COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON) || (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
      ComM_ChannelHandleTimers(Channel, internalRequest);
#endif

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
    } /* ComM_IsEnabledOfChannel(Channel) */

    /* #70 If Partial Networking is enabled and the current channel id is 0:
     *     1) handle the optional PNC Prepare Sleep Timer. Note: the timer value is same for all PNCs, it is calculated based on the cycle time of channel 0.
     *     2) if PNC Extended Functionality is enabled, handle PNCs without channels and perform state transitions if applicable. */
    /* #80 If Partial Networking is enabled and the current channel is enabled, handle PNCs with channels and perform state transitions if applicable. */
#if (COMM_PNC_SUPPORT == STD_ON)
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();

# if ( ComM_GetSizeOfChannel() > 1u ) /* ComM_GetSizeOfChannel represents the superset number of channels over all variants. */
    if (Channel == 0u)
# endif
    {
# if (COMM_PNC_PS_TIMER > 0u)
      ComM_HandlePncPrepareSleepTimer(); /* PRQA S 2987 */ /* MD_ComM_2987 */
# endif
# if (COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON)
#  if (COMM_PNC_EXISTS_PNC_WITHOUT_CHANNELS == STD_ON)
      ComM_PncModeArbitrationWithoutChannels();
#  endif
# endif
    }

# if (COMM_PNCCHANNELMAPPING == STD_ON)
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
    /* If the channel is de-activated in the current variant, the main function shall do nothing
     * besides handling of PN prepare sleep timer. */
    if (ComM_IsEnabledOfChannel(Channel))
#  endif
    {
      ComM_PncModeArbitration( Channel );
    }
# endif

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();

    /* #90 If User Mode Notification is enabled, notify the mode change of PNC users via RTE Mode Switch Port. */
# if (COMM_USERMODENOTIFUNC == STD_ON)
    if( ComM_IsNotifyPncStateChange( COMM_MASTER_PARTITION_IDX ) )
    {
      /* PNC users: a PNC changed the state from FULL_COM to NO_COM or vice versa or the last mode notification failed (ESCAN00067386) */
      if( ComM_PncStateChangeNotification() == E_OK )
      {
        /* All PNC users have been successfully notified about new mode, reset the flag.
         * Otherwise, if at least one user mode notification failed, ComM will try again in the next task (ESCAN00067423) */
        ComM_SetNotifyPncStateChange(FALSE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
    }
# endif
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_MAINFUNCTION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  ComM_ChannelStateTransition()
 **********************************************************************************************************************/
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
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelStateTransition(NetworkHandleType channel, ComM_StateType highestComModeReq)
{
  ComM_StateType  calculatedComModeReq;  /* the calculated ComM mode */
  ComM_StateType  localhighestComModeReq = highestComModeReq;
  do /* FETA_COMM_CHANNEL_STATE_TRANSITION */
  {
    /* #10 Check if a timer is running for Nm Variants FULL or LIGHT. Do not perform any state transitions if
     *    1) Nm Light timer is running and there is no FULL_COM request by users or DCM (applicable in COMM_FULL_COM_READY_SLEEP or COMM_SILENT_COM) or
     *    2) Min FULL_COM mode timer is running.
     */
#if ((COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON) || (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
    if(
# if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
       ((ComM_GetNmLightTimer(channel, COMM_MASTER_PARTITION_IDX ) > 0x00u) && (localhighestComModeReq == COMM_NO_COM_NO_PENDING_REQUEST)) ||
# endif
       (ComM_GetMinFullComModeTimer(channel, COMM_MASTER_PARTITION_IDX) > 0x00u)
      ) /* COV_COMM_TIMER_INIT_VALUE_CAN_BE_ONE */
    {
      localhighestComModeReq = ComM_GetActiveComMode(channel, COMM_MASTER_PARTITION_IDX );
    }
    /* #20 If no timer is running, calculate the next state transition step and perform as many transitions as needed
     *     to reach the target state, which is defined by the highest requested communication mode. */
    else
#endif
    {
      calculatedComModeReq = ComM_TransitionTable[localhighestComModeReq][ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX )];
      if ((ComM_GetActiveComMode(channel, COMM_MASTER_PARTITION_IDX ) < COMM_MAX_NUMBER_OF_STATES) && (calculatedComModeReq < COMM_MAX_NUMBER_OF_STATES))
      {
        if ((ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) == COMM_NO_COM_REQUEST_PENDING) && (calculatedComModeReq > COMM_NO_COM_REQUEST_PENDING))
        {
          /* #30 If the channel is in COMM_NO_COM_REQUEST_PENDING state and there is a pending request to start communication,
           *     check the Communication Allowed flag. If Communication Allowed is FALSE, stay in COMM_NO_COM_REQUEST_PENDING state
           *     and try again in the next task if request is still pending. */
          if( !ComM_IsComAllowed( channel, COMM_MASTER_PARTITION_IDX ) )
          {
            localhighestComModeReq = ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX );
          }
        }

        if (localhighestComModeReq != ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ))
        {
          ComM_TransitionFctTable[calculatedComModeReq][ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX )]( channel COMM_MASTER_PARTITION_PARA_LAST ); /* SBSW_COMM_TransitionFct_PointerCall */
          /* #40 If current channel is Nm variant LINSLAVE and current state is COMM_FULL_COM_READY_SLEEP, but the Bus Sleep Mode indication is still pending,
                 channel remains in state COMM_FULL_COM_READY_SLEEP. */
#if (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON)
          if ( !(ComM_IsBusSleepModeIndicated( channel, COMM_MASTER_PARTITION_IDX )) && (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_READY_SLEEP) && (calculatedComModeReq == COMM_SILENT_COM)
# if (COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF)
            && (ComM_GetNmTypeOfChannel(channel) == COMM_LINSLAVE_NMTYPEOFCHANNEL)
# endif
            )
          {
            /* Set localhighestComModeReq to state COMM_FULL_COM_READY_SLEEP in order to remain in state Ready Sleep and to prevent endless while loop. */
            localhighestComModeReq = COMM_FULL_COM_READY_SLEEP;
          }
          else
#endif
          {
            ComM_SetActiveComMode( channel, calculatedComModeReq, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
        }
      }
      else
      {
        localhighestComModeReq = ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX );
      }
    }
  } while (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) != localhighestComModeReq);
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ((COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON) || (COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
/**********************************************************************************************************************
 *  ComM_ChannelHandleTimers()
 **********************************************************************************************************************/
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
COMM_LOCAL_INLINE FUNC(void, COMM_CODE) ComM_ChannelHandleTimers(NetworkHandleType channel, boolean internalRequest)
{
  /* #10 If Minimum FULL_COM mode timer is running, decrement the timer.
   *     Cancel the timer in case an internal request (ComM user or Dcm) exists and the channel has Nm Variant LIGHT. */
  if(ComM_GetMinFullComModeTimer(channel, COMM_MASTER_PARTITION_IDX) > 0x00u)
  {
    ComM_DecMinFullComModeTimer(channel, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* PRQA S 3387 */ /* MD_ComM_3387 */

#  if (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON)
#   if (COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF)
    if (ComM_GetNmTypeOfChannel(channel) == COMM_LIGHT_NMTYPEOFCHANNEL) /* COV_COMM_SUBSET_OF_ELEMENTS */
#   endif
    {
      if (internalRequest == TRUE)
      {
        ComM_SetMinFullComModeTimer(channel, 0x00, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
    }
#  endif

    /* #20 If Minimum FULL_COM mode timer is expired, the communication is not required anymore and channel can leave Network Requested state.
     *     If Nm Variant is FULL the next state is Ready Sleep where ComM waits for a shutdown trigger from Nm.
     *     If Nm Variant is LIGHT the target state is NO_COM, the transition depends on configuration of Nm Light and Nm Light Silent timers. */
    if (ComM_GetMinFullComModeTimer(channel, COMM_MASTER_PARTITION_IDX ) == 0x00u)
    {
      /* Set the default state, it's accepted to overwrite it later (if Nm Type is FULL) to keep the code cleaner */
      ComM_SetBusComModeReq(channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

#  if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
#   if (COMM_ISDEF_NMTYPEOFCHANNEL == STD_OFF)
      if (ComM_GetNmTypeOfChannel(channel) == COMM_FULL_NMTYPEOFCHANNEL) /* COV_COMM_SUBSET_OF_ELEMENTS */
#   endif
      {
        ComM_SetBusComModeReq(channel, COMM_FULL_COM_READY_SLEEP, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
    }
  }

  /* #30 If Nm Light or Nm Light Silent timer is running, decrement the timer.
   *     If the timer is expired set the target state is NO_COM. Note that the variable ComM_NmLightTimer is
   *     shared by Nm Light and Nm Light Silent timers. If Nm Light timer is expired, the target state is NO_COM but
   *     the next state will be SI_COM in case Nm Light Silent timer is configured. */
# if ((COMM_NMLIGHTDURATIONOFCHANNEL == STD_ON) || (COMM_NMLIGHTSILENTDURATIONOFCHANNEL == STD_ON))
  if( ComM_GetNmLightTimer(channel, COMM_MASTER_PARTITION_IDX) > 0x00u )
  {
    ComM_DecNmLightTimer(channel, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /* PRQA S 3387 */ /* MD_ComM_3387 */

    if(ComM_GetNmLightTimer(channel, COMM_MASTER_PARTITION_IDX ) == 0x00u) /* COV_COMM_TIMER_INIT_VALUE_CAN_BE_ONE */
    {
      ComM_SetBusComModeReq(channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
  }
# endif

# if (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_OFF)
  COMM_DUMMY_STATEMENT(internalRequest); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif


/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR NM)
**********************************************************************************************************************/

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_NetworkStartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( COMM_MULTIPARTITION == STD_ON )
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
# endif
    /* #30 If parameter checks are successful and Nm support is enabled, forward the indication to the passive wake-up processing. */
    ComM_ProcessPassiveWakeUp( Channel COMM_PARTITION_PARA_LAST );
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_NETWORKSTARTINDICATION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_NetworkMode()
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
FUNC(void, COMM_CODE) ComM_Nm_NetworkMode(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_StateType activeComMode = COMM_NO_COM_NO_PENDING_REQUEST;
  ComM_StateType busComModeReq = COMM_NO_COM_NO_PENDING_REQUEST;
# endif
# if (COMM_SILENTSUPPORTOFCHANNEL == STD_ON) && (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel ); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  NetworkHandleType channelIdx;
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful and Nm support is enabled, process Network Mode notification. */
    /* #40 If Prepare Bus-Sleep Mode is supported and Network Mode is notified while ComM is in SI_COM and Nm
     *     has notified Prepare Bus Sleep Mode before it, immediately enter the target wake-up state of the channel
     *     (Network Requested or Ready Sleep) and trigger BusSM to enter FULL_COM to ensure that Nm message
     *     can be sent as first message on the bus. */
# if (COMM_SILENTSUPPORTOFCHANNEL == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

    /* Check if an event occurred on a satellite which has changed the active com mode since last master sync. */
    if ( ComM_GetActiveComModeSyncCounter( Channel, PARTITION_IDX ) == (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_SILENT_SUPPORT */
    {
      activeComMode = ComM_GetActiveComMode( Channel, PARTITION_IDX );
    }
    else
    {
      activeComMode = ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX );
    }

    if ( ComM_GetBusComModeReqSyncCounter( Channel, PARTITION_IDX ) == (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_SILENT_SUPPORT */
    {
      busComModeReq = ComM_GetBusComModeReq( Channel, PARTITION_IDX );
    }
    else
    {
      busComModeReq = ComM_GetBusComModeReq( Channel, COMM_MASTER_PARTITION_IDX );
    }

#   if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
    if ( (busComModeReq & COMM_FULL_COMMUNICATION) == 0u )
    {
      for ( channelIdx = 0u; channelIdx < ComM_GetSizeOfChannel(); channelIdx++ )
      {
        if ( ComM_GetEcuMWakeUpIndicationSyncCounter( channelIdx, ComM_GetPartitionConfigIdxOfChannel( channelIdx ) ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channelIdx, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          /* Set busComModeReq to WakeUpState to ensure that following condition is not entered. */
          busComModeReq = ComM_GetWakeupStateOfChannel( Channel );
          break;
        }
      }
    }
#   endif

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();

    if ( (activeComMode == COMM_SILENT_COM) && (busComModeReq == COMM_SILENT_COM) )
#  else
    if((ComM_GetActiveComMode(Channel, PARTITION_IDX) == COMM_SILENT_COM ) && ( ComM_GetBusComModeReq(Channel, PARTITION_IDX) == COMM_SILENT_COM ))
#  endif
    {
      /* ESCAN00066282 Note: ComM_ActiveComMode is set first to ensure that condition is not re-entered. */
      ComM_SetActiveComMode(Channel, ComM_GetWakeupStateOfChannel(Channel), PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  if (COMM_MULTIPARTITION == STD_ON)
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#   endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2(); /* Critical section will be left inside the following transition function. */

        /* Increment sync counter for ComM_ActiveComMode by 1 (relative to the corresponding Master-Counter) */
        ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
      ComM_TransitionFctTable[ComM_GetWakeupStateOfChannel(Channel)][COMM_SILENT_COM](Channel COMM_PARTITION_PARA_LAST); /* SBSW_COMM_TransitionFct_PointerCall */

      /* SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2() -> done in transition function (ComM_TF_NoCom_NetReq() or ComM_TF_NoCom_FullReadySleep()). */
    }
# endif

# if (COMM_DEV_ERROR_DETECT == STD_OFF)
#  if(COMM_SILENTSUPPORTOFCHANNEL == STD_OFF)
    COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_NETWORKMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if ((COMM_NMSUPPORTOFCHANNEL == STD_ON) && (COMM_SILENTSUPPORTOFCHANNEL == STD_ON))
/*********************************************************************************************************************
 *  ComM_Nm_PrepareBusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( COMM_MULTIPARTITION == STD_ON )
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
# endif
    /* #30 If parameter checks are successful and Prepare Bus-Sleep mode is supported, initiate the state transition to SI_COM. */
    ComM_SetBusComModeReq(Channel, COMM_SILENT_COM, PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    ComM_SetActiveComMode(Channel, COMM_SILENT_COM, PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

# if (COMM_MULTIPARTITION == STD_ON)
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#  endif
    {
      /* Increment sync counter for ComM_BusComModeReq and ComM_ActiveComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
# endif

    ComM_TF_Full_SiCom(Channel COMM_PARTITION_PARA_LAST);

  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_PREPAREBUSSLEEPMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_BusSleepMode()
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
FUNC(void, COMM_CODE) ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
# endif
  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if (!ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX))
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( COMM_MULTIPARTITION == STD_ON )
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
# endif
    /* #30 If parameter checks are successful and support of Network Management is enabled, process the Bus Sleep Mode indication. */
# if( COMM_BUS_TYPE_FR_PRESENT == STD_ON )
    if ( (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) != COMM_FULL_COM_NETWORK_REQUESTED)
#  if (COMM_ISDEF_BUSTYPEOFCHANNEL == STD_OFF)
      && (ComM_GetBusTypeOfChannel( Channel ) == COMM_BUS_TYPE_FR)
#  endif
      )
    {
      /* #40 Store the status that shutdown on a FlexRay channel has started (this is to counteract the so-called 'clique problem' on FlexRay). */
      ComM_SetFrShutdown(Channel, TRUE, PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
# endif

#  if ( COMM_MULTIPARTITION == STD_ON)
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#   endif
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2(); /* Critical Section will either be left directly below, or in TF_SiCom_NoCom. */

      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    else
#   endif
#  endif
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    {
      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#  endif

    /* #50 There are two use cases how Nm calls the function which can be identified through ComM_ActiveComMode: */
# if (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON)
    if (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED)
    {
      /* #60 ComM_Nm_BusSleepMode() is called in context of Nm_NetworkRelease() while ComM performs the state transition COMM_FULL_COM_NETWORK_REQUESTED to COMM_FULL_COM_READY_SLEEP.
       *     This is only possible if LinNm without coordination (ESCAN00061388, ESCAN00072736) or J1939Nm (ESCAN00071329) is configured on the channel.
       *     Do nothing here and perform the transition to NoCom in the next ComM_MainFunction().
       *     This check is also a safety mechanism against potentially wrong Nm behavior:
       *         It ensures that ComM remains in state COMM_FULL_COM_NETWORK_REQUESTED and does not interrupt the established communication in case of a wrong call of ComM_Nm_BusSleepMode(). */

#  if ( COMM_MULTIPARTITION == STD_ON)
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#   endif
      {
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
#  endif
    }
    else
# endif
    {
      /* #70 ComM_Nm_BusSleepMode() is called from the main function of a Nm module, this is the normal case. ComM shall perform the transition to NoCom immediately. */
      ComM_TF_SiCom_NoCom(Channel COMM_PARTITION_PARA_LAST);

      /* SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2() -> done in ComM_TF_SiCom_NoCom(). */
    }

    /* #80 If FrSM reached NO_COM state, set the FlexRay shutdown status as finished. */
# if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
#  if (COMM_ISDEF_BUSTYPEOFCHANNEL == STD_OFF)
    if (ComM_GetBusTypeOfChannel(Channel) == COMM_BUS_TYPE_FR)
#  endif
    {
      if ((ComM_GetBusSmState(Channel, PARTITION_IDX) == COMM_NO_COMMUNICATION) && (ComM_IsFrShutdown(Channel, PARTITION_IDX)))
      {
        ComM_SetFrShutdown(Channel, FALSE, PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_BUSSLEEPMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_RestartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( COMM_MULTIPARTITION == STD_ON )
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
# endif
    /* #30 If parameter checks are successful and Nm support is enabled, forward the indication to the passive wake-up processing. */
    ComM_ProcessPassiveWakeUp( Channel COMM_PARTITION_PARA_LAST );
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_RESTARTINDICATION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_StateChangeNotification()
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
FUNC(void, COMM_CODE) ComM_Nm_StateChangeNotification( CONST(NetworkHandleType, AUTOMATIC) Channel, CONST(Nm_StateType, AUTOMATIC) NmPreviousState, CONST(Nm_StateType, AUTOMATIC) NmCurrentState )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_PncIterType pnc;
  ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;
  ComM_SizeOfPncSignalType signalIndex;
  boolean sendSignal = FALSE;

# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
  boolean isSignalStoredPncBitSet = FALSE;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if ( COMM_MULTIPARTITION == STD_ON )
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
# endif
    /* PRQA S 0715 SUPPRESS_0715 */ /* MD_MSR_1.1 */
    if (ComM_IsPncLimitationUsedOfChannelPb(Channel)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */ /* COV_COMM_SUBSET_OF_ELEMENTS */
    {
      if (NmCurrentState == NM_STATE_REPEAT_MESSAGE)
      {
        /* #30 If Nm entered NM_STATE_REPEAT_MESSAGE state, set the status bit accordingly. */
        if(ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED )
        {
          /* #300 If PNC routing is limited on the channel, immediately send PNC bits = 1 for all PNC's connected to the channel
           *      and being in state PNC_REQUESTED.
           */
          for (signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
          {
            if ((ComM_GetTypeOfPncSignal(signalIndex) == COMM_EIRA_TX_TYPEOFPNCSIGNAL) && (ComM_GetChannelIdxOfPncSignal(signalIndex) == Channel))
            {
              for (pnc = 0u; pnc < ComM_GetSizeOfPnc(); pnc++)
              {
                for (pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb(pnc); pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb(pnc); pncChIdx++) /* FETA_COMM_CSL_USE_CASE */
                {
# if ( COMM_MULTIPARTITION == STD_ON )
                  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
                  if ( ComM_GetPncSignalValuesSyncCounter( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pnc ), PARTITION_IDX ) ==
                     ( ComM_GetPncSignalValuesSyncCounter( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pnc ), COMM_MASTER_PARTITION_IDX ) + 1u ) ) /* COV_COMM_MP_ONLY_ONE_SATELLITE_WITH_EIRA_TX_SIGNAL */
                  {
                    isSignalStoredPncBitSet = (boolean)ComM_IsSignalStoredPncBitSet( signalIndex, pnc, PARTITION_IDX ); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
                  }
                  else
                  {
                    isSignalStoredPncBitSet = (boolean)ComM_IsSignalStoredPncBitSet( signalIndex, pnc, COMM_MASTER_PARTITION_IDX ); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
                  }
                  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();

                  /* PRQA S 3415 3 */ /* MD_ComM_3415 */
                  if ((isSignalStoredPncBitSet == FALSE) &&
# else
                  /* PRQA S 3415 3 */ /* MD_ComM_3415 */
                  if ((!ComM_IsSignalStoredPncBitSet(signalIndex, pnc, COMM_MASTER_PARTITION_IDX)) &&
# endif
                      (ComM_GetPncChannelMapping(pncChIdx) == Channel) &&
                      (ComM_PncGetGwChannelState(Channel, ComM_GetActivePncComMode(pnc, COMM_MASTER_PARTITION_IDX)) == COMM_FULL_COM_NETWORK_REQUESTED))
                  {
                    /* #310 Attaching the channel: PNC Routing Limitation becomes temporarily disabled, send PNC bit = 1 if
                     *      1) PNC state is COMM_PNC_REQUESTED (the highest activation), then send bit = 1 on each channel or
                     *      2) PNC state is COMM_PNC_REQUESTED_PASSIVE (lower activation), then send bit = 1 on ACTIVE channels only.
                     */
                    ComM_PncSetBitInSignal(signalIndex, pnc COMM_PARTITION_PARA_LAST );
                    sendSignal = TRUE;
                  }
                }
              }

              if (sendSignal == TRUE)
              {
                /* Com Signal can be send directly (no sync needed), since the state change notification is always called in the partition context of the corresponding channel. */
                (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), PARTITION_IDX ) ); /* SBSW_COMM_CALL_WITH_PTR_TO_CONST */ /* PRQA S 0315 */ /* MD_ComM_0315 */ /*lint !e661, !e662 */
              }
              break; /* There is only one EIRA_TX signal per channel */
            }
          }
        }

        /* Example sequence: Routing Limitation is enabled on a channel and a PNC is already in PNC_REQUESTED.
         * Channel user requests FULL_COM, channel enters COMM_NETWORK_REQUESTED and Nm enters NM_STATE_REPEAT_MESSAGE.
         * Then the user requests NO_COM while Nm is still in NM_STATE_REPEAT_MESSAGE. In this case
         * ComM_PncToChannelRoutingState[Channel] shall be COMM_PNCROUTING_NM_REPEAT_MESSAGE and PNC bit=1 shall be
         * sent as long as Nm is in Repeat Message state.
         */
# if (COMM_MULTIPARTITION == STD_ON)
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#  endif
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
          ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          /* Increment sync counter for ComM_PncToChannelRoutingState by 1 (relative to corresponding Master-Counter) */
          ComM_SetPncToChannelRoutingStateSyncCounter( Channel, (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
        }
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        else
#  endif
# endif
# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        {
          ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, PARTITION_IDX ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
# endif
      }
      else if (NmPreviousState == NM_STATE_REPEAT_MESSAGE)
      {
        /* #40 If Nm has left NM_STATE_REPEAT_MESSAGE, clear the status bit. If PNC routing status changes to COMM_PNCROUTING_LIMITED,
         *     ComM will set PNC bits to 0 for all PNC's connected to the channel and being in state PNC_REQUESTED in the next ComM_MainFunction().
         */
# if (COMM_MULTIPARTITION == STD_ON)
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT */
#  endif
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
          ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          /* Increment sync counter for ComM_PncToChannelRoutingState by 1 (relative to corresponding Master-Counter) */
          ComM_SetPncToChannelRoutingStateSyncCounter( Channel, (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
        }
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        else
#  endif
# endif
# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
        {
          ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }
# endif
      }
      else
      {
        /* nothing to do */
      }
    }
    /* PRQA L:SUPPRESS_0715 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_STATECHANGENOTIFICATION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_ForwardSynchronizedPncShutdown()
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
FUNC( void, COMM_CODE ) ComM_Nm_ForwardSynchronizedPncShutdown( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONSTP2CONST( uint8, AUTOMATIC, COMM_APPL_VAR ) PncBitVectorPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_INTERMEDIATEPNCCOORDINATOROFPNC == STD_ON)
  ComM_PncIterType pncIndex;
  ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
#  endif
# endif
  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
    || (!ComM_IsEnabledOfChannel( Channel ))
#  endif
    )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'PncBitVectorPtr': it must be a valid pointer. */
  else if ( PncBitVectorPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If the ECU is in the role of an intermediate PNC coordinator, check if the affected PNCs are qualified for a forward synchronized PNC shutdown indication request.
     *     1) The affect PNC's bit is set to '1' in the PNC bit vector and
     *     2) The indicated channel is assigned to this PNC and has attribute GW type set to COMM_GATEWAY_TYPE_PASSIVE
    */
# if (COMM_INTERMEDIATEPNCCOORDINATOROFPNC == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
#  endif

    if ( ComM_GetGwTypeOfChannel( Channel ) == COMM_GATEWAY_TYPE_PASSIVE )
    {
      for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ )
      {
        if ( (PncBitVectorPtr[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) > 0u )
        {
          for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
                pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( pncIndex ); pncChIdx++ ) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
          {
            if ( (ComM_GetPncChannelMapping( pncChIdx ) == Channel) ) /* COV_COMM_FOR_LOOP_OPTIMIZATION */
            {
              /* #50 Store the request for the synchronized PNC shutdown. Forward the synchronized PNC shutdown request towards the Nm on all active channels as soon as the affected PNC is released.*/
#  if (COMM_MULTIPARTITION == STD_ON)
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
              if (PARTITION_IDX != COMM_MASTER_PARTITION_IDX) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_GWTYPE_PASSIVE */
#   endif
              {
                /* Increment the sync counter for ComM_ForwardNmSyncPncShutdownIndicated by 1 (relative to the corresponding Master-Counter) */
                ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, (uint8)(ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
                ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS */
              }
#   if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
              else
#   endif
#  endif
              {
                ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              }
              break;
            }
          }
        }
      }
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_NM_FORWARDSYNCHRONIZEDPNCSHUTDOWN, errorId );
  }
# else
  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR DCM)
**********************************************************************************************************************/

#if (COMM_DCM_INDICATION == STD_ON)
/*********************************************************************************************************************
 *  ComM_DCM_ActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_DCM_ActiveDiagnostic(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
#  if ( (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) )
  boolean isPassiveOrLinSlaveOfChannel = FALSE;
#  endif
#  if ( COMM_MULTIPARTITION == STD_ON )
#   if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel ); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#   endif
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check that Nm Variant of the channel is not PASSIVE or LINSLAVE. */
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
#  if ( (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) )
    isPassiveOrLinSlaveOfChannel = (boolean)ComM_IsPassiveOrLinSlaveOfChannel(Channel); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    /* PRQA S 2991, 2995 1 */ /* MD_ComM_2995_2991 */
    if (isPassiveOrLinSlaveOfChannel == TRUE) /* COV_COMM_NM_LINSLAVE_OR_PASSIVE_ONLY TX */ /*lint !e506 */
    {
      errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
    }
    else
#  endif
    /* #40 If parameter checks are successful, store the active status of the diagnostic session on the channel. */
    {
#  if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
#   if (COMM_MULTIPARTITION == STD_ON)
#    if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#    endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_SetDcmRequestActive( Channel, TRUE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        /* Increment sync counter for ComM_DcmRequestActive by 1 (relative to the corresponding Master-Counter) */
        ComM_SetDcmRequestActiveSyncCounter( Channel, (uint8)(ComM_GetDcmRequestActiveSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
      }
#    if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      else
#    endif
#   endif
      {
        ComM_SetDcmRequestActive( Channel, TRUE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
    }
# else
    errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
#  if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_OFF)
  /* Don't need a condition because errorId == COMM_E_DIAGNOSTIC_NOT_SUPPORTED in this case */
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  else
#   if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
  if (errorId != COMM_E_NO_ERROR) /* COV_COMM_NM_LINSLAVE_OR_PASSIVE_ONLY TX */
#   endif
#  endif
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_DCM_ACTIVEDIAGNOSTIC, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

#if (COMM_DCM_INDICATION == STD_ON)
/*********************************************************************************************************************
 *  ComM_DCM_InactiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_DCM_InactiveDiagnostic(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
#  if ( (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) )
  boolean isPassiveOrLinSlaveOfChannel = FALSE;
#  endif
#  if ( COMM_MULTIPARTITION == STD_ON )
#   if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel ); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#   endif
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check that Nm Variant of the channel is not PASSIVE or LINSLAVE. */
# if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_ON)
#  if ( (COMM_EXISTS_PASSIVE_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON) )
    isPassiveOrLinSlaveOfChannel = (boolean) ComM_IsPassiveOrLinSlaveOfChannel(Channel); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    /* PRQA S 2991, 2995 1 */ /* MD_ComM_2995_2991 */
    if (isPassiveOrLinSlaveOfChannel == TRUE) /* COV_COMM_NM_LINSLAVE_OR_PASSIVE_ONLY TX */ /*lint !e506 */
    {
      errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
    }
    else
#  endif
    /* #40 If parameter checks are successful, store the inactive status of the diagnostic session on the channel. */
    {
#  if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
#   if (COMM_MULTIPARTITION == STD_ON)
#    if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#    endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_SetDcmRequestActive( Channel, FALSE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        /* Increment sync counter for ComM_DcmRequestActive by 1 (relative to the corresponding Master-Counter) */
        ComM_SetDcmRequestActiveSyncCounter( Channel, (uint8)(ComM_GetDcmRequestActiveSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
      }
#    if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      else
#    endif
#   endif
      {
        ComM_SetDcmRequestActive( Channel, FALSE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
    }
# else
    errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
#  if (COMM_KEEP_AWAKE_CHANNELS_SUPPORT == STD_OFF)
  /* Don't need a condition because errorId == COMM_E_DIAGNOSTIC_NOT_SUPPORTED in this case */
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  else
#   if ( (COMM_EXISTS_FULL_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_LIGHT_NMTYPEOFCHANNEL == STD_ON) || (COMM_EXISTS_NONE_NMTYPEOFCHANNEL == STD_ON) )
  if (errorId != COMM_E_NO_ERROR) /* COV_COMM_NM_LINSLAVE_OR_PASSIVE_ONLY TX */
#   endif
#  endif
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_DCM_INACTIVEDIAGNOSTIC, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR EcuM)
**********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_EcuM_WakeUpIndication()
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
FUNC(void, COMM_CODE) ComM_EcuM_WakeUpIndication(NetworkHandleType Channel)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON )
# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
  uint8 ComM_ChannelIndex;
# endif
#endif
#if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel); /* PRQA S 2842 */ /* MD_ComM_2842_2843 */
#endif
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX ) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
# endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If Synchronous Wake-up is enabled, notify passive wake-up to all ComM channels being in NO_COM or SI_COM state.
     *     This is done by setting the bus-side requested state to FULL_COM (Network Requested or Ready Sleep). */
#if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON )
# if ( COMM_MULTIPARTITION == STD_ON )
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#  endif
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

      ComM_SetEcuMWakeUpIndicationSyncCounter( Channel, (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    else
#  endif
# endif
# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
    {
#  if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
      ComM_SetEcuMWakeUpIndicated( 0u, TRUE, COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_ZERO_INDEX_ACCESS */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#  endif

      for ( ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++ )
      {
        if ( (ComM_GetActiveComMode( ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
        {
          ComM_SetBusComModeReq( ComM_ChannelIndex, ComM_GetWakeupStateOfChannel( ComM_ChannelIndex ), COMM_MASTER_PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

          /* #31 If NM Gateway Extension is enabled notify Nm_PassiveStartUp() for each channel with Nm. */
#  if( COMM_NM_GW_EXT_ENABLED == STD_ON )
#   if (COMM_ISDEF_NMSUPPORTOFCHANNEL == STD_OFF)
          if ( ComM_IsNmSupportOfChannel( ComM_ChannelIndex ) )
#   endif
          {
            (void)Nm_PassiveStartUp( ComM_ChannelIndex );
          }
#  endif
        }
      }

      /* #40 If Synchronous Wake-up is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#  if (COMM_PNC_SUPPORT == STD_ON)
#   if (COMM_PNC_PS_TIMER > 0u) /* ESCAN00067357 */
      ComM_ActivateAllPncs();
#   endif
#  endif
    }
# endif

#else /* #if ( COMM_SYNCHRONOUS_WAKE_UP == STD_OFF ) */

    /* #50 If Synchronous Wake-up is disabled, notify passive wake-up to the channel by setting the bus-side
     *     requested state to FULL_COM (Network Requested or Ready Sleep). Ignore the wake-up in FULL_COM mode. */
    /* The ActiveComMode can always be checked in the master partition since no wakeup indication from EcuM can be called when the channels (i.e. the Nm) is already shutting down. */
    if((ComM_GetActiveComMode(Channel, COMM_MASTER_PARTITION_IDX) & COMM_FULL_COMMUNICATION) == 0u) /* ESCAN00091475 */
    {

# if ( COMM_MULTIPARTITION == STD_ON )
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#  endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

        ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      else
#  endif
# endif
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
      {
        ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif

      /* #51 If NM Gateway Extension is enabled notify Nm_PassiveStartUp(). */
# if( COMM_NM_GW_EXT_ENABLED == STD_ON )
#  if (COMM_ISDEF_NMSUPPORTOFCHANNEL == STD_OFF)
      if (ComM_IsNmSupportOfChannel(Channel)) /* COV_COMM_SUBSET_OF_ELEMENTS */
#  endif
      {
        (void)Nm_PassiveStartUp(Channel);
      }
# endif
    }
# if ( COMM_MANAGINGCHANNELOFCHANNEL == STD_ON )
    /* #52 If the indicated channel is a managed channel, notify the passive wake-up also to the corresponding managing channel
           by setting the bus-side requested state to FULL_COM. Ignore the wake-up in FULL_COM mode. */
    if ( ComM_GetManagingChannelIdOfChannel(Channel) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL )
    {
      if ((ComM_GetActiveComMode(ComM_GetManagingChannelIdOfChannel(Channel), COMM_MASTER_PARTITION_IDX) & COMM_FULL_COMMUNICATION) == 0u)
      {
#  if ( COMM_MULTIPARTITION == STD_ON )
        if ( PARTITION_IDX == COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_MANAGED_CHANNELS_ON_MASTER */
#  endif
        {
          ComM_SetBusComModeReq( ComM_GetManagingChannelIdOfChannel( Channel ), ComM_GetWakeupStateOfChannel( ComM_GetManagingChannelIdOfChannel( Channel ) ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        }

#  if( COMM_NM_GW_EXT_ENABLED == STD_ON )
        (void)Nm_PassiveStartUp(ComM_GetManagingChannelIdOfChannel(Channel));
#  endif
      }
    }
# endif
#endif /* #if ( COMM_SYNCHRONOUS_WAKE_UP == STD_ON ) */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_ECUM_WAKEUPINDICATION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
  COMM_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif
# endif
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#if ((COMM_WAKEUPENABLEDOFPNC == STD_ON) && (COMM_PNC_PS_TIMER > 0u))
/*********************************************************************************************************************
 *  ComM_EcuM_PNCWakeUpIndication()
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
FUNC(void, COMM_CODE) ComM_EcuM_PNCWakeUpIndication( PNCHandleType PNCid )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_WRONG_PARAMETERS;
  ComM_PncIterType pncIndex;
  ComM_PncChannelMappingEndIdxOfPncPbType pncChannelMappingIndex;
  ComM_ChannelIterType channelIndex = 0u;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (COMM_PNC_CHECK_ENABLED_AT_RUNTIME == STD_ON)
    if(ComM_HasPncPb())
# endif
    {
      /* Get the internal PNC index from the given PNC identifier */
      for (pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++)
      {
        if (PNCid == ComM_GetPncIdOfPnc(pncIndex))
        {
          break;
        }
      }

      /* #20 Check parameter 'PNCid': it must be a valid PNC identifier. */
      if (pncIndex < ComM_GetSizeOfPnc())
      {
        /* #30 Wakeup the PNC if it is in NO_COM state and no wake-up occurred before. */
        if ((ComM_GetActivePncComMode(pncIndex, COMM_MASTER_PARTITION_IDX) == COMM_PNC_NO_COMMUNICATION) && (ComM_GetBusPncComModeReq(pncIndex, PARTITION_IDX) == COMM_PNC_NO_COMMUNICATION)) /*lint !e661 */
        {
          ComM_SetBusPncComModeReq(pncIndex, COMM_PNC_PREPARE_SLEEP, PARTITION_IDX); /* SBSW_COMM_PNC_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */ /*lint !e661 */
        }

        /* #40 Propagate the wake-up to all channels that are mapped to the PNC and are in NO_COM or SI_COM state. */
        for (pncChannelMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb(pncIndex);
             pncChannelMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb(pncIndex);
             pncChannelMappingIndex++)
        {
          channelIndex = ComM_GetPncChannelMapping(pncChannelMappingIndex);

          if ((ComM_GetActiveComMode(channelIndex, PARTITION_IDX) & COMM_FULL_COMMUNICATION) == 0u)
          {
            ComM_SetBusComModeReq(channelIndex, ComM_GetWakeupStateOfChannel(channelIndex), PARTITION_IDX); /* SBSW_COMM_BusComModeReq_over_PncChannelMapping_CSL03 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
        }

        /* #50 If Synchronous Wake-up is enabled, propagate the wake-up to all channels that are in NO_COM or SI_COM state. */
# if (COMM_SYNCHRONOUS_WAKE_UP == STD_ON)
        for(channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++)
        {
          if((ComM_GetActiveComMode(channelIndex, PARTITION_IDX) & COMM_FULL_COMMUNICATION) == 0u)
          {
            ComM_SetBusComModeReq(channelIndex, ComM_GetWakeupStateOfChannel(channelIndex), PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
          }
        }
# endif

# if (COMM_DEV_ERROR_REPORT == STD_ON)
        errorId = COMM_E_NO_ERROR;
# endif
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report errors if applicable. */
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_ECUM_PNCWAKEUPINDICATION, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR BusSM)
**********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_BusSM_ModeIndication()
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
/* PRQA S 3673 1 */ /* MD_ComM_3673 */
FUNC(void, COMM_CODE) ComM_BusSM_ModeIndication(NetworkHandleType Channel, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
#if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
# if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
# endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer and its value must be a valid communication mode. */
  else if (ComMode == NULL_PTR)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else if (*ComMode > COMM_FULL_COMMUNICATION)
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if ( COMM_MULTIPARTITION == STD_ON )
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel(Channel);
#endif
    if ( ComM_GetBusSmState( Channel, PARTITION_IDX ) != *ComMode )
    {
      /* #40 If BusSM indicated a new mode, store it and propagate to BswM. */
      /* ESCAN00065236 */
      ComM_SetBusSmState( Channel, *ComMode, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
#if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
# if ( COMM_MULTIPARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
      {
        /* BswM notification is synchronized to Master-Partition */
      }
      else
# endif
      {
        BswM_ComM_CurrentMode( Channel, *ComMode );
      }
#endif

      /* #50 If a passive wake-up occurred during shutdown of FR, restore the target requested mode to start communication in the next main function. */
#if (COMM_BUS_TYPE_FR_PRESENT == STD_ON)
# if (COMM_ISDEF_BUSTYPEOFCHANNEL == STD_OFF)
      if ( ComM_GetBusTypeOfChannel( Channel ) == COMM_BUS_TYPE_FR )
# endif
      {
        if ( *ComMode == COMM_NO_COMMUNICATION )
        {
          ComM_SetFrShutdown( Channel, FALSE, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

          /* ESCAN00081330: ComM_BusComModeReqFrShutdown can be COMM_FULL_COM_NETWORK_REQUESTED or COMM_FULL_COM_READY_SLEEP depending on Min Full Com Mode Duration. */
          if ( (ComM_GetBusComModeReqFrShutdown( Channel, PARTITION_IDX ) != COMM_NO_COM_NO_PENDING_REQUEST)
# if ( (COMM_MULTIPARTITION == STD_ON) && (COMM_SYNCHRONOUS_WAKE_UP == STD_ON) )
               || (ComM_GetBusComModeReqFrShutdown( Channel, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COM_NO_PENDING_REQUEST)
# endif
               ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUSTYPE_FR */
          {
# if (COMM_MULTIPARTITION == STD_ON)
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUSTYPE_FR */
#  endif
            {
              SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

              ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetBusComModeReqFrShutdown( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetBusComModeReqFrShutdownSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
            }
#  if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            else
#  endif
# endif
# if (COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON)
            {
              ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
              ComM_SetBusComModeReqFrShutdown( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
            }
# endif
          }
        }
      }
#endif

      /* #60 Store the new mode to handle User Mode Notification in the next main function. */
#if (COMM_USERMODENOTIFUNC == STD_ON)
# if (COMM_USERMODENOTIFUNC_PNC_USER_ONLY == STD_OFF)
#  if ( COMM_MULTIPARTITION == STD_ON )
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX )
#   endif
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_SetStateChange( Channel, *ComMode, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        /* Increment sync counter by 1 (relative to Master-Counter). */
        ComM_SetStateChangeSyncCounter( Channel, (uint8)(ComM_GetStateChangeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
      }
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      else
#   endif
#  endif
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
      {
        ComM_SetStateChange( Channel, *ComMode, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
#  endif
# endif
#endif

      /* #70 Notify Dcm about the new mode. */
#if (COMM_DCM_INDICATION == STD_ON)
      switch (*ComMode)
      {
      case COMM_FULL_COMMUNICATION:
        Dcm_ComM_FullComModeEntered(Channel);
        break;

      case COMM_SILENT_COMMUNICATION:
        Dcm_ComM_SilentComModeEntered(Channel);
        break;

      case COMM_NO_COMMUNICATION:
        Dcm_ComM_NoComModeEntered(Channel);
        break;

      default: /* COV_COMM_MISRA */
        break;
      }
#endif

      /* #80 If NO_COM is indicated and Reset after Forcing NO_COM is enabled, check if a reset can be performed. */
#if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
      if (*ComMode == COMM_NO_COMMUNICATION)
      {
        ComM_ChannelResetAfterForcingNoComm( COMM_PARTITION_PARA_ONLY );
      }
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_BUSSMMODEINDICATION, errorId );
  }
#else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

#if ( COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL == STD_ON )
/*********************************************************************************************************************
 *  ComM_BusSM_BusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_BusSM_BusSleepMode( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
# if ( COMM_MULTIPARTITION == STD_ON )
  ComM_PartitionConfigIdxOfChannelType partitionIdx = 0u;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that module is initialized. */
  if ( !ComM_IsPostInitialized(0u, COMM_MASTER_PARTITION_IDX) )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ((Channel >= ComM_GetSizeOfChannel())
#  if (COMM_ENABLEDOFCHANNEL == STD_ON)
   || (!ComM_IsEnabledOfChannel(Channel))
#  endif
       )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (COMM_MULTIPARTITION == STD_ON)
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
# endif
    if ( (ComM_GetActiveComMode(Channel, COMM_MASTER_PARTITION_IDX) & COMM_FULL_COMMUNICATION) != 0u )
    {
      /* #30 If current channel is in FULL_COMMUNICATION mode, store the Bus Sleep Mode indicated by the BusSM. */
      ComM_SetBusSleepModeIndicated(Channel, TRUE, PARTITION_IDX); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
# if (COMM_MULTIPARTITION == STD_ON)
      /* PRQA S 2995, 2991 1 */ /* MD_ComM_2995_2991 */
      if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
      {
        /* Increment sync counter for ComM_BusSleepModeIndicated by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusSleepModeIndicatedSyncCounter( Channel, (uint8)(ComM_GetBusSleepModeIndicatedSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      }
# endif
    }

    /* #40 Set externally requested Com Mode to COMM_NO_COM_NO_PENDING_REQUEST. */
#  if ( COMM_MULTIPARTITION == STD_ON)
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    /* PRQA S 2995, 2991 1 */ /* MD_ComM_2995_2991 */
    if ( PARTITION_IDX != COMM_MASTER_PARTITION_IDX ) /* COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE */
#   endif
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#   if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    else
#   endif
#  endif
#  if ( COMM_EXISTS_CHANNEL_ON_MASTER_PARTITION == STD_ON )
    {
      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, PARTITION_IDX ); /* SBSW_COMM_CHANNEL_CSL02 */ /* SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS */
    }
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (COMM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != COMM_E_NO_ERROR)
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, COMM_SID_CBK_BUSSMBUSSLEEPMODE, errorId );
  }
# else
  COMM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
}
#endif

/**********************************************************************************************************************
 *  Verify the configuration of the Communication Manager
 *********************************************************************************************************************/

/* Check ComM Generated Data */
#if ( COMM_MULTIPARTITION == STD_OFF )
# if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) != 1u )
#  error: "ComM configuration issue: The size of ComM_PostInitialized must be 1 if COMM_MULTIPARTITION is STD_OFF."
# endif
#endif

/* Check configuration of Multi-Partition */
#if ( COMM_MULTIPARTITION == STD_ON ) && ( COMM_WAKEUPENABLEDOFPNC == STD_ON )
# error: "ComM configuration issue: PNC Wake-Up Indication must not be enabled if COMM_MULTIPARTITION is STD_ON."
#endif

#if ( COMM_MULTIPARTITION == STD_ON ) && ( COMM_MASTER_PARTITION_IDX >= ComM_GetSizeOfPartitionIdentifiers())
# error: "ComM configuration issue: COMM_MASTER_PARTITION_IDX must be in the range of ComM_PartitionIdentifiers."
#endif

#if ( COMM_MULTIPARTITION == STD_OFF ) && ( COMM_MASTER_PARTITION_IDX != 0u )
# error: "ComM configuration issue: ComM Master Partition Index must be defined as 0 if COMM_MULTIPARTITION is STD_OFF."
#endif

#if ( COMM_MULTIPARTITION == STD_OFF ) && ( COMM_SINGLEPARTITION != 0u )
# error: "ComM configuration issue: ComM Single Partition must be defined as 0 if COMM_MULTIPARTITION is STD_OFF."
#endif


/* module specific MISRA deviations:
MD_ComM_0771:
      Reason:       Multiple break statements used to exit from for-loop immediately in case an error was detected due to wrong generated data.
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_2018:
      Reason:       Default label is provided as a defensive measure against possible future code changes and to comply with rule 15.3.
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_3218:
      Reason:       Some compilers are not able to compile code with static declarations inside functions.
      Risk:         There is no risk.
      Prevention:   N/A
MD_ComM_3415:
      Reason:       The function does not have any side-effects (it does not change the module' state').
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_3451_3449:
      Reason:       It is ensured by code generator that the declarations remain compatible.
                    1. The first declaration is in ComM_Private_Cfg.h, which is not included via ComM.h.The second declaration is in ComM_Lcfg.h, which is included via ComM.h and therefore visible to other modules.
                    2. The first declaration is in ComM.h. The second declaration is in Rte_ComM.h. Multiple external declarations of the functions exist so that other components can use that function as well.
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_3673:
      Reason:       The function parameter list is required by the AUTOSAR specification.
      Risk:         There is no risk.
      Prevention:   N/A.
MD_ComM_2842_2843:
      Reason:       A function parameter is used to access an array element. The parameter is always either passed by an internal ComM function (e.g. by ComM_MainFunction) that ensures that the parameter
                    is in a valid range or the function itself, which uses the function parameter, validates the value of the parameter during runtime. The parameter cannot be written by other BSW modules
                    or application. Additional run-time check is therefore omitted.
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_2995_2991:
      Reason:       A run-time check of generated data is performed. This is done by either checking the generated array size that shall always return true or by checking a generated, channel specific value (e.g. Nm Variant), for which
                    the statement always returns true in certain configurations (e.g. only Nm Variant LINSLAVE or PASSIVE).
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_2996_2992:
      Reason:       A run-time check of generated data is performed. This is done by checking the generated array size that shall always return false.
      Risk:         There is no risk.
      Prevention:   Covered by code review.
MD_ComM_2985:
      Reason:       The operation is needed if the right hand operand is non zero.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_2982:
      Reason:       The assignment is needed to set the default value.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_1039:
      Reason:       The array can have the length one depending on configuration. However, the array is never used as a flexible array member.
                    It is always accessed with the index zero in this case.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_0315:
      Reason:       Com_SendSignal and Com_RecieveSignal have a void pointer as a function parameter. This is defined by AUTOSAR.
      Risk:         No risk, because the underlying uint8 pointer type is known.
      Prevention:   No prevention necessary.
MD_ComM_1533:
      Reason:       The value of the object is generated, therefore cannot be moved to the static file ComM.c.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_1336:
      Reason:       In order to keep a generic function pointer prototype for all forwarding functions, the parameter name is not defined.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_1339:
      Reason:       The value of the object is never modified
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_2823:
      Reason:       It is ensured by the generator that the value of the pointer is never NULL.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_2742:
      Reason:       In precompile configurations, this expression is always constant, but can vary in PB-L or PB-S configurations.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_2983:
      Reason:       There might be configurations, in which this assignment is redundant and the value is never subsequently used.
                    Nevertheless, for improved readability this assignment is always performed.
      Risk:         No risk.
      Prevention:   None.
MD_ComM_2987:
      Reason:       This function call produces no side effects and is redundant.
                    Global data is set inside the called function by using the ComStackLib. Therefore, the function call is required.
      Risk:         No risk.
      Prevention:   None.
MD_ComM_4304:
      Reason:       In precompile configurations, this expression is always constant, but can vary in PB-L or PB-S configurations.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_3387:
      Reason:       The timer variable is only written inside the master partition (task context), i.e. no other instance is changing the timer value.
                    Therefore, side effects other than that caused by the decrement are excluded.
      Risk:         No Risk.
      Prevention:   Covered by code review.
MD_ComM_0404:
      Reason:       The sync counter on the master partition is only written from the master partition (task context), i.e. changing this value via
                    interrupt is excluded. The sync counter on the satellite partition can only be changed to (master counter + 1) and only from
                    the satellite partition. Therefore, the order of the read accesses to these two volatile objects is not significant.
      Risk:         No Risk.
      Prevention:   Covered by code review.
*/

/* SBSW_JUSTIFICATION_BEGIN

\ID                SBSW_COMM_CSL01
\DESCRIPTION       Access to an array using the GetSizeOf macro of the this array.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

\ID                SBSW_COMM_UserReqFullCom_over_ChannelPb_or_ByteMask_CSL03
\DESCRIPTION       Access to ComM_UserReqFullCom via indirection over ComM_UserByteMask or ComM_ChannelPb structure respectively.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                SBSW_COMM_UserReqPncFullCom_over_UserPncByteMask_or_PncPb_CSL03
\DESCRIPTION       Access to ComM_UserReqPncFullCom via indirection over ComM_UserPncByteMask or ComM_PncPb structure respectively.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                SBSW_COMM_BusComModeReq_over_PncChannelMapping_CSL03
\DESCRIPTION       Access to ComM_BusComModeReq via indirection over ComM_PncChannelMapping structure.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                SBSW_COMM_PNC_CSL02
\DESCRIPTION       Access to an array that has a size relation to ComM_Pnc.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                SBSW_COMM_USER_CSL02
\DESCRIPTION       Access to an array that has a size relation to ComM_User.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                SBSW_COMM_CHANNEL_CSL02
\DESCRIPTION       Access to an array that has a size relation to ComM_Channel.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                SBSW_COMM_SYNCCOUNTER_CSL02
\DESCRIPTION       Access to an SyncCounter array that has a size relation (same size) to its root array:
                   - ComM_PncSignalValues -> ComM_PncSignalValuesSyncCounter
\COUNTERMEASURE \R The size relation is not ensured by the CSL, but via runtime check. After that CSL02 applies.

\ID                SBSW_COMM_CHANNEL_CSL03
\DESCRIPTION       Access to an array with a 'ChannelIndex' via indirection over PncSignal.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                SBSW_COMM_MASTER_SYNCCOUNTER_IND
\DESCRIPTION       Access of ComM_BusPncComModeReqMasterSyncCounter via indirection over ComM_PncPartitionIndirection structure and indirection over ComM_Pnc.
                   The index which is used to access the array is always checked.
\COUNTERMEASURE \R A runtime check ensures that the array index never exceeds the size of ComM_BusPncComModeReqMasterSyncCounter.

\ID                SBSW_COMM_USER_CHECK
\DESCRIPTION       Access to an array with a 'User'-index.
\COUNTERMEASURE \R A runtime check ensures that the used 'User'-index is smaller than the size of the accessed array.

\ID                SBSW_COMM_MANAGING_CHANNEL_CHECK
\DESCRIPTION       Access to an array with a 'ManagingChannelId'-index.
\COUNTERMEASURE \R A runtime check ensures that the used 'ManagingChannelId'-index is smaller than the size of the accessed array.

\ID                SBSW_COMM_PARAMETER_PTR_WRITE
\DESCRIPTION       The function writes to the pointer parameter.
\COUNTERMEASURE \N The caller ensures that the parameters of the following functions point to a valid memory location:
                   'Status': ComM_GetStatus(), ComM_GetInhibitionStatus();
                   'State': ComM_GetState();
                   'ComMode': ComM_GetMaxComMode(), ComM_GetRequestedComMode(), ComM_GetCurrentComMode();
                   'CounterValue': ComM_ReadInhibitCounter();
                   'Versioninfo': ComM_GetVersionInfo().

\ID                SBSW_COMM_CALL_WITH_PTR_TO_CONST
\DESCRIPTION       A pointer to constant data is passed to a called function.
\COUNTERMEASURE \N The pointed data is constant and cannot be written.

\ID                SBSW_COMM_CALL_BUSSM_GET_CURRENT_COMMODE
\DESCRIPTION       A pointer is passed to a BusSM function as parameter and could be used within the function directly to write to a memory location.
\COUNTERMEASURE \N This function is called by ComM_GetCurrentComMode() only, which provides a pointer to a local variable.
                   This pointer is passed to the callee (BusSM) without changes. This ensures that the callee always gets a valid pointer.

\ID                SBSW_COMM_PASSED_PTR_WRITE_ACCESS
\DESCRIPTION       A pointer is passed to ComM_InternalGetCurrentBusSmMode() and is used within the function directly to write to a memory location.
\COUNTERMEASURE \N This function is called by ComM_GetCurrentComMode() only, which provides a pointer to a local variable.

\ID                SBSW_COMM_CALL_WITH_PTR_TO_LOCAL_VARIABLE
\DESCRIPTION       A pointer is passed to a function as parameter and is used within the function directly to write to a memory location.
\COUNTERMEASURE \N The passed pointer references a local stack variable and is therefore always valid.

\ID                SBSW_COMM_CALL_WITH_PTR_TO_GLOBAL_VARIABLE
\DESCRIPTION       A pointer to a global variable is passed to a function as parameter and is used within the function directly to write to a memory location.
                   The pointed variables are always ComM specific sync counters and states/timers. Within the called function, the sync counters
                   are compared and - if needed - the sync counter and state/timer is written accordingly.
\COUNTERMEASURE \N The pointed data is initialized by ComM.
                   The validity of accessing the correct array element, which is passed as pointer, is ensured by CSL02.

\ID                SBSW_COMM_INTERNAL_BUSSM_CALL_WITH_PTR_PARAM
\DESCRIPTION       A pointer is received via function parameter and is passed to another function (ComM_BusSM_ModeIndication) as parameter.
                   The forwarding function does not access the pointer.
\COUNTERMEASURE \N The validity of the passed pointer is checked in the called function (ComM_BusSM_ModeIndication()).

\ID                SBSW_COMM_ARRAY_CHANNEL_REQUESTORS_WRITE
\DESCRIPTION       The function writes to the array handleArray[]. The index is limited by a generated define containing the array size.
                   The type of the array is defined by RTE because it is used to exchange data via Sender-Receiver Interface ComM_CurrentChannelRequest.
\COUNTERMEASURE \S Verify that the array size generated by ComM matches to the array size in the type definition of RTE. The following procedure shall be
                   applied to each channel that has activated the ComM parameter 'Full Comm Request Notification Enabled'.
                   1) ComM_Cfg.h contains array size definition in the format COMM_MAX_CR_<ShortNameOfChannel>
                   2) Rte_Type.h contains the definition of the corresponding structure type in the format ComM_UserHandleArrayType_<ShortNameOfChannel>
                   3) Verify that the structure member 'handleArray' has the same size as the corresponding define value of ComM in 1).
                   4) Verify the content of the generated functions ComM_CurrentChannelRequestInit and ComM_CurrentChannelRequestNotification to ensure that
                      the proper define COMM_MAX_CR_<ShortNameOfChannel> is used to limit the array index when writing to
                      ComM_UserHandleArrayType_<ShortNameOfChannel>.handleArray[]. [SMI-94]

\ID                SBSW_COMM_CALL_COM_RECEIVE_SIGNAL
\DESCRIPTION       This function passes a pointer to Com_ReceiveSignal(), which uses the pointer to write to a memory location.
\COUNTERMEASURE \S Verify that the value of ComSignalLength (byte) in Com module is smaller or equal to the value of COMM_PNC_SIGNAL_LENGTH (can be found in ComM_Cfg.h).
                   This shall be verified for each ComPncSignal referenced by Partial Network Clusters and having ComMPncComSignalDirection = RX. [SMI-95]

\ID                SBSW_COMM_TransitionFct_PointerCall
\DESCRIPTION       Calling a function pointer defined in ComM_TransitionFctTable[].
\COUNTERMEASURE \R A runtime check ensures that array index never exceeds the size of ComM_TransitionFctTable[]. It is ensured by code inspection that the elements
                   of ComM_TransitionFctTable[] are valid state transition functions and the size of the array is COMM_MAX_NUMBER_OF_STATES.

\ID                SBSW_COMM_UserModeNotiFunc_PointerCall
\DESCRIPTION       Calling a function pointer defined in ComM_UserModeNotiFunc[].
\COUNTERMEASURE \S The user of MICROSAR Safe shall verify that each element of ComM_UserModeNotiFunc[] is either a valid function pointer or NULL_PTR.
                   This measure is only needed if at least one ComM user has enabled the parameter 'User Mode Notification'. [SMI-1046]
                \R A runtime check prevents dereferencing a NULL pointer.
                \R Qualified use-case CSL01 of ComStackLib because ComM_GetSizeOfUserModeNotiFunc() is used to check the array index.

\ID                SBSW_COMM_BusSMMode_PointerCall
\DESCRIPTION       Calling a function pointer defined in ComM_Channel struct.
\COUNTERMEASURE \R A runtime check ensures that array index never exceeds the size of ComM_Channel.

\ID                SBSW_COMM_CSL_VAR_ACCESS
\DESCRIPTION       A ComStackLib generated variable is accessed.
\COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

\ID                SBSW_COMM_CSL02_PARTITION_INDEX_ACCESS
\DESCRIPTION       In Multi-Partition configurations, the following arrays are accessed with a 'PartitionIndex' (via indirection over
                   PartitionIdentifiers structure) with same index using ComStackLib:
                   - Init-Arrays:     InitStatus.
                   - NvM-Arrays:      NvMStoreReq.
                   - Pnc-Arrays:      PncSignalValues, UserReqPncFullCom, PncPSleepTimer, ActivePncComMode, BusPncComModeReq,
                                      BusPncComModeReqSyncCounter, PncToChannelRoutingState, PncLimitation.
                   - User-Arrays:     FullComRequesters, LastStateChange.
                   - Channel-Arrays:  BusComModeReq, BusSleepModeIndicated, BusComModeReqFrShutdown, FrShutdown, ActiveComMode, BusSmState, MinFullComModeTimer,
                                      NmLightTimer, ComAllowed, ResetRequired, DcmRequestActive, StateChange, UserReqFullCom, Inhibition
                   - Sync-Arrays:     BusComModeReqSyncCounter, BusPncComModeReqSyncCounter, BusPncComModeReqMasterSyncCounter, BusSleepModeIndicatedSyncCounter, BusComModeReqFrShutdownSyncCounter,
                                      ActiveComModeSyncCounter, BusSmStateSyncCounter, MinFullComModeTimerSyncCounter, NmLightTimerSyncCounter,
                                      InitiateResetSyncCounter, DcmRequestActiveSyncCounter, StateChangeSyncCounter, PncSignalValuesSyncCounter, PncToChannelRoutingStateSyncCounter,
                                      ForwardNmNetworkReleaseSyncCounter, ForwardNmNetworkRequestSyncCounter, ForwardComSendSignalSyncCounter, ForwardComSendSignalIndex,
                                      ForwardNmPassiveStartUpSyncCounter, ForwardRequestBusSmModeSyncCounter, ForwardingBusSmMode, ForwardUserReqSyncCounter,
                                      ForwardingUserReqMode, ForwardSynchronizedPncShutdownIndicatedSyncCounter.
                   The 'PartitionIdx' variable, which is used to access the arrays, is always checked.
                   In Single-Partition configurations, the access with 'PartitionIndex' of these arrays is always done for index 0.
\COUNTERMEASURE \N For Multi-Partition: Qualified use-case CSL02 of ComStackLib
                   For Single-Partition: The macro COMM_MASTER_PARTITION_IDX, which is defined
                   to 0 and checked via an error-directive, is used to access the array.

\ID                SBSW_COMM_CSL03_PARTITION_INDEX_ACCESS
\DESCRIPTION       In Multi-Partition configurations, the following arrays are accessed with 'PartitionIndex' via indirection over
                   PartitionIdentifiers structure:
                   - Init-Arrays:     InitStatus.
                   - NvM-Arrays:      NvMStoreReq.
                   - Pnc-Arrays:      PncSignalValues, UserReqPncFullCom, PncPSleepTimer, ActivePncComMode, BusPncComModeReq,
                                      BusPncComModeReqSyncCounter, PncToChannelRoutingState, PncLimitation.
                   - User-Arrays:     FullComRequesters, LastStateChange.
                   - Channel-Arrays:  BusComModeReq, BusSleepModeIndicated, BusComModeReqFrShutdown, FrShutdown, ActiveComMode, BusSmState, MinFullComModeTimer,
                                      NmLightTimer, ComAllowed, ResetRequired, DcmRequestActive, StateChange, UserReqFullCom, Inhibition
                   - Sync-Arrays:     BusComModeReqSyncCounter, BusPncComModeReqSyncCounter, BusPncComModeReqMasterSyncCounter, BusSleepModeIndicatedSyncCounter, BusComModeReqFrShutdownSyncCounter,
                                      ActiveComModeSyncCounter, BusSmStateSyncCounter, MinFullComModeTimerSyncCounter, NmLightTimerSyncCounter,
                                      InitiateResetSyncCounter, DcmRequestActiveSyncCounter, StateChangeSyncCounter, PncSignalValuesSyncCounter, PncToChannelRoutingStateSyncCounter,
                                      ForwardNmNetworkReleaseSyncCounter, ForwardNmNetworkRequestSyncCounter, ForwardComSendSignalSyncCounter, ForwardComSendSignalIndex,
                                      ForwardNmPassiveStartUpSyncCounter, ForwardRequestBusSmModeSyncCounter, ForwardingBusSmMode, ForwardUserReqSyncCounter,
                                      ForwardingUserReqMode, ForwardSynchronizedPncShutdownIndicated.
                   In Single-Partition configurations, the access with 'PartitionIndex' of these arrays is always done with index 0.
\COUNTERMEASURE \N For Multi-Partition: Qualified use-case CSL03 of ComStackLib
                   For Single-Partition: The macro COMM_MASTER_PARTITION_IDX, which is defined
                   to 0 and checked via an error-directive, is used to access the array.

\ID                SBSW_COMM_ZERO_INDEX_ACCESS
\DESCRIPTION       Access of PostInitialized, EcuMWakeUpIndicated and NmStartUpIndicated is only and always performed for 'Index' 0.
                   In case of PostInitialized, the access is additionally done always and only for 'PartitionIndex' COMM_MASTER_PARTITION_IDX.
\COUNTERMEASURE \R A runtime check ensures that the size of the PostInitialized array on the Master-Partition is 1.
                   In Single-Partition configurations, a preprocessor error directive checks that the size of ComM_PostInitialized is 1.

\ID                SBSW_COMM_CHECKSYNCCOUNTER_PTR_CALL
\DESCRIPTION       Call to ComM_CheckSyncCounterAndForwardFunction with pointers to local stack variables (synchronization counters) to be checked.
                   The values of the synchronization counters are compared and - if needed - incremented afterwards.
\COUNTERMEASURE \N [CM_COMM_GENERAL_FUNCTIONCALL]

\ID                SBSW_COMM_FORWARDING_FCT_PTR_CALL
\DESCRIPTION       A forwarding function is called via function pointer. The pointed function is declared and defined
                   in the static source code. The function pointer and its corresponding parameters are received within
                   the parameter list of the local function, which executes the function pointer call.
                   The function pointer and its corresponding parameter can only be obtained from local, internal functions.
                   The function pointer is not used in address calculations for memory manipulating operations.
                   The pointed forwarding functions are:
                   - Nm_NetworkRequest
                   - Nm_NetworkRelease
                   - Nm_PassiveStartUp
\COUNTERMEASURE \R Covered by code inspection. It is ensured that the pointed function is declared in the static source code.

\ID                SBSW_COMM_FORWARDING_FCT_PTR_WRITE
\DESCRIPTION       The local function writes to the object referenced by parameter pointer to a global variable.
                   The validity of the pointer is verified by the caller. The pointed variables are initialized by ComM.
\COUNTERMEASURE \N [CM_COMM_GENERAL_FUNCTIONCALL]

SBSW_JUSTIFICATION_END */

/*
\CM CM_COMM_GENERAL_FUNCTIONCALL The parameters are not used in address calculations for memory manipulating
                                 operations. Such calculations include but are not limited to: Array indexing,
                                 pointer offset calculation and related forms of pointer arithmetic. Furthermore
                                 these parameters are not forwarded to other software modules which could use
                                 them for such operations unless they implement appropriate countermeasures.
                                 And the addresses in the pointer variables are not saved in global variables
                                 so that they could be used outside the function call. In that case, they are no
                                 longer valid.
*/

/* FETA_JUSTIFICATION_BEGIN
\ID                FETA_COMM_CSL_USE_CASE
\DESCRIPTION       Iteration from a defined lower limit to a defined upper limit. The upper limit is provided as a constant by the CSL.
                   The qualified CSL APIs return the positive constant values from CSL tables. The positive values in the table depends on the configuration.
                   The stop index is a defined upper border of the loop. The CSL guarantees that the type of the loop variable is equal to the type of the upper limit.
                   Besides the stepwise loop increments, the loop variable is not modified elsewhere.
\COUNTERMEASURE \N Qualified use-cases of ComStackLib

\ID                FETA_COMM_CHANNEL_STATE_TRANSITION
\DESCRIPTION       Repetition of the channel state machine until either the target state is reached or an exit criteria is met (e.g. target state cannot be reached).
                   The iteration is performed in a do-while-loop, since there are transitions where multiple state transitions need to be executed until the target state is reached.
                   The exit criteria of this iteration is defined by the actual state and the target state. Both, the actual and the target state, cannot be changed by another task
                   while the channel state machine is executed. Thus it is ensured that the state of these variables is consistent throught the execution of the channel state machine.
\COUNTERMEASURE \T Test case TCASE-ComM-ChannelStateMachine_AllTrans verifies the finite execution time of all possible combinations of actual state and target state.

\ID                FETA_COMM_PNC_STATE_TRANSITION
\DESCRIPTION       Repetition of the PNC state machine until either the target state is reached or an exit criteria is met (e.g. target state cannot be reached).
                   The iteration is performed in a do-while-loop, since there are transitions where multiple state transitions need to be executed until the target state is reached.
                   The exit criteria of this iteration is defined by the actual state and the target state. Both, the actual and the target state, cannot be changed by another task
                   while the PNC state machine is executed. Thus it is ensured that the state of these variables is consistent throught the execution of the PNC state machine.
\COUNTERMEASURE \T Test case TCASE-487673 verifies the finite execution time of all possible combinations of actual state and target state.

 FETA_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_COMM_MISRA
  \ACCEPT XX
  \REASON [MSR_COV_MISRA]

\ID COV_COMM_GENDATA_CHECK
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON The condition is for plausibility checks of the generated data. The code is verified by code inspection.

\ID COV_COMM_MAGICNUMBER_CHECK
  \ACCEPT XF
  \REASON The condition is for magic number check of the post-build data. The code is verified by code inspection.

\ID COV_COMM_FOR_LOOP_OPTIMIZATION
  \ACCEPT TX
  \ACCEPT TF tf tx
  \REASON A run-time optimization to cut short a for loop using a break statement. The loop goes through an element list where exactly one element meets certain
          conditions inside the loop. Once the element is found, the loop can be terminated to save run-time.

\ID COV_COMM_SUBSET_OF_ELEMENTS
  \ACCEPT TX
  \ACCEPT XF
  \REASON There is an iteration loop over elements such as channels or PNCs or signals and an action (e.g. an API call) has to be performed if an element has a
          certain configuration parameter set to true. This parameter is used to select a subset of elements at run-time and it may be true for all relevant elements
          in some configurations so that the run-time decision always returns true. An encapsulation of such run-time decisions would substantially degrade
          readability of the code. The run-time decisions do not have an else branch.

\ID COV_COMM_NM_LINSLAVE_ONLY
  \REASON In some configurations it is possible to have only channels with Nm variant LINSLAVE. In this case, this runtime check will always return false for the check
          if a DCM request is active (ComM_IsDcmRequestActive()).

\ID COV_COMM_NM_LINSLAVE_OR_PASSIVE_ONLY
  \REASON In some configurations it is possible to have only channels with Nm variant LINSLAVE or PASSIVE. In this case, this runtime check will always return true.

  \ID COV_COMM_ONLY_ONE_CHANNEL_ASSIGNED_TO_ALL_USERS
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have only one channel which is assigned to all users.

\ID COV_COMM_TIMER_INIT_VALUE_CAN_BE_ONE
  \ACCEPT TX
  \ACCEPT TF tf tx tf
  \REASON A timer value can be configured to exactly one main function period of the corresponding channel. In this case after decrementing the timer its value is always 0.

\ID COV_COMM_COMMON_USER_WITH_MODE_NOTIFICATION
  \ACCEPT TX
  \REASON In some configurations a user can be mapped to all channels. If such a user has mode notification enabled,
          every state change of a channel triggers a re-calculation of the lowest mode of this user.

\ID COV_COMM_PNC_EXTENDED_NO_ERA_ON_ACTIVE_CHANNEL
  \ACCEPT XF xf xx
  \REASON In configurations with COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON it is possible to assign Gateway Type ACTIVE to so called additional channels (e.g. LIN)
          in order to keep those channels awake via PNC. But ERA signals cannot be received on those channels.

\ID COV_COMM_PNC_EXTENDED_SINGLE_ERA
  \ACCEPT TX
  \REASON In configurations with COMM_PNC_EXTENDED_FUNCTIONALITY_ENABLED == STD_ON it is possible to have only one ERA signal. When this ERA signal is received
          and PNC bit is 0 the corresponding PNC system user shall be released. Note that having multiple ERA signals is the most common use case.

\ID COV_COMM_PNC_ON_CHANNELS_NM_PASSIVE
  \ACCEPT TF tf tx
  \ACCEPT TX
  \REASON In configurations where PNCs are mapped only to channels with Nm Type PASSIVE all user requests to those PNCs are ignored.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL_AND_MIN_FULL_COM_MODE_DURATION
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with full Nm support (Nm variant FULL) and
          a minimum full com mode duration configured.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LIGHT
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with no Nm support (Nm variant LIGHT).

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_FULL
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with full Nm support (Nm variant FULL).

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NMTYPE_LINSLAVE
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with Lin Slave support (Nm variant LINSLAVE).

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_NM_SUPPORT
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with Nm support (Nm variant FULL or PASSIVE).

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUS_TYPE_FR
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with bus type FR.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_SILENT_SUPPORT
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with Nm support (Nm variant FULL or PASSIVE) and Silent Support.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_ERA_OR_EIRA_SIGNALS
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with ERA or EIRA signals.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_PNC_SUPPORT
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with PNC Support enabled.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_PNCNMREQUEST
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with PncNmRequest enabled.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_BUSTYPE_FR
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF xf tf
  \ACCEPT TF tf xf
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with BusType FR.

\ID COV_COMM_MP_NO_OR_ALL_MANAGED_CHANNELS_ON_MASTER
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all managed channels assigned to the master partition.

\ID COV_COMM_MP_ONLY_ONE_SATELLITE_CHANNEL
  \ACCEPT TX
  \REASON In some configurations it is possible to have only one channel assigned to a satellite partition. In such configurations it is possible (dependent
          on the enabled features) that for example the ComM_BusComModeReqSyncCounter is always increased on the satellite partition together with the other
          sync counters available (e.g. the ComM_ActiveComModeSynCounter). This means, it is not possible to increase any other sync counter (e.g. the ComM_ActiveComModeSynCounter)
          without increasing the sync counter ComM_BusComModeReqSyncCounter. Thus, when the global flag ComM_GlobalPartitionSyncCounter is increased, the sync counter
          for ComM_BusComModeReqSyncCounter is always increased. Therefore the check for this sync counter will always evaluate to true in such configurations.

\ID COV_COMM_MP_ONLY_ONE_SATELLITE_WITH_ERA_OR_EIRA_RX_SIGNAL
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON In some configurations it is possible to have only one satellite partition with an ERA Rx or EIRA Rx signal assigned.

\ID COV_COMM_MP_ONLY_ONE_SATELLITE_WITH_EIRA_TX_SIGNAL
  \ACCEPT XF
  \REASON In some configurations it is possible to have only one satellite partition with an EIRA Tx signal assigned.

\ID COV_COMM_MP_ALL_PB_VARIANT_CHANNELS_ON_MASTER
  \ACCEPT TX
  \REASON In some configurations it is possible to have all post-build variant de-activatable channels assigned to the master partition.

\ID COV_COMM_EXISTS_ONLY_SINGLE_PNC_USER
  \ACCEPT XF
  \REASON In some configurations it is possible to have only users mapped to exactly one PNC.

\ID COV_COMM_MP_NM_LINSLAVE_ONLY_AND_SYNC_WUP_ENABLED
  \REASON In some configurations it is possible to have only channels with Nm variant LINSLAVE and Synchronous WakeUp is enabled.
          In this case, this runtime check will always return false for the check if the BusComModeReqSyncCounter has increased on a satellite partition.

\ID COV_COMM_MP_NO_OR_ALL_CHANNELS_ON_MASTER_WITH_GWTYPE_PASSIVE
  \ACCEPT TX
  \ACCEPT XF
  \REASON In some configurations it is possible to have either no channel or all channels assigned to the master partition with GW type PASSIVE.

\ID COV_COMM_MASTER_SYNCCOUNTER_IND
  \ACCEPT TX
  \ACCEPT XX
  \REASON This statement checks if the static code is correctly generated so that all accesses to ComM_BusPncComModeReqMasterSyncCounter are within its bounds.

\ID COV_COMM_MP_NO_OR_ALL_USERS_ON_MASTER
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON In some configurations it is possible to have either no user or all users assigned to the master partition.

  \ID COV_COMM_MP_NO_OR_ALL_PNC_USERS_ON_SATELLITE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON In some configurations it is possible that all the PNC users are assigned to a satellite partition.

END_COVERAGE_JUSTIFICATION */

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
