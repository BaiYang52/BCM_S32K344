/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  WdgIf.c
 *        \brief  WdgIf source file
 *
 *      \details  This is the implementation of the module WdgIf
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

#define WDGIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "WdgIf.h"
#if (WDGIF_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif /* (WDGIF_DEV_ERROR_DETECT == STD_ON) */


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

#define WDGIF_SW_MAJOR_VERSION_SOURCE             (5u)
#define WDGIF_SW_MINOR_VERSION_SOURCE             (5u)
#define WDGIF_SW_PATCH_VERSION_SOURCE             (1u)

/* Protection against incorrect include file version (WdgIf.h) */
#if ( (WDGIF_SW_MAJOR_VERSION != WDGIF_SW_MAJOR_VERSION_SOURCE) \
   || (WDGIF_SW_MINOR_VERSION != WDGIF_SW_MINOR_VERSION_SOURCE) \
   || (WDGIF_SW_PATCH_VERSION != WDGIF_SW_PATCH_VERSION_SOURCE) )
# error "Version mismatch! Incorrect version of WdgIf.h"
#endif

#if (WDGIF_USE_STATECOMBINER == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Status for the Watchdog Interface - StateCombiner */
# define WDGIF_STATECOMBINER_OK                   (0u)      /*!< StateCombiner status OK */
# define WDGIF_STATECOMBINER_NOT_OK               (1u)      /*!< StateCombiner status NOT OK */
# define WDGIF_STATECOMBINER_RESET                (2u)      /*!< StateCombiner status issue a RESET */

# define SYNC_COUNT_INIT_VAL                      (0xFFFFu) /*!< Invalid counter value */


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

# if (WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK == STD_ON)
#  define STATECOMBINER_GETSPINLOCK(_id) (void)GetSpinlock(_id) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define STATECOMBINER_RELEASESPINLOCK(_id) (void)ReleaseSpinlock(_id) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else
#  define STATECOMBINER_GETSPINLOCK(_id) (void)Appl_GetSpinlock(_id) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define STATECOMBINER_RELEASESPINLOCK(_id) (void)Appl_ReleaseSpinlock(_id) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif /* (WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK == STD_ON) */


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if !defined (WDGIF_LOCAL) /* COV_WDGIF_COMPATIBILITY */
#  define WDGIF_LOCAL static
# endif


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define WDGIF_START_SEC_VAR_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087  */ /* MD_MSR_19.1  */

/*! Global variable to save the current state combiner status of the Watchdog */
WDGIF_LOCAL VAR(uint8, WDGIF_VAR) g_wdgif_sc_status = WDGIF_STATECOMBINER_OK;

/*! Global variable to save the inverted value of the current state combiner status of the Watchdog */
WDGIF_LOCAL VAR(uint8, WDGIF_VAR) g_wdgif_sc_status_INV = (uint8)(~(uint8)WDGIF_STATECOMBINER_OK);

# define WDGIF_STOP_SEC_VAR_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


# define WDGIF_START_SEC_VAR_INIT_16BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Global variable to save the cycle counter of the master SC device */
WDGIF_LOCAL VAR(uint16, WDGIF_VAR) g_wdgif_sc_cycle_counter[WDGIF_MAX_SUPPORTED_CORES - 1u] = /* PRQA S 3218 */ /* MD_WdgIf_3218 */
{
  0u,
  0u,
  0u,
  0u,
  0u,
  0u,
  0u,
  0u,
  0u
};

/*! Global variable to save the inverted value of the cycle counter of the master SC device */
WDGIF_LOCAL VAR(uint16, WDGIF_VAR) g_wdgif_sc_cycle_counter_INV[WDGIF_MAX_SUPPORTED_CORES - 1u] = /* PRQA S 3218 */ /* MD_WdgIf_3218 */
{
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u),
  (uint16)(~(uint16) 0u)
};

