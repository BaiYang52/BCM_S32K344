/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Icu.c
 *      Project:  DrvIcu_CanoeemuCanoe01Asr
 *       Module:  MICROSAR Icu
 *    Generator:  -
 *
 *  Description:  The ICU driver is a module using the input capture unit (ICU) for demodulation of a PWM signal,
 *                counting pulses, measuring of frequency and duty cycle, generating simple interrupts and wakeup
 *                interrupts. The ICU driver provides services for
 *                -    Signal edge notification
 *                -    Controlling wakeup interrupts
 *                -    Periodic signal time measurement
 *                -    Edge timestamping, usable for the acquisition of non-periodic signals
 *                -    Edge counting
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
 
#define ICU_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************/
#include "Icu.h"


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Icu header file */
#if (  (ICU_SW_MAJOR_VERSION != (2u) ) \
    || (ICU_SW_MINOR_VERSION != (6u))  \
    || (ICU_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (ICU_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (ICU_AR_RELEASE_PATCH_VERSION != (3u)) )
# error "Vendor specific version numbers of Icu.c and Icu.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ICU_CFG_MAJOR_VERSION != (0x03u) ) \
    || (ICU_CFG_MINOR_VERSION != (0x04u)) )
# error "Version numbers of Icu.c and Icu_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Defines are used for splitting up the channel type and position */
#define ICU_SPL_CHL               (0x3Fu)
#define ICU_MM                    (0xC0u)

#define ILLEGAL_CHANNEL           (0x3Fu)


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define ICU_SPLIT_CHANNEL(channelId) (channelId & ICU_SPL_CHL) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define ICU_ED_CHL_CFG(Channel)   (Icu_ConfigSet_pt->Icu_EdChls_at[ICU_SPLIT_CHANNEL((Channel))]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_EC_CHL_CFG(Channel)   (Icu_ConfigSet_pt->Icu_EcChls_at[ICU_SPLIT_CHANNEL((Channel))]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_TS_CHL_CFG(Channel)   (Icu_ConfigSet_pt->Icu_TsChls_at[ICU_SPLIT_CHANNEL((Channel))]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_SM_CHL_CFG(Channel)   (Icu_ConfigSet_pt->Icu_SmChls_at[ICU_SPLIT_CHANNEL((Channel))]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define ICU_ED_HW_CHL(Channel)    (ICU_ED_CHL_CFG((Channel)).Icu_ChannelId_u8) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_EC_HW_CHL(Channel)    (ICU_EC_CHL_CFG((Channel)).Icu_ChannelId_u8) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_TS_HW_CHL(Channel)    (ICU_TS_CHL_CFG((Channel)).Icu_ChannelId_u8) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define ICU_SM_HW_CHL(Channel)    (ICU_SM_CHL_CFG((Channel)).Icu_ChannelId_u8) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (ICU_LOCAL)
# define ICU_LOCAL static
#endif

#if !defined (ICU_LOCAL_INLINE)
# define ICU_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#if (ICU_DEV_ERROR_DETECT == STD_ON)
/* Variable to store the information about the initiation status of
* this driver */
ICU_LOCAL VAR(boolean, ICU_VAR)              Icu_ModuleInitialized_b = FALSE;
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

ICU_LOCAL VAR(Icu_ModeType, ICU_VAR)         Icu_OperatingMode = ICU_MODE_NORMAL;

/* Stores the pointer to the current configuration-set */
ICU_LOCAL P2CONST(Icu_ConfigType, AUTOMATIC, ICU_APPL_CONST) Icu_ConfigSet_pt = NULL_PTR;

/* 32Bit variables that will be initialized by the startup code*/
#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
/* New Sm value measured  
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_NewSmValueAvailable = 0x0000000000000000u;
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_SmFirstMeasurement = 0xFFFFFFFFFFFFFFFFu;
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_SmRunning = 0x0000000000000000u;
/* Run states of signal measurement channels
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_SmChlRunStates_u8 = 0x0000000000000000u;
#endif

/* Represents, if notification of edge detection is enabled
* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_EdChlNotifEnabled_u8 = 0x0000000000000000u;

#if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) || (ICU_REPORT_WAKEUP_SOURCE == STD_ON) )
/* Variables to store information about the wakeup-capability of a channel */
/* Bit 0 represents channel no. 0, bit 1 represents channel no. 1, etc. */
ICU_LOCAL VAR(uint64, ICU_VAR) Icu_WakeupActive = 0x0000000000000000u;
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  Icu_GetChannelMeasurementMode()
*********************************************************************************************************************/
/*! \brief      Queries the channel's measurement mode
*  \param[in]   Channel                 Channel to be queried
*  \return      Icu_MeasurementModeType Channel's measurement mode
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(Icu_MeasurementModeType, ICU_CODE) Icu_GetChannelMeasurementMode(Icu_ChannelType Channel)
{
  return (Channel & ICU_MM);
}


/**********************************************************************************************************************
*  Icu_GetChannelId()
*********************************************************************************************************************/
/*! \brief      Queries the channel's id
*  \param[in]   Channel                 Channel to be queried
*  \return      Icu_ChannelType         Channel's id
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(Icu_ChannelType, ICU_CODE) Icu_GetChannelId(Icu_ChannelType Channel)
{
  return (Channel & ICU_SPL_CHL);
}


/**********************************************************************************************************************
  Icu_ActionForEd()
***********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Edge Detection
 *  \param[in]  Channel       ChannelId
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (ICU_EDGE_DETECT_API == STD_ON)
ICU_LOCAL FUNC(void, ICU_CODE) Icu_ActionForEd (Icu_ChannelType Channel)
{
# if (ICU_REPORT_WAKEUP_SOURCE == STD_ON)
  EcuM_WakeupSourceType TempWakeupSource;
  
  if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
  {
    if (Icu_OperatingMode == ICU_MODE_SLEEP)
    {
      TempWakeupSource = (((EcuM_WakeupSourceType)1u) << ICU_ED_CHL_CFG(Channel).Icu_WakeupSource_u32);
      
      EcuM_CheckWakeup(TempWakeupSource);
    }
# else
  if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE)
  {
# endif

    ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_ACTIVE;

    if (((Icu_EdChlNotifEnabled_u8 >> Icu_GetChannelId(Channel)) & 0x1u) == 0x01u)
    {
      if (ICU_ED_CHL_CFG(Channel).Icu_Notify_pt != NULL_PTR)
      {
        /* Call the User Notification Function */
        ICU_ED_CHL_CFG(Channel).Icu_Notify_pt();
      }
    }

  }
  return;
}
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */


