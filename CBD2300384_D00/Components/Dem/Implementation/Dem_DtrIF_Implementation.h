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
/*! \addtogroup Dem_DtrIF
 *  \{
 *  \file       Dem_DtrIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) Implementation file
 *  \details    Implementation of DtrIF subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DTRIF_IMPLEMENTATION_H)
#define DEM_DTRIF_IMPLEMENTATION_H

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
#include "Dem_DtrIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/(logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_DtrData_Interface.h"
#include "Dem_MidLookup_Interface.h"
#include "Dem_DataReport_Interface.h"
#include "Dem_RecordReader_Interface.h"

/* ********************************************************************************************************************
 * SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
#define DEM_DTRIF_NUM_BYTES_PER_DTR (8u)

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_DtrIF_Private Private Methods
 * \{
 */


#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
/* ****************************************************************************
 * Dem_Dtr_TestDtrConfigured
 *****************************************************************************/
/*!
 * \brief         Tests whether Dtrs are configured.
 *
 * \details       Tests whether at least one Dtr is configured.
 *
 * \return        TRUE
 *                Dtr configured
 * \return        FALSE
 *                No Dtr is configured
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Dtr_TestDtrConfigured(
  void
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
/* ****************************************************************************
 % Dem_Dtr_TestDtrConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Dtr_TestDtrConfigured(
  void
  )
{
  boolean lReturn;
  if (Dem_DtrData_GetGlobalDtrCount() > 0u)
  {
    lReturn = TRUE;
  }
  else
  {
    lReturn = FALSE;
  }
  return lReturn;
}
#endif
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h" 

/* ********************************************************************************************************************
 * SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_DtrIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Dtr_IsValidHandle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Dtr_IsValidHandle(
  CONST(DTRIdType, AUTOMATIC) DtrId
  )
{
  boolean lReturnValue;
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    lReturnValue = Dem_DtrData_IsValidHandle(DtrId);
  }
  else
  {
    lReturnValue = FALSE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Dtr_SetDtrValues
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_SetDtrValues(                                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(DTRIdType, AUTOMATIC) DTRId,
  CONST(uint16, AUTOMATIC) TestValue,
  CONST(uint16, AUTOMATIC) LowerLimit,
  CONST(uint16, AUTOMATIC) UpperLimit
  )
{
  /* No check for support of OBD in current variant as function is only called from Dem_NvM_InitDtrData which is called at
  a time where the global config pointer is not initialized */
# if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
  if (Dem_Dtr_TestDtrConfigured() == TRUE)
# endif
  {
    Dem_DtrData_SetDtrValues(DTRId, TestValue, LowerLimit, UpperLimit);
  }
}

/* ****************************************************************************
 % Dem_Dtr_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_Init(
  void
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
#endif
    {
      Dem_DtrData_Init();
      Dem_MidLookup_Init();
    }
  }
}

/* ****************************************************************************
 % Dem_Dtr_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_PreInit(
  void
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
#endif
    {
      Dem_DtrData_PreInit();
      Dem_MidLookup_PreInit();
    }
  }
}

/* ****************************************************************************
 % Dem_Dtr_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_MainFunction(
  void
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    Dem_DtrData_MainFunction();
  }
}

/* ****************************************************************************
 % Dem_Dtr_Shutdown
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_Shutdown(
  void
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    Dem_DtrData_Shutdown();
  }
}

/* ****************************************************************************
 % Dem_Dtr_EventAvailabilityChanged
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_EventAvailabilityChanged(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
#endif
    {
      Dem_DtrData_EventAvailabilityChanged(EventId);
    }
  }
}

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_Dtr_DTCSuppressionChanged
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_DTCSuppressionChanged(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
# if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
# endif
    {
      Dem_DtrData_DTCSuppressionChanged(EventId);
    }
  }
}
#endif

/* ****************************************************************************
 % Dem_Dtr_ResetDtrsOfEvent
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
Dem_Dtr_ResetDtrsOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
#endif
    {
      if (EventId == DEM_EVENT_INVALID)
      {
        Dem_DtrData_ResetDtrsWithoutEvent();
      }
      else
      {
        Dem_DtrData_ResetDtrsOfEvent(EventId);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_Dtr_SetDtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Dtr_SetDtr(
  CONST(DTRIdType, AUTOMATIC) DTRId,
  CONST(sint32, AUTOMATIC) TestResult,
  CONST(sint32, AUTOMATIC) LowerLimit,
  CONST(sint32, AUTOMATIC) UpperLimit,
  CONST(Dem_DTRControlType, AUTOMATIC) Ctrlval
  )
{
  Std_ReturnType lReturnValue;
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    /* Check for configured Dtr is done on API top level */
    lReturnValue = Dem_DtrData_SetDtr(DTRId, TestResult, LowerLimit, UpperLimit, Ctrlval);
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Dtr_UpdateMidVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Dtr_UpdateMidVisibility(
  void
  )
{
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
# if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == TRUE)
# endif
    {
      Dem_MidLookup_UpdateMidVisibility();
    }
  }
}

