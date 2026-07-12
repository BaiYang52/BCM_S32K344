/* ********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_J1939DTC
 *  \{
 *  \file       Dem_J1939DTC_Implementation.h
 *  \brief      Subcomponent for handling requests to filter J1939 DTCs.
 *  \details    Subcomponent for handling requests to filter J1939 DTCs for J1939 Diagnostic Messages.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939DTC_IMPLEMENTATION_H)
#define DEM_J1939DTC_IMPLEMENTATION_H

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
#include "Dem_J1939DTC_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
#include "Dem_Core_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_ClientAccess_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_J1939DTC_IMPLEMENTATION_FILENAME "Dem_J1939DTC_Implementation.h"

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
 * \defgroup Dem_J1939DTC_PrivateProperties Private Properties
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
 * \addtogroup Dem_J1939DTC_PublicProperties
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
 * \defgroup Dem_J1939DTC_Private Private Methods
 * \{
 */

 /* ****************************************************************************
  * Dem_J1939DTC_Filter_InitMemoryIdAndIterator()
  *****************************************************************************/
 /*!
  * \brief         Initialize memory Id and iterator for a 'filter J1939 DTCs'
  *                request.
  *
  * \details       Initialize memory Id and iterator. The iterator is used to
  *                test all events of an event memory or all permanent entries
  *                for a 'filter J1939 DTCs' request.
  *
  * \param[in]     DTCFilterPtr
  *                Pointer to the DTC filter that is initialized.
  * \param[in]     TranslatedMemoryId
  *                Unique identifier of the memory to filter.
  *
  * \pre           Function must be invoked with a valid TranslatedMemoryId.
  *                .i.e. only invoked if Dem_FaultMemory_Memory_TranslateDTCOrigin()
  *                returns E_OK.
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_InitMemoryIdAndIterator(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  uint16 TranslatedMemoryId
);

/* ****************************************************************************
 * Dem_J1939DTC_EventIter_Init()
 *****************************************************************************/
/*!
 * \brief         Init the iterator.
 *
 * \details       Initializes the iterator to iterate over permanent entries 
 *                or event Ids.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     IterPtr
 *                Pointer to the Iterator.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_Init(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_EventIter_InitToEventId()
*****************************************************************************/
/*!
 * \brief         Init the iterator.
 *
 * \details       Initializes the iterator to enumerate all event Ids
 *                of the events belonging to the filtered event memory.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     IterPtr
 *                Pointer to the Iterator.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_InitToEventId(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_EventIter_InitToPermanentEntry()
 *****************************************************************************/
/*!
 * \brief         Init the iterator.
 *
 * \details       Initializes the iterator to iterate over all event Ids
 *                stored in permanent memory.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     IterPtr
 *                Pointer to the Iterator.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_InitToPermanentEntry(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_EventIter_GetEventId
 *****************************************************************************/
/*!
 * \brief         Get the event Id of element pointed to by the iterator.
 *
 * \details       Get the event Id of element pointed to by the iterator.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Event Id
 *                The event Id of the element pointed tp by the iterator
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_J1939DTC_EventIter_GetEventId(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_UpdateIterator
 *****************************************************************************/
/*!
 * \brief         Update the iterator for a 'filter J1939 DTCs' request.
 *
 * \details       Update the iterator for a 'filter J1939 DTCs' request with
 *                the next event to test.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     IterPtr
 *                Pointer to the iterator.
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_UpdateIterator(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * \brief         Translate the filtered DTC Origin to the DEM internal
 *                memory handle.
 *
 * \details       Translate the filtered DTC Origin to the DEM internal
 *                memory handle.
 *
 * \param[in]     DTCOrigin
 *                Identifier of the event memory to translate.
 * \param[in]     DTCStatusFilter
 *                The DTC status to filter.
 *
 * \param[out]    TranslatedMemoryId
 *                Translated Memory Identifier.
 *
 * \return        TRUE
 *                Translation successful.
 * \return        FALSE
 *                Translation unsuccessful.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939DTC_TranslateDTCOrigin(
  Dem_DTCOriginType DTCOrigin,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) TranslatedMemoryId
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_InitDTCStatus
 *****************************************************************************/
