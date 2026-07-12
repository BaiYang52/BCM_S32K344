/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *             File:  Os_Cfg.h
 *           Config:  BCM_S32K344.dpa
 *      ECU-Project:  BCM_S32K344
 *
 *        Generator:  MICROSAR RTE Generator Version 4.32.0
 *                    RTE Core Version 4.32.0
 *          License:  CBD2300384
 *
 *      Description:  Os definitions
 *********************************************************************************************************************/

#ifndef _OS_CFG_H_
# define _OS_CFG_H_

/* Os definitions */

/* Tasks */
# define AppRunTask (0U)
# define Default_Init_Task (1U)
# define Default_Init_Task_Trusted (2U)
# define IdleTask_OsCore0 (3U)
# define SwcInitTask (4U)
# define OS_TASKID_COUNT (5U)


/* Counters */
# define SystemTimer (0U)

/* Alarms */
# define Rte_Al_TE2_AppRunTask_0_10ms (0U)
# define Rte_Al_TE_CtAppSwc_RCtAppSwc_1000ms (1U)
# define Rte_Al_TE_CtAppSwc_RCtAppSwc_10ms (2U)

/* Events */
# define Rte_Ev_Cyclic2_AppRunTask_0_10ms (0x01ULL)
# define Rte_Ev_Run_CtAppSwc_RCtAppSwc_1000ms (0x02ULL)
# define Rte_Ev_Run_CtAppSwc_RCtAppSwc_10ms (0x04ULL)

/* Spinlocks */

/* Resources */

/* ScheduleTables */

/* Cores */

/* Trusted Functions */


#endif /* _OS_CFG_H_ */