/*! Global variable to save the synchronization cycle count */
WDGIF_LOCAL VAR(uint16, WDGIF_VAR) g_wdgif_sc_sync_count = SYNC_COUNT_INIT_VAL; /* PRQA S 3218 */ /* MD_WdgIf_3218 */

/*! Global variable to save the inverted value of the synchronization cycle count */
WDGIF_LOCAL VAR(uint16, WDGIF_VAR) g_wdgif_sc_sync_count_INV = (uint16)(~(uint16)SYNC_COUNT_INIT_VAL); /* PRQA S 3218 */ /* MD_WdgIf_3218 */

# define WDGIF_STOP_SEC_VAR_INIT_16BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define WDGIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  wdgif_statecombiner_check_slave_timing()
 *********************************************************************************************************************/
/*! \brief        Checks the slave timing in case of enabled state combiner
 *  \details      wdgif_check_slave_timing monitors the timing of the configured slave State Combiner instance(s)
 *                execution and reports its state to the upper layer.
 *  \param[in]    StateCombinerConfigPtr     Pointer to the WdgIf configuration
 *  \return       WDGIF_STATECOMBINER_OK      State Combiner function executed successfully
 *  \return       WDGIF_STATECOMBINER_NOT_OK  Timing error for slave(s) detected
 *  \return       WDGIF_STATECOMBINER_RESET   Unrecoverable error detected, reset issued
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       WDGIF_USE_STATECOMBINER is STD_ON
 *  \trace        DSGN-WdgIf22756, DSGN-WdgIf22804, DSGN-WdgIf22805
 */
WDGIF_LOCAL FUNC(uint8, WDGIF_CODE) wdgif_statecombiner_check_slave_timing
(
  P2CONST(WdgIf_StateCombinerConfigType, AUTOMATIC, WDGIF_APPL_CONST) StateCombinerConfigPtr
);

/**********************************************************************************************************************
 *  wdgif_statecombiner_setmode()
 *********************************************************************************************************************/
/*! \brief        Sets the watchdog mode in case of enabled state combiner
 *  \details      wdgif_statecombiner_setmode switches the watchdog mode of the corresponding State Combiner instance
 *                according to the parameter DeviceIndex.
 *  \param[in]    WdgInstance   Index of the State Combiner instance for which the function is called
 *  \param[in]    Mode          Watchdog mode which is being requested
 *  \return       E_OK      State Combiner function invoked and executed successfully
 *  \return       E_NOT_OK  Error detected or driver function returned an error
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       WDGIF_USE_STATECOMBINER is STD_ON
 *  \trace        DSGN-WdgIf22749
 */
WDGIF_LOCAL FUNC(Std_ReturnType, WDGIF_CODE) wdgif_statecombiner_setmode
(
  VAR(uint8, AUTOMATIC)          WdgInstance,
  VAR(WdgIf_ModeType, AUTOMATIC) Mode
);

/**********************************************************************************************************************
 *  wdgif_statecombiner_settriggercondition()
 *********************************************************************************************************************/
/*! \brief        Sets the trigger window in case of enabled state combiner
 *  \details      wdgif_statecombiner_settriggercondition sets a new timeout and new window start and services the
 *                corresponding State Combiner instance according to the parameter DeviceIndex.\n
 *                It sets a new timeout and new window start for the watchdog unit.
 *  \param[in]    WdgInstance
 *  \param[in]    Timeout
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       WDGIF_USE_STATECOMBINER is STD_ON
 *  \trace        DSGN-WdgIf22756, DSGN-WdgIf22804, DSGN-WdgIf22805
 */
