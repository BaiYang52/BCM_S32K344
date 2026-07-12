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
/*!        \file  Com_CprQueue.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_CPRQUEUE_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CprQueue.h"
#include "Com.h"

#include "SchM_Com.h"

#include "vstdlib.h"



/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/
#if !defined (COM_LOCAL_INLINE)
# define COM_LOCAL_INLINE LOCAL_INLINE
#endif


/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_GetWriteIdx
 *********************************************************************************************************************/
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
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetWriteIdx(Com_CCprQueueIterType routingQueueIndex,
                                                                  P2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, COM_APPL_DATA) writeIdx, PduLengthType payloadLengthWithHeader)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */  

  Com_CprQueueBufferReadIdxOfCprQueueType   readIdx          = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
  Com_CprQueueBufferWriteIdxOfCprQueueType  localWriteIdx    = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);

  Com_CprQueueBufferStartIdxOfCprQueueType  startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);
  Com_CprQueueBufferEndIdxOfCprQueueType    endIdxOfBuffer   = Com_GetCprQueueBufferEndIdxOfCprQueue(routingQueueIndex) - 1u;  /* ComStackLibs EndIdx is actually the first out of bounds index */

  Com_CprQueueBufferPendingWriteIdxOfCprQueueType nextWriteSlot = (Com_CprQueueBufferPendingWriteIdxOfCprQueueType)(localWriteIdx + payloadLengthWithHeader);

  /* #10 If buffer is full, return E_NOT_OK */
  if(((localWriteIdx + 1u) == readIdx) || ((readIdx == startIdxOfBuffer) && (localWriteIdx == endIdxOfBuffer))) /* COV_COM_CROSSPARTITIONQUEUE_SPECIALSTATE */
  {
    /* Buffer is full */
    retVal = E_NOT_OK;
  }
  /* #20 else if wrap around of write index has occurred */
  else if(localWriteIdx < readIdx)      /* write is possible up to the read Idx. Still needs to be checked for sufficient size */
  {
    Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSize = readIdx - localWriteIdx - 1u;
    /* #30 If CrossPartitionRoutingQueue has enough space */
    if(linearBufferSize >= payloadLengthWithHeader)  /* COV_COM_CROSSPARTITIONQUEUE_SPECIALSTATE */
    {
      *writeIdx = localWriteIdx;  /* SBSW_COM_WRITEACCESS_STACKPTR_01 */
      /* #40 Set write index to internal write index and increase pending-write-index to next slot */
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot);   /* SBSW_COM_CSL03 */
      retVal = E_OK;
    }
    /* #50 Otherwise, return E_NOT_OK */
    else
    {
      retVal = E_NOT_OK;
    }
  }
  /* #60 else no wrap around of write index has occurred or both indicies have wrapped around */
  else
  {     /* readIdx < localWriteIdx OR readIdx == localWriteIdx */
    Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSizeAtEnd;
    Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSizeAtFront;

    linearBufferSizeAtEnd = (localWriteIdx == endIdxOfBuffer) ? 0u : (Com_CprQueueBufferWriteIdxOfCprQueueType)(endIdxOfBuffer - localWriteIdx);  /* COV_COM_CROSSPARTITIONQUEUE_SPECIALSTATE */
    linearBufferSizeAtFront = (readIdx == startIdxOfBuffer) ? 0u : (readIdx - startIdxOfBuffer - 1u);
    /* #70 If there is enough space at the end of the queue, set write index to internal write index and increase pending-write-index to next slot and return E_OK */
    if(linearBufferSizeAtEnd >= payloadLengthWithHeader)
    {
      *writeIdx = localWriteIdx;  /* SBSW_COM_WRITEACCESS_STACKPTR_01 */
      /* Update write Idx */
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot);    /* SBSW_COM_CSL03 */
      retVal = E_OK;
    }
    /* #80 Otherwise, if there is enough space at the beginning of the queue */
    else if(linearBufferSizeAtFront >= payloadLengthWithHeader)    /* COV_COM_CROSSPARTITIONQUEUE_SPECIALSTATE */
    {
      if(localWriteIdx < Com_GetSizeOfCprQueueBuffer()) /* COV_COM_INDEX_IN_BOUNDS_CHECK */
      {
        /* #90 Mark remaining bytes at end as unused */
        Com_SetCprQueueBuffer(localWriteIdx, COM_CPRQUEUE_HEADERVALUE_UNUSED); /* SBSW_COM_CSL01 */
      }

      /* #100 Set write index to start index and increase pending-write-index to next slot */
      *writeIdx = startIdxOfBuffer;  /* SBSW_COM_WRITEACCESS_STACKPTR_01 */

      /* Update write Idx */
      nextWriteSlot = (Com_CprQueueBufferPendingWriteIdxOfCprQueueType)(startIdxOfBuffer + payloadLengthWithHeader);
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot); /* SBSW_COM_CSL03 */

      /* #110 Return E_OK */
      retVal = E_OK;
    }
    /* #120 Otherwise, return E_NOT_OK */
    else
    {
      /* neither fits at the end nor the beginning: do nothing */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_GetReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetReadIdx(Com_CCprQueueIterType routingQueueIndex,
                                                                 P2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, COM_APPL_DATA) readIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */


  Com_CprQueueBufferReadIdxOfCprQueueType localReadIdx = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);

  /* If CrossPartitionRoutingQueue is empty, return E_NOT_OK */
  if(localReadIdx == writeIdx)
  {
    retVal = E_NOT_OK;
  }
  /* If the current element is marked with the unused pattern, return the start index of the queue, otherwise return the stored read index */
  else
  {
    if(Com_GetCprQueueBuffer(localReadIdx) == COM_CPRQUEUE_HEADERVALUE_UNUSED)
    {   /* adjust localReadIdx if the end of the buffer is not used */
      localReadIdx = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);
    }
    *readIdx = localReadIdx;  /* SBSW_COM_WRITEACCESS_STACKPTR_01 */
    retVal = E_OK;
  }

  return retVal;
}
#endif


# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Init(void)
{
  Com_CCprQueueIterType routingQueueIndex;
  for(routingQueueIndex = 0u; routingQueueIndex < Com_GetSizeOfCCprQueue(); routingQueueIndex++)
  {
    Com_CprQueueBufferStartIdxOfCprQueueType startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);

    Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);  /* SBSW_COM_CSL_VAR_ACCESS */
    Com_SetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);  /* SBSW_COM_CSL_VAR_ACCESS */
    
    Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);  /* SBSW_COM_CSL_VAR_ACCESS */
    Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);  /* SBSW_COM_CSL_VAR_ACCESS */
  }
}
#endif


# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex)
{
  Com_SetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex));  /* SBSW_COM_CSL01 */
}
#endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex)
{
  Com_SetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, 0u);  /* SBSW_COM_CSL01 */
}
#endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueue_ReadFromCprQueueDirectly(Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  return Com_GetCprQueueBuffer(readIdx);
}
# endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_Serialize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Serialize(Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx, uint32 index, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data, uint8 header, uint8 sizeOfIndex)
{
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdxIntern = writeIdx;

  /* The caller of this function checks that no overflows occurs before writing data into the queue */

  /* Serialize Header Value */
  Com_SetCprQueueBuffer(writeIdxIntern, header); /* SBSW_COM_CSL01 */
  writeIdxIntern += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;
    
  /* Serialize index (size of datatype depends on configuration) */
  {
    uint32 indexTmp = index; /* PRQA S 2981 */ /* MD_MSR_RetVal */
    uint8 iterator = 0u;
    for(; iterator < sizeOfIndex; iterator++)
    {
      indexTmp = index >> (iterator * 8u); /* PRQA S 2985 */ /* MD_Com_2985 */
      Com_SetCprQueueBuffer(writeIdxIntern, (uint8) indexTmp); /* SBSW_COM_CSL01 */
      writeIdxIntern++;
    }
  }    
  /* Serialize Payload */
  VStdMemCpyRamToRam(Com_GetAddrCprQueueBuffer(writeIdxIntern), data->SduDataPtr, data->SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_COM_MEM_CPY_2RAM */
}
#endif

#define COM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

  MD_Com_2985: Misra Rule 2.2
     Reason:      The value of the result is only that of the left-hand operand for the first iteration. It depends on the configuration it there will be more than one iteration step.
     Risk:        No risk, as the bitwise operation is only redundant.
     Prevention:  No prevention necessary.

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_CprQueue.c
 *********************************************************************************************************************/
