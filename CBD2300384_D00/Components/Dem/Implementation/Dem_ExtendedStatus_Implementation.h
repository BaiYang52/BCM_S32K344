/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_ExtendedStatus
 *  \{
 *  \file       Dem_ExtendedStatus_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EXTENDEDSTATUS_IMPLEMENTATION_H)
#define DEM_EXTENDEDSTATUS_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_ExtendedStatus_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_EventInternalStatus_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_EventIF_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_EXTENDEDSTATUS_IMPLEMENTATION_FILENAME "Dem_ExtendedStatus_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ExtendedStatus_PublicProperties
 * \{
 */


 /* Extended Status bit 'TestPassedSinceLastClear' ----------------------------------------------- */
 /* ****************************************************************************
 % Dem_Event_ExtendedStatus_GetTPSLC()
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_ExtendedStatus_GetTPSLC(
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
)
{
  return  (((Status)& DEM_EXT_STATUS_TPSLC) != 0u);
}


/* ****************************************************************************
 % Dem_Event_ExtendedStatus_SetTPSLC()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_ExtendedStatusType, DEM_CODE)
Dem_Event_ExtendedStatus_SetTPSLC(
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
)
{
  return  ((Status) | DEM_EXT_STATUS_TPSLC);
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_Private Private Methods
 * \{
 */


 /* ****************************************************************************
 * Dem_DTCInternalStatus_Reset_ExtendedStatus_TPSLC
 *****************************************************************************/
 /*!
 * \brief         Reset test passed since last clear state of extended status of event
 *
 * \details       -
 *
 * \param[in]     Status
 *                Current extended status of the event
 *
 * \return        Event extended status with flag cleared for TPSLC.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_ExtendedStatusType, DEM_CODE)
Dem_DTCInternalStatus_Reset_ExtendedStatus_TPSLC(
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

 /* ****************************************************************************
 % Dem_DTCInternalStatus_Reset_ExtendedStatus_TPSLC()
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_ExtendedStatusType, DEM_CODE)
Dem_DTCInternalStatus_Reset_ExtendedStatus_TPSLC(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_Event_ExtendedStatusType, AUTOMATIC)  Status
)
{
  return  ((Dem_Event_ExtendedStatusType)((Status)& DEM_EXT_STATUS_TPSLC_MASK));
}



/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ExtendedStatus_Public
 * \{
 */


 /* ****************************************************************************
 % Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC)  DTCContext
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    if (Dem_Core_Event_GetEventReadinessGroup(DTCContext->EventId) != DEM_CFG_READINESS_NONE)
    {
    /* If event combination is supported and event is in a combined group */
  # if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
      if (DTCContext->GroupId != DEM_CFG_COMBINED_GROUP_INVALID)
      {
        /* If all combined events are tested this cycle and no one is currently test failed */
        if ((Dem_UDSStatus_Test_TF(DTCContext->NewDtcStatus) == FALSE)
          && (Dem_UDSStatus_Test_TNCTOC(DTCContext->NewDtcStatus) == FALSE)
          )
        {
          Dem_DTC_ExtendedStatusType lExtendedStatus;
          lExtendedStatus = Dem_Event_GetExtendedStatus(DTCContext->EventId);
          /* If TestPassedSinceLastClear is not set already */
          if (Dem_Event_ExtendedStatus_GetTPSLC(lExtendedStatus) == FALSE)
          {
            Dem_Cfg_CombinedGroupIterType lCombinedIter;

            /* Set all events of combined group to TestPassedSinceLastClear */
            for (Dem_Cfg_CombinedGroupIterInit(DTCContext->GroupId, &lCombinedIter);                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
              Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
              Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
            {
              Dem_EventIdType lSubEventId;
              lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
                                                                                                                                   /* If event is available */
              if (Dem_Event_TestEventUnavailable(lSubEventId) == FALSE)
              {
                /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
                Dem_EnterCritical_DiagMonitor();
                lExtendedStatus = Dem_Event_GetExtendedStatus(lSubEventId);
                lExtendedStatus = Dem_Event_ExtendedStatus_SetTPSLC(lExtendedStatus);
                Dem_Event_SetExtendedStatus(lSubEventId, lExtendedStatus);

                Dem_LeaveCritical_DiagMonitor();
                /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
              }
            }

            Dem_Readiness_Completion_TriggerCompletionRecalculation(Dem_Core_Event_GetEventReadinessGroup(DTCContext->EventId));
          }
        }
      }
      else
    # endif
      {
        /* Otherwise*/
        Dem_DTC_ExtendedStatusType lExtendedStatus;
        lExtendedStatus = Dem_Event_GetExtendedStatus(DTCContext->EventId);
        if (Dem_Event_ExtendedStatus_GetTPSLC(lExtendedStatus) == FALSE)
        {
          /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
          Dem_EnterCritical_DiagMonitor();

          /* Set extended status of event to TestPassedSinceLastClear */
          lExtendedStatus = Dem_Event_GetExtendedStatus(DTCContext->EventId);
          lExtendedStatus = Dem_Event_ExtendedStatus_SetTPSLC(lExtendedStatus);
          Dem_Event_SetExtendedStatus(DTCContext->EventId, lExtendedStatus);

          Dem_LeaveCritical_DiagMonitor();
          /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

          Dem_Readiness_Completion_TriggerCompletionRecalculation(Dem_Core_Event_GetEventReadinessGroup(DTCContext->EventId));
        }
      }
    }
  }

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCContext)                                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EXTENDEDSTATUS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtendedStatus_Implementation.h
 *********************************************************************************************************************/