/**********************************************************************************************************************
 *  Icu_ActionForEc()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Edge Counting
 *  \param[in]  Channel       ChannelId
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (ICU_EDGE_COUNT_API == STD_ON)
ICU_LOCAL FUNC(void, ICU_CODE) Icu_ActionForEc (Icu_ChannelType Channel)
{
  if (ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 == TRUE)
  {
    ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16++;
  }
  return;
}
#endif


/**********************************************************************************************************************
 *  Icu_ActionForTs()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is Timestamp mode
 *  \param[in]  Channel       ChannelId
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (ICU_TIMESTAMP_API == STD_ON)
ICU_LOCAL FUNC(void, ICU_CODE) Icu_ActionForTs (Icu_ChannelType Channel)
{
  Icu_ChannelType ChannelId = Icu_GetChannelId(Channel);
  Icu_TsArrayType *TsData = &Icu_ConfigSet_pt->Icu_TsData_at[ChannelId];
  
  /* Check if Timestamping is activated for this channel */
  if ( TsData->RunState == TRUE)
  {
   TsData->BufferPtr[TsData->BufferIndex] = (Icu_ValueType) VttIcu_GetTimerValue(ICU_TS_HW_CHL(Channel));

    /* Store the Index and Increase it's value */
    TsData->BufferIndex++;

    /*   If the buffer index reaches the end of the buffer and the bufer is a circualar buffer
    *    then reset the index to start from the beginning. Otherwise stop the Timestamping
    */
    if ( TsData->BufferIndex == TsData->BufferSize )
    {
      if (ICU_TS_CHL_CFG(Channel).Icu_BufferType_t == ICU_CIRCULAR_BUFFER)
      {
        TsData->BufferIndex = 0u;
      }
      else
      {
        Icu_StopTimestamp(Channel);
      }
    }

    /* Check if notifications are enabled */
    if (TsData->NotifyInfo == TRUE)
    {
      /* Increase the Notification Counter */
      TsData->NotifyCnt++;
      /* If the notification counter reaches the notification intervall, call the notification function and reset
      * the counter
      */
      if (TsData->NotifyCnt == TsData->NotifyInterval)
      {
        if (ICU_TS_CHL_CFG(Channel).Icu_Notify_pt != NULL_PTR) 
        {
          ICU_TS_CHL_CFG(Channel).Icu_Notify_pt();
        }
        TsData->NotifyCnt = 0u;
      }
    }
  }
  return;
}
#endif


/**********************************************************************************************************************
 *  Icu_ActionForSm()
 *********************************************************************************************************************/
/*! \brief      Function which is called from the ISR if measurement mode is signal measurement
 *  \param[in]  Channel       ChannelId
 *  \note       Called in interrupt context
 *********************************************************************************************************************/
#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
ICU_LOCAL FUNC(void, ICU_CODE) Icu_ActionForSm (Icu_ChannelType Channel)
{
  Icu_ChannelType ChannelId = Icu_GetChannelId(Channel);
  Icu_SignalMeasurementPropertyType Icu_Property;
  Icu_ValueType Icu_OldTimerValue, Icu_LatestTimerValue;
  uint8 Icu_Overflow = 0u;

  Icu_Property = ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t;
  Icu_OldTimerValue = Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId]; /*   old  value */
  Icu_LatestTimerValue = (Icu_ValueType) VttIcu_GetTimerValue(ICU_SM_HW_CHL(Channel));  /* latest value */

  /* check a overflow condition */
  if ( Icu_LatestTimerValue < Icu_OldTimerValue )
  {
    Icu_Overflow = 1u;
  }

  if ( Icu_Property == ICU_DUTY_CYCLE )
  {
    if (((Icu_SmFirstMeasurement >> ChannelId) & 0x1u) == 0x01u)
    {
      /* indicate that the first measurement was done */
      Icu_SmFirstMeasurement &= ~(((uint64)1u) << ChannelId);

      /* only the very first time this following if-sequence is not executed */
      if (((Icu_SmRunning >> ChannelId) & 0x1u) == 0x01u)
      {
        if (Icu_Overflow == 1u)
        {
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = 0xFFFFFFFFu - Icu_OldTimerValue + Icu_LatestTimerValue;
        }
        else
        {
          Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = Icu_LatestTimerValue - Icu_OldTimerValue;
        }

        Icu_NewSmValueAvailable |= (((uint64)1u) << ChannelId);
        Icu_ConfigSet_pt->Icu_SmChlStatus_at[ChannelId] = ICU_ACTIVE;
      }

      /* store the latest value */
      Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
    }
    else
    {
      /* set the active time of the specified dutycycle with the measured time */
      if (Icu_Overflow == 1u)
      {
        Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = 0xFFFFFFFFu - Icu_OldTimerValue + Icu_LatestTimerValue;
      }
      else
      {
        Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = Icu_LatestTimerValue - Icu_OldTimerValue;
      }

      /* Indicate that the next measurement should be treated like a first measurement, but in Running mode 
       * (not the very first) 
       */
      Icu_SmFirstMeasurement  |= (((uint64)1u) << ChannelId);
      Icu_SmRunning |= (((uint64)1u) << ChannelId);
    }
  }
  else /* ICU_LOW_TIME, ICU_HIGH_TIME, ICU_PERIOD_TIME */
  {
    if (((Icu_SmFirstMeasurement >> ChannelId) & 0x1u) == 0x01u)
    {
      /* indicate that the first measurement was done */
      Icu_SmFirstMeasurement &= ~(((uint64)1u) << ChannelId);

      /* store the value */
      Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
    }
    else
    {
      switch (Icu_Property)
      {
        case ICU_LOW_TIME:
        case ICU_HIGH_TIME:
          /* set the active time of the specified dutycycle array with the measured time */
          if (Icu_Overflow == 1u)
          {
            Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = 0xFFFFFFFFu - Icu_OldTimerValue + Icu_LatestTimerValue;
          }
          else
          {
            Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].ActiveTime = Icu_LatestTimerValue - Icu_OldTimerValue;
          }
          Icu_SmFirstMeasurement |= (((uint64)1u) << ChannelId);
          break;

        case ICU_PERIOD_TIME:
          /* set the active time of the specified dutycycle array with the measured high time */
          if (Icu_Overflow == 1u)
          {
            Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = 0xFFFFFFFFu - Icu_OldTimerValue + Icu_LatestTimerValue;
          }
          else
          {
            Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[ChannelId].PeriodTime = Icu_LatestTimerValue - Icu_OldTimerValue;
          }
          Icu_ConfigSet_pt->Icu_SmValue_at[ChannelId] = Icu_LatestTimerValue;
          break;
        default: /* PRQA S 2513 */ /* MD_MSR_EmptyClause */
          break;
      }

      /* indicate that a new signal measurement value is available */
      Icu_NewSmValueAvailable |= (((uint64)1u) << ChannelId);
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[ChannelId] = ICU_ACTIVE;
    }
  }
  return;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif


/**********************************************************************************************************************
*  Icu_IsChannelEdgeDetection()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is a edge detection channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsChannelEdgeDetection(Icu_ChannelType Channel)
{
  return (Icu_GetChannelMeasurementMode(Channel) == ICU_MODE_EDGE_DETECTION); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
}


#if (ICU_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
*  Icu_IsChannelSignalMeasurement()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is a signal measurement channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsChannelSignalMeasurement(Icu_ChannelType Channel)
{
  return (Icu_GetChannelMeasurementMode(Channel) == ICU_MODE_SIGNAL_MEASUREMENT); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
}
#endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */


/**********************************************************************************************************************
*  Icu_IsChannelTimestamp()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is a time stamp channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsChannelTimestamp(Icu_ChannelType Channel)
{
  return (Icu_GetChannelMeasurementMode(Channel) == ICU_MODE_TIMESTAMP); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
}


#if (ICU_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
*  Icu_IsChannelEdgeCount()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is a edge count channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsChannelEdgeCount(Icu_ChannelType Channel)
{
  return (Icu_GetChannelMeasurementMode(Channel) == ICU_MODE_EDGE_COUNTER); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
}
#endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */


#if (ICU_DEV_ERROR_DETECT == STD_ON)
# if (ICU_SET_MODE_API == STD_ON)
/**********************************************************************************************************************
*  Icu_CheckBusyOperation()
*********************************************************************************************************************/
/*! \brief      Function which checks if any channel is ongoing / busy
*  \return     Indication              Number of ongoing channels
*********************************************************************************************************************/
ICU_LOCAL FUNC(uint8, ICU_CODE) Icu_CheckBusyOperation(void)
{
  uint8 Index;
  uint8 Indication = 0u;

  /* Check if one of the Channels is running */
# if (ICU_TIMESTAMP_API == STD_ON)
  for (Index = 0u; Index < Icu_ConfigSet_pt->Icu_TsChlCount_u8; Index++)
  {
    if (Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState == TRUE)
    {
      Indication++;
    }
  }
# endif /* (ICU_TIMESTAMP_API == STD_ON) */

# if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
  if (Icu_SmChlRunStates_u8 > 0u)
  {
    Indication++;
  }
# endif /* (ICU_SIGNAL_MEASUREMENT_API == STD_ON) */

# if (ICU_EDGE_COUNT_API == STD_ON)
  for (Index = 0u; Index < Icu_ConfigSet_pt->Icu_EcChlCount_u8; Index++)
  {
    if (ICU_EC_CHL_CFG(Index).Icu_EcActive_u8 == TRUE)
    {
      Indication++;
    }
  }
# endif /* (ICU_EDGE_COUNT_API == STD_ON) */
  return Indication;
}
#endif /* (ICU_SET_MODE_API == STD_ON) */


/**********************************************************************************************************************
*  Icu_IsEdgeDetectionChannelInvalid()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is an invalid edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is an invalid edge detection channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsEdgeDetectionChannelInvalid(Icu_ChannelType Channel)
{
  boolean retVal = FALSE;
  if (Icu_IsChannelEdgeDetection(Channel) == TRUE)
  {
    retVal = (Icu_GetChannelId(Channel) >= Icu_ConfigSet_pt->Icu_EdChlCount_u8); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  }
  return retVal;
}

# if ( (ICU_SIGNAL_MEASUREMENT_API == STD_ON) \
    || (ICU_GET_INPUT_STATE_API == STD_ON) \
    || (ICU_GET_DUTY_CYCLE_VALUES_API == STD_ON) \
    || (ICU_GET_TIME_ELAPSED_API == STD_ON) )
/**********************************************************************************************************************
*  Icu_IsSignalMeasurementChannelInvalid()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is an invalid edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is an invalid signal measurement channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsSignalMeasurementChannelInvalid(Icu_ChannelType Channel)
{
  boolean retVal = FALSE;
  if (Icu_IsChannelSignalMeasurement(Channel) == TRUE)
  {
    retVal = (Icu_GetChannelId(Channel) >= Icu_ConfigSet_pt->Icu_SmChlCount_u8); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  }
  return retVal;
}
#endif


/**********************************************************************************************************************
*  Icu_IsTimeStampChannelInvalid()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a invalid edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is an invalid time stamp channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsTimeStampChannelInvalid(Icu_ChannelType Channel)
{
  boolean retVal = FALSE;
  if (Icu_IsChannelTimestamp(Channel) == TRUE)
  {
    retVal = (Icu_GetChannelId(Channel) >= Icu_ConfigSet_pt->Icu_TsChlCount_u8); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  }
  return retVal;
}


/**********************************************************************************************************************
*  Icu_IsEdgeCountChannelInvalid()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is a invalid edge detection channel
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is an invalid edge count channel, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) Icu_IsEdgeCountChannelInvalid(Icu_ChannelType Channel)
{
  boolean retVal = FALSE;
  if (Icu_IsChannelEdgeCount(Channel) == TRUE)
  {
    retVal = (Icu_GetChannelId(Channel) >= Icu_ConfigSet_pt->Icu_EcChlCount_u8); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  }
  return retVal;
}


/**********************************************************************************************************************
*  IcuIsChannelWakeUpCapable()
*********************************************************************************************************************/
/*! \brief      Queries if the channel is wakeup capable
*  \param[in]   Channel                 Channel to be queried
*  \return      boolean                 True if channel is capable, otherwise False
*********************************************************************************************************************/
ICU_LOCAL_INLINE FUNC(boolean, ICU_CODE) IcuIsChannelWakeUpCapable(Icu_ChannelType Channel)
{
  boolean retVal = FALSE;
  if (Icu_IsChannelEdgeDetection(Channel) == TRUE)
  {
    retVal = ICU_ED_CHL_CFG(Channel).Icu_WakeupCapability_u8; /* PRQA S 2985, 4404 */ /* MD_Icu_2985_2986, MD_MSR_AutosarBoolean */
  }
  return retVal;
}
#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Icu_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_InitMemory (void)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Initialize all module-global variables */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  Icu_ModuleInitialized_b = FALSE;
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */

  Icu_ConfigSet_pt = NULL_PTR;
  
  Icu_OperatingMode = ICU_MODE_NORMAL;

#if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) || (ICU_REPORT_WAKEUP_SOURCE == STD_ON) )
  Icu_WakeupActive = 0x00000000u;
#endif  

  return;
}


/**********************************************************************************************************************
 *  Icu_Init()
 ***********************************************************************************************************************/
/*! \brief      Service for driver initialization. The Initialization function initializes all relevant registers of
 *              the configured hardware with the values of the structure referenced by the parameter ConfigPtr.
 *  \param[in]  ConfigPtr               Pointer to a selected configuration structure
 *  \pre        Input parameter must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_Init_Vtt (P2CONST(Icu_ConfigType, AUTOMATIC, ICU_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint8 Index;

  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_PARAM_CONFIG */
  if (ConfigPtr == NULL_PTR)
  {
    /* ConfigPtr is not defined */
    errorId = ICU_E_PARAM_CONFIG; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_UNINIT */
  else if (Icu_ModuleInitialized_b == TRUE)
  {
    /* ConfigPtr is not defined */
    errorId = ICU_E_ALREADY_INITIALIZED; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
#endif  /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ------------------------------------------------- */
    Icu_OperatingMode = ICU_MODE_NORMAL; /* Set driver to 'ICU_MODE_NORMAL' */

    Icu_ConfigSet_pt = ConfigPtr;

#if (ICU_EDGE_DETECT_API == STD_ON)
    /* Init Edge Detection Structure */
    for (Index=0u; Index < Icu_ConfigSet_pt->Icu_EdChlCount_u8; Index++)
    {
      Icu_ConfigSet_pt->Icu_EdChls_at[Index].Icu_EdActive_u8 = FALSE;
      Icu_ConfigSet_pt->Icu_EdChls_at[Index].Icu_Ed_InputState = ICU_IDLE;
    }
    Icu_EdChlNotifEnabled_u8 = 0u;
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
# if (ICU_GET_INPUT_STATE_API == STD_ON)
    for (Index=0u; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
    {
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[Index] = ICU_IDLE;
    }
# endif /* (ICU_GET_INPUT_STATE_API == STD_ON) */

    Icu_NewSmValueAvailable = 0x00000000u;
    Icu_SmChlRunStates_u8 = 0x00000000u;
    Icu_SmFirstMeasurement = 0xFFFFFFFFu;
    Icu_SmRunning = 0x00000000u;

    for (Index=0u; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
    {
      Icu_ConfigSet_pt->Icu_SmValue_at[Index] = 0u;
    }
#endif /* (ICU_SIGNAL_MEASUREMENT_API == STD_ON) */

#if (ICU_EDGE_COUNT_API == STD_ON)
    /* Init Edge Counting Structure */
    for (Index=0u;Index< Icu_ConfigSet_pt->Icu_EcChlCount_u8;Index++)
    {
      Icu_ConfigSet_pt->Icu_EcChls_at[Index].Icu_EcActive_u8 = FALSE;
      Icu_ConfigSet_pt->Icu_EcChls_at[Index].Icu_EdgeValue_u16 = 0x0000u;
    }
#endif /* (ICU_EDGE_COUNT_API == STD_ON) */

#if (ICU_TIMESTAMP_API == STD_ON)
    /* Init Timestamping Structure */
    for (Index=0u; Index< Icu_ConfigSet_pt->Icu_TsChlCount_u8;Index++)
    {
      Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState = FALSE;
      Icu_ConfigSet_pt->Icu_TsData_at[Index].NotifyInfo = FALSE;
    }
#endif /* (ICU_TIMESTAMP_API == STD_ON) */

    for (Index=0u; Index<Icu_ConfigSet_pt->Icu_MaxNrOfChannels; Index++)
    {
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Index] = ILLEGAL_CHANNEL;
    }

#if (ICU_DEV_ERROR_DETECT == STD_ON)
    Icu_ModuleInitialized_b = TRUE;
#endif  /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_INIT, errorId);
  }
#else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  
  return;  
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* End of Icu_Init() */



#if (ICU_DE_INIT_API == STD_ON)
/**********************************************************************************************************************
 *  Icu_DeInit()
 *********************************************************************************************************************/
/*! \brief      Service for ICU De-Initialization. After the call of this service, the state of the peripherals used
 *              by configuration will be the same as after power on reset. Values of registers which are not writeable
 *              are excluded.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DeInit (void)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint8 Index;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    /* The module hasn't been initialized */
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
  /* ----- Implementation ----------------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
    Icu_ModuleInitialized_b = FALSE;    /* Reset the module status */
#endif
  
    for (Index=0u; Index < Icu_ConfigSet_pt->Icu_MaxNrOfChannels; Index++)
    {
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Index] = ILLEGAL_CHANNEL;
    }

    /* Clear the global configuration pointer */
    Icu_ConfigSet_pt = NULL_PTR;
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_DE_INIT, errorId);
  }
