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
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_Interrupt  HAL Interrupt
 *  \brief      Primitives for interrupt state manipulation
 *  \details
 *  This component provides the following interrupt primitives:
 *  - Enter/leave critical sections
 *  - Disable interrupt sources
 *
 *  ### Interrupt Priority Levels ###
 *  The kernel distinguishes between the following logical groups of interrupt priority levels:
 *
 *  Interrupt Priority Level    | Description
 *  :-------------------------- | :-------------------------------------------------------------------------------------
 *  Timing Protection Interrupt | The timing protection interrupt is handled on this level.
 *  Category 1 ISRs             | AUTOSAR category 1 ISRs are handled on this level.
 *  Category 2 ISRs             | AUTOSAR category 2 ISRs are handled on this level.
 *
 *  The kernel needs some primitive interrupt functions, to enter/leave critical sections on these interrupt priority
 *  levels.  Os_Hal_IntSuspend() and Os_Hal_IntResume() work either up to timing protection level or up to category 1
 *  level, depending whether timing protection is level or NMI based.
 *
 *  ![Interrupt Priorities and Relevant HAL Functions](InterruptPriorities.png)
 *
 *  \{
 *
 *  \file
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */

#ifndef OS_HAL_INTERRUPTINT_H
# define OS_HAL_INTERRUPTINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Hal_Interrupt_Types.h"

/* Os module declarations */
# include "Os_CommonInt.h"

/* Os Hal dependencies */
# include "Os_Hal_CompilerInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/*! \brief    Data type which contains the complete interrupt state.
 *  \details  This data type contains:
 *               - The global interrupt flag
 *               - The interrupt level
 */
struct Os_Hal_IntStateType_Tag
{
  uint32 GlobalInterruptFlag;  /*!< Global interrupt flag state. */
  uint32 InterruptLevel;       /*!< Interrupt level. */
};

/*! \brief    Hardware specific configuration for ISRs.
 *  \details  It typically contains the interrupt priority level and the interrupt source of the ISR.
 *            The kernel uses this data type to:
 *            - Disable an interrupt source as part of killing applications.
 *            - Implement interrupt resources.
 */
struct Os_Hal_IntIsrConfigType_Tag
{
  Os_Hal_IntLevelType Level;    /*!< Interrupt level. */

  /*!< Core interrupt source number, corresponding to the core specific interrupt controller source. */
  Os_Hal_IntSourceType CoreInterruptSource;
};

struct Os_Hal_IntIsrMapConfigType_Tag
{
  /*!< System interrupt source number, corresponding to the peripheral interrupt source number. */
  Os_Hal_IntSourceType SystemInterruptSource;
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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_IntEnableGlobal()
 **********************************************************************************************************************/
/*! \brief          Enables global core interrupts.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableGlobal, (void));


/***********************************************************************************************************************
 *  Os_Hal_IntDisableGlobal()
 **********************************************************************************************************************/
/*! \brief          Disables global core interrupts.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableGlobal, (void));


/***********************************************************************************************************************
 *  Os_Hal_IntDisableAndReturnGlobalState()
 **********************************************************************************************************************/
/*! \brief          Disables global core interrupts and returns earlier global state of interrupts.
 *  \details        -
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_Hal_IntGlobalStateType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableAndReturnGlobalState, (void));


/***********************************************************************************************************************
 *  Os_Hal_IntSetGlobalState()
 **********************************************************************************************************************/
/*! \brief          Update the core local interrupt global state.
 *  \details        -

 *  \param[in]      State   Desired global interrupt state.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntSetGlobalState,
(
  P2CONST(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the current interrupt priority level.
 *  \details        -
 *  \param[out]     Level   Current interrupt priority level. Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntSetLevel()
 **********************************************************************************************************************/
/*! \brief          Set the interrupt priority level to the given value.
 *  \details        -
 *
 *  \param[in]      Level   Desired interrupt priority level.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntSetLevel,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetGlobalEnable()
 **********************************************************************************************************************/
/*! \brief          Returns the global flag value which is used to enable all interrupts.
 *  \details        -
 *
 *  ### Implementation Hint ###
 *  For platforms which support only level: Return the level to enable all interrupts.
 *
 *  \param[out]     State   Global flag value to enable all interrupts.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetGlobalEnable,
(
  P2VAR(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat2LockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock all category 2 ISRs.
 *  \details        -
 *
 *  \param[out]     Level   Category 2 Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetCat2LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat1LockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock all category 1 ISRs.
 *  \details        This function is called only if Os_Hal_IntGetCat1LockType() returns
 *                  OS_HAL_INT_DISABLE_ALL_BY_LEVEL.
 *
 *
 *  \param[out]     Level   Category 1 Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 *                  Function must only be called if Os_Hal_IntGetCat1LockType() returns
 *                    OS_HAL_INT_DISABLE_ALL_BY_LEVEL.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetCat1LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetTPLockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock the TP ISR.
 *  \details        If there is no timing protection interrupt it returns the level
 *                  which is used to lock all category 1 ISRs.
 *  \details        -
 *
 *  \param[out]     Level   TP Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetTPLockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetTaskLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt level which is used to enable interrupt handling on task level.
 *  \details        -
 *
 *  \param[out]     Level   Task level to enable interrupt handling. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetTaskLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetInterruptState()
 **********************************************************************************************************************/
/*! \brief          Returns the current interrupt state.
 *  \details        This function is used to transfer the current interrupt state between different context
 *                  (e.g. non-trusted functions).
 *  \param[out]     InterruptState   Location where to store the current interrupt state. Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetInterruptState,
(
  P2VAR(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSource()
 **********************************************************************************************************************/
/*! \brief          Disable interrupt delivery for the given interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig   Interrupt configuration of the ISR which shall be disabled.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSourceMapped()
 **********************************************************************************************************************/
/*! \brief          Disable interrupt delivery for the given mapped interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall be disabled.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSource()
 **********************************************************************************************************************/
/*! \brief          Enable interrupt delivery for the given interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig   Interrupt configuration of the ISR which shall be enabled.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSourceMapped()
 **********************************************************************************************************************/
/*! \brief          Enable interrupt delivery for the given mapped interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall be enabled.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableSourceMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsClearPendingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports clearing of a pending interrupt.
 *  \details      --no details--
 *
 *  \retval       !0    Pending interrupts can be cleared.
 *  \retval       0     Pending interrupts cannot be cleared.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsClearPendingSupported,( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntClearPending()
 **********************************************************************************************************************/
/*! \brief          Clears the pending flag of the given interrupt source.
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration of the ISR which shall be cleared.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntClearPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntClearPendingMapped()
 **********************************************************************************************************************/
/*! \brief          Clears the pending flag of the given mapped interrupt source.
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall be cleared.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntClearPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*! \brief          Checks if the interrupt source is enabled
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration of the ISR which shall be checked.
 *                              Parameter must not be NULL.
 *  \retval         0           not enabled
 *  \retval         !0          enabled
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptSourceEnabled,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabledMapped()
 **********************************************************************************************************************/
/*! \brief          Checks if the mapped interrupt source is enabled
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall be checked.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \retval         0           not enabled
 *  \retval         !0          enabled
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptSourceEnabledMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));



/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPending()
 **********************************************************************************************************************/
/*! \brief          Checks if the specified ISR has a pending request
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration of the ISR which shall be cleared.
 *                              Parameter must not be NULL.
 *  \retval         0           no pending request.
 *  \retval         !0          pending request.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPendingMapped()
 **********************************************************************************************************************/
/*! \brief          Checks if the specified mapped ISR has a pending request
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall be cleared.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \retval         0           no pending request.
 *  \retval         !0          pending request.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptPendingMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterrupt()
 **********************************************************************************************************************/
/*! \brief          Signal hardware that interrupt processing has been completed.
 *  \details        -
 *
 *  \param[in]      IsrConfig  Interrupt configuration of the ISR which shall signal an end of interrupt.
 *                             Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterruptMapped()
 **********************************************************************************************************************/
/*! \brief          Signal hardware that mapped interrupt processing has been completed.
 *  \details        -
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR which shall signal an end of interrupt.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEndOfInterruptMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsrLevelIsLe()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given ISR configuration has an interrupt priority level, which is lower than
 *                  or equal to the given interrupt priority level.
 *  \details        -
 *
 *  \param[in]      IsrConfig   The ISR configuration which shall be checked. Parameter must not be NULL.
 *  \param[in]      Level       The interrupt priority level to compare with. Parameter must not be NULL.
 *
 *  \retval         !0  The configured interrupt priority level of Isr is logically lower than or equal to Level.
 *  \retval         0   The configured interrupt priority level of Isr is logically greater than Level.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 *  \note           Candidate for compiler attributes OS_NOSIDEEFFECTS.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsrLevelIsLe,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntLevelIsGt()
 **********************************************************************************************************************/
/*! \brief          Returns whether given interrupt priority level LevelX is logically greater than LevelY.
 *  \details        -
 *
 *
 *  \param[in]      LevelX   Left interrupt priority level. Parameter must not be NULL.
 *  \param[in]      LevelY   Right interrupt priority level. Parameter must not be NULL.
 *
 *  \retval         !0  LevelX is logically greater than LevelY.
 *  \retval         0   LevelX is logically less than or equal to LevelY.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 *  \note           Candidate for compiler attributes OS_NOSIDEEFFECTS.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntLevelIsGt,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelX,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelY
));


/***********************************************************************************************************************
 *  Os_Hal_IntTrapRestore()
 **********************************************************************************************************************/
/*! \brief          Restore interrupt state which has been saved during trap.
 *  \details        This function shall restore the interrupt state which was active before the trap handler has been
 *                  triggered. The interrupt state given as parameter has been stored in \ref Os_Exc_SysCall().
 *
 *  \param[in]      InterruptState   Previous interrupt state to restore. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *  \pre            Called after trap.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntTrapRestore,
(
  P2CONST(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntTrapUpdate()
 **********************************************************************************************************************/
/*! \brief          Copies the current interrupt state to the interrupt state which has been saved during trap.
 *  \details        This function is called by the trap handler. It allows modification of the interrupt state of the
 *                  trap caller. The interrupt state given as parameter has been stored in \ref Os_Exc_SysCall().
 *
 *  \param[out]     InterruptState   User space interrupt information. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *  \pre            Called after trap.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_IntTrapUpdate,
(
  P2VAR(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInit()
 **********************************************************************************************************************/
/*! \brief          Initialize given interrupt source so interrupt handling is possible.
 *  \details
 *  Initialization includes:
 *    - Binding of interrupt sources to cores.
 *    - Setting the interrupt priority of each configured ISR.
 *
 *  The enabled-state interrupt sources shall not be touched. (This might not be possible on some interrupt
 *  controllers.)
 *
 *  \param[in,out]  InterruptSource   Interrupt source which shall be initialized. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntInterruptSourceInit,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) InterruptSource
));

/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInitMapped()
 **********************************************************************************************************************/
/*! \brief          Initialize given mapped interrupt source so interrupt handling is possible.
 *  \details
 *  Initialization includes:
 *    - Binding of interrupt sources to cores.
 *    - Setting the interrupt priority of each configured ISR.
 *
 *  The enabled-state interrupt sources shall not be touched. (This might not be possible on some interrupt
 *  controllers.)
 *
 *  \param[in,out]  IsrConfig         Interrupt source which shall be initialized. Parameter must not be NULL.
 *  \param[in,out]  IsrMapConfig      Associated mapped interrupt configuration of the ISR. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntInterruptSourceInitMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_InitializeVectorTable()
 **********************************************************************************************************************/
/*! \brief          Initialize interrupt handling of the interrupt controller.
 *  \details        -
 *  \param[in]      VectorTable    Vector table base address.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_InitializeVectorTable,
(
  Os_Hal_VectorTableType VectorTable
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_InitSource()
 **********************************************************************************************************************/
/*! \brief        Initialize the given interrupt source without activating it.
 *  \details      --no details--
 *
 *  \param[in]    IsrConfig   Interrupt source configuration. Parameter must not be NULL.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_InitSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_EnableMappedSource()
 **********************************************************************************************************************/
/*! \brief        Enable the interrupt handling for the given interrupt.
 *  \details      Mapping between core and system interrupt has been configured.
 *  \param[in]    SystemInterruptSource   System interrupt source number.
 *  \param[in]    CoreInterruptSource     Core interrupt source number.
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_EnableMappedSource,
(
  Os_Hal_IntSourceType SystemInterruptSource,
  Os_Hal_IntSourceType CoreInterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_EnableNotMappedSource()
 **********************************************************************************************************************/
/*! \brief        Enable the interrupt handling for the given interrupt.
 *  \details      Mapping between core and system interrupt has not been configured.
 *  \param[in]    CoreInterruptSource   Core interrupt source number.
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_EnableNotMappedSource,
(
  Os_Hal_IntSourceType CoreInterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsClearPendingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the Interrupt controller supports clearing of a pending interrupt.
 *  \details      --no details--
 *  \retval       !0    Pending interrupts can be cleared.
 *  \retval       0     Pending interrupts cannot be cleared.
 *  \context      OS_INTERNAL
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IsClearPendingSupported,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_DisableSource()
 **********************************************************************************************************************/
/*! \brief        Disable the interrupt handling for the given interrupt source number.
 *  \details      --no details--
 *
 *  \param[in]    Source   Interrupt source number which shall be disabled.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_DisableSource,
(
  Os_Hal_IntSourceType Source
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_DisableMappedSource()
 **********************************************************************************************************************/
/*! \brief        Disable the interrupt handling for the given mapped interrupt source number.
 *  \details      --no details--
 *
 *  \param[in]    Source   Interrupt source number which shall be disabled.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_DisableMappedSource,
(
  Os_Hal_IntSourceType Source
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_TriggerSoftwareInterrupt()
 **********************************************************************************************************************/
/*! \brief          Manually trigger an interrupt.
 *  \details        Set the interrupt pending flag for the interrupt source number.
 *  \param[in]      Source    Interrupt source number which shall be triggered.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_TriggerSoftwareInterrupt,
(
  Os_Hal_IntSourceType Source
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_GetCurrentLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the current interrupt priority level.
 *  \details        -
 *  \return         Current interrupt priority level.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_Hal_IntLevelType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_GetCurrentLevel,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_EndOfInterrupt()
 **********************************************************************************************************************/
/*! \brief          Signal the interrupt controller the end of interrupt.
 *  \details        Clear the pending flag for the interrupt source number.
 *  \param[in]      IsrConfig   Interrupt source configuration. Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_EndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_SetCurrentLevel()
 **********************************************************************************************************************/
/*! \brief          Set the interrupt priority level.
 *  \details        -
 *  \param[in]      Level    Interrupt priority level which shall be active after calling this function.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_SetCurrentLevel,
(
  Os_Hal_IntLevelType Level
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given interrupt source number is enabled.
 *  \details        -
 *  \param[in]      Source   Interrupt source number which shall be returned.
 *  \retval         0    Source is disabled.
 *  \retval         1    Source is enabled.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IsInterruptSourceEnabled,
(
  Os_Hal_IntSourceType Source
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsMappedInterruptSourceEnabled()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given mapped interrupt source number is enabled.
 *  \details        -
 *  \param[in]      CoreInterruptSource     Core interrupt source number which shall be returned.
 *  \param[in]      SystemInterruptSource   System interrupt source number which shall be returned.
 *  \retval         0    Source is disabled.
 *  \retval         1    Source is enabled.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IsMappedInterruptSourceEnabled,
(
    Os_Hal_IntSourceType CoreInterruptSource,
    Os_Hal_IntSourceType SystemInterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IsInterruptPending()
 **********************************************************************************************************************/
/*! \brief          Check whether the corresponding interrupt source is requested.
 *  \details        -
 *  \param[in]      IsrConfig   Interrupt source configuration. Parameter must not be NULL.
 *  \retval         0    Source is not pending.
 *  \retval         1    Source is pending.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IntLevelIsGt()
 **********************************************************************************************************************/
/*! \brief          Returns whether given interrupt priority level LevelX is logically greater than LevelY.
 *  \details        -
 *  \param[in]      LevelX   Left interrupt priority level.
 *  \param[in]      LevelY   Right interrupt priority level.
 *  \retval         !0  LevelX is logically greater than LevelY.
 *  \retval         0   LevelX is logically less than or equal to LevelY.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IntLevelIsGt,
(
  CONST(Os_Hal_IntLevelType, OS_CONST) LevelX,
  CONST(Os_Hal_IntLevelType, OS_CONST) LevelY
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports interrupt mapping.
 *  \details      --no details--
 *
 *  \retval       !0    Interrupt mapping is supported.
 *  \retval       0     Interrupt mapping is not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_IntIsInterruptMappingSupported, ( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntIsGlobal2LevelSwitchSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the platform supports the global disable to level lock switch (!0) or not (0).
 *  \details      If the platform does not have global disable flag then the feature cannot be supported.
 *                Feature is supported if platform allows to disable global, switch priority level and resume global
 *                interrupt state and a pending interrupt with lower priority does not occur after this sequence.
 *
 *  \retval       !0    Supported.
 *  \retval       0     Not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsGlobal2LevelSwitchSupported, (void));


/***********************************************************************************************************************
 *  Os_Hal_IntIsPeripheralInterruptTriggerSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports peripheral interrupt software trigger.
 *  \details      --no details--
 *
 *  \retval       !0    Peripheral interrupts can be triggered.
 *  \retval       0     Peripheral interrupts cannot be triggered.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsPeripheralInterruptTriggerSupported,( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntIsLevelSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports interrupt priority levels.
 *  \details      --no details--
 *
 *  \retval       !0    The interrupt controller supports level switching.
 *  \retval       0     The interrupt controller does not support level switching.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsLevelSupported,( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports interrupt mapping.
 *  \details      --no details--
 *
 *  \retval       !0    Interrupt mapping is supported.
 *  \retval       0     Interrupt mapping is not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptMappingSupported, ( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPostActionSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether interrupt post action handling is supported.
 *  \details      See \ref Os_Hal_IntPostAction and \ref Os_Hal_IntPostActionMapped.
 *
 *  \retval       !0    Interrupt post action handling is supported.
 *  \retval       0     Interrupt post action handling is not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptPostActionSupported, ( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntPostAction()
 **********************************************************************************************************************/
/*! \brief          Execute interrupt specific post action.
 *  \details        This function is called before \ref Os_Hal_IntEndOfInterrupt().
 *
 *  \param[in]      IsrConfig  Interrupt configuration of the ISR for which the post action needs to be executed.
 *                             Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntPostAction,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntPostActionMapped()
 **********************************************************************************************************************/
/*! \brief          Execute interrupt specific post action.
 *  \details        This function is called before \ref Os_Hal_IntEndOfInterruptMapped().
 *
 *  \param[in]      IsrConfig     Interrupt configuration of the ISR for which the post action needs to be executed.
 *                                Parameter must not be NULL.
 *  \param[in]      IsrMapConfig  Associated mapped interrupt configuration of the ISR.
 *                                Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntPostActionMapped,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntIsrMapConfigType, AUTOMATIC, OS_APPL_DATA) IsrMapConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_EnableWriteAccess()
 **********************************************************************************************************************/
/*! \brief          Enable write access to VTOR.
 *  \details        --no details--
 *  \param[in]      VectorTable    Vector table base address.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_EnableWriteAccess,
(
  Os_Hal_VectorTableType VectorTable
));


/***********************************************************************************************************************
 *  OS_HAL_INTC_NVIC_RouteSource()
 **********************************************************************************************************************/
/*! \brief          Route the given source to the given running core.
 *  \details        --no details--
 *  \param[in]      IsrConfig     Interrupt source configuration. Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
OS_HAL_INTC_NVIC_RouteSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_EnableMappedSource()
 **********************************************************************************************************************/
/*! \brief        Enable the interrupt handling for the given interrupt.
 *  \details      Mapping between core and system interrupt has been configured.
 *  \param[in]    SystemInterruptSource   System interrupt source number.
 *  \param[in]    CoreInterruptSource     Core interrupt source number.
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_NVIC_EnableMappedSource,
(
  Os_Hal_IntSourceType SystemInterruptSource,
  Os_Hal_IntSourceType CoreInterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_DisableMappedSource()
 **********************************************************************************************************************/
/*! \brief        Disable the interrupt handling for the given mapped interrupt source number.
 *  \details      --no details--
 *
 *  \param[in]    Source   Interrupt source number which shall be disabled.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_NVIC_DisableMappedSource,
(
  Os_Hal_IntSourceType Source
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given mapped interrupt source number is enabled.
 *  \details        -
 *  \param[in]      CoreInterruptSource     Core interrupt source number which shall be returned.
 *  \param[in]      SystemInterruptSource   System interrupt source number which shall be returned.
 *  \retval         0    Source is disabled.
 *  \retval         1    Source is enabled.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_NVIC_IsMappedInterruptSourceEnabled,
(
  Os_Hal_IntSourceType CoreInterruptSource,
  Os_Hal_IntSourceType SystemInterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IsClearPendingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the Interrupt controller supports clearing of a pending interrupt.
 *  \details      --no details--
 *  \retval       !0    Pending interrupts can be cleared.
 *  \retval       0     Pending interrupts cannot be cleared.
 *  \context      OS_INTERNAL
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_NVIC_IsClearPendingSupported,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_INTC_NVIC_IntIsInterruptMappingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports interrupt mapping.
 *  \details      --no details--
 *
 *  \retval       !0    Interrupt mapping is supported.
 *  \retval       0     Interrupt mapping is not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_INTC_NVIC_IntIsInterruptMappingSupported,
(
  void
));


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_INTERRUPTINT_H */

/*!
 * \}
 */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptInt.h
 *********************************************************************************************************************/
