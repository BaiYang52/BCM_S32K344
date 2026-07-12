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
/*! \addtogroup Dem_MemObdFreezeFrame
 *  \{
 *  \file       Dem_MemObdFreezeFrame_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the Obd Freeze Frame Memory subcomponent which manages storage and erasing of the 
 *              legislative Obd Freeze Frame
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMOBDFREEZEFRAME_IMPLEMENTATION_H)
#define DEM_MEMOBDFREEZEFRAME_IMPLEMENTATION_H

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_MemObdFreezeFrame_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_MemoryIF_Interface.h"
#include "Dem_MemState_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_DataStorage_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMOBDFREEZEFRAME_IMPLEMENTATION_FILENAME "Dem_MemObdFreezeFrame_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemObdFreezeFrame_PrivateProperties Private Properties
 * \{
 */
#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameGetVisibility
 *****************************************************************************/
/*!
 * \brief         Get the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \details       Get the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \return        The visibility flag of the freeze frame
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *                && DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameGetVisibility(
  CONST(uint8, AUTOMATIC)  Index
  );
#endif

/* ****************************************************************************
 * Dem_MemObdFreezeFrame_IsPendingStorageFreezeFrameSupported
 *****************************************************************************/
/*!
 * \brief        Tests if storage of OBD Freeze Frame at pending is supported.
 *
 * \details      Tests if storage of OBD Freeze Frame at pending is supported.
 *
 *
 * \return       TRUE
 *               storage of OBD Freeze Frame at pending is supported.
 * \return       FALSE
 *               storage of OBD Freeze Frame at pending is not supported
 * \pre          -
 * \config       -
 *
 * \context      TASK
 * \synchronous  TRUE
 * \reentrant    TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_IsPendingStorageFreezeFrameSupported(
  void
  );

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameSetVisibility
 *****************************************************************************/
