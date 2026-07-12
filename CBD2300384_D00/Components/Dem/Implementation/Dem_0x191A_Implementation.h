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
/*! \addtogroup Dem_0x191A
 *  \{
 *  \file       Dem_0x191A_Implementation.h
 *  \brief      Subcomponent for handling Service 0x19 Subfunction 0x1A.
 *  \details    Subcomponent for handling Service 0x19 Subfunction 0x1A, which filters DTCs by a user defined record number.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_0X191A_IMPLEMENTATION_H)
#define DEM_0X191A_IMPLEMENTATION_H

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_0x191A_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FilterDataIF_Interface.h"

#include "Dem_RecordReader_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_0X191A_IMPLEMENTATION_FILENAME "Dem_0x191A_Implementation.h"

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
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_0x191A_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_0x191A_PublicProperties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_0x191A_Private Private Methods
 * \{
 */
/* ****************************************************************************
 * Dem_FilterData_0x191A_ERecIter_Next
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *                Feature Extended Data Record and Service 0x19 1A is enabled
 * \config        -
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Next(
  CONSTP2VAR(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_FilterData_0x191A_ERecIter_Exists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Return value of Dem_FilterData_0x191A_ERecIter_Get() is valid
 * \return        FALSE
 *                Return value of Dem_FilterData_0x191A_ERecIter_Get() is invalid
 *
 * \pre           IterPtr has been initialized
 *                Feature Service 0x19 1A is enabled and extended data records are
 *                configured.
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Exists(
  CONSTP2CONST(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_FilterData_0x191A_ERecIter_Get
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Event Id that has Extended Data Record 
 *                configured
 *
 * \pre           IterPtr has been initialized
 *                Feature Service 0x19 1A is enabled and extended data records are
 *                configured.
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Get(
  CONSTP2CONST(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_FilterData_0x191A_ERecIter_Init
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator
 *
 * \details       Initializes the iterator to enumerate data elements
 *
 * \param[in]     ERecDataCollectionTableIndex
 *                The index of the extended data record in the data collection table
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           Feature Service 0x19 1A is enabled and extended data records are
 *                configured.
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Init(
  Dem_Cfg_ERecIndexType ERecDataCollectionTableIndex,
  CONSTP2VAR(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 /* ****************************************************************************
 % Dem_FilterData_0x191A_ERecIter_Next
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Next(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
  IterPtr->mIdx += 1u;                                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}


  /* ****************************************************************************
 % Dem_FilterData_0x191A_ERecIter_Exists
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Exists(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}

/* ****************************************************************************
 % Dem_FilterData_0x191A_ERecIter_Get
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Get(
  CONSTP2CONST(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetEventTable_ERec2EventIdInd(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_FilterData_0x191A_ERecIter_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_0x191A_ERecIter_Init(
  Dem_Cfg_ERecIndexType ERecDataCollectionTableIndex,
  CONSTP2VAR(Dem_FilterData_0x191A_ERecNumberIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ERecDataCollectionTableIndex);                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
  IterPtr->mIdx = Dem_Cfg_GetEventTable_ERec2EventIdIndStartIdxOfDataCollectionTable(ERecDataCollectionTableIndex);              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetEventTable_ERec2EventIdIndEndIdxOfDataCollectionTable(ERecDataCollectionTableIndex);             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_0x191A_Public
 * \{
 */