WDGIF_LOCAL FUNC(void, WDGIF_CODE) wdgif_statecombiner_settriggercondition
(
  VAR(uint8, AUTOMATIC)  WdgInstance,
  VAR(uint16, AUTOMATIC) Timeout
);


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  wdgif_statecombiner_check_slave_timing()
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 */
WDGIF_LOCAL FUNC(uint8, WDGIF_CODE) wdgif_statecombiner_check_slave_timing
(
  P2CONST(WdgIf_StateCombinerConfigType, AUTOMATIC, WDGIF_APPL_CONST) StateCombinerConfigPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) status_sc;
  VAR(uint8, AUTOMATIC) ret_sc;
  VAR(uint8, AUTOMATIC) i;
  VAR(uint32, AUTOMATIC) slave_counter_value;
  VAR(uint32, AUTOMATIC) min_increments;
  VAR(uint32, AUTOMATIC) max_increments;
  VAR(WdgIf_StateCombinerSpinlockIdType, AUTOMATIC) spinlock_id;
  P2VAR(WdgIf_StateCombinerSharedMemory, AUTOMATIC, WDGIF_APPL_DATA) cfg_sdata;
  P2VAR(WdgIf_StateCombinerSharedMemory, AUTOMATIC, WDGIF_APPL_DATA) slave_sdata;
  P2CONST(WdgIf_StateCombinerSlaveTriggerPatternType, AUTOMATIC, WDGIF_APPL_CONST) slave_trigger_pattern;

  /* ----- Implementation ----------------------------------------------- */
  spinlock_id = StateCombinerConfigPtr->WdgIfStateCombinerSpinlockId;
  cfg_sdata = StateCombinerConfigPtr->WdgIfStateCombinerSData;

  /* #1 Check if global variables are invalid (bit flipping) */
  if ( ((uint8)~g_wdgif_sc_status != g_wdgif_sc_status_INV)
    || ((uint16)~g_wdgif_sc_sync_count != g_wdgif_sc_sync_count_INV)
     )
  {
    /* #11 Aborted and return WDGIF_STATECOMBINER_RESET */
    ret_sc = WDGIF_STATECOMBINER_RESET;
  }
  /* #2 Check if global variables are valid */
  else
  {
    /* #21 Check if global variable for storing the synchronization start-up cycles is set to its initial value */
    if (g_wdgif_sc_sync_count == SYNC_COUNT_INIT_VAL)
    {
      /* #211 Global variable for storing the synchronization start-up cycles is set to value
       *      WdgIfStateCombinerStartUpSyncCycles specified in the configuration structure */
      g_wdgif_sc_sync_count = StateCombinerConfigPtr->WdgIfStateCombinerStartUpSyncCycles + 1u;
      g_wdgif_sc_sync_count_INV = (uint16)(~(g_wdgif_sc_sync_count));
    }

    /* #22 Check if the global variable for storing the synchronization start-up cycles is greater than zero */
    if (g_wdgif_sc_sync_count > 0u)
    {
      /* #221 Decrement global variable for storing the synchronization start-up cycles */
      g_wdgif_sc_sync_count--;
      g_wdgif_sc_sync_count_INV = (uint16)(~(g_wdgif_sc_sync_count));
    }
    
    /* #23 Repeat the following steps for all slave devices */
    status_sc = WDGIF_STATECOMBINER_OK;
    ret_sc = g_wdgif_sc_status;
    i = 0u;
    
    do  /* FETA_WDGIF_STATECOMBINER_CHECK_SLAVE_TIMING */
    {
      slave_sdata = (&cfg_sdata[i]);
      slave_trigger_pattern = StateCombinerConfigPtr->WdgIfStateCombinerSlaveTriggerPattern[i];

      /* #231 Get the spin lock */
      STATECOMBINER_GETSPINLOCK (spinlock_id);

      slave_counter_value = slave_sdata->SlaveCounterValue;
      
      /* #232 Check if global shared variable values are corrupted */
      if ( ((uint16)~g_wdgif_sc_cycle_counter[i] != g_wdgif_sc_cycle_counter_INV[i]) 
        || ((uint32)~slave_counter_value != slave_sdata->SlaveCounterValue_INV)
         )
      {
        /* #2321 Release the spinlock */
        STATECOMBINER_RELEASESPINLOCK (spinlock_id);

        /* #2322 Aborted and return WDGIF_STATECOMBINER_RESET */
        status_sc = WDGIF_STATECOMBINER_RESET;
      }
      /* #233 Check if global synchronization start-up cycles counter is greater than zero */
      else if (g_wdgif_sc_sync_count > 0u)
      {
        /* #2331 Reset value of the slave instance trigger counter to zero */
        slave_sdata->SlaveCounterValue = 0u; /* SBSW_WDGIF_002 */
        slave_sdata->SlaveCounterValue_INV = ~((uint32) 0u); /* SBSW_WDGIF_002 */

        /* #2332 Release the spinlock */
        STATECOMBINER_RELEASESPINLOCK (spinlock_id);
      }
      /* #234 Do the following in all other cases */
      else
      {
        /* #2341 Release the spinlock */
        STATECOMBINER_RELEASESPINLOCK (spinlock_id);

        /* #2342 Increment the number of elapsed master cycles since the last timing check of the slave instance */
        g_wdgif_sc_cycle_counter[i]++; /* SBSW_WDGIF_003 */
        g_wdgif_sc_cycle_counter_INV[i] = (uint16)~g_wdgif_sc_cycle_counter[i]; /* SBSW_WDGIF_003 */

        /* #2343 Check if counter is equal to the predefined State Combiner Reference Cycle of the particular slave */
        if (g_wdgif_sc_cycle_counter[i] == slave_trigger_pattern->WdgIfStateCombinerReferenceCycle)
        {
          /* #23431 Read the expected minimum / maximum State Combiner Slave Increments */
          min_increments = slave_trigger_pattern->WdgIfStateCombinerSlaveIncrementsMin;
          max_increments = slave_trigger_pattern->WdgIfStateCombinerSlaveIncrementsMax;

          /* #23432 Check if the trigger counter of slave State Combiner instance is inside of the min / max range of
           *        the allowed trigger number */
          if ( (slave_counter_value < min_increments)
            || (slave_counter_value > max_increments)
             )
          {
            /* #234321 Set the State Combiner status to WDGIF_STATECOMBINER_NOT_OK if outside of the min / max range */
            status_sc = WDGIF_STATECOMBINER_NOT_OK;
          }
          else /* #23433 Do the following if the trigger counter of slave State Combiner instance is inside of the
                *      min / max range */
          {
            /* #234331 Get the spin lock */
            STATECOMBINER_GETSPINLOCK (spinlock_id);

            /* #234332 Reset slave trigger counter value to zero */
            slave_sdata->SlaveCounterValue = 0u; /* SBSW_WDGIF_002 */
            slave_sdata->SlaveCounterValue_INV = ~((uint32) 0u); /* SBSW_WDGIF_002 */

            /* #234333 Release the spinlock */
            STATECOMBINER_RELEASESPINLOCK (spinlock_id);

            /* #234334 Reset counters for particular slave instance */
            g_wdgif_sc_cycle_counter[i] = 0u; /* SBSW_WDGIF_003 */
            g_wdgif_sc_cycle_counter_INV[i] = (uint16)(~(uint16) 0u); /* SBSW_WDGIF_003 */
          }
        }
      }

      /* check status (with priority), save it if higher than current */
      ret_sc = ((status_sc > ret_sc) ? status_sc : ret_sc);
      /* check next slave instance */
      i++;
    } while ( (i < StateCombinerConfigPtr->WdgIfStateCombinerNumberOfSlaves) && (ret_sc != WDGIF_STATECOMBINER_RESET) );
}

  /* #3 Return currently determined State Combiner status value */
  return ret_sc;
} /* wdgif_statecombiner_check_slave_timing() */ /* PRQA S 6080 */ /* MD_MSR_STMIF  */