#else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
}   /* End of Icu_DeInit() */
#endif /* (ICU_DE_INIT_API == STD_ON) */


#if (ICU_SET_MODE_API == STD_ON)
/**********************************************************************************************************************
 *  Icu_SetMode()
 *********************************************************************************************************************/
/*! \brief      Service for ICU mode selection. This service sets the operation mode to the given mode parameter. The
 *              component can be set to sleep mode or normal mode with this service.
 *
 *  \param[in]  Mode                    Icu-Mode to set
 *  \pre        Channel must be either ICU_MODE_NORMAL or ICU_MODE_SLEEP.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_SetMode (Icu_ModeType Mode)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint8 Index;
# if ( (ICU_EDGE_DETECT_API == STD_ON) \
    && ((ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON)) )
  uint32 tempWakeupActiv;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_MODE */
  else if ((ICU_MODE_NORMAL != Mode) && (ICU_MODE_SLEEP != Mode))
  {
    errorId = ICU_E_PARAM_MODE; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_CheckBusyOperation() > 0u)
  {
    /* If one of the channels is running the indicationcounter will be > 0 so the errorhook has to be called */
    errorId = ICU_E_BUSY_OPERATION; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    Icu_OperatingMode = Mode;
    
    if (ICU_MODE_SLEEP == Mode)
    {
# if (ICU_TIMESTAMP_API == STD_ON)
      for (Index=0u; Index < Icu_ConfigSet_pt->Icu_TsChlCount_u8; Index++)
      {
        if (Icu_ConfigSet_pt->Icu_TsData_at[Index].RunState == TRUE)
        {
          Icu_StopTimestamp (ICU_MODE_TIMESTAMP | Index);
        }
      }
# endif /* (ICU_TIMESTAMP_API == STD_ON) */

# if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
      for (Index=0u; Index < Icu_ConfigSet_pt->Icu_SmChlCount_u8; Index++)
      {
        if (((Icu_SmChlRunStates_u8 >> Index) & 0x1u) == 0x01u)
        {
          Icu_StopSignalMeasurement (ICU_MODE_SIGNAL_MEASUREMENT| Index);
        }
      }
# endif /* (ICU_SIGNAL_MEASUREMENT_API == STD_ON) */

# if (ICU_EDGE_COUNT_API == STD_ON)
      for (Index=0u; Index < Icu_ConfigSet_pt->Icu_EcChlCount_u8; Index++)
      {
        if (ICU_EC_CHL_CFG(Index).Icu_EcActive_u8 == TRUE )
        {
          Icu_DisableEdgeCount (ICU_MODE_EDGE_COUNTER | Index);
        }
      }
# endif /* (ICU_EDGE_COUNT_API == STD_ON) */

# if (ICU_EDGE_DETECT_API == STD_ON)
      for (Index=0u; Index < Icu_ConfigSet_pt->Icu_EdChlCount_u8; Index++)
      {
#  if ( (ICU_DISABLE_WAKEUP_API == STD_ON) || (ICU_ENABLE_WAKEUP_API == STD_ON) )
        if (ICU_ED_CHL_CFG(Index).Icu_WakeupCapability_u8 == FALSE)
        {
          if (ICU_ED_CHL_CFG(Index).Icu_EdActive_u8 == TRUE)
          {
            Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index); /* PRQA S 2986 */ /* MD_Icu_2985_2986 */
          }
        }
        else 
        {
          tempWakeupActiv = (uint32)((((uint64)Icu_WakeupActive) >> Icu_GetChannelId(Index)) & 0x1u);
          if (tempWakeupActiv == 0u)
          {
            if (ICU_ED_CHL_CFG(Index).Icu_EdActive_u8 == TRUE)
            {
              Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index); /* PRQA S 2986 */ /* MD_Icu_2985_2986 */
            }
          }
        }
#  else
        Icu_DisableEdgeDetection (ICU_MODE_EDGE_DETECTION | Index);
#  endif
      }
# endif /* (ICU_EDGE_DETECT_API == STD_ON) */
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_SET_MODE, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */ /* End of Icu_SetMode() */
#endif /* (ICU_SET_MODE_API == STD_ON) */


#if (ICU_DISABLE_WAKEUP_API == STD_ON)
/**********************************************************************************************************************
 *  Icu_DisableWakeup()
 *********************************************************************************************************************/
/*! \brief      This service shall disable the wakeup capability of a single ICU channel. This service is only feasible
 *              for ICU channels configured statically as wakeup capable true.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel has to be configured as a wakeup-capable channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableWakeup (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeDetection(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (IcuIsChannelWakeUpCapable(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    Icu_WakeupActive &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_DISABLE_WAKEUP, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  
  return;
} /* End of Icu_DisableWakeup() */
#endif /* (ICU_DISABLE_WAKEUP_API == STD_ON) */


#if (ICU_ENABLE_WAKEUP_API == STD_ON)
/**********************************************************************************************************************
 *  Icu_EnableWakeup()
 *********************************************************************************************************************/
