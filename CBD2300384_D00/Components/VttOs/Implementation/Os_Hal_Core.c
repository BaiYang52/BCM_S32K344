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
 *  \addtogroup  Os_Hal_Core
 *  \{
 *
 *  \file        Os_Hal_Core.c
 *  \brief       Core related primitives which don't have to be inlined.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */


#define OS_HAL_CORE_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Hal_Core.h"

/* Os kernel module dependencies */
#include "Os_Error_Lcfg.h"

/* Os hal dependencies */
#include "Os_Hal_Os.h"
#include "Os_Hal_Cfg.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/* Check the version of OS header file */
#if (  (OS_HAL_SW_MAJOR_VERSION != (2u)) \
    || (OS_HAL_SW_MINOR_VERSION != (55u)) \
    || (OS_HAL_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Os_Hal_Core.c and Os_Hal_Os.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (OS_CFG_HAL_MAJOR_VERSION != (2u)) \
    || (OS_CFG_HAL_MINOR_VERSION != (36u)) )
# error "Version numbers of Os_Hal_Core.c and Os_Hal_Cfg.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

P2VAR(Os_Hal_ContextType, AUTOMATIC, AUTOMATIC) Os_Hal_CoreId2CurrentContext[OS_CFG_COREPHYSICALID_COUNT];

uint8 _OS_CODE_START[1] = { 0 };                                                                                        /* PRQA S 0602 */ /* MD_Os_Hal_Rule1.3_0602 */
uint8 _OS_CODE_LIMIT[1] = { 0 };                                                                                        /* PRQA S 0602 */ /* MD_Os_Hal_Rule1.3_0602 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_CoreInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Hal_CoreInit(P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config)
{
  /* #10 Configure the number of tasks on the local core. */
  CANoeEmuProcessor_ConfigureNumberOfTasks((sint32)(Config->NumberOfCANoeEmuTasks));

  /*
   * Highest possible level before CANoeEmuProcessor_SetInterruptRoutingMask is 0 because
   * CANoeEmuProcessor_SetInterruptLevel reduces the level to the lowest configured maskable
   * interrupt.
   * Keep this interrupt level by raising the level to OS_CFG_MAX_INT_LEVEL.
   */

  /* #20 Set interrupt level on the local core. */
  (void)CANoeEmuProcessor_SetInterruptLevel(OS_CFG_MAX_INT_LEVEL);
}


/***********************************************************************************************************************
 *  Os_Hal_ConsumeStack()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Hal_ConsumeStack(P2CONST(Os_StackConfigType, AUTOMATIC, OS_CONST) Stack)
{
  /* #10 Consume the given stack. */
  *Stack->HighAddress = ~OS_HAL_STACK_FILL_PATTERN;                                                                     /* SBSW_OS_HAL_PWA_CALLER */
}

/***********************************************************************************************************************
 *  Os_Hal_CoreInitHardware()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Hal_CoreInitHardware(P2CONST(Os_Hal_SystemConfigType, AUTOMATIC, OS_CONST) Config)
{

  OS_IGNORE_UNREF_PARAM(Config);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* #10 Initialize interrupt router. */
  CANoeEmuProcessor_InitInterruptRouter();
  CANoeEmuProcessor_DisableInterruptsAtStartOfISR(1);

}


/***********************************************************************************************************************
 *  Os_Hal_CoreFreeze()
 **********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_Hal_CoreFreeze,                                             /* COV_OS_HALVTTCOVNOTCALLED */
(
  void
))
{
  CANoeAPI_Printf("[%s] OS Kernel Panic - Entering endless loop...", CANoeAPI_FileNameOfDLL());                         /* SBSW_OS_HAL_FC_STATICPOINTER */

  /* #10 Loop endlessly. */
# if (CANOEAPI_VERSION < 239)                                                                                           /* COV_OS_HALVTTCANOEAPIVERSION_002 */
  for( ; ; )                                                                                                            /* FETA_OS_HALVTTCOREFREEZE */
  {
    CANoeAPI_ConsumeTicks(1000);
  }
# else
  CANoeEmuProcessor_CoreFreeze();
# endif
}


#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_OS_HALVTTCOREFREEZE
 \DESCRIPTION    The loop locks the core in order to implement the kernel panic.
 \COUNTERMEASURE \N None as the loop is endless by requirement.

\ID FETA_OS_HALVTTENTRYISRRUN
 \DESCRIPTION    In this loop the ISR is executed and afterwards a switch to the next task is performed. In order to
                 avoid the overhead of a rollback the infinite loop is introduced.
 \COUNTERMEASURE \N None as the loop is simply an optimization.

\ID FETA_OS_HALVTTOSPITCOUNT
 \DESCRIPTION    In this loop all previously generated PIT timers are destroyed. The number of PIT timers is tracked by
                 the static variable VttOs_PitCount and limited by OS_CANOE_PIT_COUNT.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

FETA_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

Justifications for Variant Coverage:

\ID COV_OS_HALVTTCANOEAPIVERSION_001
  \ACCEPT TX
  \REASON VTT OS requires a specific CANoe API version. The test suite uses at least the required version, so this
          check is always true. The required version is checked by code inspection.

\ID COV_OS_HALVTTCANOEAPIVERSION_002
  \ACCEPT TX
  \REASON Check for compatibility with older CANoe API versions is always true. The uncovered code has been tested
          manually and has been checked by code inspection.

\ID COV_OS_HALVTTCANOEAPIVERSION_003
  \ACCEPT TX
  \REASON Special CANoe API version for static code analysis only. Required to stub some CANoe API functionality for
          static code analysis. No risk as this code is not part of the production code and also checked by code
          inspection.