/*!
 * \brief         Set the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \details       Set the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \param[in]     Visible
 *                TRUE: Set the visibility flag of the freeze frame entry
 *                FALSE: Reset the visibility flag of the freeze frame entry
 *
 * \return        The stored EventId of the freeze frame
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameSetVisibility(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(boolean, AUTOMATIC)  Visible
  );  
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameGetVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameGetVisibility(
  CONST(uint8, AUTOMATIC)  Index
  )
{
  return Dem_Cfg_FreezeFrameData[Index].IsVisible;
}
#endif

/* ****************************************************************************
 % Dem_MemObdFreezeFrame_IsPendingStorageFreezeFrameSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_IsPendingStorageFreezeFrameSupported(
  void
  )
{
  return DEM_CFG_SUPPORT_PENDING_STORAGE_FREEZE_FRAME == STD_ON;
}


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) 
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameSetVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameSetVisibility(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(boolean, AUTOMATIC)  Visible
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMOBDFREEZEFRAME_IMPLEMENTATION_FILENAME, __LINE__);                                       /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_FreezeFrameData[Index].IsVisible = Visible;                                                                          /* SBSW_DEM_ARRAY_WRITE_FREEZEFRAMEDATA */
  }
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemObdFreezeFrame_Private Private Methods
 * \{
 */


#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_SelectNewestLowPrioFF
 *****************************************************************************/
/*!
 * \brief         Selects the index of the newest Obd freeze frame with low prio 
 *                readiness group
 *
 * \details       If there are multiple low prio Obd freeze frames stored 
 *                select the one with most recent timestamp
 *
 * \param[in]     SelectVisible
 *                TRUE:  Select newest low prio visible freeze frame
 *                FALSE: Select newest low prio invisible freeze frame
 *
 * \return        Freeze Frame Index of the event if found, otherwise
 *                Dem_Cfg_ObdFFSlotCountInVariant()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectNewestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  );
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_SelectOldestHighPrioFF
 *****************************************************************************/
/*!
 * \brief         Selects the index of the oldest Obd freeze frame with high
 *                prio readiness group
 *
 * \details       If there are multiple high prio Obd freeze frames stored
 *                select the one with oldest timestamp
 *
 * \param[in]     SelectVisible
 *                TRUE:  Select oldest high prio visible freeze frame
 *                FALSE: Select oldest high prio invisible freeze frame
 *
 * \return        Freeze Frame Index of the event if found, otherwise
 *                Dem_Cfg_ObdFFSlotCountInVariant()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectOldestHighPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FindDisplaceableInvisibleFreezeFrameForVisibleEvent
 *****************************************************************************/
/*!
 * \brief         Selects a displaceable Obd Freeze Frame entry for the freeze
 *                frame of an immediately visible event.
 *                Determines, if freeze frame visibility must be updated.
 *
 * \details       This function checks, if the freeze frame of a visible event
 *                is allowed to displace an invisible freeze frame related to 
 *                a non-misfire and non-fuel system event.
 *                It also determines, if an invisible freeze frame related to
 *                a misfire or fuel system event must be made visible
 *                immediately.
 *
 * \param[out]    SetHighPrioFFVisibleBeforeDcyQualified
 *                TRUE: invisible freeze frame related to a misfire or fuel
 *                system event must be set visible before DCY qualification
 *                FALSE: no visibility change required
 *
 * \return        Index of the displaceable entry. If no index is found, the
 *                number of configured freeze frames will be returned.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  );
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_GetEventOfObdFF
 *****************************************************************************/
/*!
 * \brief         Get the stored EventId of the freeze frame at position
 *                'Index'.
 *
 * \details       If the given slot is occupied, returns the event Id
 *                otherwise returns the invalid event Id
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \return        The stored EventId of the freeze frame
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemObdFreezeFrame_GetEventOfObdFF(
  CONST(uint8, AUTOMATIC)  Index
  );
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameTestVisible
 *****************************************************************************/
/*!
 * \brief         Tests whether a given freeze frame slot contains a visible
 *                freeze frame
 *
 * \details       Tests whether a given freeze frame slot contains a visible
 *                freeze frame
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \return        TRUE: Freeze frame stored at index is visible
 *                FALSE: Freeze frame stored at index is not visible
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameTestVisible(
  CONST(uint8, AUTOMATIC) Index
  );
#endif

/* ****************************************************************************
 * Dem_MemObdFreezeFrame_IsFreezeFrameStorageValid
 *****************************************************************************/
/*!
 * \brief         Tests if freeze frame storage is valid for a certain event.
 *
 * \details       Tests if freeze frame storage is valid for a certain event,
 *                depending on obd freeze frame storage configuration.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE:  Freeze frame storage for event is valid
 *                FALSE: Freeze frame storage for event is invalid
 *
 * \pre           Only invoked if OBDII or OBDOnUDS is enabled in variant
 * \config        -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_IsFreezeFrameStorageValid(
  Dem_EventIdType  EventId
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_SelectNewestLowPrioFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectNewestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  )
{
  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lHighestTimestamp;

  lBestCandidate = Dem_Cfg_ObdFFSlotCountInVariant();
  lFreezeFrameIndex = Dem_Cfg_ObdFFSlotCountInVariant();
  lHighestTimestamp = 0u;


  while (lFreezeFrameIndex != 0U)
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;
    lEventId = Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex);

    if (Dem_MemObdFreezeFrame_FreezeFrameTestVisible(lFreezeFrameIndex) == SelectVisible)
    {
      if ((Dem_Core_Event_TestHighPriorReadinessGroup(lEventId) == FALSE)
        && (Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex) > lHighestTimestamp))                                             
      {
        lBestCandidate = lFreezeFrameIndex;
        lHighestTimestamp = Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex);
      }
      else
      {
        /* no candidate found, lBestCandidate is still Dem_Cfg_ObdFFSlotCountInVariant()*/
      }
    }
  }
  return lBestCandidate;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_MemObdFreezeFrame_SelectOldestHighPrioFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectOldestHighPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  )
{
  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lLowestTimestamp;

  lBestCandidate = Dem_Cfg_ObdFFSlotCountInVariant();
  lFreezeFrameIndex = Dem_Cfg_ObdFFSlotCountInVariant();
  lLowestTimestamp = 0xFFFFFFFFU;


  while (lFreezeFrameIndex != 0U)
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;
    lEventId = Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex);

    if (Dem_MemObdFreezeFrame_FreezeFrameTestVisible(lFreezeFrameIndex) == SelectVisible)
    {
      if ((Dem_Core_Event_TestHighPriorReadinessGroup(lEventId) == TRUE)
        && (Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex) < lLowestTimestamp))                                             
      {
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex);
      }
      else
      {
        /* no candidate found, lBestCandidate is still Dem_Cfg_ObdFFSlotCountInVariant()*/
      }
    }
  }
  return lBestCandidate;
}
#endif
#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_MemObdFreezeFrame_FindDisplaceableInvisibleFreezeFrameForVisibleEvent
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
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  )
{
  uint8 lSelectedIndex;
  lSelectedIndex = Dem_Cfg_ObdFFSlotCountInVariant();

  if (Dem_Mem_IsFreezeFrameVisibleInMode02() == FALSE)
  {
    lSelectedIndex = Dem_MemObdFreezeFrame_SelectNewestLowPrioFF(FALSE);
    if (lSelectedIndex == Dem_Cfg_ObdFFSlotCountInVariant())
    {
      *SetHighPrioFFVisibleBeforeDcyQualified = TRUE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  return lSelectedIndex;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_GetEventOfObdFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemObdFreezeFrame_GetEventOfObdFF(
CONST(uint8, AUTOMATIC)  Index
)
{
  Dem_EventIdType lEventId;
  if (Index < Dem_Cfg_ObdFFSlotCountInVariant())
  {
    lEventId = Dem_Cfg_FreezeFrameData[Index].EventId;                                                                           /* PRQA S 2841, 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */ 
  }
  else
  {
    lEventId = DEM_EVENT_INVALID;
  }
  return lEventId;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameTestVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameTestVisible(
  CONST(uint8, AUTOMATIC) Index
  )
{
  boolean IsFFVisible;
# if (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
  IsFFVisible = Dem_MemObdFreezeFrame_FreezeFrameGetVisibility(Index);
# else
  IsFFVisible = TRUE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index);                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
  return IsFFVisible;
}
#endif

/* ****************************************************************************
 % Dem_MemObdFreezeFrame_IsFreezeFrameStorageValid
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemObdFreezeFrame_IsFreezeFrameStorageValid(
  Dem_EventIdType EventId
  )
{
  Dem_DTC_UDSStatusType lDTCStatus;
  boolean lIsFreezeFrameStorageValid = FALSE;
  lDTCStatus = Dem_DTC_RetrieveStatus(EventId);

  if ((Dem_UDSStatus_Test_CDTC(lDTCStatus) == TRUE))
  {
    /* If the CDTC Bit is set the freeze frame storage is always valid */
    lIsFreezeFrameStorageValid = TRUE;
  }
  else
  {
    if ( (Dem_MemObdFreezeFrame_IsPendingStorageFreezeFrameSupported() == TRUE) 
      || (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE) )
    { 
      /* For freeze frame storage at pending (always for OBDonUDS, configurable for OBDII) 
      the freeze frame storage is only valid if PDTC is set */
      if (Dem_UDSStatus_Test_PDTC(lDTCStatus) == TRUE)
      {
        lIsFreezeFrameStorageValid = TRUE;
      }
    }
  }
  
  return lIsFreezeFrameStorageValid;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
 

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_MemObdFreezeFrame_Public
 * \{
 */

/* ****************************************************************************
% Dem_MemObdFreezeFrame_SelectFFDisplaceIndex
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
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectFFDisplaceIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  DisplacePendingFreezeFrame,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified                                               /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  uint8 lBestCandidate;
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
  boolean lReplaceMisfireOrFuelSys;
#endif
  lBestCandidate = Dem_Cfg_ObdFFSlotCountInVariant();
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacePendingFreezeFrame);                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetHighPrioFFVisibleBeforeDcyQualified);                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  if (Dem_Core_Event_TestHighPriorReadinessGroup(EventId) == TRUE)
  {
    lBestCandidate = Dem_MemObdFreezeFrame_SelectNewestLowPrioFF(FALSE);
    if (lBestCandidate == Dem_Cfg_ObdFFSlotCountInVariant())
    {
      lBestCandidate = Dem_MemObdFreezeFrame_SelectNewestLowPrioFF(TRUE);
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
      if ((lBestCandidate != Dem_Cfg_ObdFFSlotCountInVariant())
        && (DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == FALSE))                             
      {
        *SetHighPrioFFVisibleBeforeDcyQualified = TRUE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
#endif
    }
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
    lReplaceMisfireOrFuelSys = TRUE;
#endif
  }
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
  else
  {
    lReplaceMisfireOrFuelSys = FALSE;
  }
  if ( (lBestCandidate == Dem_Cfg_ObdFFSlotCountInVariant())
    && (DisplacePendingFreezeFrame == TRUE)
    )
  {
    /* All OBD freeze frame slots occupied by high prio monitors */
    lBestCandidate = Dem_Mem_FindDisplaceablePendingFreezeFrame(lReplaceMisfireOrFuelSys);
  }
#endif
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
  if( ( lBestCandidate == Dem_Cfg_ObdFFSlotCountInVariant())
    && (Dem_Event_TestObdVisibilityDelayed(EventId) == FALSE))
  {
    lBestCandidate = Dem_MemObdFreezeFrame_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(SetHighPrioFFVisibleBeforeDcyQualified);  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
#endif

  return lBestCandidate;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacePendingFreezeFrame);                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetHighPrioFFVisibleBeforeDcyQualified);                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  return Dem_Cfg_ObdFFSlotCountInVariant();
#endif
}

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{

  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lLowestTimestamp;

  lBestCandidate = Dem_Cfg_ObdFFSlotCountInVariant();
  lLowestTimestamp = 0xFFFFFFFFU;

  for (lFreezeFrameIndex = 0u; lFreezeFrameIndex < Dem_Cfg_ObdFFSlotCountInVariant(); lFreezeFrameIndex++)
  {
    if (Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex) == DEM_EVENT_INVALID)
    { /* Freeze Frame slot not in use, continue with next slot */
      continue;                                                                                                                  
    }

    if (Dem_MemObdFreezeFrame_FreezeFrameTestVisible(lFreezeFrameIndex) == FALSE)
    { /* Freeze Frame not visible, continue with next slot */
      continue;                                                                                                                  
    }

    if (Dem_Core_DTC_IsObdRelated(Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex)) == FALSE)
    { /* Event of stored Freeze Frame is not OBD related (anymore), continue with next slot */
      continue;                                                                                                                  
    }

    if (Dem_Core_Event_TestHighPriorReadinessGroup(Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex)) == TRUE)
    {
      if ((Dem_Core_Event_TestHighPriorReadinessGroup(Dem_MemObdFreezeFrame_GetEventOfObdFF(lBestCandidate)) == FALSE)
        || (Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex) < lLowestTimestamp))                                              
      {/* current candidate has higher prio readiness group than best candidate
          or has same prio readiness group but is older than best candidate or 
          best candidate is not set yet -> set new best candidate and continue 
          with next slot */
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_Mem_FreezeFrameGetTimestamp(lBestCandidate);
      }
    }
    /* Best candidate has low prio readiness group or is not set yet */
    else if (Dem_Core_Event_TestHighPriorReadinessGroup(Dem_MemObdFreezeFrame_GetEventOfObdFF(lBestCandidate)) == FALSE)
    {
      if (Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex) < lLowestTimestamp)
      { /* current and best candidate have the same priority so if timestamp of current index is older than best candidate-> set new best candidate and continue with next slot */
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_Mem_FreezeFrameGetTimestamp(lBestCandidate);
      }
    }
    else
    {
      /* no candidate found, lBestCandidate is still Dem_Cfg_ObdFFSlotCountInVariant()*/
    }
  }

  return lBestCandidate;
}
#endif