/*! \brief      This service (re-)enables the wakeup capability of a single ICU channel. This service is only feasible
 *              for ICU channels configured as wakeup capable true.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel has to be configured as a wakeup-capable channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableWakeup (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */  
  else if (Icu_IsChannelEdgeDetection(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (IcuIsChannelWakeUpCapable(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    Icu_WakeupActive |= (((uint64)1u) << Icu_GetChannelId(Channel));
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_ENABLE_WAKEUP, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  
  return;
} /* End of Icu_EnableWakeup() */
#endif /* (ICU_ENABLE_WAKEUP_API == STD_ON) */


/**********************************************************************************************************************
 *  Icu_SetActivationCondition()
 *********************************************************************************************************************/
/*! \brief      This service sets the activation-edge according to the Activation parameter for the given channel. This
 *              service overrides the edge setting of the configuration.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  Activation              Type of activation
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *              - ICU_MODE_EDGE_COUNTER
 *  \pre        Activation must be ICU_RISING_EDGE, ICU_FALLING_EDGE, ICU_BOTH_EDGES
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_SetActivationCondition (Icu_ChannelType Channel, Icu_ActivationType Activation)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelSignalMeasurement(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeCountChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_ACTIVATION */
  else if ( (ICU_RISING_EDGE  != Activation)
         && (ICU_FALLING_EDGE != Activation)
         && (ICU_BOTH_EDGES   != Activation) )
  {
    errorId = ICU_E_PARAM_ACTIVATION; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
#endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
#if (ICU_EDGE_DETECT_API == STD_ON)
    if (Icu_IsChannelEdgeDetection(Channel) == TRUE)
    {
      ICU_ED_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
      ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
        
      /* Update the trigger edge if the Channel is running */
      if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE) /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
      {
        VttIcu_StartEdgeMonitoring (ICU_ED_HW_CHL(Channel), Activation); /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
      }
    }
    else
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */

#if (ICU_TIMESTAMP_API == STD_ON)
    if (Icu_IsChannelTimestamp(Channel) == TRUE)
    {
      ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation;

      /* Update the trigger edge if the Channel is running */
      if (Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].RunState == TRUE)
      {
        VttIcu_StartTimeMeasurement (ICU_TS_HW_CHL(Channel), ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t,
                                      ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
      }
    }
    else
#endif /* (ICU_TIMESTAMP_API == STD_ON) */

#if (ICU_EDGE_COUNT_API == STD_ON)
    /* if (Icu_IsChannelEdgeCount(Channel) == TRUE) */
    {
      /* 
       * If-Query above not needed
       * If application enters this part it is already ensured that channel is a edge detection channel.
       * Line is here only for readability.
       */
      ICU_EC_CHL_CFG(Channel).Icu_DefaultStartEdge_t = Activation;

      /* Update the trigger edge if the Channel is running */
      if (ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 == TRUE)
      {
        VttIcu_StartEdgeMonitoring (ICU_EC_HW_CHL(Channel), Activation);
      }
    }
#endif  /* ( ICU_EDGE_COUNT_API == STD_ON) */
    {
      /* Intentionally left empty */
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_SET_ACTIVATION_CONDITION, errorId);
  }
#else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
} /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */ /* End of Icu_SetActivationCondition() */


/***********************************************************************************************************************
 *  Icu_DisableNotification()
 *********************************************************************************************************************/
/*! \brief      This service disables the notification on the given Icu channel.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableNotification (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsChannelSignalMeasurement(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsChannelEdgeCount(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    if ( (Icu_IsChannelEdgeDetection(Channel) == TRUE)
      && (((Icu_EdChlNotifEnabled_u8 >> Icu_GetChannelId(Channel)) & 0x1u) == 0x01u))
    {
        Icu_EdChlNotifEnabled_u8 &= ~(((uint64)1u) << Icu_GetChannelId(Channel));
    }
    else /* if (Icu_IsChannelTimestamp(Channel) == ICU_MODE_TIMESTAMP) */
    {
      Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].NotifyInfo = FALSE;
      Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].NotifyCnt = 0u;
    }
  }
  
/* ----- Development Error Report --------------------------------------- */
#if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_DISABLE_NOTIFICATION, errorId);
  }
#else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */ /* End of Icu_DisableNotification() */


/***********************************************************************************************************************
 *  Icu_EnableNotification()
 *********************************************************************************************************************/
/*! \brief     This service enables the notification functionality on the given Icu channel. Once the notification
 *              functionality is enabled, pre-configured notification functions are called by the component.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_TIMESTAMP
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableNotification (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
#if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsChannelSignalMeasurement(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsChannelEdgeCount(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
#endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    if (Icu_IsChannelEdgeDetection(Channel) == TRUE)
    {
      Icu_EdChlNotifEnabled_u8 |= (((uint64)1u) << Icu_GetChannelId(Channel));
    }
    else /* if (Icu_IsChannelTimestamp(Channel) == TRUE) */
    {
      Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].NotifyCnt = 0u;
      Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].NotifyInfo = TRUE;
    }
  }
/* ----- Development Error Report --------------------------------------- */
#if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_ENABLE_NOTIFICATION, errorId);
  }
#else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* End of Icu_EnableNotification() */



#if (ICU_GET_INPUT_STATE_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_GetInputState()
 *********************************************************************************************************************/
/*! \brief      This service returns the status of the Icu input. If an activation edge has been detected, the service
 *              will return ICU_ACTIVE, if not, the service will return ICU_IDLE. Once the state has been read, the
 *              service resets the state to ICU_IDLE.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     ICU_ACTIVE, ICU_IDLE
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be configured for one of the following modes:
 *              - ICU_MODE_SIGNAL_EDGE_DETECT
 *              - ICU_MODE_SIGNAL_MEASUREMENT
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_InputStateType, ICU_CODE) Icu_GetInputState (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  Icu_InputStateType Icu_StatusTemp_tloc = ICU_IDLE;

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelTimestamp(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsChannelEdgeCount(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsSignalMeasurementChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else 
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
  /* ----- Implementation ----------------------------------------------- */
# if (ICU_EDGE_DETECT_API == STD_ON)
    if (Icu_IsChannelEdgeDetection(Channel) == TRUE)
    {
      if (ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 == TRUE) /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
      {
        /* Read the Input State into local variable */
        Icu_StatusTemp_tloc = ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
        ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
      }
    }
    else /* if (Icu_IsChannelSignalMeasurement(Channel) == TRUE) */
    {
      if (((Icu_SmChlRunStates_u8 >> Icu_GetChannelId(Channel)) & 0x1u) == 0x01u)
      {
        Icu_StatusTemp_tloc = Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)];
        Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)] = ICU_IDLE;
      }
    }
# endif
  }
  
/* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_INPUT_STATE, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  /* Return the status of the current channel */
  return (Icu_StatusTemp_tloc);
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* End Icu_GetInputState() */
#endif /* (ICU_GET_INPUT_STATE_API == STD_ON) */



#if (ICU_TIMESTAMP_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_StartTimestamp()
 *********************************************************************************************************************/
