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
/*!
 *  \defgroup   Dem_Support Support
 *  \{
 *  \file       Dem_Support_Interface.h
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

#if !defined (DEM_SUPPORT_INTERFACE_H)
#define DEM_SUPPORT_INTERFACE_H

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
#include "Dem_Support_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Support_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Support_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position for a Readiness Group in 
 *                DID F501 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position for a Readiness Group in 
 *                DID F501 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DID F501 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DID F501 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted
 *****************************************************************************/
/*!
 * \brief         Set supported and not completed states for event's readiness
 *                group and respective data context.
 *
 * \details       Set supported and not completed states for event's readiness
 *                group and respective data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in,out] DestinationBuffer
 *                Destination buffer with supported bit set and corresponding
 *                completion bit set to not completed for event's readiness
 *                group and respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted(
  uint8 DataContext,
  Dem_EventIdType EventId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates
 *****************************************************************************/
/*!
 * \brief         Set static supported and not completed states for
 *                respective data context.
 *
 * \details       Set static supported and not completed states in 
 *                destination buffer for respective data context from 
 *                generated code, if readiness is not calculated during
 *                run-time. Otherwise set all relevant bytes in buffer to 0.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[out]    DestinationBuffer
 *                Destination buffer with supported bits set and corresponding
 *                completion bits set to not completed for supported
 *                readiness groups and respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Support_SetReadinessGroupDisabled
 *****************************************************************************/
/*!
 * \brief         Disable OBD Readiness Group
 *
 * \details       Sets bit corresponding to readiness group in Pid41DisabledMask
 *                (0 = enabled, 1= disabled)
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupDisabled(
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Support_ConsiderDisabledReadinessGroups
 *****************************************************************************/
/*!
 * \brief         Consider disabled readiness groups for supported states in
 *                PID41.
 *
 * \details       Set all disabled readiness groups in given destination
 *                buffer to not supported.
 *
 * \param[in,out] DestinationBuffer
 *                Destination buffer with supported bits reset.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_ConsiderDisabledReadinessGroups(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_SUPPORT_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Support_Interface.h
 *********************************************************************************************************************/
