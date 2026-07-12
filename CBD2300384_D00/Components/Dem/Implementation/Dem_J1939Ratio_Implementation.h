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
/*! \addtogroup Dem_J1939Ratio
 *  \{
 *  \file       Dem_J1939Ratio_Implementation.h
 *  \brief      Subcomponent for handling requests to filter Ratios.
 *  \details    Subcomponent for handling requests to filter Ratios for J1939 Diagnostic Message (DM) 20.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_J1939RATIO_IMPLEMENTATION_H)
#define DEM_J1939RATIO_IMPLEMENTATION_H

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
#include "Dem_J1939Ratio_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
#include "Dem_Core_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_FilterDataIF_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_J1939RATIO_IMPLEMENTATION_FILENAME "Dem_J1939Ratio_Implementation.h"

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
 * \defgroup Dem_J1939Ratio_PrivateProperties Private Properties
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
 * \addtogroup Dem_J1939Ratio_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_FilterData_J1939Ratio_IsJ1939RatioFilteringEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_J1939Ratio_IsJ1939RatioFilteringEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_RATIO == STD_ON);
}

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
 * \defgroup Dem_J1939Ratio_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_J1939Ratio_SpnIter_GetSpnValue
 *****************************************************************************/
/*!
 * \brief         Get the SPN value of the element pointed to by the iterator.
 *
 * \details       Get the SPN value of the element pointed to by the iterator.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 * \return        SPN Value
 *                The SPN value of the element pointed tp by the iterator.
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_J1939Ratio_SpnIter_GetSpnValue(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_J1939Ratio_SpnIter_Get
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator.
 *
 * \details       Get the element pointed to by the iterator.
 *
 * \param[in]    IterPtr
 *                Pointer to the iterator
 * \return        Spn Handle
 *                Handle to SPN descriptor.
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SpnToEventsWithRatioTableIterType, DEM_CODE)
Dem_J1939Ratio_SpnIter_Get(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_J1939Ratio_SpnEventIter_Init
*****************************************************************************/
/*!
 * \brief         Initialize the iterator
 *
 * \details       Initializes the iterator to enumerate the events of an SPN
 *                with a IUMPR ratio assigned.
 *
 * \param[in]     Spn Handle
 *                Handle to SPN descriptor.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939Ratio_SpnEventIter_Init(
  Dem_Cfg_SpnToEventsWithRatioTableIterType SpnHandle,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_J1939Ratio_SpnEventIter_GetEventId
*****************************************************************************/
/*!
 * \brief         Get the event Id of the element pointed to by the iterator.
 *
 * \details       Get the event Id of the element pointed to by the iterator.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator.
 * \return        Event Id
 *                Id of an event with a IUMPR ratio assigned to the SPN.
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_J1939Ratio_SpnEventIter_GetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FilterData_J1939Ratio_GetWorstRatioOfSpn
 *****************************************************************************/
 /*!
 * \brief         Get the worst ratio assigned to the SPN.
 *
 * \details       Get the worst ratio assigned to the SPN.
 *
 * \param[out]    Spn Handle
 *                Handle to SPN descriptor.
 * \return        Ratio Index
 *                Index to the worst ratio.
 *                DEM_CFG_RATIOINDEX_INVALID if no ratio was found.
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_RatioIdIndexType, DEM_CODE)
Dem_FilterData_J1939Ratio_GetWorstRatioOfSpn(
  Dem_Cfg_SpnToEventsWithRatioTableIterType SpnHandle
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ****************************************************************************
 % Dem_J1939Ratio_SpnIter_GetSpnValue()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_J1939Ratio_SpnIter_GetSpnValue(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (uint32) Dem_Cfg_GetSPNOfSpnToEventsWithRatioTable(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_J1939Ratio_SpnIter_Get()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SpnToEventsWithRatioTableIterType, DEM_CODE)
Dem_J1939Ratio_SpnIter_Get(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (Dem_Cfg_SpnToEventsWithRatioTableIterType) IterPtr->mIdx;
}

/* ****************************************************************************
 % Dem_J1939Ratio_SpnEventIter_Init()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939Ratio_SpnEventIter_Init(
  Dem_Cfg_SpnToEventsWithRatioTableIterType SpnHandle,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SpnHandle);                                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */
  IterPtr->mIdx = Dem_Cfg_GetJ1939EventIdsWithRatioTableStartIdxOfSpnToEventsWithRatioTable(SpnHandle);                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetJ1939EventIdsWithRatioTableEndIdxOfSpnToEventsWithRatioTable(SpnHandle);                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */   
}

