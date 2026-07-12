/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Fls.c
 *      Project:  DrvFls_VttCanoe01Asr
 *       Module:  MICROSAR Fls
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Fls implements a Flash driver for the CANoe Emulation. Instead of a real Flash
 *                peripheral a text file is used as non-volatile memory.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define FLS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fls.h"

#if (STD_ON == FLS_DEV_ERROR_REPORT)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Fls header file */
#if (  (FLS_SW_MAJOR_VERSION != (0x03u)) \
    || (FLS_SW_MINOR_VERSION != (0x06u)) \
    || (FLS_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (FLS_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (FLS_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Fls.c and Fls.h are inconsistent!"
#endif

/* Check the version of the configuration header file */
#if (  (FLS_CFG_MAJOR_VERSION != (0x04u)) \
    || (FLS_CFG_MINOR_VERSION != (0x08u)) )
  #error "Version numbers of Fls.c and Fls_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */

#if !defined (FLS_LOCAL)
# define FLS_LOCAL static
#endif

#if !defined (FLS_LOCAL_INLINE)
# define FLS_LOCAL_INLINE LOCAL_INLINE
#endif
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* Fls_JobResult_t holds the result of the last job */
FLS_LOCAL VAR(MemIf_JobResultType, FLS_VAR) Fls_JobResult_t = MEMIF_JOB_OK;
/* Fls_ReadMode_t holds the currrent mode (norma/fast) */
FLS_LOCAL VAR(MemIf_ModeType, FLS_VAR)      Fls_ReadMode_t  = MEMIF_MODE_SLOW;
/* PointToConfig_pt holds the pointer to the configuration structure  */
FLS_LOCAL P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_CONST) PointToConfig_pt = NULL_PTR;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
/* Fls_CurrJob_t holds the struct holds status of the current job */
VAR(Fls_Job_Struct, FLS_VAR)      Fls_CurrJob_t   = {FLS_NO_JOB, 0, 0, 0, 0, NULL_PTR, NULL_PTR};
/* Fls_Status_t holds the status of the Fls */
VAR(MemIf_StatusType, FLS_VAR)    Fls_Status_t    = MEMIF_UNINIT;

/**********************************************************************************************************************
 *  TYPE DEFINITIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
FLS_LOCAL FUNC(boolean, FLS_CODE) Fls_IsMemoryErased(Fls_AddressType FlsAddress, Fls_LengthType Length);

FLS_LOCAL FUNC(void, FLS_CODE) Fls_ProcessBlankCheckJob(void);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Fls_IsMemoryErased
 *********************************************************************************************************************/
/*!
 * \brief
 * - #10 Iterate over the given memory.
 *     - #20 Read current byte from flash.
 *        - #30 If one byte is NOT equal to the FLS_ERASE_VALUE, return FALSE.
 * 
 */
FLS_LOCAL FUNC(boolean, FLS_CODE) Fls_IsMemoryErased(
  Fls_AddressType FlsAddress, 
  Fls_LengthType Length)
{
    boolean retVal = TRUE;
    Fls_LengthType index;

    for(index = 0; index < Length; index++)
    {
        uint8 value = VttFls_Read(FlsAddress + index);
        if(value != FLS_ERASE_VALUE)
        {
            retVal = FALSE;
            break;
        }
    }
    
    return retVal;
}

/**********************************************************************************************************************
 *  Fls_ProcessBlankCheckJob
 *********************************************************************************************************************/
/*!
 * \brief
 * - #10 If memory is blank/erased set job result to MEMIF_JOB_OK. Otherwise set JobResult to MEMIF_BLOCK_INCONSISTENT.
 * - #20 Set the modul status to IDLE.
 * - #30 If notification are configured, invoke the matching one.
 * 
 */
FLS_LOCAL FUNC(void, FLS_CODE) Fls_ProcessBlankCheckJob(void)
{
    Fls_JobResult_t = MEMIF_JOB_OK;

    if(Fls_IsMemoryErased(Fls_CurrJob_t.StartAddress, Fls_CurrJob_t.JobLength) == FALSE)
    {
        Fls_JobResult_t = MEMIF_BLOCK_INCONSISTENT;
    }

    Fls_Status_t = MEMIF_IDLE;

    /* Is an end callback aligned */
    if ((PointToConfig_pt->JobEndNotif != NULL_PTR) && (Fls_JobResult_t == MEMIF_JOB_OK))
    {
        PointToConfig_pt->JobEndNotif();
    }

    /* Is an error callback aligned? */
    if ((PointToConfig_pt->JobErrorNotif != NULL_PTR) && (Fls_JobResult_t == MEMIF_BLOCK_INCONSISTENT))
    {
        PointToConfig_pt->JobErrorNotif();
    }

}


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Fls_InitMemory
 *********************************************************************************************************************/
/*! \brief      This function initializes all module global variables, in case startup code does not work properly
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_InitMemory (void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* Initialize all module-global variables */
  Fls_Status_t     = MEMIF_UNINIT;        /* status of the Fls */
  Fls_JobResult_t  = MEMIF_JOB_OK;        /* result of the last job */
  Fls_ReadMode_t   = MEMIF_MODE_SLOW;     /* Currrent mode (normal/fast) */
  PointToConfig_pt = NULL_PTR;            /* Pointer to conf. struct. */

  /* Init the struct that holds status of the current job */
  Fls_CurrJob_t.JobType      = FLS_NO_JOB;
  Fls_CurrJob_t.StartAddress = 0;
  Fls_CurrJob_t.CurrAddress  = 0;
  Fls_CurrJob_t.JobLength    = 0;
  Fls_CurrJob_t.CurrByte     = 0;
  Fls_CurrJob_t.WriteFlsPointer = NULL_PTR;
  Fls_CurrJob_t.ReadFlsPointer  = NULL_PTR;

  return;
} /* End of Fls_InitMemory() */


