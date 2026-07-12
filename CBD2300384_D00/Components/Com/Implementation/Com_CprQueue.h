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
/*!        \file  Com_CprQueue.h
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CPRQUEUE_H)
# define COM_CPRQUEUE_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

# define COM_CPRQUEUE_HEADERVALUE_UNUSED        0x00u
# define COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE    1u

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */


# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
  Com_CprQueue_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the CrossPartitionRoutingQueue values.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Init(void);
# endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 * \brief       This function updates the readIdx.
 *  \details    The pointer to the data retrived by Peek() is no longer valid after this.
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex);
# endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*! \brief      This function resets the NumberOfBytesRead.
 *  \details    Called before a batch of read operations that should in sum not exceed a threshold of bytes read.
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex);
# endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_GetWriteIdx
 *********************************************************************************************************************/
/*! \brief      Returns a valid write Idx if the data fits into the buffer
 *  \details    -
 *  \param[in]  routingQueueIndex        Id of the queue to be used
 *  \param[out] writeIdx                 The returned writeIdx. Only valid for successful return value.
 *  \param[in]  payloadLengthWithHeader  length of the data to be put into the queue
 *  \return     E_OK                     Data fits into the buffer.
 *              E_NOT_OK                 Data does not fit into the buffer.
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre        -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetWriteIdx(Com_CCprQueueIterType routingQueueIndex,
                                                        P2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, COM_APPL_DATA) writeIdx, PduLengthType payloadLengthWithHeader);
# endif

# if((COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_GetReadIdx
 *********************************************************************************************************************/
/*! \brief       Get a valid readIdx.
 *  \details -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  readIdx                  the valid readIdx.
 *  \return      E_OK                     Some data is available to be read.
 *               E_NOT_OK                 No data is available to be read.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetReadIdx(Com_CCprQueueIterType routingQueueIndex,
                                                       P2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, COM_APPL_DATA) readIdx);
# endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*! \brief       Get the value in the queue at the position readIdx.
 *  \details -
 *  \param[in]   readIdx                  the valid readIdx.
 *  \return      queueValue               the value in the queue at the position readIdx.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueue_ReadFromCprQueueDirectly(Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
# endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) || (COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueue_Serialize
 *********************************************************************************************************************/
/*! \brief      This function serializes the parameter into the queue starting at the writeIdx position.
 *  \details    -
 *  \param[in]  writeIdx      Index to the position in the queue where the data shall be written
 *  \param[in]  index         Handle Id to be serialized
 *  \param[in]  data          pointer to the data to be serialized
 *  \param[in]  header        header to be serialized
 *  \param[in]  sizeOfIndex   size of the index that is serialized
 *  \pre        -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Serialize(Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx, uint32 index, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data, uint8 header, uint8 sizeOfIndex);
#endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CPRQUEUE_H */

/**********************************************************************************************************************
  END OF FILE: Com_CprQueue.h
**********************************************************************************************************************/
