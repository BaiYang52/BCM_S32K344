/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \defgroup   Os_Hal HAL
 *  \file       Os_Hal_EntryInt.h
 *  \brief      The HAL provides the platform specific low level part of the OS. It has to be implemented by the
 *              platform.
 *  \details    All files given here define the HAL and can be seen as a template to implement a new platform.
 */
/**
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_Entry  HAL Entry
 *  \brief      This component handles hardware exceptions and interrupts.
 *  \details
 *  Most of the HAL consists of macros and functions that are called by the kernel via the
 *  HAL interface. These perform the required low level operations by accessing the hardware and then return.
 *  The entry component is an exception to this. It handles exceptions: either synchronous hardware traps or
 *  asynchronous device interrupts. So we have some kind of control inversion here.
 *
 *  First the control is passed to the HAL, which then passes it to the kernel or the application.
 *  After the kernel has finished, control is passed back to the HAL for in order to restore the CPU
 *  state and resume processing from the point at which the exception occurred.
 *
 *  The exact implementation of this code is under the control of the HAL implementer. As long as it interacts
 *  correctly with the functions defined in Kernel Interface it may take any form. However there is typically
 *  the entry type pattern which is listed in this component description.
 *
 *  If there are any statements within a HAL function, which have influence to the control flow, the platform
 *  developer has to ensure that all paths are covered by tests (MC/DC). If this it not possible, the
 *  functionality has to be ensured by review.
 *
 *  Timing Protection Interrupt
 *  ---------------------------
 *  The kernel handles timing protection interrupts in an ISR. The ISR has to be executed with highest priority
 *  (or NMI).
 *
 *  Inter Processor Interrupt
 *  -------------------------
 *  The kernel handles inter processor interrupts in an ISR.
 *
 *  #### ISR Loop Concept
 *  The ISRHost is kept in a infinite loop to avoid the overhead of doing a rollback and starting the
 *  ISRHost every time.
 *
 *  #### Overview ISR Handling and Entry functions
 *
 *  During the IRQ hook the OS expects the HAL to be still in the context of the interrupted
 *  thread. This is emulated in VTT by having a second task called IsrHost.
 *
 *  CANoeIRQ_Handler -> Os_Hal_EntryIrqRun() -> Os_Hal_EntryIsrRun() -> Os_Hal_EntryExecuteThread() ->
 * +-----------------+------------------------+----------------------+---------------------------+
 *                   -  Switches to the       - Inform Os about the  - Executes ISR and calls    -
 *                   -  second IsrHost        - triggered interrupt. - Isr_Epilogue.             -
 *                   -  task context and      - Calls Os_IsrRun().   -                           -
 *                   -  starts the CANoeemu   - Switches back to     -                           -
 *                   -  IsrHost task.         - task context.        -                           -
 * +-----------------+------------------------+----------------------+---------------------------+
 *
 *  For more details regarding ISR handling read the Os_Hal_ContextInt.h description.
 *
 *   \{
 *  \brief      This component handles hardware exceptions and interrupts.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_ENTRYINT_H
# define OS_HAL_ENTRYINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_Entry_Types.h"

/* Os kernel module dependencies */
# include "Os_KernelInt.h"

/* Os hal dependencies */


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
 *  Os_Exc_MemProt()
 **********************************************************************************************************************/
/*! \brief        This handler is triggered, if a memory fault is detected.
 *  \details      The handler shall call the kernel's Os_MemFault() function.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode.
 *  \pre          Interrupts disabled.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Exc_MemProt, (void));


/***********************************************************************************************************************
 *  Os_Hal_EntryIrqRun()
 **********************************************************************************************************************/
/*! \brief        Handles the IRQ.
 *  \details      Hal IRQ Handler(hook) called by CANoe IRQ Handler. It is the first function that is called after
 *                the IRQ is triggered and is used to switch to the second IsrHost task context.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryIrqRun,
(
  P2CONST(Os_IsrConfigType, AUTOMATIC, OS_CONST) Isr
));


/***********************************************************************************************************************
 *  Os_Hal_EntryIsrRun()
 **********************************************************************************************************************/
/*! \brief        Informs OS about the triggered interrupt.
 *  \details      Start function called by CANoe when CANoeIsrHost context is entered.From IRQ hook VTT switches the
 *                context to the IsrHost task and calls this function, which then tells the OS core that an
 *                interrupt was triggered(By calling Os_IsrRun).
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_Hal_EntryIsrRun()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryIsrRun, (void));


/***********************************************************************************************************************
 *  Os_Hal_EntryExecuteThread()
 **********************************************************************************************************************/
/*! \brief        Executes context entry function without parameter.
 *  \details      Start function called by CANoe when Task or ISR context is entered.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryExecuteThread, (void));


/***********************************************************************************************************************
 *  Os_Hal_EntryExecuteThreadWithParam()
 **********************************************************************************************************************/
/*! \brief        Executes context entry function with parameter.
 *  \details      Start function called by CANoe when TF or Hook context is entered.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryExecuteThreadWithParam, (void));

/***********************************************************************************************************************
 *  Os_Hal_EntryISRAbort()
 **********************************************************************************************************************/
/*! \brief        Aborts the ISR in the current context.
 *  \details      As each VTT context has a second CANoe task instance named IsrHost, the function is
 *                used to abort the IsrHost and the task in the current contex. By reseting the start address of
 *                both and jumping to the return address.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_EntryISRAbort, (void));


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#endif /* OS_HAL_ENTRYINT_H */


/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_EntryInt.h
 **********************************************************************************************************************/
