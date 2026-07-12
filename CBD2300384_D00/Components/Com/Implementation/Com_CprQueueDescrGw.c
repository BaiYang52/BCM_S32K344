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
/*!        \file  Com_CprQueueDescrGw.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_CPRQUEUEDESCRGW_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CprQueueDescrGw.h"
#include "Com_CprQueue.h"
#include "Com_DescriptionGateway.h"
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

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)                                           
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_Deserialize
 *********************************************************************************************************************/
/*! \brief      This function deserializes the parameter from the queue starting at the readIdx position.
 *  \details    -
 *  \param[in]  routingQueueIndex   Index to the queue
 *  \param[in]  readIdx             Index to the position in the queue where the data shall be read
 *  \param[in]  gwTxPduIdx          Handle Id of the DescriptionRouting to be deserialized
 *  \param[in]  data                pointer to the data to be deserialized
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueDescrGw_Deserialize(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwTxPduDescriptionInfoIterType, AUTOMATIC, COM_APPL_DATA) gwTxPduIdx,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetHeaderSize
 *********************************************************************************************************************/
/*! \brief      This function returns the size of the header which is put in front of the DescriptionRouting Payload in the CrossPartitionRoutingQueue.
 *  \details    -
 *  \return     the header size
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_GetHeaderSize(void);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_Deserialize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueDescrGw_Deserialize(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwTxPduDescriptionInfoIterType, AUTOMATIC, COM_APPL_DATA) gwTxPduIdx,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  /* Skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;
  
  /* Deserialize gwTxPduIdx (size of datatype depends on configuration) */
  {
    uint8 iterator = 0u;
    Com_GwTxPduDescriptionInfoIterType gwTxPduIdxTmp = 0u;
    for(; iterator < sizeof(Com_GwTxPduDescriptionInfoIterType); iterator++)
    {
      gwTxPduIdxTmp = gwTxPduIdxTmp | (((Com_GwTxPduDescriptionInfoIterType) Com_GetCprQueueBuffer(readIdxTmp)) << (iterator*8u)); /* PRQA S 2985 */ /* MD_Com_2985 */
      readIdxTmp++;
    }
    *gwTxPduIdx = gwTxPduIdxTmp;  /* SBSW_COM_WRITEACCESS_STACKPTR_01 */
  }

  /* Return pointer pointing to payload */
  data->SduLength = Com_GetSourceMaxLengthOfGwTxPduDescriptionInfo(*gwTxPduIdx);  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
  data->SduDataPtr = Com_GetAddrCprQueueBuffer(readIdxTmp);     /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
  readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->SduLength;
  
  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);  /* SBSW_COM_CSL03 */
}
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_GetHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_GwTxPduDescriptionInfoIterType);
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ReadData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ReadData(Com_CCprQueueIterType routingQueueIndex, P2VAR(Com_GwTxPduDescriptionInfoIterType, AUTOMATIC, COM_APPL_DATA) gwTxPduIdx, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  /* Read data from queue */
  Com_CprQueueDescrGw_Deserialize(routingQueueIndex, readIdx, gwTxPduIdx, data);  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, ((Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType)(Com_CprQueueDescrGw_GetHeaderSize() + data->SduLength)));  /* SBSW_COM_CSL03 */        
}
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_WriteData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_WriteData(Com_CCprQueueIterType routingQueueIndex, Com_GwTxPduDescriptionInfoIterType gwTxPduIdx,  CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */ 
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx;
  
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  /* If data fits into the CrossPartitionRoutingQueue, serialize data, update write index and return E_OK */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, data->SduLength + Com_CprQueueDescrGw_GetHeaderSize()) == E_OK)  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */     
  {   
    Com_CprQueue_Serialize(writeIdx, (uint32) gwTxPduIdx, data, COM_CPRQUEUE_HEADERVALUE_DESCRIPTION, sizeof(Com_GwTxPduDescriptionInfoIterType)); /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
    /* Update the WriteIdx */
    Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex)); /* SBSW_COM_CSL03 */
    retVal = E_OK;
  }
  /* Otherwise return E_NOT_OK */
  else
  {
    retVal = E_NOT_OK;
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();

  return retVal;
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
