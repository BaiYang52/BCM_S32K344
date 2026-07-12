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
 *  \addtogroup Dem_PidStatistics
 *  \{
 *  \file       Dem_PidStatistics_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of PidStatistics Logical Unit for gathering PIDs.
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PIDSTATISTICS_IMPLEMENTATION_H)
#define DEM_PIDSTATISTICS_IMPLEMENTATION_H

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

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_PidStatistics_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_GlobalState_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Client_Interface.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_PIDSTATISTICS_IMPLEMENTATION_FILENAME "Dem_PidStatistics_Implementation.h"

/*! Maximum PID value for Diagnostic Readiness 2 (DM21) */
#define DEM_PIDSTATISTICS_DM21_MAX_PID_VALUE                          ((uint16)64255U)

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
* \defgroup Dem_PidStatistics_PublicProperties Public Properties
* \{
*/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_IsJ1939Readiness2Enabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_IsJ1939Readiness2Enabled(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_READINESS2 == STD_ON);
}

/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_PidStatistics_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_ExtractPID()
 *****************************************************************************/
/*!
 * \brief         Extracts the PID from the array
 *
 * \details       Extracts the PID from the array
 *
 * \param[in]     PIDarray
 *                The array in which the PID is stored
 *
 * \return        The value of the PID
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ExtractPID(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) PIDarray 
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PidStatistics_LatchPID()
 *****************************************************************************/
/*!
 * \brief         Latches the given PID at '64255'
 *
 * \details       For PIDs, all values above '64255' shall be reported as
 *                '64255', according to AUTOSAR and J1939-73
 *
 * \param[in]     PID
 *                The PID that shall be latched
 *
 * \return        The latched value of the PID
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_LatchPID(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 PID
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
*  UNIT PRIVATE FUNCTION DEFINITIONS
*********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_PidStatistics_PrivateProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_ExtractPID()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ExtractPID(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) PIDarray
  )
{
  uint8 lHighByte = PIDarray[0];
  uint8 lLowByte = PIDarray[1];
  uint16 lPid = Dem_Make16Bit(lHighByte, lLowByte);

  return lPid;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_LatchPID()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_LatchPID(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 PID
  )
{
  uint16 lPid = PID;
  if (lPid > DEM_PIDSTATISTICS_DM21_MAX_PID_VALUE)
  { /* latch at 64255 */
    lPid = DEM_PIDSTATISTICS_DM21_MAX_PID_VALUE;
  }
  return lPid;
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
 * \addtogroup Dem_PidStatistics_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_GetDiagnosticReadiness2
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
Dem_GlobalDiagnostics_PidStatistics_GetDiagnosticReadiness2(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness2                           /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_GlobalDiagnostics_PidStatistics_IsJ1939Readiness2Enabled() == TRUE)                                                    /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
#if (DEM_CFG_SUPPORT_J1939_READINESS2 == STD_ON)
    {
      /* Read data of PID21 and latch it */
      uint8 PID21value[2] = {0};
      Dem_Data_DestinationBuffer lDestinationBuffer;
      Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID21value[0]), 0x02);                                               /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
      (void)Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21(&lDestinationBuffer);                                            /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
      DiagnosticReadiness2->DistanceTraveledWhileMILisActivated                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        = Dem_GlobalDiagnostics_PidStatistics_LatchPID(Dem_GlobalDiagnostics_PidStatistics_ExtractPID(&PID21value[0]));          /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
    {
      /* Read data of PID31 and latch it */
      uint8 PID31value[2] = {0};
      Dem_Data_DestinationBuffer lDestinationBuffer;
      Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID31value[0]), 0x02);                                               /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
      (void)Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31(&lDestinationBuffer);                                            /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
      DiagnosticReadiness2->DistanceSinceDTCsCleared                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        = Dem_GlobalDiagnostics_PidStatistics_LatchPID(Dem_GlobalDiagnostics_PidStatistics_ExtractPID(&PID31value[0]));          /* SBSW_DEM_POINTER_FORWARD_STACK */
    }

    DiagnosticReadiness2->MinutesRunbyEngineWhileMILisActivated 
      = Dem_GlobalDiagnostics_PidStatistics_LatchPID(Dem_Mem_GetEngineTimeMilOn());                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    DiagnosticReadiness2->TimeSinceDiagnosticTroubleCodesCleared
      = Dem_GlobalDiagnostics_PidStatistics_LatchPID(Dem_Mem_GetEngineTimeSinceClear());                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif

    lReturnValue = E_OK;
  }

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DiagnosticReadiness2)                                                                         /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lReturnValue;
}

