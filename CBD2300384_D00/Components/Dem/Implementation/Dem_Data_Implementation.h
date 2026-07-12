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
/*! \addtogroup Dem_Data
 *  \{
 *  \file       Dem_Data_Implementation.h
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

#if !defined (DEM_DATA_IMPLEMENTATION_H)
#define DEM_DATA_IMPLEMENTATION_H

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
#include "Dem_Data_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_RecordReader_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DATA_IMPLEMENTATION_FILENAME "Dem_Data_Implementation.h"

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
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /*MD_MSR_MemMap */
/*!
 * \defgroup Dem_Data_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /*MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Data_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Data_GetPtrToWriteIndexOfDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  return &(DestinationBuffer->Buffer[DestinationBuffer->WriteIndex]);
}

/* ****************************************************************************
 % Dem_Data_GetRemainingSizeOfDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Data_GetRemainingSizeOfDestinationBuffer(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  uint16 lRemainingSize = 0U;

  if (DestinationBuffer->WriteIndex < DestinationBuffer->BufferSize)
  {
    lRemainingSize = (DestinationBuffer->BufferSize - DestinationBuffer->WriteIndex);
  }

  return lRemainingSize;
}

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Data_PermanentEntryRemove
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryRemove(
  CONST(uint8, AUTOMATIC)  PermanentIndex
  )
{
  /* Free the given permanent slot */
  Dem_Mem_PermanentSetEvent(PermanentIndex, DEM_EVENT_INVALID);

  Dem_Internal_AssertReturnVoid(Dem_MemState_MemoryGetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) > 0, DEM_E_INCONSISTENT_STATE)

  Dem_MemState_MemorySetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_MemState_MemoryGetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) - 1u);
  Dem_Mem_PermanentSetStatus(PermanentIndex, DEM_ESM_PERMANENT_NONE);
  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                              DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Data_PermanentEntryFind
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Data_PermanentEntryFind(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lPermanentIndex;

  for (lPermanentIndex = 0u; lPermanentIndex < Dem_Cfg_GlobalPermanentMaxCount(); ++lPermanentIndex)
  {
    if (Dem_Mem_PermanentGetEvent(lPermanentIndex) == EventId)
    {
      break;
    }
  }
  return lPermanentIndex;
}
#endif


#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
/* ****************************************************************************
 % Dem_Data_GetObdOdometer
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
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_GetObdOdometer(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_VAR_NOINIT)  Odometer
  )
{
  Std_ReturnType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ( (Dem_Cfg_ObdDistanceInformation() >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataIsStoredInNV(Dem_Cfg_ObdDistanceInformation()) == FALSE)
    || (Dem_Cfg_DataCallbackType(Dem_Cfg_ObdDistanceInformation()) != DEM_CFG_DATA_FROM_CS_PORT)
    || (Dem_Cfg_DataCbkRead(Dem_Cfg_ObdDistanceInformation()) == NULL_PTR)
    || (Dem_Cfg_DataSize(Dem_Cfg_ObdDistanceInformation()) != 4u) )                                                              
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    *Odometer = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_NOT_OK;
  }
  else
# endif
  {
    uint8 lBuffer[4];

    lReturnValue = ((Dem_ReadDataUint8FPtrType)Dem_Cfg_DataCbkRead(Dem_Cfg_ObdDistanceInformation()))(&lBuffer[0]);           /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_LOCAL_DESTINATIONBUFFER */
    if (lReturnValue == E_OK)
    {
      *Odometer = Dem_Make32Bit(lBuffer[0], lBuffer[1], lBuffer[2], lBuffer[3]);                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *Odometer = 0u;                                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON) */