/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameSetVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameSetVisible(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  Index
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  Dem_MemObdFreezeFrame_FreezeFrameSetVisibility(Index, TRUE);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)
#endif
}

/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameResetVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameResetVisible(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  Index
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  Dem_MemObdFreezeFrame_FreezeFrameSetVisibility(Index, FALSE);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)
#endif
}

/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameFreeEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameFreeEntry(
  CONST(uint8, AUTOMATIC) Index
  )
{
  Dem_Mem_FreezeFrameSetEvent(Index, DEM_EVENT_INVALID);
}


#if ((DEM_CFG_SUPPORT_OBDII == STD_ON) \
  && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON))
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_SetFFOfEventVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_SetFFOfEventVisible(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  uint8 lFreezeFrameIndex;
  Dem_EventIdType lMemoryRepresentativeEvent;

  lFreezeFrameIndex = Dem_Cfg_ObdFFSlotCountInVariant();
  lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

  while (lFreezeFrameIndex != 0U)
  {
    lFreezeFrameIndex--;

    if (Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex) == lMemoryRepresentativeEvent)
    {
      Dem_MemObdFreezeFrame_FreezeFrameSetVisible(lFreezeFrameIndex);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
        DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_MemObdFreezeFrame_SetOldestHighPrioFreezeFrameVisible
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
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemObdFreezeFrame_SetOldestHighPrioFreezeFrameVisible(
void
)
{
  Dem_EventIdType lEventId;
  uint8 lFreezeFrameIndex;

  lEventId = DEM_EVENT_INVALID;
  lFreezeFrameIndex = Dem_MemObdFreezeFrame_SelectOldestHighPrioFF(FALSE);

  if (lFreezeFrameIndex != Dem_Cfg_ObdFFSlotCountInVariant())
  {
    lEventId = Dem_MemObdFreezeFrame_GetEventOfObdFF(lFreezeFrameIndex);
    if (lEventId != DEM_EVENT_INVALID)
    {
      Dem_MemObdFreezeFrame_FreezeFrameSetVisible(lFreezeFrameIndex);
    }
  }

  return lEventId;
}
#endif


/* ****************************************************************************
 % Dem_MemObdFreezeFrame_ClearOBDFreezeFrame
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_ClearOBDFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE) &&
      (Dem_Core_DTC_IsObdRelated(EventId) == TRUE))
  { 
    /* Clear the OBD FreezeFrame (if available) */
    uint8 lSelectedIndex;
    lSelectedIndex = Dem_Mem_FreezeFrameFindIndex(EventId);

    if (lSelectedIndex != Dem_Cfg_ObdFFSlotCountInVariant())
    { /* No need to clear the complete FreezeFrame entry as it is only valid if the EventId is valid */
      Dem_MemObdFreezeFrame_FreezeFrameFreeEntry(lSelectedIndex);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
        DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
      /* if this event is visible in Mode02, clear this cache also */
      if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) && (Dem_Mem_FreezeFrameIndexMode02 == lSelectedIndex))
      {
        Dem_Mem_FreezeFrameIndexMode02 = Dem_Cfg_ObdFFSlotCountInVariant();
      }
#endif
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_EventDisplaced
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_EventDisplaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    if ( (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
      || ( (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE) 
        && (Dem_UDSStatus_Test_CDTC(Dem_DTC_RetrieveStatus(EventId)) == FALSE)
        && (Dem_UDSStatus_Test_PDTC(Dem_DTC_RetrieveStatus(EventId)) == FALSE) ) )
    {
      Dem_MemObdFreezeFrame_ClearOBDFreezeFrame(EventId);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr(
  CONST(uint8, AUTOMATIC) Index
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEM_IMPLEMENTATION_FILENAME, __LINE__);                                                                            /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_FreezeFrameData[0].FreezeFrameData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_Cfg_FreezeFrameData[Index].FreezeFrameData[0][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
/* ****************************************************************************
 % Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(
  uint8 Index,
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
    || ((uint8)FreezeFrameType >= DEM_CFG_MAX_NUM_OBDFREEZEFRAME))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEM_IMPLEMENTATION_FILENAME, __LINE__);                                                                            /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_FreezeFrameData[0].FreezeFrameData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_Cfg_FreezeFrameData[Index].FreezeFrameData[FreezeFrameType][0]);
  }
  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_FaultMemory_MemObdFreezeFrame_UpdateOBDIIFreezeFrame
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
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_UpdateOBDIIFreezeFrame(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot,
  uint8 RequiredUpdates
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  Dem_EventIdType lMemoryRepresentativeEventId;

  lMemoryRepresentativeEventId = DEM_EVENT_INVALID;

  if ((RequiredUpdates & DEM_DATA_FF_UPDATE_REQUIRED) != 0u)
  {
    if (FreezeFrameSlot != Dem_Cfg_ObdFFSlotCountInVariant())
    {
      lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_DATA) != 0u)
      {
        if (Dem_Mem_FreezeFrameGetEvent(FreezeFrameSlot) != lMemoryRepresentativeEventId)
        {
          /* initial storage of OBD FF, no update*/
          Dem_MemObdFreezeFrame_FreezeFrameResetVisible(FreezeFrameSlot);
          Dem_Mem_FreezeFrameSetEvent(FreezeFrameSlot, lMemoryRepresentativeEventId);
        }
        Dem_Mem_FreezeFrameSetTimestamp(FreezeFrameSlot, Dem_MemState_GetCurrentTimestamp());
        Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
        if (Dem_DataStorage_PrestorageApplyObdFF(Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr(FreezeFrameSlot),              /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
        {
          Dem_FaultMemory_DataStorage_CollectObdIIFreezeFrame(EventId,
            Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr(FreezeFrameSlot),                                                   /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
            Dem_Cfg_GlobalObdFreezeFrameMaxSize());
        }
      }
      else
      {
        /* No data update, check if time stamp update is needed by visibility
           change at confirmation */
        if ((RequiredUpdates & DEM_DATA_FF_UPDATE_TIMESTAMP) != 0u)
        {
          Dem_Mem_FreezeFrameSetTimestamp(FreezeFrameSlot, Dem_MemState_GetCurrentTimestamp());
          Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);
        }
      }
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_VISIBILITY) != 0u)
      {
        Dem_MemObdFreezeFrame_FreezeFrameSetVisible(FreezeFrameSlot);
      }

    }
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
    else
    {
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_VISIBILITY) != 0u)
      {
        lMemoryRepresentativeEventId = Dem_MemObdFreezeFrame_SetOldestHighPrioFreezeFrameVisible();
      }
    }
