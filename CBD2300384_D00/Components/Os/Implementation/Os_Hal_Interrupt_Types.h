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
 *  \addtogroup Os_Hal_Interrupt
 *  \{
 *
 *  \file  Os_Hal_Interrupt_Types.h
 *  \brief  Defines some data structures related to interrupt handling
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_INTERRUPT_TYPES_H
# define OS_HAL_INTERRUPT_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! \brief    Timing protection interrupt state.
 *  \details  The kernel uses this data type to suspend/resume the timing protection interrupt.
 *            This type might be some kind of bit mask (e.g. processor status word), if the timing protection ISR is a
 *            NMI. Or this type might be some kind of level, if the timing protection ISR is a regular ISR with an
 *            interrupt priority level. */
typedef uint32 Os_Hal_IntTpStateType;

/*! \brief    Interrupt state.
 *  \details  The kernel uses this data type to suspend/resume interrupts, to protect OS internal critical sections.
 *            This type is typically some kind of bit mask (e.g. processor status word). */
typedef uint32 Os_Hal_IntGlobalStateType;

/*! \brief    Interrupt level.
 *  \details  The kernel uses this data type to suspend/resume category 2 ISRs up to a certain level.
 *            This data type is needed to implement interrupt resources. */
typedef uint32 Os_Hal_IntLevelType;

/*! \brief    Interrupt state (with trap information).
 *  \details  The kernel uses this data type to restore the caller's interrupt state on OS service requests.
 *            On TriCore this data type also contains a pointer to the user space CSA. */
typedef uint32 Os_Hal_IntTrapStateType;


struct Os_Hal_IntStateType_Tag;
typedef struct Os_Hal_IntStateType_Tag Os_Hal_IntStateType;


/*! \brief    Interrupt source.
 *  \details  The kernel uses this data type to disable an interrupt source.
 *            This data type is needed to implement killing of applications.
 *            It is typically an interrupt index or interrupt source address. */
typedef uint32 Os_Hal_IntSourceType;


struct Os_Hal_IntIsrConfigType_Tag;
typedef struct Os_Hal_IntIsrConfigType_Tag Os_Hal_IntIsrConfigType;

struct Os_Hal_IntIsrMapConfigType_Tag;
typedef struct Os_Hal_IntIsrMapConfigType_Tag Os_Hal_IntIsrMapConfigType;

typedef P2FUNC(void, OS_CODE, Os_Hal_VectorTableEntryType)(void);
typedef CONSTP2CONST(Os_Hal_VectorTableEntryType, TYPEDEF, OS_CONST)  Os_Hal_VectorTableType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


#endif /* OS_HAL_INTERRUPT_TYPES_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Interrupt_Types.h
 **********************************************************************************************************************/