/* ****************************************************************************
 % Dem_Data_PermanentEntryUpdate
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
Dem_Data_PermanentEntryUpdate(                                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus,
  CONST(uint8, AUTOMATIC)  NewDtcStatus
  )
{
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  uint8 lPermanentIndex;

  /* If the event has a permanent DTC */
  lPermanentIndex = Dem_Data_PermanentEntryFind(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    boolean lExitPassive;
    boolean lEntryRemoved;
    uint8 lPermanentStateNew;
    uint8 lPermanentStateOld;

    lExitPassive = FALSE;
    lPermanentStateNew = Dem_Mem_PermanentGetStatus(lPermanentIndex);
    lPermanentStateOld = lPermanentStateNew;

    /* Mark the permanent entry as visible */
    lPermanentStateNew = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentStateNew);

    switch (DEM_ESM_GET_PERMANENT_STATE(lPermanentStateOld))
    {
    /* In state PERMANENT_ACTIVE: */
    case DEM_ESM_PERMANENT_ACTIVE:
      if ( (Dem_UDSStatus_Test_WIR(OldDtcStatus) == TRUE)
        && (Dem_UDSStatus_Test_WIR(NewDtcStatus) == FALSE))                                                                      
      {
        Dem_Data_PermanentEntryRemove(lPermanentIndex);
      }
      break;

    /* In state PERMANENT_PASSIVE: */
    case DEM_ESM_PERMANENT_PASSIVE:
# if (DEM_CFG_SUPPORT_PERMANENT_MANDATORY_PFC == STD_OFF)
      /* Unless MANDATORY_PFC is configured, leave the PASSIVE state if the Event requests the MIL */
      if (Dem_UDSStatus_Test_WIR(NewDtcStatus) == TRUE)
      {
        lExitPassive = TRUE;
      }
      else
# endif
      /* Otherwise, if the event was not tested failed this cycle */
      if (Dem_UDSStatus_Test_TFTOC(OldDtcStatus) == FALSE)
      {
        /* Test if the PCF conditions are met, if so set the PFC condition for the permanent entry */
        if (DEM_OPERATIONCYCLE_TEST_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
        }
        /* Test for a qualified passed test result, if so set the TESTED condition for the permanent entry */
        if (Dem_UDSStatus_Test_TNCTOC(OldDtcStatus) == FALSE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        }
        /* If all conditions are met for the permanent entry, exit the passive state */
        if ( (DEM_ESM_TEST_PERMANENT_CONDITION_TESTED(lPermanentStateNew) == TRUE)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_PFC(lPermanentStateNew) == TRUE)
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_DCY(lPermanentStateNew) == TRUE)
# endif
          )
        {
          lExitPassive = TRUE;
        }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* Otherwise, if DCY qualification is configured as permanent clear condition, set the DCY condition
         * for the permanent entry */
        else
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
        }
# endif
      }
      else
      {
        /* If the event was tested failed, reset the PFC and TESTED conditions for the permanent entry */
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* If DCY qualification is configured as permanent clear condition, also set the DCY condition for the
         * permanent entry */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
# endif
      }
      break;

    default:                                                                                                                     /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* Do nothing */
      break;
    } /* end switch */

    /* On exit of state PASSIVE */
    lEntryRemoved = FALSE;
    if (lExitPassive == TRUE)
    {
      if (Dem_UDSStatus_Test_WIR(NewDtcStatus) == FALSE)
      { /* If the event no longer requests the MIL, remove the permanent entry. */
        Dem_Data_PermanentEntryRemove(lPermanentIndex);
        lEntryRemoved = TRUE;
      }
      else
      { /* Otherwise, go to ACTIVE state and reset the PFC and TESTED conditions for the permanent entry. */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_STATE(lPermanentStateNew, DEM_ESM_PERMANENT_ACTIVE);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
      }
    }

    /* If the permanent entry was changed, mark it for immediate NV synchronization */
    if ((lPermanentStateNew != lPermanentStateOld) && (lEntryRemoved == FALSE))
    {
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentStateNew);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  } /* (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount()) */
#else
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldDtcStatus)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(NewDtcStatus)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Data_PermanentEntryUpdateOnDCyEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryUpdateOnDCyEnd(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus
  )
{
#if (DEM_FEATURE_NEED_PERMANENT_FOR_WWHOBD == STD_ON)
  uint8 lPermanentIndex;

  /* If the event has a permanent DTC */
  lPermanentIndex = Dem_Data_PermanentEntryFind(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    uint8 lPermanentState;

    lPermanentState = Dem_Mem_PermanentGetStatus(lPermanentIndex);

    if (DEM_ESM_GET_PERMANENT_STATE(lPermanentState) == DEM_ESM_PERMANENT_PASSIVE)
    {
      /* Otherwise, if the event was not tested failed this cycle */
      if (Dem_UDSStatus_Test_TFTOC(OldDtcStatus) == FALSE)
      {
        /* Test for a qualified passed test result, if so set the TESTED condition for the permanent entry */
        if (Dem_UDSStatus_Test_TNCTOC(OldDtcStatus) == FALSE)
        {
          Dem_Data_PermanentEntryRemove(lPermanentIndex);
        }
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldDtcStatus)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Data_PermanentEntryClear
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryClear(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)   ProcessClearDTC
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  uint8 lPermanentIndex;

  lPermanentIndex = Dem_Data_PermanentEntryFind(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    uint8 lPermanentState;
    lPermanentState = Dem_Mem_PermanentGetStatus(lPermanentIndex);

    /* Go to PASSIVE state and reset the conditions to remove the PDTC. Keep its visibility state. */
    lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_PASSIVE);
    lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentState);

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      if (Dem_DTC_TestPfcRequired(EventId) == TRUE)
      {
        lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentState);
      }
      else
      {
        lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentState);
      }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
      /* If DCY qualification is configured as permanent clear condition */
      if (DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
      { /* If the DCY is qualified, RESET the DCY condition for the permanent entry */
        lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_DCY(lPermanentState);
      }
      else
      { /* Otherwise, set the DCY condition for the permanent entry */
        lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentState);
      }
