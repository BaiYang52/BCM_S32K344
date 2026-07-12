/**********************************************************************************************************************
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
 *       \file    Eep_30_XXi2c01.c
 *       \brief   Eep I2c implementation
 *
 *       \details  Eep provides API services to upper layer (Ea). Eep performs jobs on underlying EEPROM device via
 *                 I2C bus communication.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Eep_30_XXi2c01.h"
#include "Eep_30_XXi2c01_Cbk.h"

/* Eep Processing */
#include "Eep_30_XXi2c01_Proc.h"

/**********************************************************************************************************************
 * VERSION CHECK
 *********************************************************************************************************************/

/* AUTOSAR version has to match the definitions in the static header file */
#if ( (4u != EEP_30_XXI2C01_AR_RELEASE_MAJOR_VERSION) || \
      (0u != EEP_30_XXI2C01_AR_RELEASE_MINOR_VERSION)    \
    )
# error "AUTOSAR specification version numbers of Eep_30_XXi2c01.c and Eep_30_XXi2c01.h are inconsistent!"
#endif

/* implementation version has to match the definitions in the static header file */
#if ( (2u != EEP_30_XXI2C01_SW_MAJOR_VERSION) || \
      (0u != EEP_30_XXI2C01_SW_MINOR_VERSION)    \
    )
# error "Software version numbers of Eep_30_XXi2c01.c and Eep_30_XXi2c01.h are inconsistent!"
#endif

/* generator version has to match the definitions in the generated header file */
#if ( (2u != EEP_30_XXI2C01_CFG_MAJOR_VERSION) || \
      (1u != EEP_30_XXI2C01_CFG_MINOR_VERSION)    \
    )
# error "Generator version numbers of Eep_30_XXi2c01.c and Eep_30_XXi2c01_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EEP_30_XXI2C01_INST1_INDEX      0x00u
#define EEP_30_XXI2C01_INST2_INDEX      0x01u

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EEP_LOCAL /* COV_EEP_COMPATIBILITY */
# define EEP_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EEP_30_XXI2C01_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkInit()
**********************************************************************************************************************/
/*!
 * \brief      Checks if module is initialized.
 * \details    -
 * \param[in]  Index
 * \return     E_OK: Module is initialized
 *             E_NOT_OK: Module is not yet initialized
 * \pre        Development error detection is enabled.
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkInit(uint8 Index);

/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkDataPointer()
**********************************************************************************************************************/
 /*!
  * \brief      Checks if the input buffer pointer related to the current job is a NULL_PTR.
  * \details    Checks DesDataBufferPtr for read class jobs and SrcDataBufferPtr for write class jobs.
  * \param[in]  DesDataBufferPtr
  * \param[in]  SrcDataBufferPtr
  * \param[in]  JobType
  * \return     E_OK: DataBufferPtr is ok
  *             E_NOT_OK: DataBufferPtr is invalid
  * \pre        Development error detection is enabled.
  * \context    TASK
  * \reentrant  TRUE
  * \synchronous TRUE
  */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkDataPointer(Eep_30_XXi2c01_ConstBufferPtrType DesDataBufferPtr, Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr, Eep_30_XXi2c01_JobType JobType);

