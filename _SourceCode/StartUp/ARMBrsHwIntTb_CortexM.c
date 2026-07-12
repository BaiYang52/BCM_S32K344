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
/** \file  File:  ARMBrsHwIntTb_CortexM.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for all platforms with ARM core Cortex-M
 *     Template:  This file is reviewed according to Brs_Template@Implementation[1.03.05]
 *
 *  \brief Description:  This is a global, hardware-independent file for the ARM-BRS.
 *                       It consists of the core exception table for Startup and a 2nd one to be copied into RAM
 *                       for FBL projects.
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to BrsHw.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EXAMPLE CODE ONLY
 *  -------------------------------------------------------------------------------------------------------------------
 *  This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
 *  The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
 *  intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
 *  implementations must be tested with diligent care and must comply with all quality requirements which are necessary
 *  according to the state of the art before their use.
 *********************************************************************************************************************/

#define ARMBRSHWINTTB_CORTEXM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ARMBrsHw_CortexM.h"

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)                  

#else
  #error "Unknown compiler specified!"
#endif


/**********************************************************************************************************************
 *  PROTOTYPES OF LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if defined (BRS_FIRST_EXECUTION_INSTANCE)
BRS_ISR_KEYWORD void BrsHw_CoreExceptionHandler(void);
#endif

#if defined (BRS_ENABLE_FBL_SUPPORT) && defined (BRS_FBL_EXCEPTIONTABLE_IN_RAM)
BRS_ISR_KEYWORD void BrsHw_CoreExceptionHandler_Ram(void);
#endif

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
/* This code is only needed for the first instance/executable in the system */

/********************************************************************************************
 *                                                                                          *
 *                 Startup Core Exception Handler                                           *
 *                                                                                          *
 ********************************************************************************************/
BRS_ISR_KEYWORD void BrsHw_CoreExceptionHandler(void)
{
  BrsMainExceptionHandler(kBrsInterruptHandlerNotInstalled, BRSERROR_MODULE_BRSHW, (uint16)0);
}


extern void brsStartupEntry(void);
extern void _STACK_C0_LIMIT(void);

/********************************************************************************************
 *                                                                                          *
 *                 Startup Core Exception Table                                             *
 *                                                                                          *
 ********************************************************************************************/
#define BRS_START_SEC_EXCVECT_CONST
#include "Brs_MemMap.h"

void (* const intvect_CoreExceptions[])(void) = {

  _STACK_C0_LIMIT, /* SP reset value */

# if defined (BRS_COMP_IAR)
  brsStartupEntry,  /* Reset Handler */
# else
#  if defined (BRS_INSTRUCTION_SET_THUMB)
  (void (*)()) ((unsigned long)brsStartupEntry + (unsigned long)1), /* Reset Handler */
#  else
  brsStartupEntry, /* Reset Handler */
#  endif
# endif
  BrsHw_CoreExceptionHandler, /* NMI Handler */
  BrsHw_CoreExceptionHandler, /* Hard Fault Handler */
  BrsHw_CoreExceptionHandler, /* M3,M4,M7,M33: MPU Fault Handler */
  BrsHw_CoreExceptionHandler, /* M3,M4,M7,M33: Bus Fault Handler */
  BrsHw_CoreExceptionHandler, /* M3,M4,M7,M33: Usage Fault Handler */
  BrsHw_CoreExceptionHandler,
  BrsHw_CoreExceptionHandler,
  BrsHw_CoreExceptionHandler,
  BrsHw_CoreExceptionHandler,
  BrsHw_CoreExceptionHandler, /* SVCall Handler */
  BrsHw_CoreExceptionHandler, /* M3,M4,M7,M33: Debug Monitor Handler */
  BrsHw_CoreExceptionHandler,
  BrsHw_CoreExceptionHandler, /* PendSV Handler */
  BrsHw_CoreExceptionHandler  /* SysTick Handler */
};

#define BRS_STOP_SEC_EXCVECT_CONST
#include "Brs_MemMap.h"

#endif /*BRS_FIRST_EXECUTION_INSTANCE*/

#if defined (BRS_ENABLE_FBL_SUPPORT) && defined (BRS_FBL_EXCEPTIONTABLE_IN_RAM)
/********************************************************************************************
 *                                                                                          *
 *                 Core Exception Handler in RAM                                            *
 *                                                                                          *
 ********************************************************************************************/
#define BRS_START_SEC_RAM_CODE
#include "Brs_MemMap.h"
BRS_ISR_KEYWORD void BrsHw_CoreExceptionHandler_Ram(void)
{
#if defined (BRSHW_BRSHW_COREEXCEPTIONHANDLER_RAM_CALLOUT)
  BrsTestsuite_BrsHw_CoreExceptionHandler_Ram();
#endif

  volatile uint8 BrsMain_Continue;
  BrsMain_Continue = 0;

  while (BrsMain_Continue == 0)
  {
    /* Set BrsMain_Continue to 1 to continue here.
     *  If the debugger is not able to show the stack properly, this mechanism can be used to find the
     *  source of the exception. */
  }
}
#define BRS_STOP_SEC_RAM_CODE
#include "Brs_MemMap.h"

/********************************************************************************************
 *                                                                                          *
 *                 Core Exception Table in RAM                                              *
 *                                                                                          *
 ********************************************************************************************/
#define BRS_START_SEC_EXCVECTRAM_CONST
#include "Brs_MemMap.h"

void (* const intvect_CoreExceptions_Ram[])(void) = {

  _STACK_C0_LIMIT, /* SP reset value */

#if defined (BRS_COMP_IAR)
  brsStartupEntry,  /* Reset Handler */
#else
# if defined (BRS_INSTRUCTION_SET_THUMB)
  (void (*)()) ((unsigned long)brsStartupEntry + (unsigned long)1), /* Reset Handler */
# else
  brsStartupEntry, /* Reset Handler */
# endif
#endif
  BrsHw_CoreExceptionHandler_Ram, /* NMI Handler */
  BrsHw_CoreExceptionHandler_Ram, /* Hard Fault Handler */
  BrsHw_CoreExceptionHandler_Ram, /* M3,M4,M7,M33: MPU Fault Handler */
  BrsHw_CoreExceptionHandler_Ram, /* M3,M4,M7,M33: Bus Fault Handler */
  BrsHw_CoreExceptionHandler_Ram, /* M3,M4,M7,M33: Usage Fault Handler */
  BrsHw_CoreExceptionHandler_Ram,
  BrsHw_CoreExceptionHandler_Ram,
  BrsHw_CoreExceptionHandler_Ram,
  BrsHw_CoreExceptionHandler_Ram,
  BrsHw_CoreExceptionHandler_Ram, /* SVCall Handler */
  BrsHw_CoreExceptionHandler_Ram, /* M3,M4,M7,M33: Debug Monitor Handler */
  BrsHw_CoreExceptionHandler_Ram,
  BrsHw_CoreExceptionHandler_Ram, /* PendSV Handler */
  BrsHw_CoreExceptionHandler_Ram  /* SysTick Handler */
};

#define BRS_STOP_SEC_EXCVECTRAM_CONST
#include "Brs_MemMap.h"

#endif /*BRS_ENABLE_FBL_SUPPORT&&BRS_FBL_EXCEPTIONTABLE_IN_RAM*/
