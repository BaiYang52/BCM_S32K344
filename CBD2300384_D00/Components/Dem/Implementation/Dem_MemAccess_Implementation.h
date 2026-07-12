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
/*! \addtogroup Dem_MemAccess
 *  \{
 *  \file       Dem_MemAccess_Implementation.h
 *  \brief      Basic routines for event memories.
 *  \details    In general, returns a memory entry or allows queries concerning the whole event memory
 *              - find free memory entries including displacement and aging
 *              - allocate memory entries
 *              - free event memory entries and trigger removal of event related data (OBD freeze frame, time series & permanent entries).
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMACCESS_IMPLEMENTATION_H)
#define DEM_MEMACCESS_IMPLEMENTATION_H

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
#include "Dem_MemAccess_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_MemoryIF_Interface.h"
#include "Dem_Mem_Interface.h"
#include "Dem_MemState_Interface.h"
#include "Dem_TimeSeries_Interface.h"
#include "Dem_MemObdFreezeFrame_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMACCESS_IMPLEMENTATION_FILENAME "Dem_MemAccess_Implementation.h"

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
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_MemAccess_PublicProperties
 * \{
 */

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
 * \defgroup Dem_MemAccess_Private Private Methods
 * \{
 */

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
 /* ****************************************************************************
 * Dem_Mem_TimeSeriesDisplace_GetNextCandidate
 *****************************************************************************/
 /*!
 * \brief         Get the next time series memory entry candidate for the displacement check.
 *
 * \details       Get the next time series memory entry candidate for the displacement check.
 *
 * \param[out]    CandidateInfo
 *                Buffer that receives the candidate's characteristic data.
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of TimeSeries memory.
 * \param[in]     ChronoIndexIterator
 *                Iterator on the chronology of elements to be displaced.
 *                The iterator must be in range
 *                [1..Dem_MemState_MemoryGetCurrentSize(MemoryInfo)[.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mem_TimeSeriesDisplace_GetNextCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
);
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_GetNextCandidate
 *****************************************************************************/
/*!
 * \brief         Get the next event memory entry candidate for the displacement check.
 *
 * \details       Get the next event memory entry candidate for the displacement check.
 *
 * \param[out]    CandidateInfo
 *                Buffer that receives the candidate's characteristic data.
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of Event memory
 * \param[in]     ChronoIndexIterator
 *                Iterator on the chronology of elements to be displaced.
 *                The iterator must be in range
 *                [1..Dem_MemState_MemoryGetCurrentSize(MemoryInfo)[.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemAccess_Displace_GetNextCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
  );
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_UpdateBestMatch
 *****************************************************************************/
/*!
 * \brief         Update the best match info by the candidate info.
 *
 * \details       Update the best match info by the candidate info.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: The best match updated with the relevant candidate info.
 * \param[in]     CandidateInfo
 *                The candidate info.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemAccess_Displace_UpdateBestMatch(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckInvalidEntry
 *****************************************************************************/
/*!
 * \brief         Check if candidate is an invalid entry.
 *
 * \details       Check if candidate is the invalid entry. If so, it is the
 *                best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckInvalidEntry(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckAgedCandidate
 *****************************************************************************/
/*!
 * \brief         Check if candidate is aged.
 *
 * \details       Check if candidate is aged, so can be displaced immediately.
 *                If so, it is the best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckAgedCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckHasAgingCounter
 *****************************************************************************/
/*!
 * \brief         Check if candidate has an exclusive aging counter.
 *
 * \details       Check if candidate has an exclusive aging counter, so can be
 *                displaced immediately. If so, it is the best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckHasAgingCounter(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckPriority
 *****************************************************************************/
/*!
 * \brief         Check if candidate's priority qualifies the displacement.
 *
 * \details       Check if candidate has an event priority, that qualifies it
 *                or disqualifies it for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 * \return        Dem_MemAccess_Displace_FoundBestMatch
 *                Candidate is the best match for sure.
 *                'BestMatchInfo' is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckPriority(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckObd
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status qualifies the displacement.
 *
 * \details       Check if candidate is passive, and this qualifies it
 *                for the displacement.
 *                Otherwise it needs further tests.
 *                BestMatchInfo's OldestDisplaceableIndex value may be set 
 *                if it is currently undefined.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: OldestDisplaceableIndex value may be set if it is
 *                      currently undefined. Other values are unmodified.
 * \param[in]     CandidateInfo
 *                The candidate to be checked.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 * \return        Dem_MemAccess_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF &&
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckObd(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) )
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckTimeSeries
 *****************************************************************************/
/*!
 * \brief         Check if the readout status of the time series entry candidate
 *                qualifies the displacement.
 *
 * \details       Check if candidate has ongoing readout.
 *                If not, it can be tested further.
 *                BestMatchInfo's OldestDisplaceableIndex value may be set 
 *                if it is currently undefined.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: OldestDisplaceableIndex value may be set if it is
 *                      currently undefined. Other values are unmodified.
 * \param[in]     CandidateInfo
 *                The candidate to be checked.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 * \return        Dem_MemAccess_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF &&
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON &&
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckTimeSeries(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckPassiveState
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status (TestFailed bit) qualifies the
 *                displacement.
 *
 * \details       Check if candidate is passive, and this qualifies it
 *                for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 * \return        Dem_MemAccess_Displace_FoundBestMatch
 *                Candidate is the best match for sure.
 *                'BestMatchInfo' is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckPassiveState(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON))
/* ****************************************************************************
 * Dem_MemAccess_Displace_CheckReadinessState
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status (TNCTOC bit) qualifies the
 *                displacement.
 *
 * \details       Check if candidate's readiness (TNCTOC bit) is a better match
 *                than the current 'best match' for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_MemAccess_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_MemAccess_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckReadinessState(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 * Dem_MemAccess_SelectCustomizedDisplacedIndex
 *****************************************************************************/
/*!
 * \brief         Selects an event to displace in a filled memory
 *
 * \details       Selects an event to displace in a filled memory
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of Event memory.
 * \param[in]     DisplacingEventId
 *                Unique handle of the Event to be entered into the memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The memory index of the displaced memory entry if one could
 *                be identified. DEM_MEM_INVALID_MEMORY_INDEX if displacement
 *                was not possible.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_SelectCustomizedDisplacedIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  DisplacingEventId
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
% Dem_Mem_TimeSeriesDisplace_GetNextCandidate
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Mem_TimeSeriesDisplace_GetNextCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
)
{
  CandidateInfo->MemoryIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, ChronoIndexIterator);                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

  CandidateInfo->EventId = Dem_Memory_TimeSeries_GetEntryPtrSafe(CandidateInfo->MemoryIndex)->EventId;                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  {
    CandidateInfo->MemoryIndexAging = Dem_Memory_FindIndex(CandidateInfo->EventId);                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
# endif
  CandidateInfo->UdsStatus = Dem_DTC_ApplyCombinedStatus(Dem_DTC_GetDTCStatus(CandidateInfo->EventId));                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    CandidateInfo->OldestDisplaceableIndex = DEM_MEM_INVALID_MEMORY_INDEX;                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_GetNextCandidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemAccess_Displace_GetNextCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
  )
{
  CandidateInfo->MemoryIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, ChronoIndexIterator);                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  CandidateInfo->MemoryIndexAging = CandidateInfo->MemoryIndex;                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
  CandidateInfo->EventId = Dem_MemoryEntry_GetEventId(Dem_MemoryEntry_GetId(CandidateInfo->MemoryIndex));                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  CandidateInfo->UdsStatus = Dem_DTC_ApplyCombinedStatus(Dem_DTC_RetrieveStatus(CandidateInfo->EventId));                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  CandidateInfo->OldestDisplaceableIndex = DEM_MEM_INVALID_MEMORY_INDEX;                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_UpdateBestMatch
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemAccess_Displace_UpdateBestMatch(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
)
{
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Cfg_MemoryIndexType  lOldestDisplaceableIndex;
    lOldestDisplaceableIndex = BestMatchInfo->OldestDisplaceableIndex;
    *BestMatchInfo = *CandidateInfo;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    BestMatchInfo->OldestDisplaceableIndex = lOldestDisplaceableIndex;                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
# endif
  {
    *BestMatchInfo = *CandidateInfo;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckInvalidEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckInvalidEntry(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;

  if (Dem_Event_TestValidHandle(CandidateInfo->EventId) == FALSE)
  {
    /* always replace an invalid memory entry */
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_MemAccess_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckAgedCandidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckAgedCandidate(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;

  if (
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      (CandidateInfo->MemoryIndexAging != DEM_MEM_INVALID_MEMORY_INDEX) &&
# endif
      (Dem_DTC_GetAgingCounter(CandidateInfo->EventId, Dem_MemoryEntry_GetId(CandidateInfo->MemoryIndexAging))                   
         == DEM_OPERATIONCYCLE_CYCLECOUNT_AGED))
  { /* aged event can be displaced immediately */
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_MemAccess_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckHasAgingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckHasAgingCounter(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;

  if (
#  if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      (CandidateInfo->MemoryIndexAging != DEM_MEM_INVALID_MEMORY_INDEX) &&
#  endif
      (DEM_MEM_TEST_AGING_ONLY(                                                                                                  /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
         Dem_MemoryEntry_GetState(
           Dem_MemoryEntry_GetId(CandidateInfo->MemoryIndexAging)
      )) == TRUE))
  { /* aging counter can be displaced immediately */
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_MemAccess_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckPriority
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
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckPriority(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;

  if (Dem_Cfg_EventPriority(CandidateInfo->EventId) < Dem_Cfg_EventPriority(BestMatchInfo->EventId))
  {
    /* Never displace events with higher priority */
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
  }
  else
  if (Dem_Cfg_EventPriority(CandidateInfo->EventId) > Dem_Cfg_EventPriority(BestMatchInfo->EventId))
  {
    /* always prefer an event with lowest priority */
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
    /* If configured always use first match */
    lCheckResult = Dem_MemAccess_Displace_FoundBestMatch;
# else
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
# endif
  }
  else
  {
    /* No candidate identified */
    lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;
  }

  return lCheckResult;
}
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckObd
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
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckObd(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,                                            /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BestMatchInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CandidateInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */

  /* Skip OBD displacement algorithms*/
  lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;

# if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
  if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE))
  {
    /* Ignore stored events which are emission related
       AND (having stored the OBD FreezeFrame  OR  are pending  OR  requesting the MIL) */
#  if (DEM_CFG_SUPPORT_OBDII == STD_ON)
    if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
      && (Dem_Mem_FreezeFrameIndexMode02 != Dem_Cfg_ObdFFSlotCountInVariant())
      && (Dem_Mem_FreezeFrameGetEvent(Dem_Mem_FreezeFrameIndexMode02) == CandidateInfo->EventId))                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      /* stored event id is visible in Mode02 -> skip */
      lCheckResult = Dem_MemAccess_Displace_RatingDone;
    }
    else
#  endif
    if ((Dem_Core_DTC_IsObdRelated(CandidateInfo->EventId) == TRUE)
      && (Dem_UDSStatus_Test_PDTC(CandidateInfo->UdsStatus) == TRUE))
    {
      lCheckResult = Dem_MemAccess_Displace_RatingDone;
    }
    else
    if ((Dem_Core_Event_TestMilAssigned(CandidateInfo->EventId) == TRUE)
      && (Dem_UDSStatus_Test_WIR(CandidateInfo->UdsStatus) == TRUE))
    {
      lCheckResult = Dem_MemAccess_Displace_RatingDone;
    }
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
    else
    if (BestMatchInfo->OldestDisplaceableIndex == DEM_MEM_INVALID_MEMORY_INDEX)
    {
      /* no oldest displaceable index identified yet */
      BestMatchInfo->OldestDisplaceableIndex = CandidateInfo->MemoryIndex;                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;
    }
#  endif
    else
    {
      /* MISRA case: lCheckResult is already initialized */
    }
  }
# endif

  return lCheckResult;
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckTimeSeries
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
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckTimeSeries(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,                                            /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BestMatchInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CandidateInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */

  /* Default: Candidate is not classified yet and further test needed on this candidate */
  lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;

  /* Check: Candidate time series entry can not be displaced due to ongoing readout */
  if (Dem_Memory_TimeSeries_TestEntryDisplaceable(CandidateInfo->MemoryIndex) == FALSE)
  {
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
  }
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  else if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == FALSE)
  {
    if (BestMatchInfo->OldestDisplaceableIndex == DEM_MEM_INVALID_MEMORY_INDEX)
    {
      /* no oldest displaceable index identified yet */
      BestMatchInfo->OldestDisplaceableIndex = CandidateInfo->MemoryIndex;                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
# endif
  else
  {
    /* MISRA case: lCheckResult is already initialized */
  }

  return lCheckResult;
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckPassiveState
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
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckPassiveState(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;

# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_OFF)
  if (Dem_UDSStatus_Test_TF(BestMatchInfo->UdsStatus) == FALSE)
  {
    /* If current best candidate is passive, it is a better match as it is older */
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
  }
  else
# endif
  if (Dem_UDSStatus_Test_TF(CandidateInfo->UdsStatus) == FALSE)
  {
    /* did not find a passive event before, so candidate is a better match */
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
    /* If configured always use first match */
    lCheckResult = Dem_MemAccess_Displace_FoundBestMatch;
# else
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
# endif
  }
  else
  {
    /* no candidate identified */
    lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_Displace_CheckReadinessState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_MemAccess_Displace_CheckResultType, DEM_CODE)
Dem_MemAccess_Displace_CheckReadinessState(
  CONSTP2VAR(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_MemAccess_DisplacementInfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_MemAccess_Displace_CheckResultType lCheckResult;

  if ( (Dem_UDSStatus_Test_TNCTOC(BestMatchInfo->UdsStatus) == FALSE)
    && (Dem_UDSStatus_Test_TNCTOC(CandidateInfo->UdsStatus) == TRUE) )                                                           
  {
    Dem_MemAccess_Displace_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
# error "Not supported, if first match displacement is enabled according DPRS"
# else
    lCheckResult = Dem_MemAccess_Displace_RatingDone;
# endif
  }
  else
  {
    /* no candidate identified */
    lCheckResult = Dem_MemAccess_Displace_NeedMoreTests;
  }

  return lCheckResult;
}
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_MemAccess_SelectCustomizedDisplacedIndex
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_SelectCustomizedDisplacedIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  DisplacingEventId                                                                           /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  Dem_Cfg_MemoryIndexType lDisplacedIndex;
  uint8 lChronoIndexIterator;

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  Dem_Cfg_MemoryIndexType lLowestPrioIndex;
  uint8 lLowestPrio;
# endif

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacingEventId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  lLowestPrio = Dem_Cfg_EventPriority(DisplacingEventId);
  lLowestPrioIndex = DEM_MEM_INVALID_MEMORY_INDEX;
# endif

  lDisplacedIndex = DEM_MEM_INVALID_MEMORY_INDEX;

  /*
   * find oldest DTC in Memory (by scanning elements in chronological order, starting with oldest),
   * that has a lower priority (=higher number) than the new DTC
   */

  /* Start with index of the oldest element (0) of the stack */
  for (lChronoIndexIterator = 0u;
       lChronoIndexIterator < Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId);
       ++lChronoIndexIterator)
  {
    Dem_Cfg_MemoryIndexType lMemoryIndex;
    Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    uint8 lStoredPriority;
# endif

    lMemoryIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, lChronoIndexIterator);
    lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    lStoredPriority = Dem_Cfg_EventPriority(Dem_MemoryEntry_GetEventId(lMemoryEntryId));
# endif
    /* Event cannot still be aged and reach here since an event would cross FDC threshold before failing */
    if (DEM_DTC_TEST_PC_STATUS_ADTC(Dem_MemoryEntry_GetPreConfirmedStatus(lMemoryEntryId)) == TRUE)
    {
      /* aged event can be displaced immediately */
      lDisplacedIndex = lMemoryIndex;
      break;
    }
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    else
    if (lStoredPriority > lLowestPrio)
    {
      /* Found oldest event entry with lower priority - remember in case there still is an aged event */
      lLowestPrioIndex = lMemoryIndex;
      lLowestPrio = lStoredPriority;
    }
# endif
    else
    {
      /* Not a candidate for displacement */
    }
  }

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  if (lDisplacedIndex == DEM_MEM_INVALID_MEMORY_INDEX)
  { /* no aged event detected */
    if (lLowestPrioIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    { /* No aged event, but an old, lower prioritized event was found */
      lDisplacedIndex = lLowestPrioIndex;
    }
  }
# endif

  return lDisplacedIndex;
}
#endif

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
 * \addtogroup Dem_MemAccess_Public
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 % Dem_MemAccess_SelectDisplacedIndex
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
 */
DEM_LOCAL FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_SelectDisplacedIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  DisplacingEventId
)
{
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  Dem_MemAccess_DisplacementInfoType lBestMatchInfo;
  Dem_MemAccess_DisplacementInfoType lCandidateInfo;
  Dem_MemAccess_Displace_CheckResultType lCheckResult;
  uint8 lChronoIndexIterator;

  /* Initialize Best Candidate to 'displacement not possible' */
  lBestMatchInfo.EventId = DisplacingEventId;
  lBestMatchInfo.MemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
  lBestMatchInfo.UdsStatus = DEM_UDS_STATUS_TF;
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  lBestMatchInfo.OldestDisplaceableIndex = DEM_MEM_INVALID_MEMORY_INDEX;
#  endif
#  if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  lBestMatchInfo.MemoryIndexAging = DEM_MEM_INVALID_MEMORY_INDEX;
#  endif

  /* find lowest priority DTC in memory. scan in chronological order, so the
   * first found entry is the older one (to break ties) */
  for (lChronoIndexIterator = 0u;                                                                                                /* PRQA S 0771 */ /* MD_DEM_15.4_opt */
    lChronoIndexIterator < Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);
    ++lChronoIndexIterator)
  {
    /* Get next Candidate to be checked */
#  if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
    if (TranslatedMemoryId == DEM_CFG_MEMORYID_TIMESERIES)
    {
      Dem_Mem_TimeSeriesDisplace_GetNextCandidate(&lCandidateInfo, TranslatedMemoryId, lChronoIndexIterator);                    /* SBSW_DEM_POINTER_FORWARD_STACK */
      /* Check: Candidate time series entry can not be displaced due to ongoing readout */
      if (Dem_MemAccess_Displace_CheckTimeSeries(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_RatingDone)         /* SBSW_DEM_POINTER_FORWARD_STACK */
      {
        continue;
      }
    }
    else
#  endif
    {
      Dem_MemAccess_Displace_GetNextCandidate(&lCandidateInfo, TranslatedMemoryId, lChronoIndexIterator);                        /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
    /* Check: Candidate is invalid entry */
    if (Dem_MemAccess_Displace_CheckInvalidEntry(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_FoundBestMatch)     /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* always replace an invalid memory entry */
      break;
    }

    /* Check: Candidate is aged */
#  if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
    if (Dem_MemAccess_Displace_CheckAgedCandidate(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_FoundBestMatch)    /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* aged event can be displaced immediately */
      break;
    }
#  endif /* (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) */

    /* Check: Candidate only holds an exclusive aging counter */
#  if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
    if (Dem_MemAccess_Displace_CheckHasAgingCounter(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_FoundBestMatch)  /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* aging counter can be displaced immediately */
      break;
    }
#  endif /* (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) */

    /* Check: event priorities */
    lCheckResult = Dem_MemAccess_Displace_CheckPriority(&lBestMatchInfo, &lCandidateInfo);                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
    if (lCheckResult == Dem_MemAccess_Displace_RatingDone)
    {
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
      (void)Dem_MemAccess_Displace_CheckObd(&lBestMatchInfo, &lCandidateInfo);                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
#  endif
      continue;                                                                                                                  
    }
    else if (lCheckResult == Dem_MemAccess_Displace_FoundBestMatch)
    {
      break;
    }
    else
    {
      /* check next category */
    }

    /* Check: OBD enabled */
    if (Dem_MemAccess_Displace_CheckObd(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_RatingDone)                /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      continue;                                                                                                                  
    }

    /* Check: consider passive for displacement */
#  if (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON)
    lCheckResult = Dem_MemAccess_Displace_CheckPassiveState(&lBestMatchInfo, &lCandidateInfo);                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
    if (lCheckResult == Dem_MemAccess_Displace_RatingDone)
    {
      continue;                                                                                                                  
    }
    else if (lCheckResult == Dem_MemAccess_Displace_FoundBestMatch)
    {
      break;
    }
    else
    {
      /* check next category */
    }
#  endif /* DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON */

    /* Check: consider readiness for displacement */
#  if (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON)
    if (Dem_MemAccess_Displace_CheckReadinessState(&lBestMatchInfo, &lCandidateInfo) == Dem_MemAccess_Displace_RatingDone)       /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      continue;                                                                                                                  
    }
#  endif /* (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON) */
  }

  /* When no match found - use fallback if configured */
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (lBestMatchInfo.EventId == DisplacingEventId)
  {
    /* The oldest event can be always displaced (could be DEM_MEM_INVALID_MEMORY_INDEX!) */
    if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
#   if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      || (TranslatedMemoryId == DEM_CFG_MEMORYID_TIMESERIES)
#   endif
      )
    {
      lBestMatchInfo.MemoryIndex = lBestMatchInfo.OldestDisplaceableIndex;
    }
    else
    {
      lBestMatchInfo.MemoryIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, 0);
    }
  }
#  endif /* (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON) */

  return lBestMatchInfo.MemoryIndex;
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacingEventId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return DEM_MEM_INVALID_MEMORY_INDEX; /* Always failed */
# endif

}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080  */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)*/