/**********************************************************************************************************************
 *  wdgif_statecombiner_setmode()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
WDGIF_LOCAL FUNC(Std_ReturnType, WDGIF_CODE) wdgif_statecombiner_setmode
(
  VAR(uint8, AUTOMATIC)          WdgInstance,
  VAR(WdgIf_ModeType, AUTOMATIC) Mode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) ret = E_OK;
  P2CONST(WdgIf_StateCombinerConfigType, AUTOMATIC, WDGIF_APPL_CONST) state_combiner_config_ptr;

  /* ----- Implementation ----------------------------------------------- */
  state_combiner_config_ptr = WdgIf_Interface.WdgIfStateCombinerConfig;
  /* #1 Check if WdgInstance is equal to zero (master State Combiner instance) */
  if (WdgInstance == 0u)
  {
    /* #11 Call the driver function for performing a mode change and save the return value */
    ret = state_combiner_config_ptr->WdgIfStateCombinerFunctions->Wdg_SetMode_AR (Mode); /* SBSW_WDGIF_005 */
    /* #12 Return the return value provided by the driver */
  }
  /* #2 Check if WdgInstance is not equal to zero (slave State Combiner instance) */
  /* #21 Return E_OK */
  return ret;
} /* wdgif_statecombiner_setmode() */