/*! \brief      This service starts the capturing of timer values on the currently configured edges (rising/falling/
 *              both). The values will be written to the pre-configured buffer.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  BufferPtr               Pointer to the buffer array, where the timestamp values shall be placed
 *  \param[in]  BufferSize              Size of the external buffer
 *  \param[in]  NotifyInterval          Notification internal (number of events until the next call of the notification)
 *  \param[out] inOut                   Output parameter reference
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \pre        BufferPtr must not be NULL_PTR.
 *  \pre        BufferSize must not be '0'.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StartTimestamp 
(
  Icu_ChannelType Channel,
  P2VAR(Icu_ValueType, ICU_VAR, ICU_APPL_DATA) BufferPtr,
  uint16          BufferSize,
  uint16          NotifyInterval
)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint16 counter;
  Icu_TsArrayType *TsData;

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelTimestamp(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_BUFFER_PTR */
  else if (BufferPtr == NULL_PTR)
  {
    errorId = ICU_E_PARAM_BUFFER_PTR; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_NOTIFY_INTERVAL */
  else if (NotifyInterval == 0u)
  {
    errorId = ICU_E_PARAM_NOTIFY_INTERVAL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_BUFFER_SIZE */
  else if (BufferSize == 0u)
  {
    errorId = ICU_E_PARAM_BUFFER_SIZE; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* check if a channel is already mapped to this Hardware */
    if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)])
    {
      /* Get the TS data set */
      TsData = &Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)];
    
      /* Map the ChannelId to the Interrupt Source (Hardware) */
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)] = Channel;
    
      /* Store the pointer to the current application buffer */
      TsData->BufferPtr = BufferPtr;

      /* Reset the buffer index for the current channel */
      TsData->BufferIndex = 0u;

      /* Reset the notificaton counter */
      TsData->NotifyCnt = 0u;

      /* Disable the notification */
      TsData->NotifyInfo = FALSE;

      /* Store the informations for the current channel
      * (will be used by the interrupt service routine of this driver) */
      TsData->BufferSize = BufferSize;
      TsData->NotifyInterval = NotifyInterval;

      /* Initialize the Buffer with Zeros, needed for the first run of CaplSetTimestamp */
      for (counter = 0u; counter < BufferSize; counter++)
      {
        BufferPtr[counter] = 0u;
      }

      /* Set The Channel to RunState */
      TsData->RunState = TRUE;

      /* Activate the TimeMeasurement of the VttCntrl_Icu  */
      VttIcu_StartTimeMeasurement (ICU_TS_HW_CHL(Channel), ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t,
                                                                        ICU_TS_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_START_TIMESTAMP, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* End of Icu_StartTimestamp() */


/***********************************************************************************************************************
 *  Icu_StopTimestamp()
 *********************************************************************************************************************/
/*! \brief      This service stops the timestamp measurement of the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StopTimestamp (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelTimestamp(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_NOT_STARTED */
  else if (Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].RunState == FALSE)
  {
    errorId = ICU_E_NOT_STARTED; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */    
    /* Stop the Time Measurement of the channel */
    VttIcu_StopTimeMeasurement(ICU_TS_HW_CHL(Channel));

    /* Set Runstate to false to stop the timestamping */
    Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)].RunState = FALSE;

    /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_TS_HW_CHL(Channel)] = ILLEGAL_CHANNEL;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_STOP_TIMESTAMP, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_StopTimestamp() */



/***********************************************************************************************************************
 *  Icu_GetTimestampIndex()
 *********************************************************************************************************************/
/*! \brief      This service reads the timestamp index, which is the next to be written, on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be a Timestamping channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_IndexType, ICU_CODE) Icu_GetTimestampIndex (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  Icu_IndexType retVal = 0u;
  Icu_TsArrayType *TsData;

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL*/
  else if (Icu_IsChannelTimestamp(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsTimeStampChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the TS data set */
    TsData = &Icu_ConfigSet_pt->Icu_TsData_at[Icu_GetChannelId(Channel)];

    /* Read the index for the current channel */
    if (TsData->RunState == TRUE)
    {
      retVal = TsData->BufferIndex;
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_TIMESTAMP_INDEX, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  
  return retVal;
} /* End of Icu_GetTimestampIndex() */
#endif /* (ICU_TIMESTAMP_API == STD_ON) */


#if (ICU_EDGE_COUNT_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_ResetEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service resets the counted edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_ResetEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeCount(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeCountChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16 = 0x0000u;
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_RESET_EDGE_COUNT, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_ResetEdgeCount() */


/***********************************************************************************************************************
 *  Icu_EnableEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service enables the counting of edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeCount(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeCountChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*( ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* check if a channel is already mapped to this Hardware */
    if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)])
    {
      /* Map the ChannelId to the Interrupt Source (Hardware) */
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)] = Channel;

      /* Enable the Edge Count Channel */
      ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 = TRUE;

      /* Activate the monitoring of the VttCntrl_Icu  */
      VttIcu_StartEdgeMonitoring (ICU_EC_HW_CHL(Channel), ICU_EC_CHL_CFG(Channel).Icu_DefaultStartEdge_t);
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_COUNT, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return;
} /* End of Icu_EnableEdgeCount() */


/***********************************************************************************************************************
 *  Icu_DisableEdgeCount()
 *********************************************************************************************************************/
/*! \brief      This service disables the counting of edges on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableEdgeCount (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeCount(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeCountChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*( ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Deactivate the monitoring of the VttCntrl_Icu  */
    VttIcu_StopEdgeMonitoring(ICU_EC_HW_CHL(Channel));

    /* Disable the Edge Count Channel */
    ICU_EC_CHL_CFG(Channel).Icu_EcActive_u8 = FALSE;

    /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_EC_HW_CHL(Channel)] = ILLEGAL_CHANNEL;
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_COUNT, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_DisableEdgeCount() */


/***********************************************************************************************************************
 *  Icu_GetEdgeNumbers()
 *********************************************************************************************************************/
/*! \brief      This service reads the number of counted edges of the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     Edge number
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Edge Counting channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_EdgeNumberType, ICU_CODE) Icu_GetEdgeNumbers (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  Icu_EdgeNumberType retVal;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  retVal = 0u;
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeCount(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeCountChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*( ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get the counted edges */
    retVal = (Icu_EdgeNumberType)(ICU_EC_CHL_CFG(Channel).Icu_EdgeValue_u16);
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_EDGE_NUMBERS, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  
  return retVal;
}   /* Icu_GetEdgeNumbers() */
#endif /* (ICU_EDGE_COUNT_API == STD_ON) */


#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_StartSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service starts the capturing of a periodic signal on the given channel. The capturing itself
 *              starts on the first 'default start edge', which occurrs after the call of this function.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StartSignalMeasurement (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelSignalMeasurement(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsSignalMeasurementChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* check if a channel is already mapped to this Hardware */
    if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)])
    {
      /* Map the ChannelId to the Interrupt Source (Hardware) */
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)] = Channel;

      /* Set the appropriate bit, to indicate that this channel is running */
      Icu_SmChlRunStates_u8 |= ( ((uint64)1u) << Icu_GetChannelId(Channel) );

      /* no coherent data is available at the start of Signal Measurement*/
      Icu_NewSmValueAvailable &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));

      /* Set the appropriate bits, to indicate that it is the first measurement of the channel */
      Icu_SmFirstMeasurement |= ( ((uint64)1u) << Icu_GetChannelId(Channel) );
      Icu_SmRunning &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));

      /* Setting up the period and active time to 0 */
      Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].ActiveTime = 0u;
      Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].PeriodTime = 0u;

      /* Activiate the SignalMeasurement in the VttCntrl */
      /* Dutycycle */
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_DUTY_CYCLE )
      {
        if (ICU_SM_CHL_CFG(Channel).Icu_SignalStartEdge_t == ICU_FALLING_EDGE )
        {
          VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_BOTH_EDGES);
        }
        else
        {
          VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_BOTH_EDGES);
        }
      }

      /* HighTime */
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_HIGH_TIME )
      {
        VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_BOTH_EDGES);
      }

      /* LowTime */
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_LOW_TIME )
      {
        VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_BOTH_EDGES);
      }

      /* PeriodTime */
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_PERIOD_TIME)
      {
        if (ICU_SM_CHL_CFG(Channel).Icu_SignalStartEdge_t == ICU_FALLING_EDGE )
        {
          VttIcu_StartTimeMeasurement(ICU_SM_HW_CHL(Channel), ICU_FALLING_EDGE, ICU_FALLING_EDGE);
        }
        else
        {
          VttIcu_StartTimeMeasurement (ICU_SM_HW_CHL(Channel), ICU_RISING_EDGE, ICU_RISING_EDGE);
        }
      }

      /* Reset channel's state to ICU_IDLE */
