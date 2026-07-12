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
 *  \defgroup   Dem_PidStatistics Interface for gathering PIDs
 *  \{
 *  \file       Dem_PidStatistics_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interfaces of the  PidStatistics Logical Unit
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PIDSTATISTICS_INTERFACE_H)
#define DEM_PIDSTATISTICS_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_PidStatistics_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_IsJ1939Readiness2Enabled
 *****************************************************************************/
/*!
 * \brief         Returns True if feature 'J1939 Readiness2' is enabled.
 * \details       -
 *
 * \return        TRUE
 *                Support for 'J1939 Readiness2'
 * \return        FALSE
 *                No support for 'J1939 Readiness2'
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_IsJ1939Readiness2Enabled(
  void
  );

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
 * \defgroup Dem_PidStatistics_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_GetDiagnosticReadiness2()
 *****************************************************************************/
/*!
 * \brief         Calculates the diagnostic readiness 2 value for the given node
 *
 * \details       All PID values above 64255 will be reported as 64255.
 *                This function does not a filter/node index as the data is not
 *                node specific
 *
 * \param[out]    DiagnosticReadiness2
 *                Pointer to receive the diagnostic readiness 2 value.
 *
 *
 * \return        E_OK
 *                Diagnostic readiness 2 was written.
 * \return        E_NOT_OK
 *                Diagnostic readiness 2 was not written, or DM21 is disabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different FilterIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_GetDiagnosticReadiness2(
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness2
  );

#if (DEM_FEATURE_NEED_PID21 == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled with MIL enabled (PID 21)
 *
 * \details       Provides the distance travelled with MIL enabled (PID 21)
 *
 * \param[out]    DestinationBuffer
 *                Pointer to destination buffer, which is filled with PID 21 data.
 *
 * \return        E_OK
 *                Read PID21 successful
 * \return        E_NOT_OK
 *                OBD not supported
 * \config        DEM_FEATURE_NEED_PID21 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );
#endif

#if (DEM_FEATURE_NEED_PID31 == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31
 *****************************************************************************/
/*!
 * \brief         Provides the driven distance since last fault memory clear 
 *                (PID 31)
 *
 * \details       Provides the driven distance since last fault memory clear 
 *                (PID 31)
 *
 * \param[out]    PID31value
 *                Pointer to destination buffer, which is filled with PID 31 data.
 *
 * \return        E_OK
 *                Read PID31 successful
 * \return        E_NOT_OK
 *                OBD not supported
 * \config        DEM_FEATURE_NEED_PID31 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_PIDSTATISTICS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PidStatistics_Interface.h
 *********************************************************************************************************************/