/**********************************************************************************************************************
 *  wdgif_statecombiner_settriggercondition()
 *********************************************************************************************************************/
/*!
 *
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
 */
WDGIF_LOCAL FUNC(void, WDGIF_CODE) wdgif_statecombiner_settriggercondition
(
  VAR(uint8, AUTOMATIC)  WdgInstance,
  VAR(uint16, AUTOMATIC) Timeout
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(WdgIf_StateCombinerSharedMemory, AUTOMATIC, WDGIF_APPL_DATA) slave_sdata;
  P2CONST(WdgIf_StateCombinerConfigType, AUTOMATIC, WDGIF_APPL_CONST) state_combiner_config_ptr;
  VAR(uint8, AUTOMATIC) ret_sc;
  VAR(WdgIf_StateCombinerSpinlockIdType, AUTOMATIC) spinlock_id;

  /* ----- Implementation ----------------------------------------------- */
  state_combiner_config_ptr = WdgIf_Interface.WdgIfStateCombinerConfig;
  spinlock_id = state_combiner_config_ptr->WdgIfStateCombinerSpinlockId;
  /* #1 Check if WdgInstance is not equal to zero (slave State Combiner instance) */
  if (WdgInstance != 0u)   /* not SC master instance */
  {
    /* #11 Read the address of the appropriate slave State Combiner shared data */
    slave_sdata = &(state_combiner_config_ptr->WdgIfStateCombinerSData[WdgInstance - 1u]);
    /* #12 Get the spinlock */
    STATECOMBINER_GETSPINLOCK (spinlock_id);

    /* #13 Check if Timeout is equal to zero (immediate reset), or shared memory corruption occurred */
    if ( (Timeout == 0u) || ((uint32)~slave_sdata->SlaveCounterValue != slave_sdata->SlaveCounterValue_INV)
       )
    {
      /* #131 Corrupt slave timeout value (request for the master to issue a reset) */
      slave_sdata->SlaveCounterValue = 0u; /* SBSW_WDGIF_002 */
      slave_sdata->SlaveCounterValue_INV = 0u; /* SBSW_WDGIF_002 */
    }
    /* #14 Check if Timeout is not equal to zero and no shared memory corruption occurred */
    else
    {
      /* #141 Increment current slaves trigger counter value */
      slave_sdata->SlaveCounterValue++; /* SBSW_WDGIF_002 */
      slave_sdata->SlaveCounterValue_INV = (uint32)~slave_sdata->SlaveCounterValue; /* SBSW_WDGIF_002 */
    }

    /* #15 Release the spinlock */
    STATECOMBINER_RELEASESPINLOCK (spinlock_id);
  }
  /* #2 Check if WdgInstance is equal to zero (master State Combiner instance) */
  else /* (WdgInstance == 0u) */
  {
    /* #21 Check the timing of all slave devices */
    ret_sc = wdgif_statecombiner_check_slave_timing (state_combiner_config_ptr); /* SBSW_WDGIF_001 */

    /* #22 Check if timeout is equal to zero (immediate reset) or State Combiner status is WDGIF_STATECOMBINER_RESET */
    if ( (Timeout == 0u)
      || (ret_sc == WDGIF_STATECOMBINER_RESET)
       )
    {
      /* #221 Call the "master" wdg function with zero values - "immediate reset" */
      state_combiner_config_ptr->WdgIfStateCombinerFunctions->Wdg_SetTriggerCondition_AR (0u); /* SBSW_WDGIF_005 */
      /* #222 Set the State Combiner status to WDGIF_STATECOMBINER_NOT_OK */
      ret_sc = WDGIF_STATECOMBINER_NOT_OK;
    }
    /* #23 Check if State Combiner status is WDGIF_STATECOMBINER_OK */
    else if (ret_sc == WDGIF_STATECOMBINER_OK)
    {
      /* #231 Call the "master" wdg function - service wdg */
      state_combiner_config_ptr->WdgIfStateCombinerFunctions->Wdg_SetTriggerCondition_AR (Timeout); /* SBSW_WDGIF_005 */
    }
    /* #24 Check if State Combiner status is WDGIF_STATECOMBINER_NOT_OK */
    else
    {
      /* #241 Skip wdg servicing */
    }

    /* #25 Update global State Combiner status */
    g_wdgif_sc_status = ret_sc;
    g_wdgif_sc_status_INV = (uint8)~ret_sc;
  }
} /* wdgif_statecombiner_settriggercondition() */