/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkAddressBoundary()
**********************************************************************************************************************/
/*!
 * \brief      Checks if EepromAddress is within range.
 * \details    -
 * \param[in]  Index
 * \param[in]  EepromAddress
 * \return     E_OK: EepromAddress is valid
 *             E_NOT_OK: EepromAddress is invalid/out of range
 * \pre        Development error detection is enabled.
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkAddressBoundary(uint8 Index, Eep_30_XXi2c01_AddressType EepromAddress);

/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkLength()
**********************************************************************************************************************/
/*!
 * \brief      Checks if Length parameter is within range.
 * \details    -
 * \param[in]  Index
 * \param[in]  Length
 * \param[in]  EepromAddress
 * \return     E_OK: Length is valid
 *             E_NOT_OK: Length is invalid/out of range
 * \pre        Development error detection is enabled.
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkLength(uint8 Index, Eep_30_XXi2c01_LengthType Length, Eep_30_XXi2c01_AddressType EepromAddress);
#endif

#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_DetGetServiceId()
**********************************************************************************************************************/
/*!
 * \brief      Maps JobType to according service id which is used for development error reporting.
 * \details    -
 * \param[in]  JobType
 * \return     ServiceId
 * \pre        -
 * \config     Development error detection is enabled
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(uint8, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetGetServiceId(Eep_30_XXi2c01_JobType JobType);
#endif

/**********************************************************************************************************************
  Eep_30_XXi2c01_CheckIdle()
**********************************************************************************************************************/
/*!
 * \brief      Checks if module is currently idle.
 * \details    Checks if module is currently idle. Asynchronous jobs can only be requested if module is currently idle.
 * \return     E_OK: Module is idle
 *             E_NOT_OK: Module is busy
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CheckIdle(void);

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitStd()
**********************************************************************************************************************/
/*!
 * \brief      Performs Init service for EEP instance according to Index.
 * \details    -
 * \param[in]  Index
 * \param[in]  ConfigPtr
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitStd(uint8 Index, Eep_30_XXi2c01_ConfigPtrType ConfigPtr);

/**********************************************************************************************************************
  Eep_30_XXi2c01_SetModeStd()
**********************************************************************************************************************/
/*!
 * \brief      Performs SetMode service for EEP instance according to Index.
 * \details    -
 * \param[in]  Index
 * \param[in]  Mode
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetModeStd(uint8 Index, MemIf_ModeType Mode);


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetHandlingStd()
**********************************************************************************************************************/
/*!
 * \brief      Performs SetHandling service for EEP instance according to Index.
 * \details    -
 * \param[in]  Index
 * \param[in]  Handling
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetHandlingStd(uint8 Index, Eep_30_XXi2c01_HandlingType Handling);

/**********************************************************************************************************************
  Eep_30_XXi2c01_CancelStd()
**********************************************************************************************************************/
/*!
 * \brief      Performs Cancel service for EEP instance according to Index.
 * \details    -
 * \param[in]  Index
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CancelStd(uint8 Index);

/**********************************************************************************************************************
  Eep_30_XXi2c01_GetJobResultStd()
**********************************************************************************************************************/
/*!
 * \brief      Performs checks and retrieves current job result of component.
 * \details    -
 * \param[in]  Index
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetJobResultStd(uint8 Index);

/**********************************************************************************************************************
  Eep_30_XXi2c01_GetVersionInfoStd()
**********************************************************************************************************************/
/*!
 * \brief      Stores version information to structure referenced by input parameter.
 * \details    -
 * \param[out]  versioninfo
 * \param[in]   Index
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetVersionInfoStd(Eep_30_XXi2c01_VersionInfoPtrType versioninfo, uint8 Index);

/**********************************************************************************************************************
  Eep_30_XXi2c01_AsynchronousJob()
**********************************************************************************************************************/
/*!
 * \brief      Initiates job parameter according to requested asynchronous job.
 * \details    Initiates job parameter according to requested asynchronous job.
 *             Service is called upon Read (Incremental), Write (Incremental), Compare (Incremental), Erase requests.
 * \param[in]  Index
 * \param[in]  JobType
 * \param[in]  EepromAddress
 * \param[in]  DesDataBufferPtr
 * \param[in]  SrcDataBufferPtr
 * \param[in]  Length
 * \return     E_OK: Asynchronous job was successfully requested
 *             E_NOT_OK: Asynchronous job was declined
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_AsynchronousJob(uint8 Index,
                                                    Eep_30_XXi2c01_JobType JobType,
                                                    Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                    Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length);

#if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_TestComStd()
**********************************************************************************************************************/
/*!
 * \brief      Initiates job parameter according to test communication job.
 * \details    -
 * \param[in]  Index
 * \return     E_OK: TestCom job was successfully requested
 *             E_NOT_OK: TestCom job was declined
 * \pre        -
 * \config     Test Communication API is enabled via configuration
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 * \trace DSGN-Eep22767
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_TestComStd(uint8 Index);
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkInit()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkInit(uint8 Index)
{
    Std_ReturnType retVal = E_OK;

    /* #10 Check if corresponding ConfigPtr is NULL_PTR */
    if (Eep_30_XXi2c01_CfgPtr[Index] == NULL_PTR)
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkDataPointer()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkDataPointer(Eep_30_XXi2c01_ConstBufferPtrType DesDataBufferPtr, Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr, Eep_30_XXi2c01_JobType JobType)
{
    Std_ReturnType retVal = E_OK;

    switch (JobType)
    {
        case EEP_30_XXI2C01_JOB_READ:
            /* #10 Check if DesDataBufferPtr is NULL_PTR */
            if (DesDataBufferPtr == NULL_PTR) {
                retVal = E_NOT_OK;
            }
            break;
        case EEP_30_XXI2C01_JOB_WRITE:
        case EEP_30_XXI2C01_JOB_COMPARE:
            /* #20 Check if SrcDataBufferPtr is NULL_PTR */
            if (SrcDataBufferPtr == NULL_PTR) {
                retVal = E_NOT_OK;
            }
            break;
        default: /* case EEP_30_XXI2C01_JOB_ERASE: */
            /* #30 Erase jobs do not contain any DataBufferPtr. Thus this is always valid */
            break;

    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkAddressBoundary()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkAddressBoundary(uint8 Index, Eep_30_XXi2c01_AddressType EepromAddress)
{
    /* #10 Check if EepromAddress is within range */
    /* #20 EepromAddress needs to be extended by the configured MemBaseAddress */
    return (Std_ReturnType)(((EepromAddress + Eep_30_XXi2c01_CfgPtr[Index]->MemBaseAddress) > Eep_30_XXi2c01_CfgPtr[Index]->MemLastUsedAddress) ? E_NOT_OK : E_OK);
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_DetChkLength()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetChkLength(uint8 Index, Eep_30_XXi2c01_LengthType Length, Eep_30_XXi2c01_AddressType EepromAddress)
{
    Std_ReturnType retVal = E_OK;

    /* #10 Check if Length parameter is 0 */
    if (Length == 0u)
    {
        retVal = E_NOT_OK;
    }

    /* #20 Check if Length parameter is out of range */
    /* #30 EepromAddress needs to be extended by the configured MemBaseAddress */
    if ((Length + EepromAddress + Eep_30_XXi2c01_CfgPtr[Index]->MemBaseAddress) > (Eep_30_XXi2c01_CfgPtr[Index]->MemLastUsedAddress + 1u))
    {
        retVal = E_NOT_OK;
    }

    return retVal;
}
#endif


#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_DetGetServiceId()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(uint8, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_DetGetServiceId(Eep_30_XXi2c01_JobType JobType)
{
    uint8 retVal;

    /* #10 Map JobType to according ServiceId */
    switch (JobType)
    {
        case EEP_30_XXI2C01_JOB_READ:
            retVal = EEP_30_XXI2C01_SID_READ;
            break;
        case EEP_30_XXI2C01_JOB_WRITE:
            retVal = EEP_30_XXI2C01_SID_WRITE;
            break;
        case EEP_30_XXI2C01_JOB_COMPARE:
            retVal = EEP_30_XXI2C01_SID_COMPARE;
            break;
        default: /* case EEP_30_XXI2C01_JOB_ERASE: */
            retVal = EEP_30_XXI2C01_SID_ERASE;
            break;
    }
    return retVal;
}
#endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_CheckIdle()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CheckIdle(void)
{
    Std_ReturnType retVal = E_NOT_OK;

    /* #10 Check if Eep is idle */
    if (Eep_30_XXi2c01_GetCurrentStatus() == MEMIF_IDLE)
    {
        retVal = E_OK;
    }

    return retVal;
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_InitStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitStd(uint8 Index, Eep_30_XXi2c01_ConfigPtrType ConfigPtr)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (ConfigPtr == NULL_PTR)
    {
        errorId = EEP_30_XXI2C01_E_PARAM_CONFIG;
    }
    else
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Initialize Eep with values referenced by ConfigPtr */
        Eep_30_XXi2c01_InitCfgArray(Index, ConfigPtr); /* SBSW_EEP_03 */

        /* #20 Initialize component parameter, flags and state machine */
        Eep_30_XXi2c01_InitComponentParameter();

        /* #30 Set both mode and handling to default values from configuration */
        Eep_30_XXi2c01_SetCurrentMode(Eep_30_XXi2c01_CfgPtr[Index]->DefaultMode);
        Eep_30_XXi2c01_SetCurrentHandling(Eep_30_XXi2c01_CfgPtr[Index]->DefaultHandling);
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
      (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_INIT, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetModeStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetModeStd(uint8 Index, MemIf_ModeType Mode)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
    else
    /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_CheckIdle() == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_BUSY;
    }
    else
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Set mode of Eep module */
        Eep_30_XXi2c01_SetCurrentMode(Mode);
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
      (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_SET_MODE, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
    EEP_30_XXI2C01_DUMMY_STATEMENT(Index); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif

}


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetHandlingStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetHandlingStd(uint8 Index, Eep_30_XXi2c01_HandlingType Handling)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
    else
    /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_CheckIdle() == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_BUSY;
    }
    else
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Set handling of Eep module */
        Eep_30_XXi2c01_SetCurrentHandling(Handling);
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
        (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_SET_HANDLING, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
    EEP_30_XXI2C01_DUMMY_STATEMENT(Index); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_CancelStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_CancelStd(uint8 Index)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* #10 Perform Development Error Checks */
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */

    /* ----- IDLE CHECK ----------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_CheckIdle() == E_NOT_OK)
    {
        /* #20 Cancel currently running asynchronous job */
        Eep_30_XXi2c01_CancelJob();
    }

    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
        (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_CANCEL, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
    EEP_30_XXI2C01_DUMMY_STATEMENT(Index); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_GetJobResultStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetJobResultStd(uint8 Index)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    MemIf_JobResultType retVal;
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)
    retVal = MEMIF_JOB_FAILED;

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
    else
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(Index); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */

    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Retrieve current job result */
        retVal = Eep_30_XXi2c01_GetCurrentJobResult();
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
      (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_GET_JOB_RESULT, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif

    return retVal;
}


#if (EEP_30_XXI2C01_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_GetVersionInfoStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetVersionInfoStd(Eep_30_XXi2c01_VersionInfoPtrType versioninfo, uint8 Index)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
# if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- VERSION POINTER CHECK ------------------------------------------------------------------------*/
    if (versioninfo == NULL_PTR)
    {
        errorId = EEP_30_XXI2C01_E_PARAM_POINTER;
    }
    else

# endif /* EA_DEV_ERROR_DETECT */
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Set version information to structure referenced by versioninfo parameter */
        versioninfo->vendorID = EEP_30_XXI2C01_VENDOR_ID; /* SBSW_EEP_01 */
        versioninfo->moduleID = EEP_30_XXI2C01_MODULE_ID; /* SBSW_EEP_01 */

        versioninfo->sw_major_version = EEP_30_XXI2C01_SW_MAJOR_VERSION; /* SBSW_EEP_01 */
        versioninfo->sw_minor_version = EEP_30_XXI2C01_SW_MINOR_VERSION; /* SBSW_EEP_01 */
        versioninfo->sw_patch_version = EEP_30_XXI2C01_SW_PATCH_VERSION; /* SBSW_EEP_01 */

    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
        (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_GET_VERSION_INFO, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
    EEP_30_XXI2C01_DUMMY_STATEMENT(Index); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif
}
#endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_AsynchronousJob()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_AsynchronousJob(uint8 Index,
                                                    Eep_30_XXi2c01_JobType JobType,
                                                    Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_BufferPtrType DesDataBufferPtr,
                                                    Eep_30_XXi2c01_ConstBufferPtrType SrcDataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    Std_ReturnType retVal = E_NOT_OK;
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;
    Eep_30_XXi2c01_AddressType jobAddress;

#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)
    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
    /* ----- BUFFER POINTER CHECK ------------------------------------------------------------------------*/
    else if (Eep_30_XXi2c01_DetChkDataPointer(DesDataBufferPtr, SrcDataBufferPtr, JobType) == E_NOT_OK) /* SBSW_EEP_06 */
    {
        errorId = EEP_30_XXI2C01_E_PARAM_DATA;
    }
    /* ----- ADDRESS UPPER BOUNDARY CHECK ----------------------------------------------------------------*/
    else if (Eep_30_XXi2c01_DetChkAddressBoundary(Index, EepromAddress) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_PARAM_ADDRESS;
    }
    /* ----- LENGTH CHECK -------------------------------------------------------------------------------*/
    else if (Eep_30_XXi2c01_DetChkLength(Index, Length, EepromAddress) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_PARAM_LENGTH;
    }
    else
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */

    /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_CheckIdle() == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_BUSY;
    }
    else
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Set correct offset of EepromAddress */
        jobAddress = EepromAddress + Eep_30_XXi2c01_CfgPtr[Index]->MemBaseAddress;

        /* #20 Initialize job parameter for requested asynchronous job */
        Eep_30_XXi2c01_InitAsyncJob(jobAddress, DesDataBufferPtr, SrcDataBufferPtr, Length, Index, JobType); /* SBSW_EEP_06 */
        retVal = E_OK;
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
        (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, Eep_30_XXi2c01_DetGetServiceId(JobType), errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif

    return retVal;
} /* PRQA S 6050, 6060, 6080 */ /* MD_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STMIF */


#if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_TestComStd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EEP_LOCAL FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_TestComStd(uint8 Index)
{
    /* ----- Local Variables -----------------------------------------------------------------------------*/
    Std_ReturnType retVal = E_NOT_OK;
    uint8 errorId = EEP_30_XXI2C01_E_NO_ERROR;

    /* ----- Development Error Checks --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_DETECT == STD_ON)

    /* ----- UNINIT CHECK --------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_DetChkInit(Index) == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_UNINIT;
    }
    else
#endif /* EEP_30_XXI2C01_DEV_ERROR_DETECT */

    /* ----- BUSY CHECK ----------------------------------------------------------------------------------*/
    if (Eep_30_XXi2c01_CheckIdle() == E_NOT_OK)
    {
        errorId = EEP_30_XXI2C01_E_BUSY;
    }
    else
    {
        /* ----- Implementation ------------------------------------------------------------------------------*/
        /* #10 Initialize job parameter for requested TestCom job */
        Eep_30_XXi2c01_InitAsyncJob(0, NULL_PTR, NULL_PTR, 0, Index, EEP_30_XXI2C01_JOB_TESTCOM); /* SBSW_EEP_05 */
        retVal = E_OK;
    }
    /* ----- Development Error Report --------------------------------------------------------------------*/
#if (EEP_30_XXI2C01_DEV_ERROR_REPORT == STD_ON)
    if (errorId != EEP_30_XXI2C01_E_NO_ERROR)
    {
      (void)Det_ReportError(EEP_30_XXI2C01_MODULE_ID, Index, EEP_30_XXI2C01_SID_TEST_COM, errorId);
    }
#else
    EEP_30_XXI2C01_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3200 */ /* MD_MSR_DummyStmt */
#endif

    return retVal;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  Eep_30_XXi2c01_InitMemory()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_InitMemory(void)
{
    /* #10 Call InitMemory service for first Eep instance */
    Eep_30_XXi2c01_ChipPtr[EEP_30_XXI2C01_INST1_INDEX] = &Eep_30_XXi2c01_Chip; /* SBSW_EEP_02 */
    Eep_30_XXi2c01_CommandBufferPtr[EEP_30_XXI2C01_INST1_INDEX] = Eep_30_XXi2c01_CommandBuffer; /* SBSW_EEP_02 */
    Eep_30_XXi2c01_DataBufferPtr[EEP_30_XXI2C01_INST1_INDEX] = Eep_30_XXi2c01_DataBuffer; /* SBSW_EEP_02 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Init(Eep_30_XXi2c01_ConfigPtrType ConfigPtr)
{
    /* #10 Call Init service for first Eep instance */
    Eep_30_XXi2c01_InitStd(EEP_30_XXI2C01_INST1_INDEX, ConfigPtr); /* SBSW_EEP_03 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetMode()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetMode(MemIf_ModeType Mode)
{
    /* #10 Call SetMode service for first Eep instance */
    Eep_30_XXi2c01_SetModeStd(EEP_30_XXI2C01_INST1_INDEX, Mode);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_SetHandling()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_SetHandling(Eep_30_XXi2c01_HandlingType Handling)
{
    /* #10 Call SetHandling service for first Eep instance */
    Eep_30_XXi2c01_SetHandlingStd(EEP_30_XXI2C01_INST1_INDEX, Handling);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Read()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Read(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_BufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous read job for first Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST1_INDEX, EEP_30_XXI2C01_JOB_READ, EepromAddress, DataBufferPtr, NULL_PTR, Length); /* SBSW_EEP_04 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Write()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Write(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_ConstBufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous write job for first Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST1_INDEX, EEP_30_XXI2C01_JOB_WRITE, EepromAddress, NULL_PTR, DataBufferPtr, Length); /* SBSW_EEP_04 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Erase()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Erase(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous erase job for first Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST1_INDEX, EEP_30_XXI2C01_JOB_ERASE, EepromAddress, NULL_PTR, NULL_PTR, Length); /* SBSW_EEP_05 */
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_Compare()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Compare(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_ConstBufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous compare job for first Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST1_INDEX, EEP_30_XXI2C01_JOB_COMPARE, EepromAddress, NULL_PTR, DataBufferPtr, Length); /* SBSW_EEP_04 */
}


#if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_TestCom()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_TestCom(void)
{
    /* #10 Request an asynchronous test com job for first Eep instance */
    return Eep_30_XXi2c01_TestComStd(EEP_30_XXI2C01_INST1_INDEX);
}
#endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_GetStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetStatus(void)
{
    /* #10 Return current status for first Eep instance */
    return Eep_30_XXi2c01_GetCurrentStatus();
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_GetJobResult()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetJobResult(void)
{
    /* #10 Return current job result for first Eep instance */
    return Eep_30_XXi2c01_GetJobResultStd(EEP_30_XXI2C01_INST1_INDEX);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Cancel()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Cancel(void)
{
    /* #10 Cancel busy asynchronous job of first Eep instance */
    Eep_30_XXi2c01_CancelStd(EEP_30_XXI2C01_INST1_INDEX);
}


#if (EEP_30_XXI2C01_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_GetVersionInfo()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_GetVersionInfo(Eep_30_XXi2c01_VersionInfoPtrType versioninfo)
{
    /* #10 Store version information to structure referenced by versioninfo parameter */
    Eep_30_XXi2c01_GetVersionInfoStd(versioninfo, EEP_30_XXI2C01_INST1_INDEX); /* SBSW_EEP_01 */
}
#endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_MainFunction()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_MainFunction(void)
{
    /* #10 Execute MainFunction */
    Eep_30_XXi2c01_MainFunctionStd();
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_ComEnd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ComEnd(void)
{
    /* #10 Execute communication end callback */
    Eep_30_XXi2c01_ComEndStd();
}


/* ---------------------------- INST 2 SERVICES -------------------------------------------------------------------- */
#if (EEP_30_XXI2C01_MULTI_CHANNEL == STD_ON)

/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_InitMemory()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_InitMemory(void)
{
    /* #10 Call InitMemory service for second Eep instance */
    Eep_30_XXi2c01_ChipPtr[EEP_30_XXI2C01_INST2_INDEX] = &Eep_30_XXi2c01_Inst2_Chip; /* SBSW_EEP_02 */
    Eep_30_XXi2c01_CommandBufferPtr[EEP_30_XXI2C01_INST2_INDEX] = Eep_30_XXi2c01_Inst2_CommandBuffer; /* SBSW_EEP_02 */
    Eep_30_XXi2c01_DataBufferPtr[EEP_30_XXI2C01_INST2_INDEX] = Eep_30_XXi2c01_Inst2_DataBuffer; /* SBSW_EEP_02 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Init(Eep_30_XXi2c01_ConfigPtrType ConfigPtr)
{
    /* #10 Call Init service for second Eep instance */
    Eep_30_XXi2c01_InitStd(EEP_30_XXI2C01_INST2_INDEX, ConfigPtr); /* SBSW_EEP_03 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_SetMode()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_SetMode(MemIf_ModeType Mode)
{
    /* #10 Call SetMode service for second Eep instance */
    Eep_30_XXi2c01_SetModeStd(EEP_30_XXI2C01_INST2_INDEX, Mode);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_SetHandling()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_SetHandling(Eep_30_XXi2c01_HandlingType Handling)
{
    /* #10 Call SetHandling service for second Eep instance */
    Eep_30_XXi2c01_SetHandlingStd(EEP_30_XXI2C01_INST2_INDEX, Handling);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Read()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Read(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_BufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous read job for second Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST2_INDEX, EEP_30_XXI2C01_JOB_READ, EepromAddress, DataBufferPtr, NULL_PTR, Length); /* SBSW_EEP_04 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Write()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Write(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_ConstBufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous write job for second Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST2_INDEX, EEP_30_XXI2C01_JOB_WRITE, EepromAddress, NULL_PTR, DataBufferPtr, Length); /* SBSW_EEP_04 */
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Erase()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Erase(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous erase job for second Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST2_INDEX, EEP_30_XXI2C01_JOB_ERASE, EepromAddress, NULL_PTR, NULL_PTR, Length); /* SBSW_EEP_05 */
}

/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Compare()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Compare(Eep_30_XXi2c01_AddressType EepromAddress,
                                                    Eep_30_XXi2c01_ConstBufferPtrType DataBufferPtr,
                                                    Eep_30_XXi2c01_LengthType Length)
{
    /* #10 Request an asynchronous compare job for second Eep instance */
    return Eep_30_XXi2c01_AsynchronousJob(EEP_30_XXI2C01_INST2_INDEX, EEP_30_XXI2C01_JOB_COMPARE, EepromAddress, NULL_PTR, DataBufferPtr, Length); /* SBSW_EEP_04 */
}


# if (EEP_30_XXI2C01_TEST_COM_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_TestCom()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_TestCom(void)
{
    /* #10 Request an asynchronous TestCom job for second Eep instance */
    return Eep_30_XXi2c01_TestComStd(EEP_30_XXI2C01_INST2_INDEX);
}
# endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_GetStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_StatusType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_GetStatus(void)
{
    /* #10 Return current status for second Eep instance */
    return Eep_30_XXi2c01_GetCurrentStatus();
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_GetJobResult()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(MemIf_JobResultType, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_GetJobResult(void)
{
    /* #10 Return current job result for second Eep instance */
    return Eep_30_XXi2c01_GetJobResultStd(EEP_30_XXI2C01_INST2_INDEX);
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_Cancel()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_Cancel(void)
{
    /* #10 Cancel busy asynchronous job of second Eep instance */
    Eep_30_XXi2c01_CancelStd(EEP_30_XXI2C01_INST2_INDEX);
}


# if (EEP_30_XXI2C01_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_GetVersionInfo()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_GetVersionInfo(Eep_30_XXi2c01_VersionInfoPtrType versioninfo)
{
    /* #10 Store version information to structure referenced by versioninfo parameter */
    Eep_30_XXi2c01_GetVersionInfoStd(versioninfo, EEP_30_XXI2C01_INST2_INDEX); /* SBSW_EEP_01 */
}
# endif


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_MainFunction()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_MainFunction(void)
{
    /* #10 Execute MainFunction */
    Eep_30_XXi2c01_MainFunctionStd();
}


/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_ComEnd()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_ComEnd(void)
{
    /* #10 Execute communication end callback */
    Eep_30_XXi2c01_ComEndStd();
}

#endif


#define EEP_30_XXI2C01_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:

 MD_EEP_30_XXI2C01_9.1: rule 9.1
      Reason:     Variable seems to be unset, but it is assured by program flow that all variables are set before being used.
      Risk:       No risk.
      Prevention: Program flow has been verified by component tests and review.

 MD_Eep_30_XXI2C01_19.13: rule 19.13:
      Reason:     K&R compilers do not support the ISO glue operator '##'.
                  K&R compilers are a very old standard, thus the compatibility with these compilers is not supported.
      Risk:       No risk.
      Prevention: Glue operator '##' occurrences are marked with explicit comments to draw attention. 
*/

/* START_COVERAGE_JUSTIFICATION
    \ID COV_EEP_POSTBUILD_CONFIG
    \ACCEPT TX
    \ACCEPT XF
    \REASON This condition is not necessarily both true and false within one configuration. Condition depends on PostBuild configuration
            parameter, thus it's also not possible to restrict the appearance of this condition by preprocessor switches.

    \ID COV_EEP_COMPATIBILITY
    \ACCEPT XF
    \ACCEPT TX
    \REASON [COV_MSR_COMPATIBILITY]

    \ID COV_EEP_DEFAULT
    \ACCEPT XX
    \REASON [COV_MSR_MISRA]
END_COVERAGE_JUSTIFICATION */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EEP_01
    \DESCRIPTION GetVersionInfo API writes to the object referenced by parameter versioninfo.
                 Additionally, versioninfo is checked if it's valid.
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameters versioninfo is valid.

    \ID SBSW_EEP_02
    \DESCRIPTION Array is accessed at index corresponding to currently used EEP instance. First instance leads to array accesses with index 0.
             Second instance leads to array accesses with index 1.
    \COUNTERMEASURE \N Both the size of the array and the presence of accessing the array with index 1 is linked with the numbers of EEP modules configured.

    \ID SBSW_EEP_03
    \DESCRIPTION Constant pointer to configuration structure ConfigPtr is passed to local function.
    \COUNTERMEASURE \N The caller ensures that the configuration pointer passed to the public initialization API is valid.

    \ID SBSW_EEP_04
    \DESCRIPTION DataBufferPtr is passed to local function. This pointer is originally passed to EEP module by caller of EEP module.
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the public APIs is valid.

    \ID SBSW_EEP_05
    \DESCRIPTION NULL_PTR is passed to local function as Src/DesDataBufferPtr upon request of TestCom, Unlock and Erase jobs.
    \COUNTERMEASURE \N Input parameter DataBufferPtr i.e. NULL_PTR is not used in TestCom, Unlock and Erase jobs.

    \ID SBSW_EEP_06
    \DESCRIPTION DesDataBufferPtr and SrcDataBufferPtr are passed to local function. 
                 In Read jobs DesDataBufferPtr is equivalent to DataBufferPtr passed by user via public service, SrcDataBufferPtr is NULL_PTR.
                 In Write jobs SrcDataBufferPtr is equivalent to DataBufferPtr passed by user via public service, DesDataBufferPtr is NULL_PTR.
                 In all other jobs (i.e. Erase, TestCom) both buffers are NULL_PTR.
    \COUNTERMEASURE \N Program flow assures that only the buffer is used which is valid. Buffer access depends on current job, consequently a NULL_PTR will never be accessed.

    \ID SBSW_EEP_07
    \DESCRIPTION Pointers to local objects SrcDataBufferPtr and DesDataBufferPtr are passed to function Eep_30_XXi2c01_PrepareBuffer.
    \COUNTERMEASURE \N Passing reference of both buffers in function is legitimate because it's a reference of a local variable.

    \ID SBSW_EEP_08
    \DESCRIPTION SrcDataBufferPtr and DesDataBufferPtr are passed to function Eep_30_XXi2c01_SetDataAndStartTransfer.
    \COUNTERMEASURE \N SrcDataBufferPtr and DesDataBufferPtr are initialized depending on current job before being passed to Eep_30_XXi2c01_SetDataAndStartTransfer.
                       Upon write jobs SrcDataBufferPtr is valid and DesDataBufferPtr is NULL_PTR.
                       Upon read jobs SrcDataBufferPtr is NULL_PTR and DesDataBufferPtr is valid.
                       Upon all other jobs both buffers are NULL_PTR (i.e. TestCom, Erase)
                       Buffers are only used according to current job respectively command, which is passed to Eep_30_XXi2c01_SetDataAndStartTransfer, thus it is assured
                       that no NULL_PTR is ever accessed.

    \ID SBSW_EEP_09
    \DESCRIPTION Eep_30_XXi2c01_CommandBufferPtr is an array which contains either one or two arrays of type I2c_DataType. Array is either accessed at first
                 position or within a for-loop.
    \COUNTERMEASURE \N Number of arrays in Eep_30_XXi2c01_CommandBufferPtr is restricted by number of EEP modules configured. Length of each array depends
                       on CommandBufferSize, which is also the upper boundary for any array accesses.

    \ID SBSW_EEP_10
    \DESCRIPTION Pointer is passed to function. Pointer is contained in Eep_30_XXi2c01_CommandBufferPtr array and indexed according to currently used EEP module instance.
    \COUNTERMEASURE \N Maximum possible instance index is directly linked to number of array elements. Thus an out of bound access is impossible.

    \ID SBSW_EEP_11
    \DESCRIPTION Pointer DesBufferPtrPtr and SrcBufferPtrPtr are passed to public I2C function.
    \COUNTERMEASURE \N The caller ensures that the pointers passed are valid.

    \ID SBSW_EEP_12
    \DESCRIPTION Object referenced by DesBufferPtrPtr or SrcBufferPtrPtr is accessed and written.
    \COUNTERMEASURE \N Reference DesBufferPtrPtr and SrcBufferPtrPtr are passed to local function. Thus caller ensures that the pointers passed are valid.

    \ID SBSW_EEP_13
    \DESCRIPTION Function pointer from function pointer structure is called
    \COUNTERMEASURE \N  The compiler performs type check and therefore assures that valid function pointer is called.

    \ID SBSW_EEP_14
    \DESCRIPTION Pointer to function Ea_JobEndNotification and Ea_JobErrorNotification is called.
    \COUNTERMEASURE \R It is checked whether the pointer is NULL_PTR before execution.

    \ID SBSW_EEP_15
    \DESCRIPTION ConfigPointer array Eep_30_XXi2c01_CfgPtr is initialized at index with ConfigPtr parameter.
    \COUNTERMEASURE \N Index is directly linked to size of ConfigPointer array. Thus it's not possible to access this array out of bounds.


SBSW_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  END OF FILE: Eep_30_XXi2c01.c
 *********************************************************************************************************************/
