/*********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       \file     Eep_30_XXi2c01_Proc.h
 *       \brief    Eep I2c Low Level processing header file
 *
 *       \details  Contains state processing services, which are called by High Level implementation. Low Level
 *                 implementation accesses the underlying bus driver and processes the current job.
 *
 *********************************************************************************************************************/

#ifndef EEP_30_XXI2C01_PROC_H
# define EEP_30_XXI2C01_PROC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Eep_30_XXi2c01_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATATYPES
 *********************************************************************************************************************/

#define EEP_30_XXI2C01_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

typedef enum{
    EEP_30_XXI2C01_JOB_READ = 0,
    EEP_30_XXI2C01_JOB_WRITE,
    EEP_30_XXI2C01_JOB_COMPARE,
    EEP_30_XXI2C01_JOB_ERASE,
    EEP_30_XXI2C01_JOB_TESTCOM
} Eep_30_XXi2c01_JobType;

#define EEP_30_XXI2C01_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define EEP_30_XXI2C01_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitCfgArray()
**********************************************************************************************************************/
/*!
 * \brief      Initializes configuration array at Index with passed ConfigPtr.
 * \details    Initializes configuration array at Index with passed ConfigPtr. Service is called upon module initialization.
 * \param[in]  Index
 * \param[in]  ConfigPtr
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitCfgArray(uint8 Index, P2CONST(Eep_30_XXi2c01_ConfigType, AUTOMATIC, EEP_30_XXI2C01_PBCFG) ConfigPtr);


/**********************************************************************************************************************
  Eep_30_XXi2c01_InitComponentParameter()
**********************************************************************************************************************/
/*!
 * \brief      Initializes component parameter, flags and state machine.
 * \details    Sets status to idle, job result to ok. Resets state machine and clears all internal flags.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitComponentParameter(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_InitAsyncJob()
**********************************************************************************************************************/
/*!
 * \brief      Initializes asynchronous job.
 * \details    Initializes job parameter upon request of an asynchronous job.
 * \param[in]  EepromAddress
 * \param[out] DesDataBufferPtr - Pointer to user buffer where data will be stored
 * \param[in]  SrcDataBufferPtr - Pointer to user buffer which contains data to be written
 * \param[in]  Length
 * \param[in]  Index
 * \param[in]  JobType
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitAsyncJob(Eep_30_XXi2c01_AddressType EepromAddress,
                                                       Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                       Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                       Eep_30_XXi2c01_LengthType Length,
                                                       uint8 Index,
                                                       Eep_30_XXi2c01_JobType JobType);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetCurrentMode()
**********************************************************************************************************************/
/*!
 * \brief      Sets current mode.
 * \details    Sets current mode. Service is used in SetMode API.
 * \param[in]  Mode - Either Slow or Fast mode
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetCurrentMode(MemIf_ModeType Mode);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetCurrentHandling()
**********************************************************************************************************************/
/*!
 * \brief      Sets current handling.
 * \details    Sets current handling. Service is used in SetHandling API.
 * \param[in]  Handling - Either Recurrent or Burst handling
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetCurrentHandling(Eep_30_XXi2c01_HandlingType Handling);


/**********************************************************************************************************************
  Eep_30_XXi2c01_GetCurrentJobResult()
**********************************************************************************************************************/
/*!
 * \brief      Returns component job result to the caller (usually Ea).
 * \details    -
 * \return     Current component job result
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetCurrentJobResult(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_GetCurrentStatus()
**********************************************************************************************************************/
/*!
 * \brief      Returns component status to the caller (usually Ea).
 * \details    -
 * \return     Current component status
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
FUNC (MemIf_StatusType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetCurrentStatus(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_CancelJob()
**********************************************************************************************************************/
/*!
 * \brief      Sets necessary flags so that cancel will be processed within next call of MainFunction.
 * \details    Sets necessary flags so that cancel will be processed within next call of MainFunction. Also resets
 *             component global parameters so that user of Eep can request a new job right away.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CancelJob(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_MainFunctionStd()
**********************************************************************************************************************/
/*!
 * \brief      Service to handle the requested jobs and the internal management operations.
 * \details    -
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_MainFunctionStd(void);


/**********************************************************************************************************************
  Eep_30_XXi2c01_ComEndStd()
**********************************************************************************************************************/
/*!
 * \brief      Indicates to Eep that current job of underlying bus driver is finished.
 * \details    Indicates to Eep that current job of underlying bus driver is finished. Sets ProcessMainFunction flag.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ComEndStd(void);

# define EEP_30_XXI2C01_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* EEP_30_XXI2C01_PROC_H */

/**********************************************************************************************************************
 *  END OF FILE: Eep_30_XXi2c01_Proc.h
 *********************************************************************************************************************/