# endif
    }

    /* If the permanent entry was changed, mark it for cleared and immediate NV synchronization */
    if (lPermanentState != Dem_Mem_PermanentGetStatus(lPermanentIndex))
    {
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentState);
      if (ProcessClearDTC == TRUE)
      {
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
          DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
      }
      else
      {
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
          DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
    }
  }
#else /* (DEM_CFG_SUPPORT_PERMANENT== STD_ON) */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ProcessClearDTC)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


                                                         /* Permanent Memory */
/* ------------------------------------------------------------------------- */

/* ****************************************************************************
 % Dem_Data_PermanentEntryAdd
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
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryAdd(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_EventIdType lMasterEventId;
  uint8   lPermanentIndex;

  lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

  /* scan through the permanent memory if the given event is available */
  lPermanentIndex = Dem_Data_PermanentEntryFind(lMasterEventId);

  if (Dem_MemState_MemoryGetMaxSize(DEM_CFG_MEMORYID_PERMANENT) == lPermanentIndex)
  { /* Event not stored, try to find an empty slot */
    lPermanentIndex = Dem_Data_PermanentEntryFind(DEM_EVENT_INVALID);

    if (lPermanentIndex != Dem_MemState_MemoryGetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
    {
      uint8 lPermanentState;

      lPermanentState = DEM_ESM_PERMANENT_NONE;
      lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

      /* OBD-II or OBDonUDS enabled in this (PBS) variant */
      if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      {
        /* PDTC is immediately visible if the Event is a 0-Trip event and OBD Visibility is not delayed. 
           If DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE is enabled, it is sufficient that the WIR bit is
           externally visible to set the permanent entry visible. */
        if (
# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
          (Dem_EventInternalStatus_Test_QualifyStatus_WIR(Dem_Event_GetQualifyStatus(EventId)) == TRUE)
# else
          (Dem_Cfg_EventTripTarget(EventId) == 0U)
          && (Dem_Event_TestObdVisibilityDelayed(EventId) == FALSE)
# endif
           )
        {
          lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
        }
      }
      /* WWH-OBD enabled in this (PBS) variant */
      else
      {
        lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
      }
      Dem_Mem_PermanentSetEvent(lPermanentIndex, lMasterEventId);
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentState);

      Dem_MemState_MemorySetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_MemState_MemoryGetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) + 1u );

      /* persist status change in NVM */
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    { /* Entry was not available and no free slot detected --> set overflow indication */
      Dem_Memory_MemState_SetOverflow(DEM_CFG_MEMORYID_PERMANENT);
    }
  }
  else
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    /* Permanent DTC of event already stored */
    uint8 lPermanentStateNew;
    uint8 lPermanentStateOld;

    lPermanentStateNew = Dem_Mem_PermanentGetStatus(lPermanentIndex);
    lPermanentStateOld = lPermanentStateNew;
    lPermanentStateNew = DEM_ESM_SET_PERMANENT_STATE(lPermanentStateNew, DEM_ESM_PERMANENT_ACTIVE);

    /* If the permanent entry was changed, mark it for immediate NV synchronization */
    if (lPermanentStateNew != lPermanentStateOld)
    {
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentStateNew);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
  else
  {
    /* MISRA case */
  }
