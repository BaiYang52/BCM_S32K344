/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                   All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup  Os_Hal_Derivative
 * \{
 *
 * \file
 * \brief  This file contains derivative specific coverage justifications.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_COVERAGE_IMXRT1171INT_H
# define OS_HAL_COVERAGE_IMXRT1171INT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */

/* Os module declarations */

/* Os kernel module dependencies */

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
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* START_COVERAGE_JUSTIFICATION

Justifications for core:

\ID COV_OS_HALPLATFORMTHREADPOINTERUSERREADABILITY_TX_XF
  \ACCEPT XF
  \REASON The condition is hardware dependent always false. The unnecessary code parts should be
          removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
          always just test the code for the respective platform. No risk as the remaining code shall be optimized
          away. (Other platforms may set ACCEPT TX with a similar justification)

\ID COV_OS_HALPLATFORMCOREIDUSERREADABILITY_TX_XF
  \ACCEPT TX
  \REASON The condition is hardware dependent always true. The unnecessary code parts should be
          removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
          always just test the code for the respective platform. No risk as the remaining code shall be optimized
          away. (Other platforms may set ACCEPT XF with a similar justification)

\ID COV_OS_HALPLATFORMMULTICOREUNSUPPORTED
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON This coverage token is never reached on a single-core derivative.
          However, the passage is tested on multicore OS.

\ID COV_OS_HALPLATFORMTESTEDASMULTICORE
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT XX
  \REASON This coverage token is currently not reached on a single-core derivative.
          This is due to missing tests of very unlikely execution paths in singlecore OS.
          However, the passage is tested on multicore OS, where the execution paths are more likely.

\ID COV_OS_HALPLATFORMNEEDMORETHAN2CORES
  \ACCEPT XX
  \REASON This coverage token is never reached on a single-core derivative.

\ID COV_OS_HALPLATFORMMASTERSTARTALLOWED
  \ACCEPT XX
  \REASON This coverage token is never reached on a single-core derivative.

\ID COV_OS_HALPLATFORMNONAUTOSTARTCORE
  \ACCEPT XX
  \REASON This coverage token is never reached on a single-core derivative.

\ID COV_OS_HALPLATFORMGETSPINLOCK
  \ACCEPT XF
  \REASON Though this code may be reached, it is impossible to have a reliable test with only 1 core to cover this
          branch. Thus this is not covered a single-core derivative. However, the correctness can be proved
          by code review and unit test.

\ID COV_OS_HALPLATFORMCONTEXTDOWHILE
  \ACCEPT XF
  \REASON This macro definition uses a "do while(0)". Thus the "true" branch can never be covered.

\ID COV_OS_HALPLATFORMCONTEXTDOWHILENORETURN
  \ACCEPT XX
  \REASON This macro definition uses a "do while(0)". Since the last statement in the loop body will never return, the
          "while" condition can never be checked. However, because the caller is always executed and there is no
          branches, 100% statement coverage can be ensured.

\ID COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED
  \ACCEPT TX
  \ACCEPT XX
  \REASON This platform supports software triggering of peripheral interrupts.

\ID COV_OS_HALPLATFORMPFRTUNSUPPORTED_XX
  \ACCEPT XX
  \REASON Timer type PFRT is not available on this derivative. Functionality has been tested on another derivative.

\ID COV_OS_HALPLATFORMLEVELSUPPORT
  \ACCEPT TX
  \REASON This derivative does support interrupt level switching.

\ID COV_OS_HALPLATFORMISRMAPPINGAVAILABILITY
  \ACCEPT XX
  \ACCEPT XF
  \REASON This derivative does not meet the hardware requirements to support interrupt mapping.
          Functionality has been tested on another derivative.

\ID COV_OS_HALPLATFORMCLEARPENDINGAVAILABILITY
   \ACCEPT TX
   \REASON The condition is hardware dependent always true. The unnecessary code parts should be
           removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
           always just test the code for the respective platform. No risk as the remaining code shall be optimized
           away. (Other platforms may set ACCEPT XF with a similar justification)

\ID COV_OS_HALPLATFORMCLEARSYSTEMPENDINGAVAILABILITY
  \ACCEPT XX
  \ACCEPT XF
  \REASON This derivative does not meet the hardware requirements to support interrupt mapping.

\ID COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY
  \ACCEPT XX
  \ACCEPT XF
  \REASON This derivate does not require any post actions at the end of interrupts.

\ID COV_OS_HALPLATFORMFRT16BITAVAILABILITY
   \ACCEPT XF
   \ACCEPT XX
   \REASON The feature "16-bit FRT timer" is not available on this platform.

\ID COV_OS_HALPLATFORMFRT16BITAVAILABILITY_TX
   \ACCEPT TX
   \REASON The feature "16-bit FRT timer" is not available on this platform.

\ID COV_OS_HALPLATFORMINVSTATE
  \ACCEPT XF
  \REASON This state is never seen as true, since the checked condition is already true before the check is reached.
          Hence, no waiting in the loop is necessary.

\ID COV_OS_HALPLATFORMCOREINTERRUPTSUNSUPPORTED
  \ACCEPT XF
  \REASON The loop invariant is never true, because on this derivative all interrupts are system wide. They are initialized
          in Os_SystemInterruptHandlingInit.

\ID COV_OS_HALPLATFORMMULTICOREORTPUNSUPPORTED
  \ACCEPT XX
  \ACCEPT TX
  \ACCEPT XF
  \REASON See COV_OS_HALPLATFORMMULTICOREUNSUPPORTED.

END_COVERAGE_JUSTIFICATION */

#endif /* OS_HAL_COVERAGE_IMXRT1171INT_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Coverage_IMXRT1171Int.h
 **********************************************************************************************************************/
