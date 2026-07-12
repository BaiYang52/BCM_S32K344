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
/*!        \file  NvM_MemIfMemoryCore.c
 *        \brief  NvM_MemIfMemoryCore source file
 *      \details  Implementation of the MemIf Memory Core unit of the NvM.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define NVM_MEMIFMEMORYCORE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "NvM_MemIfMemoryCore.h"

#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON)

/* Provides the variables to transfer the information between master and satellite. */
# include "NvM_MemIfAbstraction.h" 

# include "NvM_PrivateCfg.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
 

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#ifndef NVM_LOCAL /* COV_NVM_COMPATIBILITY */
# define NVM_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * NvM_MemIfMemoryCore_ForwardRequestToMemIf()
 *********************************************************************************************************************/
/*!  \brief       Forwards request to MemIf depending on request type
 *   \details     -
 *   \pre         -
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous FALSE
 *********************************************************************************************************************/
NVM_LOCAL FUNC(void, NVM_CODE) NvM_MemIfMemoryCore_ForwardRequestToMemIf(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * NvM_MemIfMemoryCore_ForwardRequestToMemIf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NVM_LOCAL FUNC(void, NVM_CODE) NvM_MemIfMemoryCore_ForwardRequestToMemIf(void)
{
  uint8 devIdx = NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync;

  switch(NvM_MemIfAbstraction_PublishedRequest.RequestAsync)
  {
  case NVM_MEMIF_REQUEST_READ:
      NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue =  /* SBSW_NvM_AccessArray_MemIfRequestState */
              MemIf_Read(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync, /* SBSW_NvM_FuncCall_PtrParam_MemIf */
                        NvM_MemIfAbstraction_PublishedRequest.BlockNumber,
                        NvM_MemIfAbstraction_PublishedRequest.BlockOffset,
                        NvM_MemIfAbstraction_PublishedRequest.DataBufferPtr,
                        NvM_MemIfAbstraction_PublishedRequest.Length);
      break;

    case NVM_MEMIF_REQUEST_WRITE:
      NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue =  /* SBSW_NvM_AccessArray_MemIfRequestState */
              MemIf_Write(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync, /* SBSW_NvM_FuncCall_PtrParam_MemIf */
                          NvM_MemIfAbstraction_PublishedRequest.BlockNumber,
                          NvM_MemIfAbstraction_PublishedRequest.DataBufferPtr);
      break;

    case NVM_MEMIF_REQUEST_ERASE:
      NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue =  /* SBSW_NvM_AccessArray_MemIfRequestState */
              MemIf_EraseImmediateBlock(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync,
                                        NvM_MemIfAbstraction_PublishedRequest.BlockNumber);
      break;

    case NVM_MEMIF_REQUEST_INVALIDATE:
      NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue =  /* SBSW_NvM_AccessArray_MemIfRequestState */
              MemIf_InvalidateBlock(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexAsync,
                                    NvM_MemIfAbstraction_PublishedRequest.BlockNumber);
      break;

    default:    /* COV_NVM_MISRA */    
      /* Calling function already checks that the requested service is not NVM_MEMIF_REQUEST_NONE. 
         So this is unreachable code. */
      NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue = E_NOT_OK; /* SBSW_NvM_AccessArray_MemIfRequestState */
      break;
  }

  if(NvM_MemIfAbstraction_RequestState[devIdx].LastRequestReturnValue == E_OK)
  {
    /* The job result is only requested if the status is not busy. Set at first status to busy, then the job result 
       is not requested, so its value does not matter.  If the job result would be set to pending before the status 
       is set to busy, it is possible that the satellite requests the status, and because it is not busy the satellite 
       requests also the job result which is pending. This is inconsistent, because the job result can only be pending 
       if the status is busy. */
    NvM_MemIfAbstraction_RequestState[devIdx].Status = MEMIF_BUSY;           /* SBSW_NvM_AccessArray_MemIfRequestState */
    NvM_MemIfAbstraction_RequestState[devIdx].JobResult = MEMIF_JOB_PENDING; /* SBSW_NvM_AccessArray_MemIfRequestState */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/***********************************************************************************************************************
 *  NvM_MemIfMemoryCore_MainFunction()
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
FUNC(void, NVM_CODE) NvM_MemIfMemoryCore_MainFunction(void)
{
  uint8 idx;

  if(NvM_MemIfAbstraction_PublishedRequest.RequestCancel == TRUE)
  {
    if(MemIf_GetStatus(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexSync) == MEMIF_BUSY)
    {
      MemIf_Cancel(NvM_MemIfAbstraction_PublishedRequest.DeviceIndexSync);
    }
    NvM_MemIfAbstraction_PublishedRequest.RequestCancel = FALSE;
  }

  if((NvM_MemIfAbstraction_PublishedRequest.RequestSetMode == TRUE))
  {
    MemIf_SetMode(NvM_MemIfAbstraction_PublishedRequest.Mode);
    NvM_MemIfAbstraction_PublishedRequest.RequestSetMode = FALSE;
  }

  /* IF an asynchronous memory job was requested: Forward the asynchronous memory job. */
  if((NvM_MemIfAbstraction_PublishedRequest.RequestAsync != NVM_MEMIF_REQUEST_NONE))
  {
    NvM_MemIfMemoryCore_ForwardRequestToMemIf();
    NvM_MemIfAbstraction_PublishedRequest.RequestAsync = NVM_MEMIF_REQUEST_NONE;
  }

  /* Synchronization of the actual Status and JobResult of the underlying devices with the RequestState structure
   * accessible by both Cores */
  for(idx = 0; idx < MEMIF_NUMBER_OF_DEVICES; idx++)
  {
    const MemIf_StatusType stat = MemIf_GetStatus(idx);

    /* only update if status has changed */
    if (stat != NvM_MemIfAbstraction_RequestState[idx].Status)
    {
      /* Here only the status transition from busy to idle can happen. The job result is only requested if the status 
         is not busy anymore. So set at first the job result that it is consistent to the status as soon at the status
         becomes idle. */
      NvM_MemIfAbstraction_RequestState[idx].JobResult = MemIf_GetJobResult(idx);    /* SBSW_NvM_AccessArray_MemIfRequestState */
      NvM_MemIfAbstraction_RequestState[idx].Status = stat;          /* SBSW_NvM_AccessArray_MemIfRequestState */
    }
  }

}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif

/**********************************************************************************************************************
 *  END OF FILE: NvM_MemIfMemoryCore.c
 *********************************************************************************************************************/