#else /* if (WDGIF_USE_STATECOMBINER == STD_OFF) */

# define WDGIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  WdgIf_SetMode()
 *********************************************************************************************************************/
/*!
 *
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
FUNC(Std_ReturnType, WDGIF_CODE) WdgIf_SetMode
(
  VAR(uint8, AUTOMATIC)          DeviceIndex,
  VAR(WdgIf_ModeType, AUTOMATIC) WdgMode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) ret;
#if (WDGIF_USE_STATECOMBINER == STD_ON)
  P2CONST(WdgIf_InterfaceFunctionsType, AUTOMATIC, WDGIF_APPL_CONST) wdg_function_address;
#endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  /* #1 Check if the configuration is invalid (whether the array with function pointers per driver is a NULL_PTR) or
   *     if the parameter DeviceIndex is not plausible or
   *     if the function pointer for the selected driver is a NULL_PTR */
  if ( (WdgIf_Interface.WdgFunctionsPerDevice == NULL_PTR)
    || (DeviceIndex >= WdgIf_Interface.NumOfWdgs)
#if (WDGIF_USE_STATECOMBINER == STD_OFF)
    || (WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions == NULL_PTR)
#else /* if (WDGIF_USE_STATECOMBINER == STD_ON) */
    || (WdgIf_Interface.WdgIfStateCombinerConfig == NULL_PTR)
    || (WdgIf_Interface.WdgIfStateCombinerConfig->WdgIfStateCombinerSlaveTriggerPattern == NULL_PTR)
#endif /* (WDGIF_USE_STATECOMBINER == STD_OFF) */
     )
  {
    /* #11 Raise the development error with error code WDGIF_E_PARAM_DEVICE if configured */
    WdgIf_ReportDet(WDGIF_SID_SET_MODE, WDGIF_E_PARAM_DEVICE);
    
    /* #12 Set return value to E_NOT_OK */
    ret = E_NOT_OK;
  }
  /* #2 Check if the configuration is valid */
  else
  {
    /* #21 Check if WDGIF_USE_STATECOMBINER is set to STD_ON */
#if (WDGIF_USE_STATECOMBINER == STD_ON)
    wdg_function_address = WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions;

    /* #22 Check if the function pointer for the selected driver is invalid (NULL_PTR) - WdgIf instance has its own
     *     underlying Wdg */
    if (wdg_function_address == NULL_PTR)
    {
      /* #23 Perform mode change for the addressed State Combiner instance with input parameter values */
      ret = wdgif_statecombiner_setmode (WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgInstance, WdgMode);
    }
    /* #24 Check if the function pointer for the selected driver is a valid function address */
    else
#endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */
    {
      /* #25 Call the corresponding platform-specific implementation */
      ret = WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions->Wdg_SetMode_AR (WdgMode); /* SBSW_WDGIF_005 */
    }
  }

  /* #3 Return the value of the wdg servicing */
  return ret;
} /* WdgIf_SetMode() */