/**********************************************************************************************************************
 *  Fls_Init_Vtt
 *********************************************************************************************************************/
/*! \brief      Initialisation of the Fls Driver
 *  \param[in]  ConfigPtr  Pointer to flash driver configuration set
 *  \return     void
 *  \pre        Module has to be uninitialized.
 *  \context    Function could be called from task level during startup
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_Init_Vtt(P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_CONST) ConfigPtr)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  /* Is the flash currently busy */
  if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Is there a valid configuration */
  else if (ConfigPtr == NULL_PTR)
  {
    errorId = FLS_E_PARAM_CONFIG;
  }
  else
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Initialization of the emulated hardware (<Filename>.nvram) is done in VttEep_Onload() or VttFls_OnLoad() */
    Fls_Status_t = MEMIF_IDLE;
    Fls_JobResult_t = MEMIF_JOB_OK;
    PointToConfig_pt = ConfigPtr; /* set the local configuration pointer to the Fls config struct    */
    /* now the data of the config should be available inside the module  */

    Fls_ReadMode_t = PointToConfig_pt->DefaultMode;
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_INIT, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* End of Fls_Init_Vtt() */


/**********************************************************************************************************************
 *  Fls_Erase
 *********************************************************************************************************************/
/*! \brief      This function erases a data block.
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to erase
 *  \return     E_OK                    Erase command has been accepted
 *  \return     E_NOT_OK                Erase command has not been accepted
 *  \pre        Module has to be initialized.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Erase(Fls_AddressType FlsAddress, Fls_LengthType Length)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Check Address parameter */
  else if (PointToConfig_pt->Size <= FlsAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - FlsAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  else
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Set driver status to busy */
    Fls_Status_t = MEMIF_BUSY;

    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_ERASE_JOB;
    Fls_CurrJob_t.StartAddress = FlsAddress;
    Fls_CurrJob_t.CurrAddress = FlsAddress;
    Fls_CurrJob_t.CurrByte = 0;
    Fls_CurrJob_t.JobLength = Length;
    /*set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_ERASE, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  
  return retVal;
} /* End of Fls_Erase() */


/**********************************************************************************************************************
 *  Fls_Write
 *********************************************************************************************************************/
/*! \brief      Writes one or more complete flash pages. 
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  DataBufferPtr           Pointer to data source buffer in RAM
 *  \param[in]  Length                  Number of bytes to write
 *  \return     E_OK                    Write command has been accepted
 *  \return     E_NOT_OK                Write command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Write
(
  Fls_AddressType FlsAddress,
  Fls_ConstBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Check parameter DataBufferPtr for referencing NULL_PTR */
  else if (DataBufferPtr == NULL_PTR)
  {
    errorId = FLS_E_PARAM_DATA;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Check Address parameter */
  else if (PointToConfig_pt->Size <= FlsAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - FlsAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  /* check if memory which should be overwritten is erased */
  else if(Fls_IsMemoryErased(FlsAddress, Length) == FALSE)
    {   
        errorId = FLS_E_VERIFY_ERASE_FAILED;
        Fls_JobResult_t = MEMIF_JOB_FAILED;  /* Job shall explicitly fail. */
    }
  else
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
  /* ---------- Implementation ------------------------------------------ */
    /* set driver state */
    Fls_Status_t = MEMIF_BUSY;

    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_WRITE_JOB;
    Fls_CurrJob_t.StartAddress = FlsAddress;
    Fls_CurrJob_t.CurrAddress = FlsAddress;
    Fls_CurrJob_t.JobLength = Length;
    Fls_CurrJob_t.CurrByte = 0;
    Fls_CurrJob_t.WriteFlsPointer = DataBufferPtr;
    /*set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_WRITE, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  
  return retVal;
} /* End of Fls_Write() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


#if (FLS_CANCEL_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_Cancel
 *********************************************************************************************************************/
/*! \brief      This function cancels ongoing read, write or erase Job.
 *  \param      void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_Cancel(void)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = FLS_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  else
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* Set driver and job state */
    Fls_Status_t = MEMIF_IDLE;

    if (MEMIF_JOB_PENDING == Fls_JobResult_t)
    {
      Fls_JobResult_t = MEMIF_JOB_CANCELLED;
# if (FLS_30_INFINEON_EXTENTION == STD_ON)
      /* Update the current job type */
      Fls_CurrJob_t.JobType = FLS_CANCEL_JOB;
# endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

      /* Calling the Error Notification if configured */
      if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
      {
        PointToConfig_pt->JobErrorNotif();
      }
    }

    /* Update the current job type */
    Fls_CurrJob_t.JobType = FLS_NO_JOB;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_CANCEL, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* End of Fls_Cancel() */
#endif /* (FLS_CANCEL_API == STD_ON) */

#if (FLS_30_INFINEON_EXTENTION == STD_ON)
/**********************************************************************************************************************
 *  Fls_CancelNonEraseJobs
 *********************************************************************************************************************/
/*! \brief      This function cancels each ongoing job (read, compare, write jobs) in case that is no erase job  
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_CancelNonEraseJobs(void)
{
  /* ----- Function Local Variables ------------------------------------- */
  /* ---------- Implementation ------------------------------------------ */
  /* check whether Fls Job is an erase job, then do nothing */
  if (Fls_CurrJob_t.JobType != FLS_ERASE_JOB)
  {
# if (FLS_CANCEL_API == STD_ON)
    Fls_Cancel();
# else
    Fls_CurrJob_t.JobType = FLS_CANCEL_JOB;
# endif /* (FLS_CANCEL_API == STD_ON) */
  }
}
#endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

#if (FLS_GET_STATUS_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_GetStatus
 *********************************************************************************************************************/
/*! \brief      This function returns the status of the Fls driver.
 *  \param[in]  void
 *  \return     MemIf_StatusType          Status of the driver
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_StatusType, FLS_CODE) Fls_GetStatus(void)
{
  /* ---------- Implementation ------------------------------------------ */
  /* return the current Flash status */
  return  Fls_Status_t;
} /* End of Fls_GetStatus() */
#endif /* (FLS_GET_STATUS_API == STD_ON) */


#if (FLS_GET_JOB_RESULT_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_GetJobResult
 *********************************************************************************************************************/
/*! \brief      This function returns the result of the last accepted job, as long as no new job
 *  \param[in]  void
 *  \return     MemIf_JobResultType     Result of the last accepted Fls job
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_JobResultType, FLS_CODE) Fls_GetJobResult(void)
{
  /* ----- Function Local Variables ------------------------------------- */
  MemIf_JobResultType retVal;
  uint8 errorId = FLS_E_NO_ERROR;  

  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = MEMIF_JOB_FAILED;
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  else
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON)*/
  {
    /* ---------- Implementation ------------------------------------------ */
    /* return the result of the last/current job */
    retVal = Fls_JobResult_t;
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_GETJOBRESULT, errorId);
  }