#else /* (DEM_CFG_SUPPORT_PERMANENT== STD_ON) */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Data_InitDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_InitDestinationBuffer(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_SharedDataPtrType, AUTOMATIC) Buffer,
  CONST(uint16, AUTOMATIC) Size
  )
{
  DestinationBuffer->Buffer = Buffer;                                                                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  DestinationBuffer->BufferSize = Size;                                                                                          /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  DestinationBuffer->WriteIndex = 0u;                                                                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
}

/* ****************************************************************************
 % Dem_Data_ResetWriteIndexDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_ResetWriteIndexDestinationBuffer(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  DestinationBuffer->WriteIndex = 0u;                                                                                            /* SBSW_DEM_DESTINATIONBUFFER_WRITEINDEX */
}

/* ****************************************************************************
 % Dem_Data_CheckAvailableSizeDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_CheckAvailableSizeDestinationBuffer(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) RequestedSize
  )
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_OK;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < RequestedSize)                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = E_NOT_OK;
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DestinationBuffer)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RequestedSize)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Data_IncrementWriteIndexDestinationBuffer
 *****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_IncrementWriteIndexDestinationBuffer(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Size
)
{
  DestinationBuffer->WriteIndex += Size;                                                                                         /* SBSW_DEM_DESTINATIONBUFFER_WRITEINDEX */
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBuffer(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_ConstSharedDataPtrType, AUTOMATIC) SourceBuffer,
  CONST(uint16, AUTOMATIC) Size
  )
{
  if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, Size) == E_OK)                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_SharedDataPtrType lDestinationBufferPtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_MemCpy(lDestinationBufferPtr, SourceBuffer, Size);                                                                       /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_WRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, Size);                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferAtIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferAtIndex(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,                                                        /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  CONST(uint16, AUTOMATIC) WriteIndex,
  CONST(uint8, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (WriteIndex >= DestinationBuffer->BufferSize)
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[WriteIndex] = Data;                                                                                /* SBSW_DEM_WRITE_DESTINATIONBUFFER_AT_INDEX */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint8
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint8(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint8, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 1U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Data;                                                             /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 1u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint16
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 2U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetHiByte(Data);                                              /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetLoByte(Data);                                         /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 2u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint16_Intel
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16_Intel(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 2U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetLoByte(Data);                                              /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetHiByte(Data);                                         /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 2u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint32
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 4U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetHiHiByte(Data);                                            /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetHiLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 2u] = Dem_GetLoHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 3u] = Dem_GetLoLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 4u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Data_WriteDestinationBufferUint32_Intel
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32_Intel(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Data_GetRemainingSizeOfDestinationBuffer(DestinationBuffer) < 4U)                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Error_RunTimeCheckFailed(DEM_DATA_IMPLEMENTATION_FILENAME, __LINE__);                                                    /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex] = Dem_GetLoLoByte(Data);                                            /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 1u] = Dem_GetLoHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 2u] = Dem_GetHiLoByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    DestinationBuffer->Buffer[DestinationBuffer->WriteIndex + 3u] = Dem_GetHiHiByte(Data);                                       /* SBSW_DEM_ARRAYWRITE_DESTINATIONBUFFER */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, 4u);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
% Dem_Data_GetDestinationBufferWriteIndex
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Data_GetDestinationBufferWriteIndex(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  return DestinationBuffer->WriteIndex;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DATA_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Data_Implementation.h
 *********************************************************************************************************************/