\ID COV_OS_HALVTTFEATURESUPPORT
  \ACCEPT XF
  \REASON Preprocessor macros to enable/disable certain features for core coverage testing.
          The uncovered code is tested during coverage analysis tests.


Justifications for HAL code - Needs not to be covered completely:

\ID COV_OS_HALVTTCOVTOOLING
  \ACCEPT XX
  \REASON The function is called from CANoe. The coverage tool is not able to detect this call. The uncovered code has
          been tested manually and has been checked by code inspection.

\ID COV_OS_HALVTTCOVNOTCALLED
  \ACCEPT XX
  \REASON The function is never called from the Testsuite. The uncovered code is trivial and has been checked by
          code inspection.


Justifications for Core code:

\ID COV_OS_HALPLATFORMTRAPALWAYSNEEDED
   \ACCEPT TX
   \REASON The function is called only in the case the trap has to be used. Therefore the false branch is not covered.
           The risk of keeping the code is considered to be low as it is unit tested and reviewed.

\ID COV_OS_VTTSTACKHANDLING_TX_XF
  \ACCEPT TX
  \ACCEPT XF
  \REASON As the stack handling is performed by the host OS, the function Os_StackIsPartOfStack will always
          return false. This is acceptable, as this code is covered completely by all supported platforms.

\ID COV_OS_VTTSTACKSHARINGSUPPORT_XF
  \ACCEPT XF
  \REASON As the stack handling is performed by the host OS, stack sharing is not supported by VTT and this path
          is never reached by an existing testcase.
          This is acceptable, as this code is covered completely by all supported platforms.

\ID COV_OS_HALPLATFORMSYSTEMINTERRUPTSUNSUPPORTED
  \ACCEPT XF
  \REASON CANoeEmu supports only core specific interrupts. Platform thats support system wide interrupt handling will
          cover the code completely, thus this is acceptable.

\ID COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED
  \ACCEPT TX
  \ACCEPT XX
  \REASON CANoeEmu supports peripheral interrupt triggering. The else-path is completely covered and tested by all
          derivatives that do not support interrupt triggering. Thus, this is acceptable.

\ID COV_OS_HALPLATFORMFRT16BITAVAILABILITY_TX
  \ACCEPT TX
  \REASON 16bit Timer support is not part of the VTT OS implementation. All platforms that support 16bit timers will
          cover the uncovered code parts completely. Thus, this is acceptable.

END_COVERAGE_JUSTIFICATION */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_HAL_PWA_CALLER
 \DESCRIPTION    A write access is performed on a pointer, which is coming directly from the caller.
 \COUNTERMEASURE \N No measure is required, because the caller has to check the validity.

\ID SBSW_OS_HAL_PWA_COREGETCONTEXT
 \DESCRIPTION    A write access is performed to the return value of Os_Hal_CoreGetContext
 \COUNTERMEASURE \N No measure is required, because the component is in development status.

\ID SBSW_OS_HAL_FC_CALLER
 \DESCRIPTION    A function is called with one or more pointer parameters, which are coming directly from the caller.
 \COUNTERMEASURE \N No measure is required, because the caller has to check the validity.

\ID SBSW_OS_HAL_FC_LOCAL_VAR
 \DESCRIPTION    A function is called with one or more pointer parameters, which are pointing to local variables.
 \COUNTERMEASURE \N No measure is required, because local variables are always valid.

\ID SBSW_OS_HAL_FC_MIXED_VAR
 \DESCRIPTION    A function is called with one or more pointer parameters, which are pointing to local variables or
                 are passed to the calling function.
 \COUNTERMEASURE \N No measure is required, because local variables are always valid.

\ID SBSW_OS_HAL_FC_MIXED_GENDATA_VAR
 \DESCRIPTION    A function is called with one or more pointer parameters, which are coming from the GenData or passed
                 to the calling function.
 \COUNTERMEASURE \N No measure is required, because the component is in development status.

\ID SBSW_OS_HAL_FC_COREGETCONTEXT
 \DESCRIPTION    A function is called with one or more pointer parameters, which are coming from the return value of
                 Os_Hal_CoreGetContext.
 \COUNTERMEASURE \N No measure is required, because the component is in development status.

\ID SBSW_OS_HAL_FC_STATICPOINTER
 \DESCRIPTION    A function is called with one or more char pointer parameters, which are statically defined at
                 function call. The validity of the passed string is ensured by code inspection.
 \COUNTERMEASURE \N No measure is required, because statically defined pointers are always valid.

\ID SBSW_OS_HAL_AWA_COREGETID
 \DESCRIPTION    A write access is performed on an array element based on the return value of Os_Hal_CoreGetId(),
                 which in turn simply returns the value of a CANoeAPI function.
 \COUNTERMEASURE \N No measure is required, because the component is in development status.

\ID SBSW_OS_HAL_AWA_PITCOUNT
 \DESCRIPTION    A write access is performed on an array element based on the global variable VttOs_PitCount. The
                 validity of the array index is ensured by runtime assertion.
 \COUNTERMEASURE \N No measure is required, because the component is in development status.

\ID SBSW_OS_HAL_AWA_TESTSUITE
 \DESCRIPTION    A write access is performed on a global array. Anyway, this write access was only added to satisfy
                 static code analysis like MISRA. The code will not be used for production.
 \COUNTERMEASURE \N No measure is required, because the code is only relevant for static code analysis.

SBSW_JUSTIFICATION_END */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core.c
 **********************************************************************************************************************/