#  if (ICU_GET_INPUT_STATE_API == STD_ON)
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)] = ICU_IDLE;
#  endif
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_START_SIG_MEASUREMENT, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */ /* End of Icu_StartSignalMeasurement() */


/***********************************************************************************************************************
 *  Icu_StopSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service stops the capturing of a periodic signal on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_StopSignalMeasurement (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check  ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelSignalMeasurement(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsSignalMeasurementChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* deactivate the appropriate channel in VttCntrl */
    VttIcu_StopTimeMeasurement(ICU_SM_HW_CHL(Channel));

    /* Clear the appropriate bit, to indicate that this channel is stopped */
    Icu_SmChlRunStates_u8 &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));

    /* Clear the appropriate bit, to indicate that no value is available */
    Icu_NewSmValueAvailable &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));

    /* Delete the mapping the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_SM_HW_CHL(Channel)] = ILLEGAL_CHANNEL;

# if (ICU_GET_INPUT_STATE_API == STD_ON)
    /* Reset channel's state to ICU_IDLE */
    Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)] = ICU_IDLE;
# endif
  }
/* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_STOP_SIG_MEASUREMENT, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_StopSignalMeasurement() */
#endif /* (ICU_SIGNAL_MEASUREMENT_API == STD_ON) */


#if (ICU_EDGE_DETECT_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_StartSignalMeasurement()
 *********************************************************************************************************************/
/*! \brief      This service starts edgte detection on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_EnableEdgeDetection (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeDetection(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* check if a channel is already mapped to this Hardware */
    if (ILLEGAL_CHANNEL == Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)]) /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
    {
      /* Map the ChannelId to the Interrupt Source (Hardware) */
      Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)] = Channel; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */

      /* Set the channel active */
      ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 = TRUE; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */

      /* Set the ED channel input state to idle*/
      ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE;  /* PRQA S 2985 */ /* MD_Icu_2985_2986 */

      /* Activate the monitoring of the VttCntrl_Icu  */
      VttIcu_StartEdgeMonitoring (ICU_ED_HW_CHL(Channel), ICU_ED_CHL_CFG(Channel).Icu_DefaultStartEdge_t); /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_ENABLE_EDGE_DETECTION, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_EnableEdgeDetection() */


/***********************************************************************************************************************
 *  Icu_DisableEdgeDetection()
 *********************************************************************************************************************/
/*! \brief      This service stops the edge detection on the given channel.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_DisableEdgeDetection (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelEdgeDetection(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsEdgeDetectionChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Deactivate the monitoring of the VttCntrl_Icu  */
    VttIcu_StopEdgeMonitoring(ICU_ED_HW_CHL(Channel)); /* PRQA S 2985 */ /* MD_Icu_2985_2986 */

    /* Set the channel inactive */
    ICU_ED_CHL_CFG(Channel).Icu_EdActive_u8 = FALSE; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
    /* Set the ED channel input state to idle*/
    ICU_ED_CHL_CFG(Channel).Icu_Ed_InputState = ICU_IDLE; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */

    /* delete the mapping the ChannelId to the Interrupt Source (Hardware) */
    Icu_ConfigSet_pt->Icu_IsrMappingTable_at[ICU_ED_HW_CHL(Channel)] = ILLEGAL_CHANNEL; /* PRQA S 2985 */ /* MD_Icu_2985_2986 */
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_DISABLE_EDGE_DETECTION, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_DisableEdgeDetection() */
#endif /* (ICU_EDGE_DETECT_API == STD_ON) */


#if (ICU_GET_TIME_ELAPSED_API == STD_ON) /* Only effective in mode 'Signal Measurement' */
/***********************************************************************************************************************
 *  Icu_GetTimeElapsed()
 *********************************************************************************************************************/
/*! \brief      This service reads the elapsed Signal Low Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal Low Time'. The elapsed time is measured between a falling edge and the
 *              consecutive rising edge of the channel.
 *              This service reads the elapsed Signal High Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal High Time'. The elapsed time is measured between a rising edge and the
 *              consecutive falling edge of the channel.
 *              This service reads the elapsed Period Time for the given channel that is configured in Measurement
 *              Mode 'Signal Measurement, Signal Period Time'. The elapsed time is measured between consecutive rising
 *              (or falling) edges of the channel. The period start edge is configurable.
 *
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \return     Low time/high time/period time
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(Icu_ValueType, ICU_CODE) Icu_GetTimeElapsed (Icu_ChannelType Channel)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint32 Icu_TempResult_u32loc = 0u;

  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_UNINIT */
  if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelSignalMeasurement(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsSignalMeasurementChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if ( (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_HIGH_TIME)
         && (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_LOW_TIME)
         && (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_PERIOD_TIME) )
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    if (((Icu_NewSmValueAvailable >> Icu_GetChannelId(Channel)) & 0x1u) == 0x01u)
    {
      /* Measurement has completed and the driver is able to provide information on the input signal  */
      /* reset the new-value-condition */
      Icu_NewSmValueAvailable &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)] = ICU_IDLE;

      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_LOW_TIME)
      {
        Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].ActiveTime;
      }
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_HIGH_TIME)
      {
        Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].ActiveTime;
      }
      if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t == ICU_PERIOD_TIME)
      {
        Icu_TempResult_u32loc = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].PeriodTime;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_TIME_ELAPSED, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return (Icu_ValueType)Icu_TempResult_u32loc;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* End of Icu_GetTimeElapsed() */
#endif /* (ICU_GET_TIME_ELAPSED_API == STD_ON) */


#if (ICU_GET_DUTY_CYCLE_VALUES_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_GetDutyCycleValues()
 *********************************************************************************************************************/