/* ****************************************************************************
 % Dem_FilterData_0x191A_IsService19_1AEnabled
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_0x191A_IsService19_1AEnabled(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_SVC19_1A == STD_ON);
}

/* ****************************************************************************
 % Dem_FilterData_0x191A_SetDTCFilterByExtendedDataRecordNumber
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
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_0x191A_SetDTCFilterByExtendedDataRecordNumber(
  Dem_FilterData_HandleType DTCFilterId,
  uint8 ExtendedDataRecordNumber
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;
  boolean lERecFound;
  Dem_Cfg_FilterInfoTablePtrType lDTCFilterPtr;

  lDTCFilterPtr = Dem_Client_FilterData_GetFilterDataPtrSafe(DTCFilterId);
  lERecFound = Dem_Dcm_ERec_FindDataCollectionTableIndexByRecordnumber(ExtendedDataRecordNumber,
                                                                       &lDTCFilterPtr->TableIndex.DataCollectionTableIdx);       /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */

  if (lERecFound == TRUE)
  {
    lDTCFilterPtr->FilterDtcStrategy = Dem_FilterData_FilterEventBased_ExtendedDataRecord;                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    Dem_FilterData_0x191A_ERecIter_Init(lDTCFilterPtr->TableIndex.DataCollectionTableIdx,
                               &lDTCFilterPtr->Iterator.ERecIterator);                                                           /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */

    Dem_Client_FilterData_InitCombinedGroupProcessed(lDTCFilterPtr);                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_FilterData_0x191A_GetNumberOfFilteredDTCsByERecNumber
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_0x191A_GetNumberOfFilteredDTCsByERecNumber(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_ERecIndexType DataCollectionTableIdx
)
{
  uint16 lCount;
  Dem_FilterData_0x191A_ERecNumberIterType lExtendedDataRecordIter;
  Dem_Cfg_ProcessedCombinedDTCGroupType lProcessedCombinedDTCGroups;

  lCount = 0;
  Dem_DTC_InitCombinedGroupProcessed(lProcessedCombinedDTCGroups);                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */

  for (Dem_FilterData_0x191A_ERecIter_Init(DataCollectionTableIdx, &lExtendedDataRecordIter);                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_FilterData_0x191A_ERecIter_Exists(&lExtendedDataRecordIter) == TRUE);                                                 /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_FilterData_0x191A_ERecIter_Next(&lExtendedDataRecordIter))                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId;
    lEventId = Dem_FilterData_0x191A_ERecIter_Get(&lExtendedDataRecordIter);                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ( (Dem_DTC_TestCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), lProcessedCombinedDTCGroups) == FALSE )       /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */ /* SBSW_DEM_POINTER_FORWARD_STACK */
      && (Dem_Cfg_GetMemoryRepresentative(lEventId) == lEventId)
      && (Dem_Cfg_EventUdsDtc(lEventId) != DEM_CFG_DTC_UDS_INVALID)
      && (Dem_Cfg_EventDestination(lEventId) == DEM_CFG_MEMORYID_PRIMARY)
      && (Dem_DTC_TestSuppressionByRepresentiveEvent(lEventId) == FALSE))                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      Dem_DTC_SetCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), lProcessedCombinedDTCGroups);                      /* SBSW_DEM_POINTER_FORWARD_STACK */
      lCount++;
    }
  }

  return lCount;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_FilterData_0x191A_FilterDTCByERecNumber
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
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_0x191A_FilterDTCByERecNumber(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr                                                    /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Dem_EventIdType lReturnEventIdValue;
  lReturnEventIdValue = DEM_EVENT_INVALID;

  for (/*Already initialized on SetFilter - */;
    (Dem_FilterData_0x191A_ERecIter_Exists(&DTCFilterPtr->Iterator.ERecIterator) == TRUE);                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
     Dem_FilterData_0x191A_ERecIter_Next(&DTCFilterPtr->Iterator.ERecIterator))                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId;
    lEventId = Dem_FilterData_0x191A_ERecIter_Get(&DTCFilterPtr->Iterator.ERecIterator);                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ( (Dem_Client_FilterData_TestCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), DTCFilterPtr) == FALSE)         /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      && (Dem_Cfg_GetMemoryRepresentative(lEventId) == lEventId)
      && (Dem_Cfg_EventUdsDtc(lEventId) != DEM_CFG_DTC_UDS_INVALID)
      && (Dem_Cfg_EventDestination(lEventId) == DEM_CFG_MEMORYID_PRIMARY)
      && (Dem_DTC_TestSuppressionByRepresentiveEvent(lEventId) == FALSE))                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      lReturnEventIdValue = lEventId;
      Dem_Client_FilterData_SetCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), DTCFilterPtr);                       /* PRQA S 2469 */ /* MD_DEM_14.2_FalsePositive */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      Dem_FilterData_0x191A_ERecIter_Next(&DTCFilterPtr->Iterator.ERecIterator);                                                 /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      break;
    }
  }

  return lReturnEventIdValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_0X191A_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_0x191A_Implementation.h
 *********************************************************************************************************************/