#if (DEM_FEATURE_NEED_PID21 == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21
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
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID21(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  Std_ReturnType lReturn;
  uint32  lOdometerValue;
  uint32  lStoredOdometerValue;

  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    /* ----- Implementation ------------------------------------------------ */
    /* If the Distance travelled with MIL on is latched due to inactive MIL, the mileage can be taken directly.
    If the MIL is active, the difference between the stored value and the current odometer value is calculated */

/* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();
    lStoredOdometerValue = Dem_Mem_GetOdometerMilOn();
    Dem_LeaveCritical_NonAtomic32bit();
/* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */

    if (DEM_MEM_TEST_ODOMETER_ISINVALID(lStoredOdometerValue) == FALSE)
    {
      if (Dem_GlobalState_Indicator_TestMilReportedActive() == TRUE)
      { /* MIL is considered active, get current Odometer value */
        if (Dem_Data_GetObdOdometer(&lOdometerValue) != E_OK)                                                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
        { /* application currently can't provide a valid odometer */
          lOdometerValue = DEM_DATA_MAX_OBD_ODOMETER;
        }
        {
          lOdometerValue = lOdometerValue - lStoredOdometerValue;
        }
      }
      else
      { /* MIL is considered inactive */
        lOdometerValue = lStoredOdometerValue;
      }
      /* The Odometer shall be latched at 0xFFFF */
      if (lOdometerValue > DEM_DATA_MAX_OBD_ODOMETER)
      {
        lOdometerValue = DEM_DATA_MAX_OBD_ODOMETER;
      }
    }
    else
    { /* currently no odometer available */
      lOdometerValue = DEM_DATA_MAX_OBD_ODOMETER;
    }

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, 0x00, Dem_GetLoHiByte(lOdometerValue));                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, 0x01, Dem_GetLoLoByte(lOdometerValue));                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}
#endif

#if (DEM_FEATURE_NEED_PID31 == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31
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
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_PidStatistics_ReadDataOfPID31(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  Std_ReturnType lReturn;
  uint32 lCurrentOdometer;
  uint32 lLastOdometer;

  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    /* ----- Implementation ------------------------------------------------ */
    lCurrentOdometer = 0u;

    /* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
    Dem_EnterCritical_NonAtomic32bit();

    lLastOdometer = Dem_Mem_GetOdometerAtClear();

    Dem_LeaveCritical_NonAtomic32bit();
    /* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */

    if (DEM_MEM_TEST_ODOMETER_ISINVALID(lLastOdometer) == FALSE)
    {
      if (Dem_Data_GetObdOdometer(&lCurrentOdometer) == E_OK)                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
      { /* Application returned valid odometer value, validate that returned
           odometer is greater than stored odometer from last clear */
        if (lCurrentOdometer >= lLastOdometer)
        { /* use lCurrentOdometer variable for delta result */
          lCurrentOdometer = lCurrentOdometer - lLastOdometer;

          /* reported odometer value shall be latched at 0xffff */
          if (lCurrentOdometer > DEM_DATA_MAX_OBD_ODOMETER)
          {
            lCurrentOdometer = DEM_DATA_MAX_OBD_ODOMETER;
          }
        }
        else
        { /* returned odometer is smaller than stored odometer */
          lCurrentOdometer = DEM_DATA_MAX_OBD_ODOMETER;
        }
      }
      else
      { /* Application can not provide a valid odometer value, set maximum value */
        lCurrentOdometer = DEM_DATA_MAX_OBD_ODOMETER;
      }
    }
    else
    { /* currently no odometer available */
      lCurrentOdometer = DEM_DATA_MAX_OBD_ODOMETER;
    }

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, 0x00, Dem_GetLoHiByte(lCurrentOdometer));                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, 0x01, Dem_GetLoLoByte(lCurrentOdometer));                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_PIDSTATISTICS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PidStatistics_Implementation.h
 *********************************************************************************************************************/
