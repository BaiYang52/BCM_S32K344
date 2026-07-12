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
/*!        \file  Com_CprQueueSigGw.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
 #define COM_CPRQUEUESIGGW_SOURCE
 
 /**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CprQueue.h"
#include "Com_SignalGateway.h"
#include "Com.h"
#include "Com_CprQueueSigGw.h"

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

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))                                                  
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_Signal
 *********************************************************************************************************************/
/*! \brief      This function deserializes the parameter from the queue starting at the readIdx position.
 *  \details    -
 *  \param[in]  routingQueueIndex   Index to the queue
 *  \param[in]  readIdx             Index to the position in the queue where the data shall be read
 *  \param[in]  idxGwSigMapping     Handle Id of the signal gateway to be deserialized
 *  \param[in]  data                pointer to the data to be deserialized
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigMapping,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif                                                           

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))                                                           
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_SignalGroup
 *********************************************************************************************************************/
/*! \brief      This function deserializes the parameter from the queue starting at the readIdx position.
 *  \details    -
 *  \param[in]  routingQueueIndex   Index to the queue
 *  \param[in]  readIdx             Index to the position in the queue where the data shall be read
 *  \param[in]  idxGwSigGrpMapping  Handle Id of the signalGroup gateway to be deserialized
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigGrpMapping);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_GroupSignal
 *********************************************************************************************************************/
/*! \brief      This function deserializes the parameter from the queue starting at the readIdx position.
 *  \details    -
 *  \param[in]  routingQueueIndex   Index to the queue
 *  \param[in]  idxGwGrpSigMapping  Handle Id of the groupSignal gateway
 *  \param[in]  data                pointer to the data to be deserialized
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingEndIdxOfGwSigGrpMappingType idxGwGrpSigMapping,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif


#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))                                                         
/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalHeaderSize
 *********************************************************************************************************************/
/*! \brief      This function returns the size of the header which is put in front of the signal gateway Payload in the CrossPartitionRoutingQueue.
 *  \details    -
 *  \return     the header size
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalHeaderSize(void);
#endif         

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalGroupHeaderSize
 *********************************************************************************************************************/
/*! \brief      This function returns the size of the header which is put in front of the signalGroup gateway Payload in the CrossPartitionRoutingQueue.
 *  \details    -
 *  \return     the header size
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalGroupHeaderSize(void);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/


#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_Signal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigMapping,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMappingTmp = 0u;
  Com_RxAccessInfoIterType idxRxAccessInfo = 0u;
  uint8 iterator = 0u;
  
  /* skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  /* Deserialize signal gateway index */
  for(; iterator < sizeof(Com_GwSigMappingEndIdxOfGwInfoType); iterator++)
  {
    idxGwSigMappingTmp = idxGwSigMappingTmp | (((Com_GwSigMappingEndIdxOfGwInfoType) Com_GetCprQueueBuffer(readIdxTmp)) << (iterator*8u)); /* PRQA S 2985 */ /* MD_Com_2985 */
    readIdxTmp++;
  }
  *idxGwSigMapping = idxGwSigMappingTmp; /* SBSW_COM_WRITEACCESS_STACKPTR_01 */
  idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwSigMapping(*idxGwSigMapping);

  /* Return pointer pointing to payload */
  data->SduLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo); /* SBSW_COM_CSL03 */
  data->SduDataPtr = Com_GetAddrCprQueueBuffer(readIdxTmp);     /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
  readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->SduLength;

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);  /* SBSW_COM_CSL03 */
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_SignalGroup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigGrpMapping)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMappingTmp = 0u;
  uint8 iterator = 0u;
  
  /* skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  /* Deserialize signal gateway index */
  for(; iterator < sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType); iterator++)
  {
    idxGwSigGrpMappingTmp = idxGwSigGrpMappingTmp | (((Com_GwSigGrpMappingEndIdxOfGwInfoType) Com_GetCprQueueBuffer(readIdxTmp)) << (iterator*8u)); /* PRQA S 2985 */ /* MD_Com_2985 */
    readIdxTmp++;
  }
  *idxGwSigGrpMapping = idxGwSigGrpMappingTmp; /* SBSW_COM_WRITEACCESS_STACKPTR_01 */

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);  /* SBSW_COM_CSL03 */
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Deserialize_GroupSignal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_Deserialize_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingEndIdxOfGwSigGrpMappingType idxGwGrpSigMapping,
                                                            P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Com_RxAccessInfoIterType idxRxAccessInfo;
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = Com_GetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex);  /* SBSW_COM_CSL03 */
  idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);

  /* Return pointer pointing to payload */
  data->SduLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo); /* SBSW_COM_CSL03 */
  data->SduDataPtr = Com_GetAddrCprQueueBuffer(readIdxTmp);     /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
  readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->SduLength;

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);  /* SBSW_COM_CSL03 */
}
#endif                                                          

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_GwSigMappingEndIdxOfGwInfoType);
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalGroupHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalGroupHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType);
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_Signal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigMapping, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  /* Read data from queue */
  Com_CprQueueSigGw_Deserialize_Signal(routingQueueIndex, readIdx, idxGwSigMapping, data);  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, ((Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) (Com_CprQueueSigGw_GetSignalHeaderSize() + data->SduLength)));  /* SBSW_COM_CSL03 */
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_SignalGroup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigGrpMapping)
{
  /* Read signal group index from queue */
  Com_CprQueueSigGw_Deserialize_SignalGroup(routingQueueIndex, readIdx, idxGwSigGrpMapping);  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, Com_CprQueueSigGw_GetSignalGroupHeaderSize());  /* SBSW_COM_CSL03 */
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_GroupSignal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingEndIdxOfGwSigGrpMappingType idxGwGrpSigMapping, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  /* Read data from queue */
  Com_CprQueueSigGw_Deserialize_GroupSignal(routingQueueIndex, idxGwGrpSigMapping, data);  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, ((Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) data->SduLength));  /* SBSW_COM_CSL03 */ 
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) &&  (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_Signal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_Signal(Com_CCprQueueIterType routingQueueIndex, Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */ 
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx;
  
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  /* If data fits into the CrossPartitionRoutingQueue */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, data->SduLength + Com_CprQueueSigGw_GetSignalHeaderSize()) == E_OK)  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */     
  {   
    /* Serialize data of signal gateway */
    Com_CprQueue_Serialize(writeIdx, idxGwSigMapping, data, COM_CPRQUEUE_HEADERVALUE_SIGNAL, sizeof(Com_GwSigMappingEndIdxOfGwInfoType)); /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
    /* Update the WriteIdx*/
    Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex)); /* SBSW_COM_CSL03 */
    retVal = E_OK;
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();

  return retVal;
}
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_SignalGroup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */ 
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx;
  
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  /* If signal group gateway index fits into the CrossPartitionRoutingQueue */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, data->SduLength + Com_CprQueueSigGw_GetSignalGroupHeaderSize()) == E_OK)  /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */     
  {   
    /* Serialize index of signal group gateway */
    Com_CprQueue_Serialize(writeIdx, idxGwSigGrpMapping, data, COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP, sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType)); /* SBSW_COM_VALID_PTR_ENSURED_BY_CALLING_FUNC_AND_STACKPTR */
    /* Update the WriteIdx*/
    Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex)); /* SBSW_COM_CSL03 */
    retVal = E_OK;
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
 *  END OF FILE: Com_CprQueueSigGw.c
 *********************************************************************************************************************/