/*!
 * \brief         Initialize the DTC status filter.
 *
 * \details       Initialize the DTC status filter for a 'filter J1939 DTCs'
 *                request.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 * \param[in]     DTCStatusFilter
 *                The DTC status to filter.
 *
 * \return        E_OK
 *                DTC status filter was initialized.
 * \return        E_NOT_OK
 *                DTC status filter was not initialized.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DTC_Filter_InitDTCStatus(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter
);

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter
 *****************************************************************************/
/*!
 * \brief         Test if the event matches the J1939 DTC filter criteria.
 *
 * \details       Test if the event matches the J1939 DTC filter criteria.
 *
 * \param[in]     DTCStatusFilter
 *                The DTC status to filter.
 * \param[in]     NodeIndex
 *                The node Index.
 * \param[in]     FilterEmissionRelevantDTCs
 *                Specifies whether event must be emission relevant to match 
 *                the filter.
 * \param[in]     EventId
 *                Unique handle of the Event to test.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event matches the filter criteria.
 * \return        FALSE
 *                Event does not match the filter criteria.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter(
  Dem_J1939DcmDTCStatusFilterType  DTCStatusFilter,
  uint8 NodeIndex,
  boolean FilterEmissionRelevantDTCs,
  Dem_EventIdType EventId
);


/* ****************************************************************************
 * Dem_J1939DTC_Filter_GetNumberOfDTCsTested
 *****************************************************************************/
/*!
 * \brief         Get the number of J1939 DTCs tested.
 *
 * \details       Get the number of J1939 DTCs tested during J1939 DTC
 *                filtering.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 *
 * \return        The number of J1939 DTCs tested.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_J1939DTC_Filter_GetNumberOfDTCsTested(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_ResetNumberOfDTCsTested
 *****************************************************************************/
/*!
 * \brief         Reset the number of DTCs tested.
*
 * \details       Reset the number of DTCs tested during J1939 DTC filtering
 *                to zero.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_ResetNumberOfDTCsTested(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_IncrementNumberOfDTCsTested
 *****************************************************************************/
/*!
 * \brief         Increment the number of DTCs tested.
 *
 * \details       Increment the number of DTCs tested during J1939 DTC
 *                filtering by one.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_IncrementNumberOfDTCsTested(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
);

/* ****************************************************************************
 * Dem_J1939DTC_Filter_TestFilterPermanentMemory
 *****************************************************************************/
/*!
 * \brief         Test if filter is set to permanent memory.
 *
 * \details       Test if filter is set to J1939 DTCs with a permanent entry.
 *
 * \param[in]     DTCFilterPtr
 *                Pointer to the DTC filter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_J1939DTC_Filter_TestFilterPermanentMemory(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
);

/* ****************************************************************************
 * Dem_FilterData_J1939DTC_GetJ1939ReadDtcRuntimeLimit
 *****************************************************************************/
