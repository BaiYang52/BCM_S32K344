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
/*!        \file  NvM_MemIfAbstraction.c
 *        \brief  NvM_MemIfAbstraction source file
 *      \details  Implementation of the MemIf Abstraction unit of the NvM.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define NVM_MEMIF_ABSTRACTION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "NvM_MemIfAbstraction.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON)

#define NVM_START_SEC_VAR_SHARED_DATA
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Variable where the current published requests are stored.
 * At a time always only one asynchronous and one synchronous request can be triggered by NvM */
VAR(NvM_MemIfAbstraction_PublishedRequestType, NVM_PRIVATE_DATA)  NvM_MemIfAbstraction_PublishedRequest;

/*! Array where the current state of the underlying devices are stored. */
/* The array stores the states and job results of all devices. In this way the satellite always knows the status
   of all devices. Furthermore the information, of which device the status is, must not be synchronized. */
VAR(NvM_MemIfAbstraction_RequestStateType, NVM_PRIVATE_DATA)  NvM_MemIfAbstraction_RequestState[MEMIF_NUMBER_OF_DEVICES];

#define NVM_STOP_SEC_VAR_SHARED_DATA
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * NvM_MemIfAbstraction_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_Init(void)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON)
  uint8 idx;

  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_NONE;
  NvM_MemIfAbstraction_PublishedRequest.RequestCancel = FALSE;
  NvM_MemIfAbstraction_PublishedRequest.RequestSetMode = FALSE;

  /* No need to initialize other members of NvM_MemIfAbstraction_PublishedRequest structure as they are not used for
   * processing but only to save the to be forwarded parameters */

  for(idx = 0; idx < MEMIF_NUMBER_OF_DEVICES; idx++)
  {
    NvM_MemIfAbstraction_RequestState[idx].Status = MEMIF_IDLE;           /* SBSW_NvM_AccessArray_MemIfRequestState */
    NvM_MemIfAbstraction_RequestState[idx].JobResult = MEMIF_JOB_OK;      /* SBSW_NvM_AccessArray_MemIfRequestState */
    NvM_MemIfAbstraction_RequestState[idx].LastRequestReturnValue = E_OK; /* SBSW_NvM_AccessArray_MemIfRequestState */
  }
#endif
}


/***********************************************************************************************************************
 * NvM_MemIfAbstraction_Read()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_Read(
    uint8 DeviceIndex,
    uint16 BlockNumber,
    uint16 BlockOffset,
    uint8 *DataBufferPtr,
    uint16 Length)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_Read(DeviceIndex, BlockNumber, BlockOffset, DataBufferPtr, Length);   /* SBSW_NvM_FuncCall_PtrParam_MemIf */
#else

  NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync = DeviceIndex;
  NvM_MemIfAbstraction_PublishedRequest.BlockNumber = BlockNumber;
  NvM_MemIfAbstraction_PublishedRequest.BlockOffset = BlockOffset;
  NvM_MemIfAbstraction_PublishedRequest.DataBufferPtr = DataBufferPtr;
  NvM_MemIfAbstraction_PublishedRequest.Length = Length;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
    * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_READ;

  return E_OK;
#endif
}


/***********************************************************************************************************************
 * NvM_MemIfAbstraction_Write()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_Write(
    uint8 DeviceIndex,
    uint16 BlockNumber,
    uint8 * DataBufferPtr)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_Write(DeviceIndex, BlockNumber, DataBufferPtr);                    /* SBSW_NvM_FuncCall_PtrParam_MemIf */
#else

  NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync = DeviceIndex;
  NvM_MemIfAbstraction_PublishedRequest.BlockNumber = BlockNumber;
  NvM_MemIfAbstraction_PublishedRequest.DataBufferPtr = DataBufferPtr;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
    * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_WRITE;

  return E_OK;
#endif
}


# if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_EraseImmediateBlock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_EraseImmediateBlock(
    uint8 DeviceIndex,
    uint16 BlockNumber)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_EraseImmediateBlock(DeviceIndex, BlockNumber);
#else

  NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync = DeviceIndex;
  NvM_MemIfAbstraction_PublishedRequest.BlockNumber = BlockNumber;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
    * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_ERASE;

  return E_OK;