# else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* End of Fls_GetJobResult() */
#endif /* (FLS_GET_JOB_RESULT_API == STD_ON) */


/**********************************************************************************************************************
 *  Fls_Read
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \param[out] DataBufferPtr           Pointer to destination data buffer in RAM
 *  \return     E_OK                    Read command has been accepted
 *  \return     E_NOT_OK                Read command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Read
(
  Fls_AddressType FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Check the DataBufferPtr for being a NULL_PTR */
  else if (DataBufferPtr == NULL_PTR)
  {
    errorId = FLS_E_PARAM_DATA;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Check Address parameter */
  else if (PointToConfig_pt->Size <= FlsAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - FlsAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  else
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* set driver state */
    Fls_Status_t = MEMIF_BUSY;

    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_READ_JOB;
    Fls_CurrJob_t.StartAddress = FlsAddress;
    Fls_CurrJob_t.CurrAddress = FlsAddress;
    Fls_CurrJob_t.CurrByte = 0;
    Fls_CurrJob_t.JobLength = Length;
    Fls_CurrJob_t.ReadFlsPointer = DataBufferPtr;
  
    /* set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_READ, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  
  return retVal;
} /* End of Fls_Read() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (FLS_30_INFINEON_EXTENTION == STD_ON)
/**********************************************************************************************************************
 *  Fls_ReadWordsSync
 *********************************************************************************************************************/
/*! \brief      Function reads word data from the Fls synchronous (without MainFunction calls)
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of words to read
 *  \param[out] DataBufferPtr           Pointer to destination data buffer in RAM
 *  \return     E_OK                    ReadSynch command has been accepted
 *  \return     E_NOT_OK                ReadSynch command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
# if (FLS_READSYNC_API == STD_ON)
FUNC(Std_ReturnType, FLS_CODE) Fls_ReadWordsSync
(
  Fls_AddressType FlsAddress,
  uint32* DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  
  /* ---------- Implementation ------------------------------------------ */
  /* Convert to byte read for using simulated flash (Fee works with word-size) - multiply with four */
  uint32 usedVttLength = Length << 2U;
  static uint32 usedVttReadCounter;
  
  if (usedVttLength > FLS_PAGE_SIZE)
  {
    for(usedVttReadCounter = 0; usedVttReadCounter < usedVttLength; usedVttReadCounter += FLS_PAGE_SIZE)
    {
      retVal = Fls_ReadSync(FlsAddress + usedVttReadCounter, 
               (Fls_VarBytePtrType)DataBufferPtr + usedVttReadCounter, 
               FLS_PAGE_SIZE);
    }
  }
  else
  {
     retVal = Fls_ReadSync(FlsAddress, (Fls_VarBytePtrType)DataBufferPtr, usedVttLength);
  }
  
  /* if read passed, job is finished using ReadSynch NO MainFunction is neccessary */
  if (retVal == E_OK)
  {
    Fls_CurrJob_t.JobType = FLS_NO_JOB;
  }
  
  return retVal;
}
# endif /* (FLS_READSYNC_API == STD_ON) */
#endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

#if (FLS_30_INFINEON_EXTENTION == STD_ON)
/**********************************************************************************************************************
 *  Fls_GetNotifCaller
 *********************************************************************************************************************/
/*! \brief      Function returns the FLS Job that raised the job end notification
 *  \param[in]  void
 *  \param[out] void
 *  \return     Fls_JobType             Fls job which rais the notification
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Fls_JobType, FLS_CODE) Fls_GetNotifCaller(void)
{
  return(Fls_CurrJob_t.JobType);
}
#endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

#if (FLS_30_INFINEON_EXTENTION == STD_ON)

# if (FLS_USE_ERASESUSPEND == STD_ON)
/**********************************************************************************************************************
 *  Fls_SuspendErase
 *********************************************************************************************************************/
/*! \brief      This function shold suspends an ongoing erase of a sector. 
 *              ATTENTION: within a PC system (VTT) where a text file is used instead of a real Flash, 
 *                         an interuption of Erase-Job cannot be really executed
 *  \param[in]  SectorNumber   Sector on which erase is to be suspended
 *  \param[out] void
 *  \return     E_OK 
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_SuspendErase(uint32 SectorNumber)
{
  Fls_CurrJob_t.JobType = FLS_NO_JOB;
  return(E_OK);
}    
/**********************************************************************************************************************
 *  Fls_ResumeErase
 *********************************************************************************************************************/
/*! \brief      This function should resume an ongoing erase of a sector. 
 *              ATTENTION: within a PC system (VTT) where a text file is used instead of a real Flash, 
 *                         a resume cannot be really executed
 *  \param[in]  SectorNumber   Sector on which erase is to be resumed
 *  \param[out] void
 *  \return     E_OK
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_ResumeErase(uint32 SectorNumber)
{
  Fls_CurrJob_t.JobType = FLS_NO_JOB;
  return(E_OK);
}
# endif /* FLS_USE_ERASESUSPEND == STD_ON */

/**********************************************************************************************************************
 *  Fls_VerifyErase
 *********************************************************************************************************************/
/*! \brief      This function verifies the erase of a sector 
 *              ATTENTION: within a PC system (VTT) where a text file is used instead of a flash, 
 *                         an erase cannot be failed
 *  \param[in]  Offset                      offset (SectorNumber or TargetAddress) on which erase is to be resumed
 *  \param[out] UnerasedWordlineAddressPtr  not used in simulation
 *  \param[out] UnerasedWordlineCountPtr    not used in simulation
 *  \return     E_OK       Erase verification Job passed
 *              E_NOT_OK   Erase verification Job failed
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_VerifyErase(uint32 Offset, uint32 * UnerasedWordlineAddressPtr, uint8 * const UnerasedWordlineCountPtr)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ---------- Implementation ------------------------------------------ */
  /* Check if no operation ongoing and erase was finished successfully */
  if (Fls_CurrJob_t.JobType == FLS_NO_JOB)
  {
    retVal = E_OK;
  }

  /* A physical compare of flash data with ERASE-VALUE is not neccessary because text-files are used instead of Flash HW.
   * It is assumed that the writing of ERASE-VALUE works 
   */
  return retVal;
}