/**********************************************************************************************************************
 *  WdgIf_SetTriggerCondition()
 *********************************************************************************************************************/
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
 */
FUNC(Std_ReturnType, WDGIF_CODE) WdgIf_SetTriggerCondition
(
  VAR(uint8, AUTOMATIC)  DeviceIndex,
  VAR(uint16, AUTOMATIC) Timeout
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) ret;
#if (WDGIF_USE_STATECOMBINER == STD_ON)
  P2CONST(WdgIf_InterfaceFunctionsType, AUTOMATIC, WDGIF_APPL_CONST) wdg_function_address;
#endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */
    
  /* ----- Implementation ----------------------------------------------- */
  /* #1 Check if the configuration is invalid (whether the array with function pointers per driver is a NULL_PTR) or
   *     if the parameter DeviceIndex is not plausible or
   *     if the function pointer for the selected driver is a NULL_PTR */
  if ( (WdgIf_Interface.WdgFunctionsPerDevice == NULL_PTR)
    || (DeviceIndex >= WdgIf_Interface.NumOfWdgs)
#if (WDGIF_USE_STATECOMBINER == STD_OFF)
    || (WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions == NULL_PTR)
#else /* if (WDGIF_USE_STATECOMBINER == STD_ON) */
    || (WdgIf_Interface.WdgIfStateCombinerConfig == NULL_PTR)
    || (WdgIf_Interface.WdgIfStateCombinerConfig->WdgIfStateCombinerSlaveTriggerPattern == NULL_PTR)
#endif /* (WDGIF_USE_STATECOMBINER == STD_OFF) */
     )
  {
    /* #11 Raise the development error with error code WDGIF_E_PARAM_DEVICE if configured */
    WdgIf_ReportDet(WDGIF_SID_SET_TRIGGER_CONDITION, WDGIF_E_PARAM_DEVICE);
    
    /* #12 Set return value to E_NOT_OK */
    ret = E_NOT_OK;
  }
  /* #2 Check if the configuration is valid */
  else
  {
    /* #21 Check if WDGIF_USE_STATECOMBINER is set to STD_ON */
#if (WDGIF_USE_STATECOMBINER == STD_ON)
    wdg_function_address = WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions;

    /* #211 Check if the function pointer for the selected driver is invalid (NULL_PTR) - WdgIf instance has its own
     *     underlying Wdg */
    if (wdg_function_address == NULL_PTR)
    {
      /* #2111 Call state combiner function */
      wdgif_statecombiner_settriggercondition( WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgInstance,
                                                  Timeout
                                                );
    }
    /* #212 Check if the function pointer for the selected driver is a valid function address */
    else
#endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */
    {
      /* #2121 Call the corresponding platform-specific implementation with input parameter values */
      WdgIf_Interface.WdgFunctionsPerDevice[DeviceIndex].WdgFunctions->Wdg_SetTriggerCondition_AR (Timeout); /* SBSW_WDGIF_005 */
    }
    
    /* #22 Set return value to E_OK */
    ret = E_OK;
  }

  /* #3 Return the value of the wdg servicing */
  return ret;
} /* WdgIf_SetTriggerCondition() */


