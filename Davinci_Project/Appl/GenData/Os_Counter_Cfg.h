/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: Os
 *           Program: VAG AR4 (MSR_Vag_SLP5)
 *          Customer: Faurecia Clarion Electronics US
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: S32K314NHT1VPBST
 *    License Scope : The usage is restricted to CBD2300384_D00
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Os_Counter_Cfg.h
 *   Generation Time: 2026-07-12 12:56:23
 *           Project: BCM_S32K344 - Version 1.0
 *          Delivery: CBD2300384_D00
 *      Tool Version: DaVinci Configurator Classic (beta) 5.26.40 SP3
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! This version of DaVinci Configurator Classic and/or the related Basic Software Package is BETA software.               !
 ! BETA Software is basically operable, but not sufficiently tested, verified and/or qualified for use in series      !
 ! production and/or in vehicles operating on public or non-public roads.                                             !
 ! In particular, without limitation, BETA Software may cause unpredictable ECU behavior, may not provide all         !
 ! functions necessary for use in series production and/or may not comply with quality requirements which are         !
 ! necessary according to the state of the art. BETA Software must not be used in series production.                  !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/

#ifndef OS_COUNTER_CFG_H 
# define OS_COUNTER_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Defines whether any counter uses SW increment (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_SW_USED                  (STD_OFF)

/*! Defines whether any counter uses PIT (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_PIT_USED                 (STD_ON)

/*! Defines whether any counter uses High Resolution (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_HRT_USED                 (STD_OFF)

/*! Defines whether any counter uses FRT as PIT (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_PFRT_USED                (STD_OFF)

/*! Defines whether any counter uses a priority queue (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_QUEUE_USED               (STD_ON)

/*! Defines whether any counter uses an array list (STD_ON) or not (STD_OFF) */
# define OS_CFG_COUNTER_LIST_USED                (STD_OFF)

/*! Number of counter jobs: SystemTimer */
# define OS_CFG_NUM_COUNTER_SYSTEMTIMER_JOBS     (1u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


#endif /* OS_COUNTER_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Counter_Cfg.h
 *********************************************************************************************************************/
