/**********************************************************************************************************************
*  FILE REQUIRES USER MODIFICATIONS
*  Template Scope: whole file
*  -------------------------------------------------------------------------------------------------------------------
This file includes template code that must be completed and/or adapted during BSW integration.
The template code is incomplete and only intended for providing a signature and an empty implementation.
It is neither intended nor qualified for use in series production without applying suitable quality measures. 
The template code must be completed as described in the instructions given within this file and/or in the
Technical Reference. 
The completed implementation must be tested with diligent care and must comply with all quality requirements which
are necessary according to the state of the art before its use.
*********************************************************************************************************************/

/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 *  Template file for FiM memory map
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.0.0     2018-08-24  vistne  -             Initial version
 *  6.2.0     2018-11-23  vistne  -             Fixed sample directives
 *  7.0.0     2020-04-09  vistne  -             Fixed ESCAN00103001
 *  9.0.0     2021-02-12  vistne  -             Removed Author Identity
 *  9.1.0     2022-xx-yy  vistne  -             Changed ...0... to ...INVALID_OSAPPLICATION...
 *********************************************************************************************************************/

 /* PRQA S 0883 */  /* MD_DEM_1915 */
#if defined (FIM_START_SEC_INVALID_OSAPPLICATION_VAR_NOINIT_32BIT)
# undef FIM_START_SEC_INVALID_OSAPPLICATION_VAR_NOINIT_32BIT                         /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_32BIT
#endif

#if defined (FIM_STOP_SEC_INVALID_OSAPPLICATION_VAR_NOINIT_32BIT)
# undef FIM_STOP_SEC_INVALID_OSAPPLICATION_VAR_NOINIT_32BIT                          /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

#if defined (FIM_START_SEC_INVALID_OSAPPLICATION_VAR_ZERO_INIT_8BIT)
# undef FIM_START_SEC_INVALID_OSAPPLICATION_VAR_ZERO_INIT_8BIT                       /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define START_SEC_VAR_NOINIT_8BIT
#endif

#if defined (FIM_STOP_SEC_INVALID_OSAPPLICATION_VAR_ZERO_INIT_8BIT)
# undef FIM_STOP_SEC_INVALID_OSAPPLICATION_VAR_ZERO_INIT_8BIT                        /* PRQA S 0841 */ /* MD_MSR_19.6 */
# define STOP_SEC_VAR
#endif

/* Sample Preprocessor directives for a newly configured OS-Application instance <OS_APPLICATION_NAME>

  #if defined (FIM_START_SEC_<OS_APPLICATION_NAME>_VAR_NOINIT_32BIT)
  # undef FIM_START_SEC_<OS_APPLICATION_NAME>_VAR_NOINIT_32BIT
  # define START_SEC_VAR_NOINIT_32BIT
  #endif
  
  #if defined (FIM_STOP_SEC_<OS_APPLICATION_NAME>_VAR_NOINIT_32BIT)
  # undef FIM_STOP_SEC_<OS_APPLICATION_NAME>_VAR_NOINIT_32BIT
  # define STOP_SEC_VAR
  #endif
 
  #if defined (FIM_START_SEC_<OS_APPLICATION_NAME>_VAR_ZERO_INIT_8BIT)
  # undef FIM_START_SEC_<OS_APPLICATION_NAME>_VAR_ZERO_INIT_8BIT
  # define START_SEC_VAR_ZERO_INIT_8BIT
  #endif
  
  #if defined (FIM_STOP_SEC_<OS_APPLICATION_NAME>_VAR_ZERO_INIT_8BIT)
  # undef FIM_STOP_SEC_<OS_APPLICATION_NAME>_VAR_ZERO_INIT_8BIT
  # define STOP_SEC_VAR
  #endif
 
 */