/*! \brief      This service reads the coherent active time and period time for the given ICU Channel, if it is
 *              configured in Measurement Mode 'Signal Measurement, Duty Cycle Values'.
 *  \param[in]  Channel                 Numeric identifier of the Icu channel
 *  \param[in]  DutyCycleValues         Pointer to a buffer where the results (high time and period time) shall be
 *                                      placed
 *  \pre        Channel has to be a valid channel number.
 *  \pre        Channel must be an Signal Measurement channel.
 *  \pre        DutyCycleValues must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_GetDutyCycleValues
(
  Icu_ChannelType     Channel,
  P2VAR(Icu_DutyCycleType, ICU_VAR, ICU_APPL_DATA) DutyCycleValues
)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* check ICU_CFG_CHK_BUFFER_PTR */
  if (DutyCycleValues == NULL_PTR)
  {
    errorId = ICU_E_PARAM_BUFFER_PTR; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_UNINIT */
  else if (Icu_ModuleInitialized_b == FALSE)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  /* check ICU_CFG_CHK_PARAM_CHANNEL */
  else if (Icu_IsChannelSignalMeasurement(Channel) == FALSE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (Icu_IsSignalMeasurementChannelInvalid(Channel) == TRUE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else if (ICU_SM_CHL_CFG(Channel).Icu_MeasurementProperty_t != ICU_DUTY_CYCLE)
  {
    errorId = ICU_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif  /*(ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    if (((Icu_NewSmValueAvailable >> Icu_GetChannelId(Channel)) & 0x1u) == 0x01u)
    {
      /* Measurement has completed and the driver is able to provide information on the input signal  */
      /* reset the new-value-condition */
      Icu_NewSmValueAvailable &= (~(((uint64)1u) << Icu_GetChannelId(Channel)));
      Icu_ConfigSet_pt->Icu_SmChlStatus_at[Icu_GetChannelId(Channel)] = ICU_IDLE;

      /* Fill the dutycycle values with the new data */
      DutyCycleValues->ActiveTime = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].ActiveTime;
      DutyCycleValues->PeriodTime = Icu_ConfigSet_pt->Icu_SmDutyCylceData_at[Icu_GetChannelId(Channel)].PeriodTime;
    }
    else
    {
      /* Return a default: 0 - no new value was measured */
      DutyCycleValues->ActiveTime = 0u;
      DutyCycleValues->PeriodTime = 0u;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_DUTY_CYCLE_VALUES, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

# if (ICU_DEV_ERROR_DETECT == STD_ON)    
  if ( (errorId != ICU_E_NO_ERROR)
    && (errorId != ICU_E_PARAM_BUFFER_PTR) )
  {
    DutyCycleValues->ActiveTime = 0u;
    DutyCycleValues->PeriodTime = 0u;
  }
# endif  

  return;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* End of Icu_GetDutyCycleValues() */
#endif /* (ICU_GET_DUTY_CYCLE_VALUES_API == STD_ON) */


#if (ICU_GET_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  Icu_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief      This service returns the version information of this module. The version information includes:
 *              - Module Id
 *              - Vendor Id
 *              - Vendor specific version numbers
 *  \param[out] versioninfo             Pointer to where to store the version information of this module
 *  \pre        Versioninfo parameter must not be NULL_PTR.
 *  \context    Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, ICU_APPL_DATA) versioninfo)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  
  /* ----- Development Error Checks ------------------------------------- */
# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* Check the versioninfo pointer for being a NULL_PTR */
  if (versioninfo == NULL_PTR)
  {
    errorId = ICU_E_PARAM_VINFO; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID         = (uint16)(ICU_VENDOR_ID);
    versioninfo->moduleID         = (uint16)(ICU_MODULE_ID);
    versioninfo->sw_major_version = (uint8)(ICU_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (uint8)(ICU_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (uint8)(ICU_SW_PATCH_VERSION);
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_GET_VERSION_INFO, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
}   /* Icu_GetVersionInfo() */
#endif /* (ICU_GET_VERSION_INFO_API == STD_ON) */


#if (ICU_REPORT_WAKEUP_SOURCE == STD_ON)
FUNC(void, ICU_CODE) Icu_CheckWakeup (EcuM_WakeupSourceType WakeupSource)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = ICU_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Icu_APIPattern */
  uint8 loop;
  EcuM_WakeupSourceType TempWakeupSource;

# if (ICU_DEV_ERROR_DETECT == STD_ON)
  /* Check if the module is initialized */
  if (Icu_ConfigSet_pt == NULL_PTR)
  {
    errorId = ICU_E_UNINIT; /* PRQA S 2983 */ /* MD_Icu_APIPattern */
  }
  else
# endif /* (ICU_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /*In case the expected wk source Id was found, the referenced EcuM function will be called*/
    for (loop = 0u; loop < Icu_ConfigSet_pt->Icu_EdChlCount_u8 ; loop++)
    {
      TempWakeupSource = (((uint32)1u) << ICU_ED_CHL_CFG(loop).Icu_WakeupSource_u32);

      if ( (WakeupSource == TempWakeupSource)
        && (((Icu_WakeupActive >> Icu_GetChannelId(loop)) & 0x1u) == 0x01u) )
      {
        EcuM_SetWakeupEvent (TempWakeupSource);  
        break;
      }  
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (ICU_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ICU_E_NO_ERROR)
  {
    Icu_Errorhook (ICU_SID_CHECK_WAKEUP, errorId);
  }
# else
  ICU_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* End of Icu_CheckWakeup() */
#endif /* (ICU_REPORT_WAKEUP_SOURCE == STD_ON) */


/***********************************************************************************************************************
 *  Icu_IsrHandler
 *********************************************************************************************************************/
/*! \brief      This service dispatches the occurred interupt
 *  \param[in]  Irq_Source             Irq_Source to find out the channel
 *  \pre        Module has to be initialized.
 *********************************************************************************************************************/
FUNC(void, ICU_CODE) Icu_IsrHandler (Icu_InterruptSourceType Irq_Source)
{
  /* ----- Function Local Variables ------------------------------------- */
  Icu_ChannelType Channel = Icu_ConfigSet_pt->Icu_IsrMappingTable_at[Irq_Source];
  
  /* ----- Implementation ----------------------------------------------- */
  if (Channel != ILLEGAL_CHANNEL)
  {
    switch (Icu_GetChannelMeasurementMode(Channel))
    {
      case ICU_MODE_EDGE_DETECTION:
#if (ICU_EDGE_DETECT_API == STD_ON)
          Icu_ActionForEd (Channel);
#endif
        break;
      case ICU_MODE_EDGE_COUNTER:
#if (ICU_EDGE_COUNT_API == STD_ON)
          Icu_ActionForEc (Channel);
#endif
        break;
      case ICU_MODE_SIGNAL_MEASUREMENT:
#if (ICU_SIGNAL_MEASUREMENT_API == STD_ON)
          Icu_ActionForSm (Channel);
#endif
        break;
      case ICU_MODE_TIMESTAMP:
#if (ICU_TIMESTAMP_API == STD_ON)
          Icu_ActionForTs (Channel);
#endif
        break;
      default: /* PRQA S 2513 */ /* MD_MSR_EmptyClause */
        break;
    }
  }
}

/* Justification for module-specific MISRA deviations:

  MD_Icu_3408: rule 8.4
    Reason:     All functions 'Os_Isr_IcuIsr_<IsrNr>' have external linkages and seems to be defined without previous 
                declaration. However, these functions are - if used - declared by Os_Lcfg.h. So, the component does
                not declare these functions in the own header.
    Risk:       None
    Prevention: None

  MD_Icu_2985_2986: rule 2.2
    Reason:     For all four measurement modes (edge detection, edge counting, signal measurement and timestamp) one
                pattern is used to identify the channel id and measurement mode. Both information are coded in the same
                identifier. Due to the fact that edge detection mode is coded as 0x00 and only the most two significant
                bits are a placeholder for measuremnt mode
                 - a binary AND operation of the identifier with 0x3F always returns the value of the identifier and 
                   the check is unnecessary
                 - a binary OR of the identifier with 0x00 will always return the identifier and the operation has no
                   effect
    Risk:       None
    Prevention: None

  MD_Icu_APIPattern: rule 2.2, 2.3
    Reason:     Coding rules requires the "API pattern" implementation, which leads to this MISRA issue: redundant 
                assignment and initialization. Unused errorId will be solved by DUMMY_STATEMENT. The API pattern also
                requires single point of return - with disabled parameter checks the return value variable will never
                be used before modified.
    Risk:       None
    Prevention: None


*/


/**********************************************************************************************************************
 *  END OF FILE: Icu.c
 *********************************************************************************************************************/