/* ****************************************************************************
 % Dem_MemAccess_MemoryAllocateIndex
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_MemoryAllocateIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_MemoryIndexType lMemoryIndex;
  Dem_EventIdType lMemoryRepresentativeEvent;

  lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

  if (Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId) == Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId))
  { /* Memory is filled up */
    Dem_Memory_MemState_SetOverflow(Dem_Cfg_EventDestination(EventId));

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING != STD_ON)
    lMemoryIndex = Dem_MemAccess_SelectDisplacedIndex(TranslatedMemoryId, EventId);
#else
    lMemoryIndex = Dem_MemAccess_SelectCustomizedDisplacedIndex(TranslatedMemoryId, EventId);
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    if (lMemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
      /* a displaced DTC was identified, now remove that DTC */
      Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;
      Dem_EventIdType lDisplacedEventId;

      lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);
      lDisplacedEventId = Dem_MemoryEntry_GetEventId(lMemoryEntryId);

      if (Dem_Event_TestValidHandle(lDisplacedEventId) == TRUE)
      {
        Dem_Event_DataUpdateStart(lDisplacedEventId);
# if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
        Dem_DTC_CustomizedDisplacement(lDisplacedEventId, Dem_MemoryEntry_GetPreConfirmedStatus(lMemoryEntryId));
# else
        Dem_DTC_Displaced(lDisplacedEventId);
# endif

        Dem_Memory_TimeSeries_InvalidateEntryOfEvent(lDisplacedEventId, DEM_NVM_BLOCKSTATE_DIRTY);

        if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
        {
          Dem_MemObdFreezeFrame_EventDisplaced(lDisplacedEventId);
        }
        Dem_Event_DataUpdateFinish(lDisplacedEventId);
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
        if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
        {
          Dem_Data_PermanentEntryClear(lDisplacedEventId, FALSE);
        }
# endif
      }
      Dem_MemAccess_MemoryFreeIndex(TranslatedMemoryId, lMemoryIndex);
    }
    /* else: no adequate DTC found, the new entry cannot be added - return an invalid index */