/* ****************************************************************************
 % Dem_Dtr_GetSupportedMidValue
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Dtr_GetSupportedMidValue(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
CONST(uint8, AUTOMATIC)  Mid,
CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC)  MidValue
)
{
  boolean lReturnValue;

  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
# if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == FALSE)
    {
      lReturnValue = FALSE;
    }
    else
# endif
    {
      if (Dem_MidLookup_IsSupportedMid(Mid) == TRUE)
      {
        *MidValue = Dem_MidLookup_GetSupportedMidValue(Mid);                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lReturnValue = TRUE;
      }
      else
      {
        lReturnValue = FALSE;
      }
    }
  }
  else
  {
    lReturnValue = FALSE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Dtr_GetTidCountOfMid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Dtr_GetTidCountOfMid(                                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  Mid,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  TidCount
  )
{
  Std_ReturnType lReturnValue;

  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == FALSE)
    {
      lReturnValue = E_NOT_OK;
    }
    else
#endif
    {
      lReturnValue = Dem_MidLookup_GetTidCountOfMid(Mid, TidCount);                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Dtr_GetDataAndTidOfMid
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Dtr_GetDataAndTidOfMid(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC) Mid,
  CONST(uint8, AUTOMATIC) TidIndex,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  Tid,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  UaSid,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA) TestResult,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA) LowerLimit,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA) UpperLimit
)
{
  Std_ReturnType lReturnValue;

  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_DTR_CONFIGCHECK == STD_ON)
    if (Dem_Dtr_TestDtrConfigured() == FALSE)
    {
      lReturnValue = E_NOT_OK;
    }
    else
#endif
    {
      uint8 lTid;
      DTRIdType lDtrId;

      lTid = 0u;
      lDtrId = Dem_MidLookup_FindDtrId(Mid, TidIndex, &lTid);                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
      if (Dem_Dtr_IsValidHandle(lDtrId) == TRUE)
      {
        *Tid = lTid;                                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *UaSid = (uint8) Dem_DtrData_GetUaSId(lDtrId);                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

        /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
        Dem_EnterCritical_DiagMonitor();

        *TestResult = Dem_DtrData_GetTestValue(lDtrId);                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *LowerLimit = Dem_DtrData_GetLowerLimit(lDtrId);                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *UpperLimit = Dem_DtrData_GetUpperLimit(lDtrId);                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

        Dem_LeaveCritical_DiagMonitor();
        /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

        lReturnValue = E_OK;
      }
      else
      {
        lReturnValue = E_NOT_OK;
      }
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_MSR_STPAR */

#if ( (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) || (DEM_FEATURE_NEED_OBD == STD_ON) )
/* ****************************************************************************
% Dem_Dtr_GetDtrsOfEvent
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
Dem_Dtr_GetDtrsOfEvent(
 CONST(Dem_DataReport_DidCopyInfoBasePtrType, AUTOMATIC)  DidCopyInfoBasePtr                                                     /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
 )
{
  if(Dem_Data_GetRemainingSizeOfDestinationBuffer(DidCopyInfoBasePtr->DestinationBuffer) > 0u)                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_ComplexIterType lMidIter;
    DTRIdType lDtrId;
    uint16 lDTRCountWriteIdx;
    uint8 lDtrCount;

    lDtrCount = 0;
    lDTRCountWriteIdx = Dem_Data_GetDestinationBufferWriteIndex(DidCopyInfoBasePtr->DestinationBuffer);                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_IncrementWriteIndexDestinationBuffer(DidCopyInfoBasePtr->DestinationBuffer, 1u);                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    if (Dem_DtrData_IsDtrEnabled() == TRUE)
    {
      for (Dem_MidLookup_MidIterInit(0, &lMidIter);                                                                              /* PRQA S 0771 */ /* MD_DEM_15.4_opt */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_MidLookup_MidIterExists(&lMidIter) == TRUE;                                                                       /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_MidLookup_MidIterNext(&lMidIter)                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
        )
      {
        lDtrId = Dem_MidLookup_MidIterGetDtrId(&lMidIter);                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */

        if (Dem_DtrData_IsDtrOfEvent(lDtrId, DidCopyInfoBasePtr->EventId))
        {
          Dem_DtrData_GetDtrDataIfEventAvailable(DidCopyInfoBasePtr->DestinationBuffer, lDtrId);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          lDtrCount++;
        }
      }
    }
    Dem_Data_WriteDestinationBufferAtIndex(DidCopyInfoBasePtr->DestinationBuffer, lDTRCountWriteIdx, lDtrCount);                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
% Dem_Dtr_GetERec0x92SizeOfEvent
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

DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Dtr_GetERec0x92SizeOfEvent(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  uint16 lRetVal;
  lRetVal = 0;

  if (Dem_DtrData_IsDtrEnabled() == TRUE)
  {
    Dem_ComplexIterType lMidIter;
    DTRIdType lDtrId;

    for (Dem_MidLookup_MidIterInit(0, &lMidIter);                                                                                /* PRQA S 0771 */ /* MD_DEM_15.4_opt */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_MidLookup_MidIterExists(&lMidIter) == TRUE;                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_MidLookup_MidIterNext(&lMidIter)                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
        )
    {
      lDtrId = Dem_MidLookup_MidIterGetDtrId(&lMidIter);                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_DtrData_IsDtrOfEvent(lDtrId, EventId))
      {
        lRetVal += DEM_DTRIF_NUM_BYTES_PER_DTR;
      }
    }
    lRetVal++; /* One extra byte for the number of DTRs */
  }
  return lRetVal;

}


/*!
 * \}
 */

#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_DTRIF_IMPLEMENTATION_H */
/*!
 * \}
 */
/* *********************************************************************************************************************
 *  END OF FILE: Dem_DtrIF_Implementation.h
 **********************************************************************************************************************/