/* ****************************************************************************
 % Dem_J1939Ratio_SpnEventIter_GetEventId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_J1939Ratio_SpnEventIter_GetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetEventIdOfJ1939EventIdsWithRatioTable(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_FilterData_J1939Ratio_GetWorstRatioOfSpn()
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_RatioIdIndexType, DEM_CODE)
Dem_FilterData_J1939Ratio_GetWorstRatioOfSpn(
  Dem_Cfg_SpnToEventsWithRatioTableIterType SpnHandle
  )
{
  Dem_Cfg_ComplexIterType lSpnEventIter;
  Dem_RatioIdIndexType lWorstRatio = DEM_CFG_RATIOINDEX_INVALID;

  for (Dem_J1939Ratio_SpnEventIter_Init(SpnHandle, &lSpnEventIter);                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterExists(&lSpnEventIter) == TRUE;                                                                        /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterNext(&lSpnEventIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId = Dem_J1939Ratio_SpnEventIter_GetEventId(&lSpnEventIter);                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if (Dem_Event_TestSuppressionOnEventRetrieval(lEventId) == FALSE)
    {
      uint16 lNumerator;
      uint16 lDenominator;
      Dem_RatioIdIndexType lNextCandidateRatio = Dem_Event_GetRatioIdx(lEventId);

      if (lWorstRatio == DEM_CFG_RATIOINDEX_INVALID)
      {
        /* First available ratio of the SPN was found. No need to compare it. */
        lWorstRatio = lNextCandidateRatio;
      }
      else
      {
        lWorstRatio = Dem_DiagnosticObserver_Iumpr_CompareAndGetWorseObdRatio(
                        lWorstRatio,
                        lNextCandidateRatio);
      }

      Dem_Iumpr_Ratio_GetRatio(lWorstRatio, &lNumerator, &lDenominator);                                                         /* SBSW_DEM_POINTER_FORWARD_STACK */
      if (lDenominator == 0x00u)
      {
        /* The Worst ratio overall was found. Abort the minimum search. */
        break;
      }
    }
  }

  return lWorstRatio;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_J1939Ratio_Public
 * \{
 */

/* ****************************************************************************
 % Dem_J1939Ratio_SpnIter_Init()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_J1939Ratio_SpnIter_Init(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetSizeOfSpnToEventsWithRatioTable();                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_FilterData_J1939Ratio_GetNextFilteredRatio()
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FilterData_J1939Ratio_GetNextFilteredRatio(
  Dem_FilterData_HandleType FilterId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) SPN,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) Numerator,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC) Denominator
  )
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;

  if (Dem_FilterData_J1939Ratio_IsJ1939RatioFilteringEnabled() == TRUE)                                                          /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_Cfg_FilterInfoTablePtrType lFilterDataPtr = Dem_Client_FilterData_GetFilterDataPtrSafe(FilterId);

    for (/* SpnIter already initialized on SetFilter */;
          Dem_Cfg_ComplexIterExists(&lFilterDataPtr->Iterator.SpnIterator) == TRUE;                                              /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_ComplexIterNext(&lFilterDataPtr->Iterator.SpnIterator))                                                        /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_RatioIdIndexType lWorstRatioOfSpn = Dem_FilterData_J1939Ratio_GetWorstRatioOfSpn(Dem_J1939Ratio_SpnIter_Get(
                                                                                            &lFilterDataPtr->Iterator.SpnIterator));/* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (lWorstRatioOfSpn != DEM_CFG_RATIOINDEX_INVALID)
      {
        *SPN = Dem_J1939Ratio_SpnIter_GetSpnValue(&lFilterDataPtr->Iterator.SpnIterator);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */                                                                     
        Dem_Iumpr_Ratio_GetRatio(lWorstRatioOfSpn, Numerator, Denominator);                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        Dem_Cfg_ComplexIterNext(&lFilterDataPtr->Iterator.SpnIterator);                                                          /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
        lReturnValue = E_OK;
        break;
      }
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_J1939RATIO_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_J1939Ratio_Implementation.h
 *********************************************************************************************************************/