#endif
  }
  else
  { /* primary memory is not full */
    /* Memory is not full -> Find first free entry */
    lMemoryIndex = Dem_MemAccess_MemoryFindIndex(TranslatedMemoryId, DEM_EVENT_INVALID);
    /* if not found : lMemoryIndex == DEM_MEM_INVALID_MEMORY_INDEX */
  }

  if (lMemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
  {
    uint8 lMemorySize;
    Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;

    lMemorySize = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);
    lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);

    /* At this point, the event memory is not completely filled */
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (lMemorySize >= Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId))
    {
      Dem_Error_RunTimeCheckFailed(DEM_MEMACCESS_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      lMemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    }
    else
#endif
    {
      /* fill the chrono record with event id and lock the referenced PriMem slot */
      Dem_MemState_MemorySetChronology(TranslatedMemoryId, lMemorySize, lMemoryIndex);

      Dem_MemoryEntry_SetEventId(lMemoryEntryId, lMemoryRepresentativeEvent);
      Dem_MemoryEntry_SetTimestamp(lMemoryEntryId, Dem_MemState_GetCurrentTimestamp());

      Dem_MemoryEntry_InitializeEntry(lMemoryEntryId);
      Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);
      Dem_MemState_MemorySetCurrentSize(TranslatedMemoryId, (uint8)(lMemorySize + 1u));
    }
  }

  return lMemoryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) \
 &&  (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON))