#endif
}

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_InvalidateBlock()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_InvalidateBlock(
     uint8 DeviceIndex,
     uint16 BlockNumber)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_InvalidateBlock(DeviceIndex, BlockNumber);
#else

  NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync = DeviceIndex;
  NvM_MemIfAbstraction_PublishedRequest.BlockNumber = BlockNumber;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
    * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_INVALIDATE;

  return E_OK;
#endif
}
# endif


/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_Cancel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_Cancel(
    uint8 DeviceIndex)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  MemIf_Cancel(DeviceIndex);
#else
  NvM_MemIfAbstraction_PublishedRequest.DeviceIndexSync = DeviceIndex;

  /* Ensure there is no RequestAsync set afterwards, e.g. in case if last RequestAsync was not yet forwarded */
  NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_NONE;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
   * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestCancel = TRUE;
#endif
}

# if(NVM_DRV_MODE_SWITCH == STD_ON)
/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_SetMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_SetMode(
    MemIf_ModeType Mode)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  MemIf_SetMode(Mode);
#else
  NvM_MemIfAbstraction_PublishedRequest.Mode = Mode;

  /* Must be set at last as this struct member triggers the actual job forwarding within the NvM memory core part.
   * Ensure all other parameters are already set */
  NvM_MemIfAbstraction_PublishedRequest.RequestSetMode = TRUE;
#endif
}
# endif

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_GetStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(MemIf_StatusType, NVM_CODE) NvM_MemIfAbstraction_GetStatus(
    uint8 DeviceIndex)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_GetStatus(DeviceIndex);
#else
  MemIf_StatusType status;

  /* The cancel service clears the asynchronous job request. So if an asynchronous 
     job is requested here, it was setup after the cancel request. */
  if(NvM_MemIfAbstraction_PublishedRequest.RequestAsync != NVM_MEMIF_REQUEST_NONE)
  {
    status =  MEMIF_BUSY;
  }
  else if(NvM_MemIfAbstraction_PublishedRequest.RequestCancel == TRUE)
  {
    status = MEMIF_IDLE;
  }
  else
  {
    if (DeviceIndex == MEMIF_BROADCAST_ID)
    {
      uint8 i;

      status = MEMIF_IDLE;

      for (i = 0u; i < MEMIF_NUMBER_OF_DEVICES; i++)
      {
        MemIf_StatusType deviceStatus = NvM_MemIfAbstraction_RequestState[i].Status;

        if ((deviceStatus == MEMIF_BUSY) || (deviceStatus == MEMIF_BUSY_INTERNAL))
        {
          status = deviceStatus;
          break;
        }
      }
    }
    else
    {
      status = NvM_MemIfAbstraction_RequestState[DeviceIndex].Status;
    }
  }

  return status;
#endif
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_GetJobResult()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(MemIf_JobResultType, NVM_CODE) NvM_MemIfAbstraction_GetJobResult(
    uint8 DeviceIndex)
{
#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_OFF)
  return MemIf_GetJobResult(DeviceIndex);
#else
  MemIf_JobResultType jobResult;

  /* First it is checked if a new request was triggered and afterwards if cancel was requested as
   * NvM would always first cancel and then trigger.
   * In both cases the ReturnValue of the last job is not relevant any more, therefore this condition
   * is placed afterwards */
  if(NvM_MemIfAbstraction_PublishedRequest.RequestAsync != NVM_MEMIF_REQUEST_NONE)
  {
    jobResult =  MEMIF_JOB_PENDING;
  }
  else if(NvM_MemIfAbstraction_PublishedRequest.RequestCancel == TRUE)
  {
    jobResult = MEMIF_JOB_CANCELED;
  }
  else if(NvM_MemIfAbstraction_RequestState[DeviceIndex].LastRequestReturnValue != E_OK)
  {
    /* Map rejected job to job result MEMIF_JOB_FAILED */
    jobResult = MEMIF_JOB_FAILED;;
  }
  else
  {
    jobResult = NvM_MemIfAbstraction_RequestState[DeviceIndex].JobResult;
  }
  return jobResult;
#endif
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  END OF FILE: NvM_Master_Com.c
 *********************************************************************************************************************/