# endif
    Dem_Mem_FreezeFrameIndexMode02 = Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame();
  }

  if (RequiredUpdates != 0u)
  {
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
                                DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }

  return lMemoryRepresentativeEventId;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameSlot)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RequiredUpdates)
  return DEM_EVENT_INVALID;
#endif
}                                                                                                                                /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_FaultMemory_MemObdFreezeFrame_UpdateObdOnUdsFreezeFrame
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_UpdateObdOnUdsFreezeFrame(                                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot 
  )
{
#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
  Dem_EventIdType lMemoryRepresentativeEventId;

  lMemoryRepresentativeEventId = DEM_EVENT_INVALID;

  if (FreezeFrameSlot != Dem_Cfg_ObdFFSlotCountInVariant())
  {
    lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
    if (Dem_Mem_FreezeFrameGetEvent(FreezeFrameSlot) != lMemoryRepresentativeEventId)
    {
     /* Initial storage of OBD FF */
     Dem_Mem_FreezeFrameSetEvent(FreezeFrameSlot, lMemoryRepresentativeEventId);
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
      if (Dem_DataStorage_PrestorageApplyObdFF(
        Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),           /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
      {
        Dem_FaultMemory_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
          Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),         /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize());
      }

      /* Create a copy of 0x00 OBD FreezeFrame for 0xF0 OBD FreezeFrame */
      Dem_MemCpy(Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0), /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),
        Dem_Cfg_GlobalObdFreezeFrameMaxSize());
    }
    else
    {
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
      if (Dem_DataStorage_PrestorageApplyObdFF(
        Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0),            /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
      {
        /* On update */
        Dem_FaultMemory_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
          Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0),         /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize());
      }
    } 

    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
                                DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameSlot)