/* ****************************************************************************
 % Dem_MemAccess_MemoryAllocateAging
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_MemoryAllocateAging(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_MemoryIndexType lMemoryIndex;
  Dem_EventIdType lMemoryRepresentativeEvent;

  lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

  /* Find empty slot if possible, otherwise remove aged slot */
  if (Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId) == Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId))
  { /* Memory is filled up -
     * Aged environmental data is not displaced, the remaining slots (if any)
     * are still used -> no chance to allocate an aging counter */
    lMemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
  }
  else
  { /* Memory is not full -> Find first free entry */
    lMemoryIndex = Dem_MemAccess_MemoryFindIndex(TranslatedMemoryId, DEM_EVENT_INVALID);
    /* if not found : lMemoryIndex == DEM_MEM_INVALID_MEMORY_INDEX */
  }

  if (lMemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
  {
    uint8 lMemorySize;

    lMemorySize = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);

    /* At this point, the event memory is not completely filled */
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (lMemorySize >= Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId))
    {
      Dem_Error_RunTimeCheckFailed(DEM_MEMACCESS_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      lMemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    }
    else
# endif
    {
      Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;
      lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);
      /* fill the chrono record with event id and lock the referenced PriMem slot */
      Dem_MemoryEntry_SetTimestamp(lMemoryEntryId, Dem_MemState_GetCurrentTimestamp());
      Dem_MemState_MemorySetChronology(TranslatedMemoryId, lMemorySize, lMemoryIndex);

      Dem_MemoryEntry_SetEventId(lMemoryEntryId, lMemoryRepresentativeEvent);

      /* Data */
      Dem_MemoryEntry_InitializeEntry(lMemoryEntryId);
      Dem_MemoryEntry_SetState(lMemoryEntryId, DEM_MEM_SET_AGING_ONLY(0u));

      Dem_MemState_SetCurrentTimestamp(Dem_MemState_GetCurrentTimestamp() + 1u);
      Dem_MemState_MemorySetCurrentSize(TranslatedMemoryId, (uint8)(lMemorySize + 1u));
    }
  }

  return lMemoryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) */

