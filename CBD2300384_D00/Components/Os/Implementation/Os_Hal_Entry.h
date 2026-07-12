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
 *  \addtogroup   Os_Hal_Entry
 *  \{
 *
 *  \file         Os_Hal_Entry.h
 *  \brief        This component handles hardware exceptions and interrupts.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_ENTRY_H
# define OS_HAL_ENTRY_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_CoreInt.h"

/* Os hal dependencies */
# include "Os_Hal_Interrupt.h"

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

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  Os_Hal_UnhandledIRQ()
 **********************************************************************************************************************/
/*! \brief          This handler is triggered, if an interrupt occurs, which is not configured.
 *  \details        It shall call the kernel's Os_UnhandledIrq() function.
 *                  Os_UnhandledIrq() expects the exception source number to be passed.
 *                  Os_MemFault() shall be called with all interrupts, and timing protection are disabled.
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode.
 *  \pre            Interrupts disabled.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_UnhandledIRQ(void);


/***********************************************************************************************************************
 *  Os_Hal_UnhandledEXC()
 **********************************************************************************************************************/
/*! \brief    This handler is called, if an exception is triggered, which is not handled by the OS.
 *  \details  In this handler it is not safe to use the current stack pointer any longer, because the exception
 *            may be a result of an invalid stack pointer. For this reason the handler has to switch to the kernel
 *            stack before using any stack.
 *
 *            This handler shall call the kernel's Os_UnhandledExc() function.
 *            Os_UnhandledExc() expects the following parameters:
 *              <EXC-SOURCE>           The source number of the exception.
 *              <INSTRUCTION-ADDRESS>  The address of the causing instruction.
 *              <CAUSER-PRIVILEGED>    The privilege state before the exception:
 *                                       !0 Before the exception, the core was privileged.
 *                                       0  Before the exception, the core was not privileged.
 *
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode.
 *  \pre          Interrupts disabled.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_UnhandledEXC(void);


/***********************************************************************************************************************
 *  Os_Hal_SysCallHandler()
 **********************************************************************************************************************/
/*! \brief          This handler is used for SystemCall exception.
 *  \details        -
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode.
 *  \pre            Interrupts disabled.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_SysCallHandler(void);


/***********************************************************************************************************************
 *  Os_Hal_MemFault()
 **********************************************************************************************************************/
/*! \brief          This handler is used for all Memory Protection related exceptions.
 *  \details        -
 *  \context        ANY
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode.
 *  \pre            Interrupts disabled.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_MemFault(void);


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */


#endif /* OS_HAL_ENTRY_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry.h
 **********************************************************************************************************************/
