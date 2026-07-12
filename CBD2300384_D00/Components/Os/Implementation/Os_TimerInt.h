/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \ingroup     Os_Kernel
 *  \defgroup    Os_Timer Timer
 *  \brief       High level timer hardware handling.
 *  \details
 *
 *  \{
 *
 *  \file
 *  \brief       Interface to high level timer hardware handling.
 *  \details     --no details--
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_TIMERINT_H
# define OS_TIMERINT_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Timer_Types.h"

/* Os kernel module dependencies */
# include "Os_Counter_Types.h"
# include "Os_CounterInt.h"
# include "Os_Common_Types.h"
# include "Os_IsrInt.h"
# include "Os_ErrorInt.h"
# include "OsInt.h"

/* Os HAL dependencies */
# include "Os_Hal_TimerInt.h"



/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! The maximum value of \ref Os_TickType. */
# define OS_OS_TICKTYPE_MAX          (0xFFFFFFFFuL)

/*! The pattern indicating the first quarter of an OS tick value. */
# define OS_OS_TICKTYPE_QUARTER0     (0x00000000uL)

/*! The pattern indicating the third quarter of an OS tick value. */
# define OS_OS_TICKTYPE_QUARTER3     (0xC0000000uL)

/*! Mask to identify the quarter of an OS tick value. */
# define OS_OS_TICKTYPE_QUARTER_MASK (OS_OS_TICKTYPE_QUARTER3)

/*! The maximum hardware value of the free running timer. */
# define OS_FRT_TICKTYPE_MAX         (OS_HAL_TIMERFRTTICKTYPE_MAX)

/*! Half of the maximum hardware value of the free running timer. */
# define OS_FRT_TICKTYPE_HALF        (OS_HAL_TIMERFRTTICKTYPE_MAX >> 1)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/*! \def        OS_TIMERSW_GETMAXCOUNTINGVALUE()
 *  \brief      This macro is used to calculate the maximum counting value for a software counter.
 *  \details    This value is needed for modulo calculation.
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERSW_GETMAXCOUNTINGVALUE(MaxAllowedValue)                                                               \
    ((((Os_TickType)(MaxAllowedValue))*(Os_TickType)2)+(Os_TickType)1)


/*! \def        OS_TIMERPIT_GETMAXCOUNTINGVALUE()
 *  \brief      This macro is used to calculate the maximum counting value for a PIT.
 *  \details    This value is needed for modulo calculation.
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPIT_GETMAXCOUNTINGVALUE(MaxAllowedValue)                                                              \
      OS_TIMERSW_GETMAXCOUNTINGVALUE((MaxAllowedValue))


/*! \def        OS_TIMERHRT_GETMAXCOUNTINGVALUE()
 *  \brief      This macro is used to calculate the maximum counting value for a HRT.
 *  \details    This value is needed for modulo calculation.
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERHRT_GETMAXCOUNTINGVALUE(MaxAllowedValue)                                                              \
  ((((Os_TickType)(MaxAllowedValue))*(Os_TickType)4)+(Os_TickType)3)


/*! \def        OS_TIMERPFRT_GETMAXCOUNTINGVALUE()
 *  \brief      This macro is used to calculate the maximum counting value for a PFRT (software part).
 *  \details    This value is needed for modulo calculation.
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPFRT_GETMAXCOUNTINGVALUE(MaxAllowedValue)                                                             \
      OS_TIMERSW_GETMAXCOUNTINGVALUE((MaxAllowedValue))


/*! \def        OS_TIMERPFRT_HARDWAREGETMAXCOUNTINGVALUE()
 *  \brief      This macro is used to calculate the maximum counting value for a PFRT (hardware part).
 *  \details    This value is needed for modulo calculation.
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPFRT_HARDWAREGETMAXCOUNTINGVALUE(MaxAllowedValue)                                                     \
      OS_TIMERHRT_GETMAXCOUNTINGVALUE((MaxAllowedValue))


/*! \def        OS_TIMERSW_GETMAXDIFFERENTIALVALUE()
 *  \brief      This macro is used to calculate the maximum differential value between two software timer values.
 *  \details    --no details--
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERSW_GETMAXDIFFERENTIALVALUE(MaxAllowedValue)                                                           \
      ((Os_TickType)(MaxAllowedValue))


/*! \def        OS_TIMERPIT_GETMAXDIFFERENTIALVALUE()
 *  \brief      This macro is used to calculate the maximum differential value between two PIT values.
 *  \details    --no details--
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPIT_GETMAXDIFFERENTIALVALUE(MaxAllowedValue)                                                          \
      (OS_TIMERSW_GETMAXDIFFERENTIALVALUE((MaxAllowedValue)))


/*! \def        OS_TIMERHRT_GETMAXDIFFERENTIALVALUE()
 *  \brief      This macro is used to calculate the maximum differential value between two HRT values.
 *  \details    --no details--
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERHRT_GETMAXDIFFERENTIALVALUE(MaxAllowedValue)                                                          \
      ((((Os_TickType)(MaxAllowedValue))*(Os_TickType)2)+(Os_TickType)1)


/*! \def        OS_TIMERPFRT_GETMAXDIFFERENTIALVALUE()
 *  \brief      This macro is used to calculate the maximum differential value between two PFRT values.
 *  \details    --no details--
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPFRT_GETMAXDIFFERENTIALVALUE(MaxAllowedValue)                                                         \
      (OS_TIMERSW_GETMAXDIFFERENTIALVALUE((MaxAllowedValue)))


/*! \def        OS_TIMERPFRT_HARDWAREGETMAXDIFFERENTIALVALUE()
 *  \brief      This macro is used to calculate the maximum differential value between two PFRT values (hardware part).
 *  \details    --no details--
 *
 *  \param[in]  MaxAllowedValue   Max allowed value of the counter.
 */
# define  OS_TIMERPFRT_HARDWAREGETMAXDIFFERENTIALVALUE(MaxAllowedValue)                                                 \
      (OS_TIMERHRT_GETMAXDIFFERENTIALVALUE((MaxAllowedValue)))


/*! \def        OS_TIMER_GETTICKTYPEQUARTER()
 *  \brief      This macro provides the quarter of tick type values, the parameter belongs to.
 *  \details    --no details--
 *
 *  \param[in]  Ticks The tick count which shall be checked.
 */
# define OS_TIMER_GETTICKTYPEQUARTER(Ticks)((Ticks) & OS_OS_TICKTYPE_QUARTER_MASK)


/*! Type cast from Os_TimerIsrConfigType to Os_IsrConfigType by use of base element addressing. */
#define  OS_TIMER_CASTCONFIG_TIMERISR_2_ISR(Timer)   (&((Timer).Isr))


/*! Type cast from Os_IsrType to Os_ThreadType by use of base element addressing. */
#define OS_ISR_CASTDYN_TIMERSW_2_TIMERPFRT(Timer)    (&((Timer).Base))


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/*! Dynamic information of a software counter. */
typedef struct
{
  /*! The current counter value of the software counter. */
  Os_TickType Value;

  /*! The next time stamp where Os_CounterWorkJobs() has to be called in case that collection type of counter is a
      priority queue. In case of an array list this value is not updated or used. */
  Os_TickType Compare;
} Os_TimerSwType;


/*! Timer configuration for software counters.
 *  \extends Os_CounterConfigType_Tag
 */
struct Os_TimerSwConfigType_Tag
{
  /*! General counter.
   * This attribute must come first! */
  Os_CounterConfigType Counter;

  /*! Dynamic part of the software counter. */
  P2VAR(Os_TimerSwType, TYPEDEF, OS_VAR_NOINIT) Dyn;
};


/*! Configuration information of a PIT driven hardware counter.
 *  \extends Os_TimerSwConfigType_Tag
 */
struct Os_TimerPitConfigType_Tag
{
  /*! Software counter.
   * This attribute must come first! */
  Os_TimerSwConfigType SwCounter;

  /*! Hardware specific information of the PIT. */
  P2CONST(Os_Hal_TimerPitConfigType, TYPEDEF, OS_CONST) HwConfig;
};



/*! Dynamic information of a (range extended) HRT driven hardware counter. */
typedef struct
{
  /*! A snapshot of the range extended counter value.
   *  The current value is calculated each time in Os_TimerHrtGetValue(). */
  Os_TickType Value;

  /*! The next time stamp where Os_CounterWorkJobs() has to be called. */
  Os_TickType Compare;

  /*! Time value mask. Set to 0 on timer initialization, set to MaxCountingValue on timer start.
   *  Used to keep the Value field at zero until the timer is started by Os_TimerHrtStart() */
  Os_TickType ValueMask;

  /*! Time stamp of the last hardware counter read. */
  Os_TickType HwLastCounterValue;

  /*! The deviation between TickType values and FrtTickType values.
   *  This value has to be added when converting a TickType value into a FRT TickType value. */
  Os_TickType HwTimeAtLogicalZero;

} Os_TimerHrtType;



/*! Timer configuration of a (software extended) HRT driven hardware counter.
 *  \extends Os_CounterConfigType_Tag
 */
struct Os_TimerHrtConfigType_Tag
{
  /*! General counter.
   * This attribute must come first! */
  Os_CounterConfigType Counter;

  /*! Dynamic information. */
  P2VAR(Os_TimerHrtType, TYPEDEF, OS_VAR_NOINIT) Dyn;

  /*! Hardware specific information of the FRT. */
  P2CONST(Os_Hal_TimerFrtConfigType, TYPEDEF, OS_CONST) HwConfig;
};


/*! Dynamic information of a PFRT driven hardware counter. */
typedef struct
{
  /*! Dynanmic data of the base class.
   * This attribute must come first! */
  Os_TimerSwType Base;

  /*! Helds a copy of the compare value stored in hardware. */
  Os_TickType Compare;
} Os_TimerPfrtType;


/*! Configuration information of a PFRT driven hardware counter.
 *  \extends Os_TimerSwConfigType_Tag
 */
struct Os_TimerPfrtConfigType_Tag
{
  /*! Software counter.
   * This attribute must come first! */
  Os_TimerSwConfigType SwCounter;

  /*! The period of the PFRT in ticks. */
  Os_TickType Period;

  /*! The maximum allowed difference between to related tick times. */
  Os_TickType MaxDifferentialValue;

  /*! Specifies the highest counter value which occurs in hardware. */
  Os_TickType MaxCountingValue;

  /*! Hardware specific information of the FRT. */
  P2CONST(Os_Hal_TimerFrtConfigType, TYPEDEF, OS_CONST) HwConfig;
};


/*! Configuration information of a timer ISR.
 *  \extends Os_IsrConfigType_Tag */
struct Os_TimerIsrConfigType_Tag
{
  /*! The timer ISR which is responsible for the counter. */
  Os_IsrConfigType Isr;

  /*! The hardware counter, which triggers the timer ISR. */
  P2CONST(Os_CounterConfigType, TYPEDEF, OS_CONST) Counter;
};


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_TimerSwInit()
 **********************************************************************************************************************/
/*! \brief          Initializes a software timer.
 *  \details        Initialize counter value to zero.
 *
 *  \param[in,out]  Timer           The timer to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerSwInit,
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPitInit()
 **********************************************************************************************************************/
/*! \brief          Initializes a PIT driven counter.
 *  \details        Initialize software counter value and PIT hardware.
 *
 *  \param[in,out]  Timer           The timer to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitInit,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerHrtInit()
 **********************************************************************************************************************/
/*! \brief          Initializes HRT driven counter.
 *  \details        Initialize HRT hardware and the software part for range extension.
 *
 *  \param[in,out]  Timer           The timer to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtInit,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPfrtInit()
 **********************************************************************************************************************/
/*! \brief          Initializes periodic free running timer.
 *  \details        Initialize FRT hardware and the related software timer.
 *
 *  \param[in,out]  Timer           The timer to initialize. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtInit,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerSwStart()
 **********************************************************************************************************************/
/*! \brief          Starts a software timer.
 *  \details        There is nothing to be done for starting a software timer. Therefore, no preconditions.
 *
 *  \param[in,out]  Timer           The timer to initialize.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerSwStart,
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPitStart()
 **********************************************************************************************************************/
/*! \brief          Starts a PIT driven counter.
 *  \details        Initialize the PIT hardware.
 *
 *  \param[in,out]  Timer           The timer to start. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *
 *  \trace          SPEC-64005
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitStart,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerHrtStart()
 **********************************************************************************************************************/
/*! \brief          Starts a HRT driven counter.
 *  \details        Initialize HRT hardware.
 *
 *  \param[in,out]  Timer           The timer to start. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *
 *  \trace          SPEC-64005
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtStart,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerSwGetValue()
 **********************************************************************************************************************/
/*! \brief          Returns the current counter value of a software timer.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to query. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given timer shall be prevented, if processor word width is smaller than
 *                  with of Os_TickType.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerSwGetValue,
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerSwSetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Sets the compare value variable of a software timer.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *  \param[in]      ExpirationTime  The time stamp, which defines when the timer shall call Os_CounterWorkJobs().
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_TimerSwSetCompareValue,
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType ExpirationTime
));


/***********************************************************************************************************************
 *  Os_TimerSwUserModulo()
 **********************************************************************************************************************/
/*! \brief          Performs Value = Value mod MaxAllowedValue.
 *  \details        Because the modulo operator (%) is assumed to be inefficient, an optimized modulo function is used,
 *                  which has the assumption on the parameter Value as defined in the precondition. The optimization
 *                  is to simply subtract MaxAllowedValue+1 in case the value is bigger than MaxAllowedValue.
 *
 *  ![Visualization](SoftwareCounterModulo.png)
 *
 *  \param[in]      Timer     The timer to query. Parameter must not be NULL.
 *  \param[in]      Value     The value to be limited.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Value has to be in range 0 <= Value <= MaxCountingValue.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE, Os_TimerSwUserModulo,
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType Value
));


/***********************************************************************************************************************
 *  Os_TimerSwIncrement()
 **********************************************************************************************************************/
/*! \brief          Increment a given software timer.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to be incremented. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Timer belongs to the local core.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_TimerSwIncrement
(
  P2CONST(Os_TimerSwConfigType, AUTOMATIC, OS_CONST) Timer
);


/***********************************************************************************************************************
 *  Os_TimerPfrtStart()
 **********************************************************************************************************************/
/*! \brief          Starts a PFRT driven counter.
 *  \details        Initialize the underlying FRT hardware.
 *
 *  \param[in,out]  Timer           The timer to start. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *
 *  \trace          SPEC-64005
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_TimerPfrtStart
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
);


/***********************************************************************************************************************
 *  Os_TimerPitGetValue()
 **********************************************************************************************************************/
/*! \brief          Returns the current counter value of a PIT driven counter.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to query. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given timer shall be prevented, if processor word width is smaller than
 *                  with of Os_TickType.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitGetValue,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPitSetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Sets the compare value variable of a PIT driven counter.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *  \param[in]      ExpirationTime  The time stamp, which defines when the timer shall call Os_CounterWorkJobs().
 *
 *  \retval         !0  ExpirationTime is in the past or now.
 *  \retval         0   ExpirationTime is in the future.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitSetCompareValue,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType ExpirationTime
));


/***********************************************************************************************************************
 *  Os_TimerPitUserModulo()
 **********************************************************************************************************************/
/*! \brief          Performs Value = Value mod MaxAllowedValue.
 *  \details        Modulo function for periodic interrupt timer.
 *
 *  \param[in]      Timer     The counter to query. Parameter must not be NULL.
 *  \param[in]      Value     The value to be limited.
 *                            Value has to be in range -MaxAllowedValue <= Value <= MaxCountingValue.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitUserModulo,
(
  P2CONST(Os_TimerPitConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType Value
));


/***********************************************************************************************************************
 *  Os_TimerPfrtUserModulo()
 **********************************************************************************************************************/
/*! \brief          Performs Value = Value mod MaxAllowedValue.
 *  \details        Modulo function for periodic free running timer.
 *
 *  \param[in]      Timer     The counter to query. Parameter must not be NULL.
 *  \param[in]      Value     The value to be limited.
 *                            Value has to be in range -MaxAllowedValue <= Value <= MaxCountingValue.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtUserModulo,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType Value
));


/***********************************************************************************************************************
 *  Os_TimerHrtGetValue()
 **********************************************************************************************************************/
/*! \brief          Returns the current value of a high resolution timer driven counter.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to query. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            The ValueMask of the given counter is zero or the maximum possible value in TickType
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtGetValue,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerPfrtGetValue()
 **********************************************************************************************************************/
/*! \brief          Returns the current value of a periodic free running timer driven counter.
 *  \details        This function returns the value of the software timer.
 *
 *  \param[in,out]  Timer           The timer to query. Parameter must not be NULL.
 *
 *  \return         The software counter value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            The ValueMask of the given counter is zero or the maximum possible value in TickType
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtGetValue,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerHrtGetCounter()
 **********************************************************************************************************************/
/*! \brief          Returns the counter of the given timer.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to query. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_CounterConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_TimerHrtGetCounter,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));

/***********************************************************************************************************************
 *  Os_TimerFrtSetFutureCompareValue()
 **********************************************************************************************************************/
/*! \brief          Sets the compare value of a FRT driven counter into the future.
 *  \details        This function is used if the platform does not support triggering of peripheral interrupts.
 *                  The following algorithm is then used to set the compare value:
 *
 *                  1- Read the current timestamp (timeBeforeSetCompare)
 *                  2- Set the compare value to timeBeforeSetCompare + delta + iterationId
 *                  3- Read again the timestamp (timeAfterSetCompare)
 *                  4- Update delta with the timestamps difference and increment iterationId
 *                  5- Loop until the compare value is in the future or now with respect to timeAfterSetCompare
 *
 *                  If the compare value can not be updated successfully in a certain time, the status E_OS_SYS_TIMEOUT
 *                  is reported to the ErrorHook if configured.
 *
 *  \param[in]      TimerConfig           Hardware timer configuration. Parameter must not be NULL.
 *  \param[in]      MaxCountingValue      The upper limit of Value and Reference Value.
 *  \param[in]      MaxDifferentialValue  Maximum difference between Value and Reference Value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
Os_TimerFrtSetFutureCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig,
  Os_TickType MaxCountingValue,
  Os_TickType MaxDifferentialValue
));


/***********************************************************************************************************************
 *  Os_TimerHrtComputeHardwareCompareValue()
 **********************************************************************************************************************/
/*! \brief          This function returns the compare value which needs to be written to the timers hardware register.
 *  \details        This calculation is needed for systems where the physical timer register size is smaller then the
 *                  OS TickType. In this case it is possible that we need intermediate interrupts to handle the
 *                  requested time span. So the compare value that needs to be written to the hardware register may
 *                  differ from the requested expiration time. See \ref Os_TimerIsFrtShortCorrectionNeeded().
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *  \param[in]      ExpirationTime  The time stamp, where the next interrupt shall occur.
 *
 *  \return         The value that needs to be written to the timers compare register.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtComputeHardwareCompareValue,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType ExpirationTime
));


/***********************************************************************************************************************
 *  Os_TimerHrtInitCompareValue()
 **********************************************************************************************************************/
/*! \brief          Initializes the compare value of the given HRT to the given value.
 *  \details        Only initializes the compare value in the dynamic object of the timer. The compare value is not
 *                  written to hardware.
 *
 *  \param[in,out]  Timer         The timer to modify. Parameter must not be NULL.
 *  \param[in]      CompareValue  The compare value to set.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtInitCompareValue,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType CompareValue
));


/***********************************************************************************************************************
 *  Os_TimerHrtSetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Sets the compare value of a HRT driven counter.
 *  \details        If the ExpirationTime is the past or now, a peripheral interrupt has to be triggered: in the
 *                  case the platform does not support this feature following algorithm is used to set compare value:
 *
 *                  1- Read the current timestamp (timeBeforeSetCompare)
 *                  2- Set the compare value to timeBeforeSetCompare + delta + iterationId
 *                  3- Read again the timestamp (timeAfterSetCompare)
 *                  4- Update delta with the timestamps difference and increment iterationId
 *                  5- Loop until the compare value is in the future or now with respect to timeAfterSetCompare
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *  \param[in]      ExpirationTime  The time stamp, where the next interrupt shall occur.
 *                                  ExpirationTime-Now is in range -MaxAllowedValue to +MaxAllowedValue.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtSetCompareValue,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType ExpirationTime
));


/***********************************************************************************************************************
 *  Os_TimerPfrtSetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Sets the compare value of a Periodic FRT driven counter.
 *  \details        This function modifies the software timer of the PFRT.
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *  \param[in]      ExpirationTime  The time stamp, where the next interrupt shall occur.
 *                                  ExpirationTime-Now is in range -MaxAllowedValue to +MaxAllowedValue.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtSetCompareValue,
(
  P2CONST(Os_TimerPfrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType ExpirationTime
));


/***********************************************************************************************************************
 *  Os_TimerHrtGetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Get the compare value of a HRT driven counter.
 *  \details        --no details--
 *
 *  \param[in,out]  Timer           The timer to modify. Parameter must not be NULL.
 *
 *  \return         The next expiration time.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different timers.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtGetCompareValue,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerHrtUserModulo()
 **********************************************************************************************************************/
/*! \brief          Performs Value = Value mod MaxAllowedValue.
 *  \details        Cut off bits which are beyond MaxAllowedValue.
 *
 *  ![Visualization](HrtCounterModulo.png)
 *
 *  \param[in]      Timer     The timer to query. Parameter must not be NULL.
 *  \param[in]      Value     The value to be limited.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            MaxAllowedValue is 2^(n)-1.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtUserModulo,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer,
  Os_TickType Value
));


/***********************************************************************************************************************
 *  Os_TimerHrtAcknowledge()
 **********************************************************************************************************************/
/*! \brief          Acknowledges a HRT timer interrupt.
 *  \details        --no details--
 *
 *  \param[in]      Timer     The timer to acknowledge. Parameter must not be NULL
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtAcknowledge,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerHrtIsLogicalCompareValueReached()
 **********************************************************************************************************************/
/*! \brief          The function is needed for HRT timer interrupt handling and evaluates if the requested time span
 *                  has been elapsed.
 *  \details        This check is needed for systems where the physical timer register size is smaller then the
 *                  OS TickType. In this case it is possible that we need intermediate interrupts to handle the
 *                  requested time span. See \ref Os_TimerIsFrtShortCorrectionNeeded().
 *
 *  \param[in/out]  Timer
 *
 *  \retval         !0    The logical compare value is reached.
 *  \retval         0     The logical compare value is not reached.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtIsLogicalCompareValueReached,
(
  P2CONST(Os_TimerHrtConfigType, AUTOMATIC, OS_CONST) Timer
));


/***********************************************************************************************************************
 *  Os_TimerSwAbsCounter2AbsTimerValue()
 **********************************************************************************************************************/
/*! \brief        Converts an absolute counter time stamp into an absolute SW timer time stamp.
 *  \details      As there are two counter cycles within one SW Timer cycle, the counter value may be mapped to
 *                two different timer values:
 *
 *                  timerValue = CounterValue + n*(MaxAllowedValue+1) with n={0,1}
 *
 *                   Cycle             |   Value range
 *                   ------------------|-----------------
 *                   First cycle       |  0 .. MaxAllowedValue
 *                   Second cycle      |  MaxAllowedValue+1 .. 2*MaxAllowedValue+1
 *
 *                As a consequence of #40 and #60, the expiry time will be reached up to MaxAllowedValue+1 ticks
 *                ahead of Now. So this function must not be called while still jobs for Now are in the job queue!
 *
 *  \param[in]    MaxAllowedValue   The maximum value allowed value for CounterValue.
 *  \param[in]    Now               The current timer value. Now must be lower than or equal to MaxCountingValue.
 *  \param[in]    CounterValue      The counter value which shall be converted. CounterValue must be in
 *                                  range 0..MaxAllowedValue of the respective counter configuration.
 *
 *  \return       The absolute software timer value in range 0 <= Result <= MaxCountingValue
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          All jobs which have to expire Now have already been removed from the job queue.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerSwAbsCounter2AbsTimerValue,
(
  Os_TickType MaxAllowedValue,
  Os_TickType Now,
  Os_TickType CounterValue
));


/***********************************************************************************************************************
 *  Os_TimerPitAbsCounter2AbsTimerValue()
 **********************************************************************************************************************/
/*! \brief        Converts an absolute counter time stamp into an absolute PIT timer time stamp.
 *  \details      Bypass conversion to Os_TimerSwAbsCounter2AbsTimerValue().
 *
 *  \param[in]    MaxAllowedValue   The maximum value allowed value for CounterValue.
 *  \param[in]    Now               The current timer value. Now must be lower than or equal to MaxCountingValue.
 *  \param[in]    CounterValue      The counter value which shall be converted. CounterValue must be in
 *                                  range 0..MaxAllowedValue of the respective counter configuration.
 *
 *  \return       The absolute PIT value in range 0 <= Result <= MaxCountingValue
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          All jobs which have to expire Now have already been removed from the job queue.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPitAbsCounter2AbsTimerValue,
(
  Os_TickType MaxAllowedValue,
  Os_TickType Now,
  Os_TickType CounterValue
));


/***********************************************************************************************************************
 *  Os_TimerHrtAbsCounter2AbsTimerValue()
 **********************************************************************************************************************/
/*! \brief        Converts an absolute counter time stamp into an absolute HRT timer time stamp.
 *  \details      As there are four counter cycles within one (E)HRT cycle, the counter value may be mapped to
 *                four different timer values. The two MSbs of the timer value represent the timer cycle,
 *                the LSbs represent the counter value.
 *
 *                timerValue = |n|CounterValue|; \\ Bit mapping
 *                or
 *                timerValue = n*counterPeriode + CounterValue  with n={0,..,3}
 *
 *                Where n equals the upper two bits of the timerValue.
 *
 *  \param[in]    MaxAllowedValue   The maximum value allowed value for CounterValue. MaxAllowedValue must be
 *                                  (2^n)-1, where n is a native number and MaxAllowedValue <= Max(TickType)/4.
 *  \param[in]    MaxCountingValue  The max. value of the underlying timer. MaxCountingValue must be (2^n)-1, where n
 *                                  is a native number and MaxCountingValue <= Max(TickType)/2.
 *  \param[in]    Now               The current timer value. Now must be lower than or equal to MaxCountingValue.
 *  \param[in]    CounterValue      The counter value which shall be converted. CounterValue must be in
 *                                  range 0..MaxAllowedValue
 *
 *  \return       The absolute HRT value in range 0 <= Result <= MaxCountingValue
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerHrtAbsCounter2AbsTimerValue,
(
  Os_TickType MaxAllowedValue,
  Os_TickType MaxCountingValue,
  Os_TickType Now,
  Os_TickType CounterValue
));


/***********************************************************************************************************************
 *  Os_TimerPfrtAbsCounter2AbsTimerValue()
 **********************************************************************************************************************/
/*! \brief        Converts an absolute counter time stamp into an absolute PFRT time stamp.
 *  \details      Bypass conversion to Os_TimerSwAbsCounter2AbsTimerValue().
 *
 *  \param[in]    MaxAllowedValue   The maximum value allowed value for CounterValue.
 *  \param[in]    Now               The current timer value. Now must be lower than or equal to MaxCountingValue.
 *  \param[in]    CounterValue      The counter value which shall be converted. CounterValue must be in
 *                                  range 0..MaxAllowedValue of the respective counter configuration.
 *
 *  \return       The absolute software timer value in range 0 <= Result <= MaxCountingValue
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          All jobs which have to expire Now have already been removed from the job queue.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerPfrtAbsCounter2AbsTimerValue,
(
  Os_TickType MaxAllowedValue,
  Os_TickType Now,
  Os_TickType CounterValue
));


/***********************************************************************************************************************
 *  Os_TimerAdd()
 **********************************************************************************************************************/
/*! \brief          Performs Value = (Value+Add) mod MaxCountingValue+1.
 *  \details        --no details--
 *
 *  ![Visualization](TimerAdd.png)
 *
 *  \param[in]      MaxCountingValue      The counting limit of the underlying timer.
 *  \param[in]      Value                 A timer value.
 *  \param[in]      Add                   The counter value to be added.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerAdd,
(
  Os_TickType MaxCountingValue,
  Os_TickType Value,
  Os_TickType Add
));


/***********************************************************************************************************************
 *  Os_TimerSub()
 **********************************************************************************************************************/
/*! \brief          Performs Value = (Value-Sub) mod MaxCountingValue+1.
 *  \details        --no details--
 *
 *  ![Visualization](TimerSub.png)
 *
 *  \param[in]      MaxCountingValue      The counting limit of the underlying timer.
 *  \param[in]      Value                 A timer value.
 *                                        Value must be in range 0 <= Value <= MaxCountingValue.
 *  \param[in]      Sub                   The counter value to be subtracted.
 *                                        Sub must be in range 0 <= Sub   <= MaxCountingValue.
 *
 *  \return         The limited value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerSub,
(
  Os_TickType MaxCountingValue,
  Os_TickType Value,
  Os_TickType Sub
));


/***********************************************************************************************************************
 *  Os_TimerFrtSetCompareValue()
 **********************************************************************************************************************/
/*! \brief          Update the compare value of a Free Running Timer.
 *  \details        Additional correction of the hardware compare value in case the hardware timer type is larger then
 *                  the OS TickType. After the write access, the compare value is read back to ensure that it is written
 *                  correctly. If the compare value is not written correctly after a certain time,
 *                  OS_STATUS_FRT_SET_COMPARE_VALUE_FAILED is reported to the ErrorHook.
 *
 *  \param[in]      TimerConfig    Hardware timer configuration. Parameter must not be NULL.
 *  \param[in]      CompareValue   Compare value to set.
 *                                 CompareValue-Now is in range -MaxAllowedValue to +MaxAllowedValue.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerFrtSetCompareValue,
(
  P2CONST(Os_Hal_TimerFrtConfigType, AUTOMATIC, OS_CONST) TimerConfig,
  Os_TickType CompareValue
));


/***********************************************************************************************************************
 *  Os_TimerIsFrtLongCorrectionNeeded()
 **********************************************************************************************************************/
/*! \brief          Returns whether or not an adaption of the hardware timer value is needed.
 *  \details        A correction is needed if the hardware register bit width is larger then the bit width of the OS
 *                  TickType.
 *
 *  \return         !0  Correction for larger hardware timer type is needed.
 *                   0  Correction is NOT needed.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerIsFrtLongCorrectionNeeded,(void));


/***********************************************************************************************************************
 *  Os_TimerIsFrtShortCorrectionNeeded()
 **********************************************************************************************************************/
/*! \brief          Returns whether or not an adaption of the hardware timer value is needed.
 *  \details        A correction is needed if the hardware register bit width is smaller then the bit width of the OS
 *                  TickType.
 *
 *  \return         !0  Correction for smaller hardware timer type is needed.
 *                   0  Correction is NOT needed.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerIsFrtShortCorrectionNeeded,(void));


/***********************************************************************************************************************
 *  Os_TimerFrtTickType2TickType()
 **********************************************************************************************************************/
/*! \brief          Converts the given FrtTickType into an OS TickType.
 *  \details        --no details--
 *
 *  \param[in]      Value           The FrtTickType value.
 *
 *  \return         Value converted to OS TickType.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_TickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerFrtTickType2TickType,
(
  Os_Hal_TimerFrtTickType Value
));


/***********************************************************************************************************************
 *  Os_TimerTickType2FrtTickType()
 **********************************************************************************************************************/
/*! \brief          Converts the given OS TickType into an FrtTickType.
 *  \details        --no details--
 *
 *  \param[in]      Value           The TickType value.
 *
 *  \return         Value converted to hardware TickType.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerTickType2FrtTickType,
(
  Os_TickType Value
));


/***********************************************************************************************************************
 *  Os_TimerIsCompareValueCorrectionNeeded()
 **********************************************************************************************************************/
/*! \brief          Returns whether or not a compare value would overflow.
 *  \details        --no details--
 *
 *  \param[in]      NowValue       Now value.
 *  \param[in]      CompareValue   Compare value.
 *                                 CompareValue-NowValue is in range -MaxAllowedValue to +MaxAllowedValue.
 *
 *  \return       !0    Compare value correction is needed.
 *                 0    Compare value correction is NOT needed.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerIsCompareValueCorrectionNeeded,
(
  Os_TickType NowValue,
  Os_TickType CompareValue
));


/***********************************************************************************************************************
 *  Os_TimerFrtGetLongValueMask()
 **********************************************************************************************************************/
/*! \brief          Returns the bit mask to access the upper bits of a hardware timer value.
 *  \details        Returns a bit mask for the case that the hardware timer type is larger than the OS TickType.
 *
 *  \return         The bit mask to access the upper part of a hardware timer value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_Hal_TimerFrtTickType, OS_CODE, OS_ALWAYS_INLINE,
Os_TimerFrtGetLongValueMask,(void));


/***********************************************************************************************************************
 *  ISR(Os_TimerPitIsr)
 **********************************************************************************************************************/
/*! \brief        ISR for PIT timers.
 *  \details      This ISR drives a PIT hardware counter.
 *                The ISR acknowledges the hardware timer and increments the underlying software counter value.
 *                The counter will check, whether there are expired jobs and work them off.
 *
 *  \context      ISR2|OS_INTERNAL
 *
 *  \reentrant    TRUE, for different timers.
 *  \synchronous  TRUE
 *
 *  \pre          Timer belongs to the local core.
 *  \pre          Timer is a PIT counter.
 **********************************************************************************************************************/
ISR(Os_TimerPitIsr);


/***********************************************************************************************************************
 *  ISR(Os_TimerHrtIsr)
 **********************************************************************************************************************/
/*! \brief        ISR for HRT timers.
 *  \details      This ISR drives a HRT hardware counter.
 *                This ISR acknowledges the hardware timer and informs the counter to work off expired jobs.
 *
 *  \context      ISR2|OS_INTERNAL
 *
 *  \reentrant    TRUE, for different timers.
 *  \synchronous  TRUE
 *
 *  \pre          Timer belongs to the local core.
 *  \pre          Timer is a HRT counter.
 **********************************************************************************************************************/
ISR(Os_TimerHrtIsr);


/***********************************************************************************************************************
 *  ISR(Os_TimerPfrtIsr)
 **********************************************************************************************************************/
/*! \brief        ISR for PFRT timers.
 *  \details      This ISR drives a PFRT hardware counter.
 *                This ISR acknowledges the hardware timer and informs the counter to work off expired jobs.
 *
 *  \context      ISR2|OS_INTERNAL
 *
 *  \reentrant    TRUE, for different timers.
 *  \synchronous  TRUE
 *
 *  \pre          Timer belongs to the local core.
 *  \pre          Timer is a PFRT counter.
 **********************************************************************************************************************/
ISR(Os_TimerPfrtIsr);


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_TIMERINT_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_TimerInt.h
 **********************************************************************************************************************/
