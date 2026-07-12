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
 *  \defgroup   VttOs
 *  \{
 *
 *  \file       Os_Hal_Processor.h
 *  \brief
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_PROCESSOR_H
# define OS_HAL_PROCESSOR_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module declarations */

/* Os HAL dependencies */
#include "Os_Hal_Timer_Types.h"
#include "Os_Hal_CompilerInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  VttOs_OnStateChange()
 **********************************************************************************************************************/
/*! \brief       Handles ECU state changes
 *  \details     -
 *  \param[in]   Action     Cause of the state change.
 *  \param[in]   OldState   Old ECU state.
 *  \param[in]   NewState   New ECU state.
 *  \pre         -
 *  \context     OS_INTERNAL
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        -
**********************************************************************************************************************/
FUNC(void, OS_CODE) VttOs_OnStateChange
(
    uint8 Action,
    uint8 OldState,
    uint8 NewState
);


/***********************************************************************************************************************
 *  Os_Hal_ProcessorCreatePit()
 **********************************************************************************************************************/
/*! \brief       Creates a Pit timer on the simulated processor.
 *  \details     The Pit timer uses the CANoe UserTimer2 interface. The parameter is the interrupt source of the Pit
 *               timer
 *  \param[in]   Name             Timer name
 *  \param[in]   CycleTime        Interval in microseconds
 *  \param[in]   InterruptSource  Callback at the end of the interval.
 *  \pre         -
 *  \context     OS_INTERNAL
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_ProcessorCreatePit
(
    Os_Hal_TimerPitNameType Name,
    uint32 CycleTime,
    sint32 InterruptSource
);


/***********************************************************************************************************************
 *  Os_Hal_ProcessorDestroyTimers()
 **********************************************************************************************************************/
/*! \brief      Destroys all timer units of the simulated processor
 * \details     -
 * \pre         -
 * \context     OS_INTERNAL
 * \reentrant   FALSE
 * \synchronous TRUE
 * \note        -
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_ProcessorDestroyTimers
(
    void
);


/***********************************************************************************************************************
 *  Os_Hal_ProcessorPitUserTimer2Handler()
 **********************************************************************************************************************/
/*! \brief       Triggers a PIT interrupt.
 *  \details     Implements a CANoe user timer 2 handler.
 *  \param[in]   InterruptSoruce   Interrupt source of the PIT timer
 *  \param[in]   Name              Name of the PIT timer. (Unused)
 *  \pre         -
 *  \context
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_ProcessorPitUserTimer2Handler
(
  sint32 InterruptSource,
  P2CONST(Os_Hal_CANoeNameType, AUTOMATIC, OS_CONST) Name
);

#endif /* OS_HAL_PROCESSOR_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Processor.h
 **********************************************************************************************************************/