#endif
}                                                                                                                                /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_FaultMemory_MemObdFreezeFrame_GetObdOnUdsSRecNumberByFreezeFrameType
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_GetObdOnUdsSRecNumberByFreezeFrameType(                                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
)
{
  uint8 lSRecNumber;
  if (FreezeFrameType == Dem_RecordReader_0x1904_OBDonUDSFF_0xF0)
  {
    lSRecNumber = DEM_RECORDREADER_SNAPSHOTDATARECORD_OBDONUDS_UPDATEABLE;
  }
  else
  {
    lSRecNumber = DEM_RECORDREADER_SNAPSHOTDATARECORD_OBD;
  }

  return lSRecNumber;
}

/* ****************************************************************************
 % Dem_FaultMemory_MemObdFreezeFrame_InitRestoreMemoryObd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_InitRestoreMemoryObd(
  void
)
{
  /* If the primary entry of a freeze frame event is lost, also remove the
  freeze frame. The most probably scenario to get this state is to clear
  DTCs and NvM restores the previous freeze frame due to CRC errors */
  uint8 lFreezeFrameIndex;
  Dem_EventIdType lEventId;

  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)                                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    lFreezeFrameIndex = Dem_Cfg_ObdFFSlotCountInVariant();
    while (lFreezeFrameIndex != 0U)                                                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lFreezeFrameIndex--;
      lEventId = Dem_Mem_FreezeFrameGetEvent(lFreezeFrameIndex);
      if (lEventId != DEM_EVENT_INVALID)
      {
        if ((Dem_Memory_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY) < lEventId)    /* Omitted first event check due to coverage, case covered by DEM_EVENT_INVALID. */
          || (Dem_Cfg_GetMemoryRepresentative(lEventId) != lEventId)
          || (Dem_MemObdFreezeFrame_IsFreezeFrameStorageValid(lEventId) == FALSE))                                               /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          /* Invalidate the freeze frame */
          Dem_MemObdFreezeFrame_FreezeFrameFreeEntry(lFreezeFrameIndex);
          Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
            DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMOBDFREEZEFRAME_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemObdFreezeFrame_Implementation.h
 *********************************************************************************************************************/
