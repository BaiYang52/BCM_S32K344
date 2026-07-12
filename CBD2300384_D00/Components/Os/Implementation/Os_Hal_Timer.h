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
 *  \addtogroup  Os_Hal_Timer
 *  \{
 *
 *  \file
 *  \brief  HAL interface to work with timer hardware.
 *  \details
 *  This file chooses the appropriate timer implementation file depending on configuration.
 *  If there is no timer configured a stub implementation is provided.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_TIMER_H
# define OS_HAL_TIMER_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Common.h"

/* Os Hal dependencies */
# include "Os_Hal_TimerInt.h"
# include "Os_Hal_Derivative.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Interrupt.h"

# if defined(OS_HAL_TIMER_FRT_TPM)                                                                                      /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_Timer_Frt_Tpm.h"
# elif defined(OS_HAL_TIMER_FRT_FTM)
#  include "Os_Hal_Timer_Frt_Ftm.h"
# elif defined(OS_HAL_TIMER_FRT_TCPWM0)
#  include "Os_Hal_Timer_Frt_Tcpwm.h"
# elif defined(OS_HAL_TIMER_FRT_STM)                                                                                    /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_Timer_Frt_Stm.h"
# elif defined(OS_HAL_TIMER_FRT_NXP_GPT)                                                                                /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_Timer_Frt_NXP_GPT.h"
# else
#  include "Os_Hal_Timer_Frt_Dummy.h"
# endif

# if defined(OS_HAL_TIMER_PIT_SYSTICK)                                                                                  /* COV_OS_DERIVATIVEDUMMYMODULE */
#  include "Os_Hal_Timer_Pit_SysTick.h"
# else
#  include "Os_Hal_Timer_Pit_Dummy.h"
# endif

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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_TIMER_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Timer.h
 **********************************************************************************************************************/