/*!
 * \brief         Get the J1939 read DTC runtime limit
 *
 * \details       Get the J1939 read DTC runtime limit
 *
 * \return        The J1939 read DTC runtime limit
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_J1939DTC_GetJ1939ReadDtcRuntimeLimit(
  void
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
 /* ****************************************************************************
  % Dem_FilterData_J1939DTC_TranslateDTCOrigin()
  *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939DTC_TranslateDTCOrigin(
  Dem_DTCOriginType DTCOrigin,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) TranslatedMemoryId
)
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;

  if ( (DTCOrigin == DEM_DTC_ORIGIN_PRIMARY_MEMORY)
    || (Dem_Memory_UserDefinedMemory_isUserDefinedMemoryValid(DTCOrigin) == TRUE) )
  {
    Dem_DTCOriginType lMappedOrigin;

    /* If DTC status filter equals 'Permanent' it is asserted beforehand that DTC origin is always 'Primary' */
    if (DTCStatusFilter == DEM_J1939DTC_PERMANENT)
    {
      lMappedOrigin = DEM_DTC_ORIGIN_PERMANENT_MEMORY;
    }
    else
    {
      lMappedOrigin = DTCOrigin;
    }

    if (Dem_FaultMemory_Memory_TranslateDTCOrigin(lMappedOrigin, TranslatedMemoryId) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}

 /* ****************************************************************************
  % Dem_J1939DTC_Filter_InitMemoryIdAndIterator()
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
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_InitMemoryIdAndIterator(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  uint16 TranslatedMemoryId
)
{
  if (TranslatedMemoryId == DEM_CFG_MEMORYID_PERMANENT)
  {
    DTCFilterPtr->MemoryId = TranslatedMemoryId;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    DTCFilterPtr->Iterator.PermanentIndex = 0u;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
  {
    DTCFilterPtr->MemoryId = TranslatedMemoryId;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    DTCFilterPtr->Iterator.EventIndex = Dem_MemState_MemoryGetFirstEvent(TranslatedMemoryId);                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_J1939DTC_EventIter_InitToPermanentEntry()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_InitToPermanentEntry(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  IterPtr->mIdx = DTCFilterPtr->Iterator.PermanentIndex;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GlobalPermanentMaxCount();                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_J1939DTC_EventIter_InitToEventId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_InitToEventId(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  IterPtr->mIdx = DTCFilterPtr->Iterator.EventIndex;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = (Dem_Memory_GetLastEvent(DTCFilterPtr->MemoryId) + 1u);                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_J1939DTC_EventIter_Init()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_J1939DTC_EventIter_Init(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  if (Dem_J1939DTC_Filter_TestFilterPermanentMemory(DTCFilterPtr))                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_J1939DTC_EventIter_InitToPermanentEntry(DTCFilterPtr, IterPtr);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    Dem_J1939DTC_EventIter_InitToEventId(DTCFilterPtr, IterPtr);                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_J1939DTC_EventIter_GetEventId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_J1939DTC_EventIter_GetEventId(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  Dem_EventIdType lEventId;

  if (Dem_J1939DTC_Filter_TestFilterPermanentMemory(DTCFilterPtr) == TRUE)                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    lEventId = Dem_Mem_PermanentGetEvent( (uint8) (IterPtr->mIdx) );
  }
  else
  {
    lEventId = (Dem_EventIdType) (IterPtr->mIdx);
  }

  return lEventId;
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_UpdateIterator()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_UpdateIterator(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  if (Dem_J1939DTC_Filter_TestFilterPermanentMemory(DTCFilterPtr) == TRUE)                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    DTCFilterPtr->Iterator.PermanentIndex = (uint8)(IterPtr->mIdx);                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
  {
    DTCFilterPtr->Iterator.EventIndex = (uint16)(IterPtr->mIdx);                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_InitDTCStatus()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DTC_Filter_InitDTCStatus(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter
)
{
  Std_ReturnType lReturnValue;
  uint8 lDTCStatusFilterType;

  lReturnValue = E_OK;

  switch (DTCStatusFilter)
  {
  case DEM_J1939DTC_PENDING:
    lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PENDING;
    break;
  case DEM_J1939DTC_ACTIVE:
    lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_ACTIVE;
    break;
  case DEM_J1939DTC_PREVIOUSLY_ACTIVE:
    lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE;
    break;
  case DEM_J1939DTC_CURRENTLY_ACTIVE:
    lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_CURRENTLY_ACTIVE;
    break;
  case DEM_J1939DTC_PERMANENT:
    lDTCStatusFilterType = DEM_FILTERDATA_J1939FILTER_PERMANENT;
    break;
  default:
    lDTCStatusFilterType = 0u;
    lReturnValue = E_NOT_OK;
    break;
  }

  if (lReturnValue == E_OK)
  {
    DTCFilterPtr->DTCStatusFilterType = lDTCStatusFilterType;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter()
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter(
  Dem_J1939DcmDTCStatusFilterType  DTCStatusFilter,
  uint8 NodeIndex,
  boolean FilterEmissionRelevantDTCs,
  Dem_EventIdType EventId
)
{
  boolean lEventMatch;
  lEventMatch = FALSE;

  if (Dem_Core_DTC_HasJ1939DTC(EventId) == TRUE)
  {
    if (Dem_DTC_TestSuppressionOnDtcRetrieval(EventId) == FALSE)
    {
      if (Dem_Client_ClientAccess_IsEventAssignedToNode(EventId, NodeIndex) == TRUE)
      {
        uint8 lEventStatus;
        lEventStatus = Dem_DTC_GenerateExternalOnlyStatus(EventId, Dem_DTC_GetDTCStatus(EventId));

        switch (DTCStatusFilter)
        {
        case DEM_FILTERDATA_J1939FILTER_ACTIVE:
          if (Dem_DTC_TestDtcActive(EventId) == TRUE)
          {
            lEventMatch = TRUE;
          }
          break;

        case DEM_FILTERDATA_J1939FILTER_PREVIOUSLY_ACTIVE:
          if (Dem_DTC_TestDtcPreviouslyActive(EventId) == TRUE)
          {
            lEventMatch = TRUE;
          }
          break;

        case DEM_FILTERDATA_J1939FILTER_PENDING:
          if (Dem_UDSStatus_Test_PDTC(lEventStatus) == TRUE)
          {
            lEventMatch = TRUE;
          }
          break;

        case DEM_FILTERDATA_J1939FILTER_CURRENTLY_ACTIVE:
          if (Dem_UDSStatus_Test_TF(lEventStatus) == TRUE)
          {
            lEventMatch = TRUE;
          }
          break;

        case DEM_FILTERDATA_J1939FILTER_PERMANENT:
          if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
          {
            lEventMatch = TRUE;
          }
          break;

        default:                                                                                                                 /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
          /* lEventMatch == FALSE already set */
          break;
        }

        if ( (FilterEmissionRelevantDTCs == TRUE)
          && (Dem_Core_DTC_IsObdRelated(EventId) == FALSE) )
        {
          lEventMatch = FALSE;
        }
      }
    }
  }

  return lEventMatch;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_J1939DTC_Filter_ResetNumberOfDTCsTested()
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_ResetNumberOfDTCsTested(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
)
{
  DTCFilterPtr->NumberOfDTCsTested = 0u;                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_IncrementNumberOfDTCsTested()
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939DTC_Filter_IncrementNumberOfDTCsTested(
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
)
{
  DTCFilterPtr->NumberOfDTCsTested += 1u;                                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_GetNumberOfDTCsTested()
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_J1939DTC_Filter_GetNumberOfDTCsTested(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
)
{
  return DTCFilterPtr->NumberOfDTCsTested;
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_TestFilterPermanentMemory()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_J1939DTC_Filter_TestFilterPermanentMemory(
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr
)
{
  return (boolean) (DTCFilterPtr->MemoryId == DEM_CFG_MEMORYID_PERMANENT);
}


/* ****************************************************************************
 % Dem_FilterData_J1939DTC_GetJ1939ReadDtcRuntimeLimit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_J1939DTC_GetJ1939ReadDtcRuntimeLimit(
  void
)
{
  return DEM_CFG_GLOBAL_J1939_READING_DTC_RUNTIME_LIMIT;
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
 * \addtogroup Dem_J1939DTC_Public
 * \{
 */

/* ****************************************************************************
 % Dem_FilterData_J1939DTC_TestJ1939DTCFilterSupported()
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939DTC_TestJ1939DTCFilterSupported(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCKindType DTCKind,
  Dem_DTCOriginType DTCOrigin
)
{
  boolean lReturnValue;

  /* It is only supported to filter for emission-related DTCs if OBD II is supported */
  if ( (DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS)
    && (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == FALSE) )
  {
    lReturnValue = FALSE;
  }
  /* It is only supported to filter for permanent emission-related DTCs in primary memory */
  else
  if ( (DTCStatusFilter == DEM_J1939DTC_PERMANENT)
    && ( (DTCKind != DEM_DTC_KIND_EMISSION_REL_DTCS) || (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) ) )
  {
    lReturnValue = FALSE;
  }
  /* It is only supported to filter for emission-related DTCs in primary memory */
  else
  if ( (DTCKind == DEM_DTC_KIND_EMISSION_REL_DTCS)
    && (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) )
  {
    lReturnValue = FALSE;
  }
  else
  {
    lReturnValue = TRUE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_J1939DTC_Filter_Init()
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_J1939DTC_Filter_Init(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCOriginType DTCOrigin
)
{
  Std_ReturnType lReturnValue;
  uint16 lTranslatedMemoryId;

  lReturnValue = Dem_FilterData_J1939DTC_TranslateDTCOrigin(DTCOrigin, DTCStatusFilter, &lTranslatedMemoryId);                   /* SBSW_DEM_POINTER_FORWARD_STACK */

  if (lReturnValue == E_OK)
  {
    Dem_J1939DTC_Filter_InitMemoryIdAndIterator(DTCFilterPtr, lTranslatedMemoryId);                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    lReturnValue = Dem_J1939DTC_Filter_InitDTCStatus(DTCFilterPtr, DTCStatusFilter);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    if (lReturnValue == E_OK)
    {
      Dem_J1939DTC_Filter_ResetNumberOfDTCsTested(DTCFilterPtr);                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  return lReturnValue;
}


/* ****************************************************************************
 % Dem_FilterData_J1939DTC_GetNextFilteredJ1939DTC()
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939DTC_GetNextFilteredJ1939DTC(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC) DTCFilterPtr,
  boolean FilterEmissionRelevantDTCs,
  CONSTP2VAR(Dem_EventIdType, AUTOMATIC, AUTOMATIC) EventId)
{
  Std_ReturnType lReturnValue;
  Dem_Cfg_ComplexIterType lEventIter;

  lReturnValue = DEM_NO_SUCH_ELEMENT;

  for (Dem_J1939DTC_EventIter_Init(DTCFilterPtr, &lEventIter);                                                                   /* PRQA S 0771 */ /* MD_DEM_15.4_opt */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_Cfg_ComplexIterExists(&lEventIter) == TRUE);                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterNext(&lEventIter))                                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (Dem_J1939DTC_Filter_GetNumberOfDTCsTested(DTCFilterPtr) < Dem_FilterData_J1939DTC_GetJ1939ReadDtcRuntimeLimit())     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      Dem_EventIdType lEventId;
      lEventId = Dem_J1939DTC_EventIter_GetEventId(DTCFilterPtr, &lEventIter);                                        /* PRQA S 2469 */ /* MD_DEM_14.2_FalsePositive */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      
      if (Dem_Cfg_GetMasterEvent(lEventId) == lEventId)
      {
        Dem_J1939DTC_Filter_IncrementNumberOfDTCsTested(DTCFilterPtr);                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        
        if (lEventId != DEM_EVENT_INVALID)
        {
          if (Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter(DTCFilterPtr->DTCStatusFilterType, DTCFilterPtr->NodeIndex, 
                                                                FilterEmissionRelevantDTCs, lEventId) == TRUE)
          {
            *EventId = lEventId;                                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
            Dem_Cfg_ComplexIterNext(&lEventIter);                                                                                /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
            lReturnValue = E_OK;
            break;
          }
        }
      }
    }
    else
    {
      Dem_J1939DTC_Filter_ResetNumberOfDTCsTested(DTCFilterPtr);                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      lReturnValue = DEM_PENDING;
      break;
    }
  }

  Dem_J1939DTC_Filter_UpdateIterator(DTCFilterPtr, &lEventIter);                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_FilterData_J1939DTC_GetNumberOfFilteredJ1939DTC()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_FilterData_J1939DTC_GetNumberOfFilteredJ1939DTC(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr,
  boolean FilterEmissionRelevantDTCs
)
{
  uint16 lNumberOfFilteredDTC;
  Dem_Cfg_ComplexIterType lEventIter;

  lNumberOfFilteredDTC = 0u;

  for (Dem_J1939DTC_EventIter_Init(DTCFilterPtr, &lEventIter);                                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_Cfg_ComplexIterExists(&lEventIter) == TRUE);                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterNext(&lEventIter))                                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId;
    lEventId = Dem_J1939DTC_EventIter_GetEventId(DTCFilterPtr, &lEventIter);                                                     /* PRQA S 2469 */ /* MD_DEM_14.2_FalsePositive */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ( (lEventId != DEM_EVENT_INVALID)
      && (Dem_Cfg_GetMasterEvent(lEventId) == lEventId)
      && (Dem_FilterData_J1939DTC_TestEventMatchesDTCFilter(DTCFilterPtr->DTCStatusFilterType, DTCFilterPtr->NodeIndex, FilterEmissionRelevantDTCs, lEventId) == TRUE) )
    {
      ++lNumberOfFilteredDTC;
    }
  }

  return lNumberOfFilteredDTC;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939DTC_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939DTC_Implementation.h
 *********************************************************************************************************************/
