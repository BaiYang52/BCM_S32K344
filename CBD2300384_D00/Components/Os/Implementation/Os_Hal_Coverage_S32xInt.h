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

#ifndef OS_HAL_COVERAGE_S32XINT_H
# define OS_HAL_COVERAGE_S32XINT_H

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
  \ACCEPT TX
  \REASON The condition is hardware dependent always true. The unnecessary code parts should be
          removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
          always just test the code for the respective platform. No risk as the remaining code shall be optimized
          away. (Other platforms may set ACCEPT XF with a similar justification)

\ID COV_OS_HALPLATFORMCOREIDUSERREADABILITY_TX_XF
   \ACCEPT TX
   \REASON The condition is hardware dependent always true. The unnecessary code parts should be
           removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
           always just test the code for the respective platform. No risk as the remaining code shall be optimized
           away. (Other platforms may set ACCEPT XF with a similar justification)
           
\ID COV_OS_HALPLATFORMCONTEXTDOWHILE
  \ACCEPT XF
  \REASON This macro definition uses a "do while(0)". Thus the "true" branch can never be covered.

\ID COV_OS_HALPLATFORMCONTEXTDOWHILENORETURN
  \ACCEPT XX
  \REASON This macro definition uses a "do while(0)". Since the last statement in the loop body will never return, the
          "while" condition can never be checked. However, because the caller is always executed and there is no
          branches, 100% statement coverage can be ensured.

\ID COV_OS_HALPLATFORMPERIPHERALINTTRIGGERSUPPORTED
  \ACCEPT XF
  \ACCEPT XX
  \REASON This platform does not support peripheral interrupt software trigger.

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

\ID COV_OS_HALPLATFORMSPINLOCKTRYTOLOCKCONDITION
   \ACCEPT XF
   \REASON True condition may not be stimulated by tests due to race condition: At least one core have to wait
           until the spinlock is free.

\ID COV_OS_HALPLATFORMGETSPINLOCK
  \ACCEPT TF
  \REASON This justification is not relevant to S32x.

\ID COV_OS_HALPLATFORMNONAUTOSTARTCORE
   \ACCEPT TF
   \REASON This justification is not relevant to S32x.

\ID COV_OS_HALPLATFORMMASTERSTARTALLOWED
  \ACCEPT TF
  \REASON This justification is not relevant to S32x.

\ID COV_OS_HALPLATFORMSYSTEMINTERRUPTSUNSUPPORTED
  \ACCEPT XF
  \REASON The loop invariant is never true, because on this derivative we only have core interrupts. They are initialized
          in Os_CoreInterruptHandlingInit.

\ID COV_OS_HALPLATFORMS32XPERIPHERAL_RESET_XF
  \ACCEPT XF
  \REASON True condition can only be reached if core 3 is started. Only cores 0, 1, 2 are available on the tested
          derivative.

END_COVERAGE_JUSTIFICATION */

#endif /* OS_HAL_COVERAGE_S32XINT_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Coverage_S32xInt.h
 **********************************************************************************************************************/
