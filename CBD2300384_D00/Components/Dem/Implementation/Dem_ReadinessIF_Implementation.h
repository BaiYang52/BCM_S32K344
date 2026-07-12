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
/*! \addtogroup Dem_ReadinessIF
 *  \{
 *  \file       Dem_ReadinessIF_Implementation.h
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

#if !defined (DEM_READINESSIF_IMPLEMENTATION_H)
#define DEM_READINESSIF_IMPLEMENTATION_H

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
#include "Dem_ReadinessIF_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_Core_Interface.h"
#include "Dem_Completion_Interface.h"
#include "Dem_Support_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalState_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Client_Interface.h"
                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_READINESSIF_IMPLEMENTATION_FILENAME "Dem_ReadinessIF_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ReadinessIF_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_ReadinessIF_PublicProperties
 * \{
 */

 /* ****************************************************************************
   % Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported
  *****************************************************************************/
  /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported(
  uint8 DataContext
)
{
  boolean lIsCalculationSwitchOn;

  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PIDF501_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_DM05:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_DM05_CALCULATION == STD_ON);
    break;
  case DEM_READINESS_DATACONTEXT_PID41:
    lIsCalculationSwitchOn = (boolean)(DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON);
    break;
  default:
    lIsCalculationSwitchOn = FALSE;
    break;
  }
  return lIsCalculationSwitchOn;
}

 /* ****************************************************************************
  % Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported
  *****************************************************************************/
  /*!
   * Internal comment removed.
 *
 *
   */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_MAJOR_MONITORS == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported(
  void
)
{
  return DEM_CFG_SUPPORT_IGNITION_COMPRESSION == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported(
  void
)
{
  return DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInPID41
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInPID41(
  void
)
{
  return DEM_CFG_PROCESS_PID41_COMPREHENSIVE_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501(                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  boolean lReturnValue = TRUE;

  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    lReturnValue = (boolean)(DEM_CFG_PROCESS_PIDF501_COMPREHENSIVE_COMPLETE == STD_ON);
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetPid41DisabledMask
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetPid41DisabledMask(
  uint16 Pid41DisabledMaskValue
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Pid41DisabledMaskValue)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON)
    Dem_Cfg_AdminData.Pid41DisabledMask = Pid41DisabledMaskValue;
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ReadinessIF_Private Private Methods
 * \{
 */

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_CalcByteA
 *****************************************************************************/
 /*!
 * \brief         Calculates Byte A value for PID 01 and DID F501.
 *
 * \details       Calculates Byte A value for PID 01 and DID F501.
 *
 * \param[in]     NumConfirmedEvents
 *                Number of Confirmed Events
 * \return        Formatted PID01 or DID501 Byte A data
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalcByteA(
  uint16 NumConfirmedEvents
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_CalculateSupportedStatesAndConfirmedDTCCount
 *****************************************************************************/
/*!
 * \brief         Get calculated supported states and confirmed DTC count
 *                for respective data context.
 *
 * \details       Get calculated supported states and confirmed DTC count
 *                for respective data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, DID F501, DM05.
 *                
 * \param[out]    DestinationBuffer
 *                Destination buffer with supported bits set and corresponding
 *                completion bits set to not completed for respective
 *                data context.
 *
 * \return        Number of OBD relevant DTCs which are confirmed (only relevant
 *                for data contexts PID01 and DID F501 and legislations OBDII
 *                and OBDonUDS).
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalculateSupportedStatesAndConfirmedDTCCount(
  uint8 DataContext, 
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount
 *****************************************************************************/
/*!
 * \brief         Get supported states and confirmed DTC count for respective
 *                data context.
 *
 * \details       Get supported states and confirmed DTC count for respective
 *                data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, DID F501, DM05.
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer with supported bits set and corresponding
 *                completion bits set to not completed for respective
 *                data context.
 *
 * \return        Number of OBD relevant DTCs which are confirmed (only relevant
 *                for data contexts PID01 and DID F501 and legislations OBDII
 *                and OBDonUDS).
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_SetStaticCompletionMask
 *****************************************************************************/
/*!
 * \brief         Set static completion mask for respective data context.
 *
 * \details       Write static completion mask to data buffer for respective
 *                data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, DID F501, DM05.
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer with completion bits or static values
 *                set for respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetStaticCompletionMask(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues
 *****************************************************************************/
/*!
 * \brief         Set completion states or static values in data buffer.
 *
 * \details       -
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, DID F501, DM05.
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer with completion bits or static values 
 *                set for respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_CalcByteA
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
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalcByteA(
  uint16 NumConfirmedEvents
)
{
  uint8 ByteA;
  uint16 lNumConfirmedEvents; 
  lNumConfirmedEvents = NumConfirmedEvents;

  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant())
  {
    /* Number of OBD relevant confirmed events */
    uint8 lGlobalMilState;
    if (lNumConfirmedEvents > 0x7fU)
    {
      lNumConfirmedEvents = 0x7fU;
    }

    /* Get MIL state */
    if (Dem_GlobalState_Indicator_TestMilReportedActive() == TRUE)
    {
      lGlobalMilState = 0x80U;
    }
    else
    {
      lGlobalMilState = 0x00U;
    }

    /* Combine into the output variable */
    ByteA = (uint8)((lGlobalMilState | (uint8)lNumConfirmedEvents));                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
  }
  else
  {
    ByteA = 0x00u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
  }

  return ByteA;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_CalculateSupportedStatesAndConfirmedDTCCount
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
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalculateSupportedStatesAndConfirmedDTCCount(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint16 lConfirmedEvents;
  Dem_EventIdType lEventId;

  lConfirmedEvents = 0u;

  for (lEventId = Dem_MemState_MemoryGetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
    lEventId <= Dem_MemState_MemoryGetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
    lEventId++)
  {
    if ( (Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported() == TRUE)
      || ( (Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501() == FALSE) 
        && (DataContext == DEM_READINESS_DATACONTEXT_DIDF501) )
       )
    {
      Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted(DataContext, lEventId, DestinationBuffer);                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }

    /* Confirmed DTC count is only needed for PID01 and DID F501 and legislations OBD-II and ObdOnUds */
    if ( (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      && ( (DataContext == DEM_READINESS_DATACONTEXT_PID01) || (DataContext == DEM_READINESS_DATACONTEXT_DIDF501) )
      )
    {
      /* count event if it is OBD related and confirmed bit is qualified */
      if ( (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
        && (lEventId == Dem_Cfg_GetMasterEvent(lEventId) ) )
      {
        uint8 lDtcStatus;
        lDtcStatus = Dem_DTC_GenerateExternalOnlyStatus(lEventId, Dem_DTC_GetDTCStatus(lEventId));
        if (Dem_UDSStatus_Test_CDTC(lDtcStatus) == TRUE)
        {
          lConfirmedEvents++;
        }
      }
    }
  }
  return lConfirmedEvents;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint16 lConfirmedEvents;

  Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates(DataContext, DestinationBuffer);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

  lConfirmedEvents = Dem_GlobalDiagnostics_Readiness_CalculateSupportedStatesAndConfirmedDTCCount(DataContext, DestinationBuffer); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

  return lConfirmedEvents;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetStaticCompletionMask
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetStaticCompletionMask(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID01_BYTEB, DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  case DEM_READINESS_DATACONTEXT_DIDF501:
    /* Set remaining bytes in DidF501 to static values (Byte B = 0x04, Byte C = 0x00, Byte D = 0x00) */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEB, DEM_READINESS_DIDF501_SUPPORT_CMPRCMPT_MASK); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  case DEM_READINESS_DATACONTEXT_DM05:
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE4, DEM_READINESS_DM05_SUPPORT_CMPRCMPT_MASK);  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported(DataContext) == TRUE)
  {
    Dem_Readiness_Completion_SetCompletionStates(DataContext, DestinationBuffer);                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    Dem_GlobalDiagnostics_Readiness_SetStaticCompletionMask(DataContext, DestinationBuffer);                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
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
 * \addtogroup Dem_ReadinessIF_Public
 * \{
 */

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_ReadDataOfPID01
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
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_ReadDataOfPID01(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_SHARED_DATA)  PID01value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturn;
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
{
    Dem_Data_DestinationBuffer lDestinationBuffer;
    uint16 lNumConfirmedEvents;

    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID01value[0]), BufferSize);                                           /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

    /* Set support values of Byte B-F and receive confirmed DTC count */
    lNumConfirmedEvents = Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount(                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
      DEM_READINESS_DATACONTEXT_PID01, &lDestinationBuffer);

    /* Set completion values of Byte B-F */
    Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues(DEM_READINESS_DATACONTEXT_PID01, &lDestinationBuffer);     /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    /* Set Byte A value */
    Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer,                                                                  /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
                                           DEM_READINESS_PID01_BYTEA, 
                                           Dem_GlobalDiagnostics_Readiness_CalcByteA(lNumConfirmedEvents));
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}


 /* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_ReadDataOfDIDF501
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_ReadDataOfDIDF501(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  DIDF501value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturn;
  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;
    uint16 lNumConfirmedEvents;

    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(DIDF501value[0]), BufferSize);                                         /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */                
     
    /* Set support values of Byte B-F and receive confirmed DTC count */
    lNumConfirmedEvents = Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount(                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
      DEM_READINESS_DATACONTEXT_DIDF501, &lDestinationBuffer);

    /* Set completion values of Byte B-F */
    Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues(DEM_READINESS_DATACONTEXT_DIDF501, &lDestinationBuffer);   /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
     
    /* Set Byte A value */
    Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer,                                                                  /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
                                           DEM_READINESS_DIDF501_BYTEA, 
                                           Dem_GlobalDiagnostics_Readiness_CalcByteA(lNumConfirmedEvents));  
    lReturn = E_OK;
  }
  else
  {
    lReturn = E_NOT_OK;
  }
  return lReturn;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_ReadDataOfDM05
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
Dem_GlobalDiagnostics_Readiness_ReadDataOfDM05(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  /* Proceed with readiness calculation if OBDII is supported */
  if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
  {
    /* Get support states of readiness groups and set bytes in DM05 (0: not supported, 1: supported) */
    (void) Dem_GlobalDiagnostics_Readiness_GetSupportedStatesAndConfirmedDTCCount(DEM_READINESS_DATACONTEXT_DM05, DestinationBuffer); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    /* Get completion values of Byte 4-8 */
    /* Get completed state of readiness groups and set value of remaining bytes in DM05 (completed = 0, not completed = 1u) */
    Dem_GlobalDiagnostics_Readiness_SetCompletionStatesOrStaticValues(DEM_READINESS_DATACONTEXT_DM05, DestinationBuffer);        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_ReadDataOfPID41
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_ReadDataOfPID41(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value,
  uint8 BufferSize
)
{
  Std_ReturnType lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BufferSize)                                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */                                                                                                        
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    Dem_Data_DestinationBuffer lDestinationBuffer;

    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(PID41value[0]), BufferSize);                                           /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
    /* reserved */
    Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer, DEM_READINESS_PID41_BYTEA, 0x00u);                               /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates(DEM_READINESS_DATACONTEXT_PID41, &lDestinationBuffer);         /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    if (Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_PID41) == TRUE)
    {
      Dem_Data_DestinationBuffer lCompletionMaskBuffer;
      Dem_EventIdType lEventId;
      uint8 lBytePos;
      /* Set all readiness groups to supported and not completed
      (Not supported/ completed = 0, Supported/ not completed = 1u)  */
      uint8 lTempCompletedMask[DEM_READINESS_PID41_SIZE] = { 0xFFu , 0xFFu, 0xFFu, 0xFFu };

      Dem_Data_InitDestinationBuffer(&lCompletionMaskBuffer, &(lTempCompletedMask[0]), DEM_READINESS_PID41_SIZE);                /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

      for (lEventId = Dem_MemState_MemoryGetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
           lEventId <= Dem_MemState_MemoryGetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
           lEventId++)
      {
        Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted(DEM_READINESS_DATACONTEXT_PID41, lEventId, &lDestinationBuffer); /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
        Dem_Readiness_Completion_ConsiderEventStatusForCompletionStates(lEventId,                                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
          &lCompletionMaskBuffer,
          &lDestinationBuffer);
      }

      Dem_Readiness_Support_ConsiderDisabledReadinessGroups(&lDestinationBuffer);                                                /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

      Dem_Readiness_Completion_SetStaticCompletionStates(DEM_READINESS_DATACONTEXT_PID41, &lDestinationBuffer);                  /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

      /* If readiness group in CompletedMask is completed set readiness group to completed -> (completed = 0, not completed = 1u) */
      /* Byte A is reserved -> copy only values of other bytes to destination buffer */
      for (lBytePos = 1u; lBytePos < DEM_READINESS_PID41_SIZE; lBytePos++)
      {
        Dem_Data_WriteDestinationBufferAtIndex(&lDestinationBuffer,                                                              /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
          lBytePos,
          lDestinationBuffer.Buffer[lBytePos] & lCompletionMaskBuffer.Buffer[lBytePos]);
      }
    }
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Readiness_SetEventDisabled
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetEventDisabled(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  Std_ReturnType lReturnValue;
  if ( (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    && (Dem_Core_Event_GetEventReadinessGroup(EventId) != DEM_CFG_READINESS_NONE)
    && (Dem_Event_TestEventUnavailable(EventId) == FALSE)                                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    )
  {
    Dem_Readiness_Support_SetReadinessGroupDisabled(Dem_Core_Event_GetEventReadinessGroup(EventId));
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_READINESSIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ReadinessIF_Implementation.h
 *********************************************************************************************************************/