#if (WDGIF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  WdgIf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, WDGIF_CODE) WdgIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, WDGIF_APPL_DATA) versioninfo)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* #1 Check if out parameter is a NULL_PTR */
  if (versioninfo == NULL_PTR)
  {
    /* #11 Raise the development error with error code WDGIF_E_INV_POINTER if configured */
    WdgIf_ReportDet(WDGIF_SID_GET_VERSION_INFO, WDGIF_E_INV_POINTER);
  }
  /* #2 Check if out parameter is a valid pointer (not equal to NULL_PTR) */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #21 Store the version info in the memory cell referenced by the out parameter */
    versioninfo->vendorID = (WDGIF_VENDOR_ID); /* SBSW_WDGIF_004 */
    versioninfo->moduleID = (WDGIF_MODULE_ID); /* SBSW_WDGIF_004 */
    versioninfo->sw_major_version = (WDGIF_SW_MAJOR_VERSION); /* SBSW_WDGIF_004 */
    versioninfo->sw_minor_version = (WDGIF_SW_MINOR_VERSION); /* SBSW_WDGIF_004 */
    versioninfo->sw_patch_version = (WDGIF_SW_PATCH_VERSION); /* SBSW_WDGIF_004 */
  }
} /* WdgIf_GetVersionInfo() */
#endif /* WDGIF_VERSION_INFO_API == STD_ON */

#define WDGIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* Justification for module-specific MISRA deviations:

  MD_WdgIf_3218: rule 8.9
    Reason:     Global variables are actually accessed by one function. However, usage of MemMap.h prevents from
                defining objects with static storage duration, as MemMap-Sections may not be nested.
    Risk:       None
    Prevention: None

*/


/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_WDGIF_001
 \DESCRIPTION     Function call with config pointer as parameter
 \COUNTERMEASURE  \S The user has to validate the generated configuration as described in the Safety Manual.

\ID SBSW_WDGIF_002
 \DESCRIPTION     Pointer write access to target config pointer
 \COUNTERMEASURE  \S The user has to validate the generated configuration as described in the Safety Manual.

\ID SBSW_WDGIF_003
 \DESCRIPTION     Array write access
 \COUNTERMEASURE  \R The arrays g_wdgif_sc_cycle_counter, g_wdgif_sc_cycle_counter_INV are global arrays with the same
                     length (WDGIF_MAX_SUPPORTED_CORES-1). This define is statically set to 10. A runtime check confirms
                     that only valid array elements of these arrays can be retrieved. The implementation accesses the
                     arrays with an index from 0 to a maximum WdgIfStateCombinerNumberOfSlaves - which is a generated
                     element within the configuration structure. The generator itself does not allow to configure more
                     than 9 slaves.
                  \S The Safety Manual also tells the user to check that the generated number
                     WdgIfStateCombinerNumberOfSlaves is correct.

\ID SBSW_WDGIF_004
 \DESCRIPTION     Pointer write access to target pointer given as parameter
 \COUNTERMEASURE  \N The given pointer must be valid. The user has to ensure that a valid pointer is given.
                     Additionally, the pointer versioninfo is checked if it is valid.

\ID SBSW_WDGIF_005
 \DESCRIPTION     Function Pointer Call to the underlying watchdog hardware
 \COUNTERMEASURE  \S The generated, configured function pointer must be valid. The user has to ensure that a valid
                     function pointer is generated.
                     The Safety Manual tells the user how to check that function pointers.

SBSW_JUSTIFICATION_END */


/* COV_JUSTIFICATION_BEGIN

\ID COV_WDGIF_COMPATIBILITY
 \ACCEPT TX
 \REASON [COV_MSR_COMPATIBILITY]

\ID COV_WDGIF_NULL_PTR_DEFINE
 \ACCEPT XF
 \REASON If the NULL_PTR is not defined, the pointer is defined by the WdgIf itself. In all configs the NULL_PTR is
         already defined via Compiler.h.

COV_JUSTIFICATION_END */

/* FETA_JUSTIFICATION_BEGIN

 \ID FETA_WDGIF_STATECOMBINER_CHECK_SLAVE_TIMING
 \DESCRIPTION [Count] iterations to check slave instance timing.
 \COUNTERMEASURE \S SMI-530

FETA_JUSTIFICATION_END */


/**********************************************************************************************************************
 *  END OF FILE: WdgIf.c
 *********************************************************************************************************************/