/**********************************************************************************************************************
 *  Fls_VerifySectorErase
 *********************************************************************************************************************/
/*! \brief      This function verifies the erase of a sector 
 *              ATTENTION: within a PC system (VTT) where a text file is used instead of a flash, 
 *                         an erase cannot be failed
 *  \param[in]  TargetAddress               Address of Sector on which erase is to be resumed
 *  \param[in]  Sector                      Sector number
 *  \param[out] UnerasedWordlineAddressPtr  not used in simulation
 *  \param[out] UnerasedWordlineCountPtr    not used in simulation
 *  \return     E_OK       Erase verification Job passed
 *              E_NOT_OK   Erase verification Job failed
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_VerifySectorErase
(
  Fls_AddressType TargetAddress, 
  uint32 * UnerasedWordlineAddressPtr, 
  uint8 * const UnerasedWordlineCountPtr, uint8 Sector
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ---------- Implementation ------------------------------------------ */
  /* Check if no operation ongoing and erase was finished successfully */
  if (Fls_CurrJob_t.JobType == FLS_NO_JOB)
  {
    retVal = E_OK;
  }

  /* A physical compare of flash data with ERASE-VALUE is not neccessary because text-files are used instead of Flash HW.
   * It is assumed that the writing of ERASE-VALUE works 
   */
  return retVal;
}

#endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

#if (FLS_COMPARE_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_Compare
 *********************************************************************************************************************/
/*! \brief      Compares the contents of an area of flash memory with that of an application data buffer. 
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  DataBufferPtr           Pointer to data buffer that contains the compare data
 *  \param[in]  Length                  Number of bytes to compare
 *  \return     E_OK                    Compare command has been accepted
 *  \return     E_NOT_OK                Compare command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Compare
(
  Fls_AddressType FlsAddress,
  Fls_ConstBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Check DataBufferPtr for not referencing NULL_PTR */
  else if (DataBufferPtr == NULL_PTR)
  {
    errorId = FLS_E_PARAM_DATA;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Check Address parameter */
  else if (PointToConfig_pt->Size <= FlsAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - FlsAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  else  
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* set driver state */
    Fls_Status_t = MEMIF_BUSY;

    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_COMPARE_JOB;
    Fls_CurrJob_t.StartAddress = FlsAddress;
    Fls_CurrJob_t.CurrAddress = FlsAddress;
    Fls_CurrJob_t.CurrByte = 0;
    Fls_CurrJob_t.JobLength = Length;
    Fls_CurrJob_t.WriteFlsPointer = DataBufferPtr;
  
    /*set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_COMPARE, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* End of Fls_Compare() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (FLS_COMPARE_API == STD_ON) */

#if (FLS_30_INFINEON_EXTENTION == STD_ON)
/**********************************************************************************************************************
 *  Fls_CompareWordsSync
 *********************************************************************************************************************/
/*! \brief      Compares the contents of an area of flash memory synchronously (word Compare)
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  DataBufferPtr           Pointer to data buffer that contains the compare data
 *  \param[in]  Length                  Number of words to compare
 *  \return     E_OK                    CompareWordSynch command has been accepted
 *  \return     E_NOT_OK                CompareWordSynch command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_CompareWordsSync
(
  Fls_AddressType FlsAddress,
  Fls_ConstWordPtrType DataBufferPtr,
  Fls_LengthType Length)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ---------- Implementation ------------------------------------------ */

  /* it shall be ensured that Length is an uinte32, otherwise the loop does'nt work */
  uint8 FlashDataBufferRead[FLS_BUF_SIZE];
  uint32* FlashDataPtr;
  /* Convert to byte-read for using simulated flash (Fee works with word-size) - multiply with four */
  uint32 usedVttLength = Length << 2U;
  uint32 usedVttReadCounter;
        
  /* Still bytes to read? */
  for (usedVttReadCounter = 0; usedVttReadCounter < usedVttLength; usedVttReadCounter++)
  {
    /* Save the read value */
    FlashDataBufferRead[usedVttReadCounter] = VttFls_Read(FlsAddress + usedVttReadCounter);
  }
  FlashDataPtr = (uint32*)FlashDataBufferRead;

  do
  {
    /* compare the data which are read from flash with given data buffer (word Compare)*/
    if (*FlashDataPtr != *DataBufferPtr)
    {
      Length = 0U;
    }
    else
    {
      retVal = E_OK;
      FlashDataPtr++;
      DataBufferPtr++;
      Length--;
    }
  } while(Length > 0U);

  /* if compare passed, job is finished using CompareSynch NO MainFunction is neccessary */
  if (retVal == E_OK)
  {
    Fls_CurrJob_t.JobType = FLS_NO_JOB;
  }
  return retVal;
}
#endif /* (FLS_30_INFINEON_EXTENTION == STD_ON) */

#if (FLS_SET_MODE_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_SetMode
 *********************************************************************************************************************/
/*! \brief      Function used to switch between normal and fast Fls access.
 *  \param[in]  Mode                    Flash operation mode (normal/fast access)
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_SetMode(MemIf_ModeType Mode)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  /* Check the Mode for being either MEMIF_MODE_FAST or MEMIF_MODE_SLOW */
  if ( (MEMIF_MODE_FAST != Mode) && (MEMIF_MODE_SLOW != Mode) )
  {
    errorId = FLS_E_PARAM_DATA;
  }
  /* Is the driver initialized */
  else if (Fls_Status_t == MEMIF_UNINIT)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Is the driver currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  else
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* Set new read mode */
    Fls_ReadMode_t = Mode;
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_SETMODE, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* End of Fls_SetMode() */
#endif /* (FLS_SET_MODE_API == STD_ON) */


#if (FLS_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief      This function provides the module version information.
 *              The version information includes:
 *                - Module Id
 *                - Vendor Id
 *                - Vendor specific version numbers
 *  \param[out] versioninfo             Pointer to where to store the version information of this module.
 *  \return     void
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FLS_APPL_DATA) versioninfo)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = FLS_E_PARAM_VINFO;
  }
  else
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    versioninfo->vendorID = (FLS_VENDOR_ID);
    versioninfo->moduleID = (FLS_MODULE_ID);
    versioninfo->sw_major_version = (FLS_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (FLS_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (FLS_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_GETVERSIONINFO, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* End of Fls_GetVersionInfo() */
#endif  /* (FLS_VERSION_INFO_API == ON) */


#if (FLS_COPY_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_Copy
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls and copies block to Fls again
 *  \param[in]  TargetAddress           TargetAddress offset in Fls
 *  \param[in]  SourceAddress           SourceAddress offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \return     E_OK                    Copy command has been accepted
 *  \return     E_NOT_OK                Copy command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_Copy
(
  Fls_AddressType TargetAddress,
  Fls_AddressType SourceAddress,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = FLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* check if the Targetaddress is between ROM start and end address */
  else if ((PointToConfig_pt->Size) <= TargetAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* check if the Sourceaddress is between ROM start and end address */
  else if ((PointToConfig_pt->Size) <= SourceAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u)
         || (Length > (PointToConfig_pt->Size - TargetAddress))
         || (Length > (PointToConfig_pt->Size - SourceAddress)) ) 
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  /* Overlap Sourceaddress and Targetaddress? */
  else if ( ((TargetAddress <= SourceAddress) && ((TargetAddress + Length) > SourceAddress))
         || ((TargetAddress >= SourceAddress) && ((SourceAddress + Length) > TargetAddress)) )
  {
    errorId = FLS_E_ADDRESS_OVERLAP;
  }
  else   
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* set driver state */
    Fls_Status_t = MEMIF_BUSY;

    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_COPY_JOB;
    Fls_CurrJob_t.StartAddress = SourceAddress;
    Fls_CurrJob_t.CurrAddress = TargetAddress;
    Fls_CurrJob_t.CurrByte = 0;
    Fls_CurrJob_t.JobLength = Length;

    /*set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_COPY, errorId);
  }
# else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  
  return retVal;
} /* End of Fls_Copy() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (STD_ON == FLS_COPY_API) */


#if (FLS_READSYNC_API == STD_ON)
/**********************************************************************************************************************
 *  Fls_ReadSync
 *********************************************************************************************************************/
/*! \brief      Function reads a data block from the Fls synchronous
 *  \param[in]  FlsAddress              Address offset in Fls
 *  \param[in]  Length                  Number of bytes to read
 *  \param[out] DataBufferPtr           Pointer to destination data buffer in RAM
 *  \return     MEMIF_JOB_OK            Read command has been accepted
 *  \return     MEMIF_JOB_FAILED        Read command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(MemIf_JobResultType, FLS_CODE) Fls_ReadSync
(
  Fls_AddressType FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  MemIf_JobResultType retVal;
  uint8 errorId = FLS_E_NO_ERROR;
  uint8 value;
  uint16 index;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = MEMIF_JOB_FAILED;
  /* Is the driver initialized? */
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  /* Check the DataBufferPtr for being a NULL_PTR */
  else if (DataBufferPtr == NULL_PTR)
  {
    errorId = FLS_E_PARAM_DATA;
  }
  /* Is the flash currently busy */
  else if (Fls_Status_t == MEMIF_BUSY)
  {
    errorId = FLS_E_BUSY;
  }
  /* Check Address parameter */
  else if (PointToConfig_pt->Size <= FlsAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - FlsAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  }
  else 
# endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    
    /* Still bytes to read? */
	for (index = 0u; index < Length; index++)
    {
      /* Read 1 Bytes into Value from Filehandler */
      value= VttFls_Read(FlsAddress + index);
      /* Save the value  */
      (DataBufferPtr)[index] = value;
    }

    /* is the read complete? */
    if (index == Length)
    {
      /* Job is OK */
      retVal = MEMIF_JOB_OK;
    }
    else
    {
      /* is a error process callback aligned */
      if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
      {
        /* call the callback */
        PointToConfig_pt->JobErrorNotif();
      }
      /* job failed */
      retVal = MEMIF_JOB_FAILED;
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
# if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_READ_SYNC, errorId);
  }
# else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* End of Fls_ReadSync() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif /* (FLS_READSYNC_API == STD_ON) */

/**********************************************************************************************************************
 *  Fls_BlankCheck
 *********************************************************************************************************************/
/*! \brief      This function is used to simulate a blank check
 *  \param[in]  TargetAddress           Address in Fls 
 *  \param[in]  Length                  Number of bytes to be checked
 *  \return     E_OK                    BlankCheck command has been accepted
 *  \return     E_NOT_OK                BlankCheck command has not been accepted
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_BlankCheck(
  Fls_AddressType TargetAddress, 
  Fls_LengthType Length
)
{
  /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = FLS_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK; 
  /* Check Address parameter */
  if (PointToConfig_pt->Size <= TargetAddress)
  {
    errorId = FLS_E_PARAM_ADDRESS;
  }
  /* Check Length parameter */
  else if ( (Length == 0u) || (Length > (PointToConfig_pt->Size - TargetAddress)) )
  {
    errorId = FLS_E_PARAM_LENGTH;
  } 
  else 
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* configure the current job */
    Fls_CurrJob_t.JobType = FLS_BLANKCHECK_JOB;
    Fls_CurrJob_t.StartAddress = TargetAddress;
    Fls_CurrJob_t.CurrAddress = TargetAddress;
    Fls_CurrJob_t.JobLength = Length;
  
    /* set job state starts the processing */
    Fls_JobResult_t = MEMIF_JOB_PENDING;
    
    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_BLANK_CHECK, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  return retVal;
} /* End of Fls_BlankCheck() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Fls_SimulateError
 *********************************************************************************************************************/
/*! \brief      This function is used to simulate an error in the currently processed job
 *  \param[in]  void
 *  \return     E_OK                    An error was simulated
 *  \return     E_NOT_OK                No error could be simulated
 *  \pre        Module has to be initialized.
 *  \note
 *********************************************************************************************************************/
FUNC(Std_ReturnType, FLS_CODE) Fls_SimulateError(void)
{
 /* ----- Function Local Variables ------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ---------- Implementation ------------------------------------------ */
  /* Is a job pending that could be ended with a simulated error? */
  if (MEMIF_JOB_PENDING == Fls_JobResult_t)
  {
    switch (Fls_CurrJob_t.JobType)
    {
      case FLS_READ_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;

        /* is a error callback aligned? */
        if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_WRITE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned ? */
        if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_ERASE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned? */
        if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      case FLS_COMPARE_JOB:
      {
        Fls_Status_t = MEMIF_IDLE;
        Fls_JobResult_t = MEMIF_JOB_FAILED;
        /* Is an error callback aligned? */
        if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
        {
          /* call the callback */
          PointToConfig_pt->JobErrorNotif();
        }
        break;
      }

      default:
      {
        /* Nothing to do in here */
        retVal = E_NOT_OK;
        break;
      }
    }
  }
  else
  {
    /* There's nothing that could cause an error*/
    retVal = E_NOT_OK;
  }

  return retVal;
} /* End of Fls_SimulateError() */


/**********************************************************************************************************************
 *  Fls_MainFunction
 *********************************************************************************************************************/
/*! \brief      This function is cyclically called and processes the Fls Jobs read, write, erase and compare
 *  \param[in]  void
 *  \return     void
 *  \pre        Module has to be initialized.
 *  \context    Function could be called from task level (cyclically)
 *  \note
 *********************************************************************************************************************/
FUNC(void, FLS_CODE) Fls_MainFunction(void)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = FLS_E_NO_ERROR;
  uint32 index;

/* ----- Development Error Checks ------------------------------------- */
#if (FLS_DEV_ERROR_DETECT == STD_ON)
  if (MEMIF_UNINIT == Fls_Status_t)
  {
    errorId = FLS_E_UNINIT;
  }
  else
#endif /* (FLS_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ---------- Implementation ------------------------------------------ */
    /* Is currently a job to be processed? */
    if (Fls_JobResult_t == MEMIF_JOB_PENDING)
    {
      /* Which job type is to be processed? */
      switch (Fls_CurrJob_t.JobType)
      {
        /* a read job is to process ---------------------------------------------------------------------------------- */
        case FLS_READ_JOB:
        {
          Fls_LengthType blockSize;
          if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
          {
            blockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
          }
          else
          {
            blockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
          }

          /* Still bytes to read? */
          for (index = 0u; ((index < blockSize) && ((Fls_CurrJob_t.CurrByte) < (Fls_CurrJob_t.JobLength))); index++)
          {
            /* Save the value  */
            (Fls_CurrJob_t.ReadFlsPointer)[Fls_CurrJob_t.CurrByte] = VttFls_Read(Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte);
            /* increment byte counter */
            Fls_CurrJob_t.CurrByte++;
          }

          /* is the read complete? */
          if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
          {
            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (PointToConfig_pt->JobEndNotif != NULL_PTR)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /* end read job -----------------------------------------------------------------------------------------------*/

        /* a write job is to process ----------------------------------------------------------------------------------*/
        case FLS_WRITE_JOB:
        {
          uint8 value;
          Fls_LengthType blockSize;
        
          if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
          {
            blockSize = (PointToConfig_pt->NormalWriteBlockSize); /* normal write mode selected */
          }
          else
          {
            blockSize = (PointToConfig_pt->FastWriteBlockSize); /* fast write mode selected */
          }
        
          /* still bytes to write? */
          for (index = 0u; ((index < blockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength)); index++)
          { 
            value = Fls_CurrJob_t.WriteFlsPointer[Fls_CurrJob_t.CurrByte];
            VttFls_Write(Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte,value);
            /* increment byte counter */
            Fls_CurrJob_t.CurrByte++;
          }

          /* write complete? */
          if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
          {
            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (PointToConfig_pt->JobEndNotif != NULL_PTR)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /* end write job --------------------------------------------------------------------------------------------- */

        /* a erase job is to process --------------------------------------------------------------------------------- */
        case FLS_ERASE_JOB:
        {
          /* still bytes to erase? */
          for (index = 0u; (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength); index++)
          {
            VttFls_Write(Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte, FLS_ERASE_VALUE);
            /* increment byte counter */
            Fls_CurrJob_t.CurrByte++;
          }

          /* erase complete? */
          if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
          {
            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (PointToConfig_pt->JobEndNotif != NULL_PTR)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /* end erase job --------------------------------------------------------------------------------------------- */

        /* a compare job is to process ------------------------------------------------------------------------------- */
        case FLS_COMPARE_JOB:
        {
          Fls_LengthType blockSize;
          uint8 value;
          uint8 compare_u8 = TRUE;

          if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
          {
            blockSize = (PointToConfig_pt->NormalReadBlockSize); /* normal read mode selected */
          }
          else
          {
            blockSize = (PointToConfig_pt->FastReadBlockSize); /* fast read mode selected */
          }

          /* still bytes to compare? */
          for (index = 0u; ((index < blockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength) && (TRUE == compare_u8)); index++)
          {
            /* Read a Byte into value from Filehandler */
            value = VttFls_Read(Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte);

            if ( ((Fls_CurrJob_t.WriteFlsPointer)[Fls_CurrJob_t.CurrByte]) != value)
            {
              compare_u8 = FALSE; /* not equal -> break */

              Fls_Status_t = MEMIF_IDLE;
              Fls_JobResult_t = MEMIF_BLOCK_INCONSISTENT/*MEMIF_COMPARE_UNEQUAL*/;

              /* is a error callback aligned */
              if (PointToConfig_pt->JobErrorNotif != NULL_PTR)
              {
                /* call the callback */
                PointToConfig_pt->JobErrorNotif();
                break;
              }
            }
            else
            {
              /* increment byte counter */
              Fls_CurrJob_t.CurrByte++;
            }
          } /* end loop */

          /* compare complete? */
          if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
          {
            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (PointToConfig_pt->JobEndNotif != NULL_PTR)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /* end compare job ------------------------------------------------------------------------------------------- */
        /* A copy job is to process ---------------------------------------------------------------------------------- */
        case FLS_COPY_JOB:
        {
          uint8 value;
          Fls_LengthType blockSize;
        
          if (MEMIF_MODE_SLOW == Fls_ReadMode_t)
          {
            blockSize = (PointToConfig_pt->NormalWriteBlockSize); /* normal write mode selected */
          }
          else
          {
            blockSize = (PointToConfig_pt->FastWriteBlockSize); /* fast write mode selected */
          }
   
           /* still bytes to copy? */
          for (index = 0u; ((index < blockSize) && (Fls_CurrJob_t.CurrByte < Fls_CurrJob_t.JobLength)); index++)
          {
            value = VttFls_Read(Fls_CurrJob_t.StartAddress + Fls_CurrJob_t.CurrByte);
            VttFls_Write(Fls_CurrJob_t.CurrAddress + Fls_CurrJob_t.CurrByte,value);
            /* increment byte counter */
            Fls_CurrJob_t.CurrByte++;
          }

          /* is the copy complete? */
          if (Fls_CurrJob_t.CurrByte == Fls_CurrJob_t.JobLength)
          {
            Fls_Status_t = MEMIF_IDLE;
            Fls_JobResult_t = MEMIF_JOB_OK;

            /* is a end process callback aligned */
            if (PointToConfig_pt->JobEndNotif != NULL_PTR)
            {
              /* call the callback */
              PointToConfig_pt->JobEndNotif();
            }
          }
          break;
        }
        /* end copy job ------------------------------------------------------------------------------------------- */
        /* a blankCheck job is to process --------------------------------------------------------------------------*/
        case FLS_BLANKCHECK_JOB:
        {
          Fls_ProcessBlankCheckJob();
          break;
        }
        /* end blankCheck job ------------------------------------------------------------------------------------- */

        default:
        {
          /* Shall never be reached */
          break;
        }
      } /* end switch: job type */
    } /* End currentl processed job */

    /* if current job is finished, set job type to NONE*/
    if (Fls_Status_t == MEMIF_IDLE)
    {
       Fls_CurrJob_t.JobType = FLS_NO_JOB;
    }
  }
  
  /* ----- Development Error Report --------------------------------------- */
#if (FLS_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FLS_E_NO_ERROR)
  {
    (void)Fls_Errorhook(FLS_SID_MAINFUNCTION, errorId);
  }
#else
  FLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* End of Fls_MainFunction() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  END OF FILE: Fls.c
 *********************************************************************************************************************/