/* ****************************************************************************
 % Dem_MemAccess_MemoryFreeIndex
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
Dem_MemAccess_MemoryFreeIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  Dem_Cfg_MemoryIndexType lTempChronoIndex;
  uint8 lChronoIterator;

  /* reduce count of used elements in the chrono stack */
  Dem_Internal_AssertReturnVoid(
    MemoryIndex - Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId)
    < Dem_MemState_MemoryGetMaxSize(TranslatedMemoryId), DEM_E_INCONSISTENT_STATE)

    lChronoIterator = Dem_MemState_MemoryGetCurrentSize(TranslatedMemoryId);
  --lChronoIterator;

  Dem_MemState_MemorySetCurrentSize(TranslatedMemoryId, lChronoIterator);
  Dem_FaultMemory_Memory_FreeEntry(TranslatedMemoryId, MemoryIndex);

  /* go backwards through the chronological list and move down all entries, until the deleted element is overwritten */
  lTempChronoIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, lChronoIterator);
  while (lTempChronoIndex != MemoryIndex)
  {
    Dem_Cfg_MemoryIndexType lSwap;

    lSwap = lTempChronoIndex;

    --lChronoIterator;
    lTempChronoIndex = Dem_MemState_MemoryGetChronology(TranslatedMemoryId, lChronoIterator);
    Dem_MemState_MemorySetChronology(TranslatedMemoryId, lChronoIterator, lSwap);

#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if ((lChronoIterator == 0u) && (lTempChronoIndex != MemoryIndex))
    {
      /* Index not found, chronology is corrupted */
      Dem_Error_RunTimeCheckFailed(DEM_MEMACCESS_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      break;
    }
#endif
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_MemAccess_FindMemoryIndex_StoredEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_FindMemoryIndex_StoredEvent(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_MemoryIndexType lMemoryIndex;

  lMemoryIndex = Dem_MemAccess_MemoryFindIndex(TranslatedMemoryId, EventId);

#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  /* Cannot happen except after the Dem state becomes inconsistent */
  if (lMemoryIndex == DEM_MEM_INVALID_MEMORY_INDEX)
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMACCESS_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
#endif

  return lMemoryIndex;
}

/* ****************************************************************************
 % Dem_MemAccess_MemoryFindIndex
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
DEM_LOCAL FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_MemAccess_MemoryFindIndex(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_MemoryIndexType lMemoryIndex;

  if (TranslatedMemoryId >= DEM_CFG_MEMORYID_INVALID)
  {
    lMemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
  }
  else
  {
    Dem_EventIdType lMemoryRepresentativeEvent;
    Dem_Cfg_MemoryIndexType lMemoryIndexEnd;

    lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

    /* Iteration direction must be identical to Dem_Init! */
    lMemoryIndex = Dem_MemState_MemoryGetFirstIndex(TranslatedMemoryId);
    lMemoryIndexEnd = Dem_MemState_MemoryGetEndIndex(TranslatedMemoryId);

    for (; (lMemoryIndex < lMemoryIndexEnd); ++lMemoryIndex) 
    {  
      if (Dem_FaultMemory_Memory_TestEntryforEvent(TranslatedMemoryId, lMemoryIndex, lMemoryRepresentativeEvent) == TRUE)
      {
        break;
      }
    }

    /* if not found, return DEM_MEM_INVALID_MEMORY_INDEX */
    if (lMemoryIndex >= lMemoryIndexEnd)
    {
      lMemoryIndex = DEM_MEM_INVALID_MEMORY_INDEX;
    }
  }

  return lMemoryIndex;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMACCESS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemAccess_Implementation.h
 *********************************************************************************************************************/
